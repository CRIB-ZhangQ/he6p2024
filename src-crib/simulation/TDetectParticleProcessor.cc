/**
 * @file    TDetectParticleProcessor.cc
 * @brief
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2024-01-18 14:36:43
 * @note
 */

#include "TDetectParticleProcessor.h"
#include "../geo/TDetectorParameter.h"
#include "TParticleInfo.h"
#include "TReactionInfo.h"

#include "../telescope/TTelescopeData.h"

#include <TArtAtomicMassTable.h>
#include <TDataObject.h>
#include <constant.h>

#include <TClass.h>
#include <TClonesArray.h>
#include <TGeoManager.h>
#include <TLorentzVector.h>
#include <TRandom.h>

using art::TDetectParticleProcessor;

ClassImp(TDetectParticleProcessor);

TDetectParticleProcessor::TDetectParticleProcessor() : fInData(NULL), fOutData(NULL) {
    RegisterInputCollection("InputCollection", "input branch (collection) name", fInputColName, TString("input"));
    RegisterOutputCollection("OutputCollection", "output branch (collection) name", fOutputColName,
                             TString("reaction_particles"));

    StringVec_t init_s_vec;
    DoubleVec_t init_d_vec;
    RegisterProcessorParameter("ReactionParticleName", "reaction particle names", fReacPartName, init_s_vec);
    RegisterProcessorParameter("EnergyResolution", "energy resolution \% unit", fResolution, init_d_vec);
    RegisterProcessorParameter("TargetisGas", "Bool, target is targer or not", fTargetisGas, false);
    RegisterProcessorParameter("TargetName", "gas target name", fTargetName, TString(""));
    RegisterProcessorParameter("GetOnlyLightParticle", "detect (treat) only light particle", fDoOnlyLightPart, false);
    RegisterProcessorParameter("IsNotDecay", "Isotropic decay or nuclear reaction", fIsNotDecay, false);

    RegisterOptionalInputInfo("DetectorParameter", "name of telescope parameter", fParameterName,
                              TString("prm:detector"), &fDetectorPrm, "TClonesArray", "art::TDetectorParameter");
}

TDetectParticleProcessor::~TDetectParticleProcessor() {
    delete fOutData;
    fOutData = NULL;
    delete fElossTable;
    fElossTable = NULL;
}

void TDetectParticleProcessor::Init(TEventCollection *col) {
    fInGeom = reinterpret_cast<TGeoManager **>(col->GetObjectRef("geom"));
    if (!fInGeom) {
        SetStateError("gate array not found. Run TUserGeoInitializer before this.");
        return;
    }

    Info("Init", "%s => %s", fInputColName.Data(), fOutputColName.Data());

    fInData = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName.Data()));
    if (!fInData) {
        SetStateError(TString::Format("input not found: %s", fInputColName.Data()));
        return;
    }
    const TClass *const cl = (*fInData)->GetClass();
    //if (!cl->InheritsFrom(art::TParticleInfo::Class())) {
    if (!cl->InheritsFrom(art::TReactionInfo::Class())) {
        SetStateError("contents of input array must inherit from art::TParticleInfo");
    }

    if (!fDetectorPrm) {
        SetStateError("input parameter name is wrong");
    }

    if (fResolution.size() == 1) {
        for (Int_t iDet = 0; iDet < (*fDetectorPrm)->GetEntriesFast() - 1; iDet++) {
            fResolution.emplace_back(fResolution[0]);
        }
    } else if (fResolution.size() == 0) {
        for (Int_t iDet = 0; iDet < (*fDetectorPrm)->GetEntriesFast(); iDet++) {
            fResolution.emplace_back(0.0);
        }
    } else if (fResolution.size() != (*fDetectorPrm)->GetEntriesFast()) {
        SetStateError("Resolution parameter in yaml file is wrong");
        return;
    }

    // setting for SRIMlib
    fElossTable = new SRIMData();
    Bool_t isFirst = true;
    for (Int_t iPart = 0; iPart < fReacPartName.size(); iPart++) {
      std::vector<std::vector<SRIMtable *>> srim_vv;
        for (Int_t iDet = 0; iDet < (*fDetectorPrm)->GetEntriesFast(); iDet++) {
            std::vector<SRIMtable *> srim_v;
            TParameterObject *inPrm = static_cast<TParameterObject *>((*fDetectorPrm)->At(iDet));
            TDetectorParameter *Prm = dynamic_cast<TDetectorParameter *>(inPrm);
            StringVec_t material_vec = Prm->GetMaterial();
            if (isFirst) {
                fMaxRadius = Prm->GetMaxRadius();
                isFirst = false;
            }

            for (Int_t iMat = 0; iMat < material_vec.size(); iMat++) {
                srim_v.emplace_back(fElossTable->GetTable(fReacPartName[iPart], material_vec[iMat]));
            }
            srim_vv.emplace_back(srim_v);
            srim_v.clear();
        }
        fEloss_vvvec.emplace_back(srim_vv);
        srim_vv.clear();
    }

    if (fTargetisGas) {
        for (Int_t iPart = 0; iPart < fReacPartName.size(); iPart++) {
            fTargetEloss_vec.emplace_back(fElossTable->GetTable(fReacPartName[iPart], fTargetName));
        }
    }


    fOutData = new TClonesArray("art::TTelescopeData");
    Info("Init", "output branch used art::TTelescopeData");
    fOutData->SetName(fOutputColName);
    col->Add(fOutputColName, fOutData, fOutputIsTransparent);

    gRandom->SetSeed(time(NULL));
}

void TDetectParticleProcessor::Process() {
    fOutData->Clear("C");
    TGeoManager *geom = static_cast<TGeoManager *>(*fInGeom);

    Int_t itr = 0;
    for (Int_t iData = 0; iData < (*fInData)->GetEntriesFast(); ++iData) {
        if (fDoOnlyLightPart && iData == 0 && fIsNotDecay)
            continue;

        const TDataObject *const inData = static_cast<TDataObject *>((*fInData)->At(iData));
        //const TParticleInfo *const Data = dynamic_cast<const TParticleInfo *>(inData);
        const TReactionInfo *const Data = dynamic_cast<const TReactionInfo *>(inData);

        Double_t energy = Data->GetEnergy();
        if (energy < 0.01)
            continue; // stop in the target

        // reaction position
        Double_t reac_X = Data->GetX();
        Double_t reac_Y = Data->GetY();
        Double_t reac_Z = Data->GetZ();
        TVector3 first_position(reac_X, reac_Y, reac_Z);
        TVector3 velocity = (Data->GetLorentzVector()).Vect().Unit();
        //TTrack track = Data->GetTrack();
        //TVector3 first_position(track.GetX(current_z), track.GetY(current_z), current_z);

        // check if beam particle hits the detector
        geom->SetCurrentPoint(first_position.X(), first_position.Y(), first_position.Z());
        geom->SetCurrentDirection(velocity.X(), velocity.Y(), velocity.Z());

        // exclude TOP boundary
        geom->GetCurrentNode();
        geom->FindNode();

        geom->FindNextBoundary(fMaxRadius);
        Double_t distance = geom->GetStep();
        geom->Step();
        Bool_t isHit = geom->IsStepEntering();
        if (!isHit)
            continue;

        TString hitname = geom->GetPath();
        TString det_id = hitname[hitname.Sizeof() - 2];
        //std::cout<<"hitname = "<<hitname<<", detID = "<<det_id<<std::endl;
        if (hitname.Sizeof() < 8 || det_id.Atoi() >= (*fDetectorPrm)->GetEntriesFast())
            continue; // for safety path=/TOP_1/hoge_0

        if (energy < 0.01)
            continue; // stop in the target

        // prepare output branch
        TTelescopeData *outData = static_cast<TTelescopeData *>(fOutData->ConstructedAt(itr));
        outData->SetID(det_id.Atoi() + 1); // 1 start (not 0)
        TVector3 det_position(distance * velocity.X(), distance * velocity.Y(), distance * velocity.Z());
        det_position += first_position;
        outData->SetPosition(det_position);

        TParameterObject *inPrm = static_cast<TParameterObject *>((*fDetectorPrm)->At(det_id.Atoi()));
        TDetectorParameter *Prm = dynamic_cast<TDetectorParameter *>(inPrm);

        det_position -= TVector3(Prm->GetCenterRotPos(0), Prm->GetCenterRotPos(1), Prm->GetCenterRotPos(2));
        det_position.RotateY(-Prm->GetAngle());

        outData->SetXID(GetStripID(det_position.X(), Prm->GetStripNum(0), Prm->GetSize(0)));
        outData->SetYID(GetStripID(det_position.Y(), Prm->GetStripNum(1), Prm->GetSize(1)));
        if (outData->GetXID() == -1 || outData->GetYID() == -1) {
            fOutData->RemoveAt(itr);
            continue;
        }

        StringVec_t material_vec = Prm->GetMaterial();
        Double_t energy_total = 0.0;
        for (Int_t iMat = 0; iMat < material_vec.size(); iMat++) {
            if (energy > 0.01) {
                Double_t new_energy = fEloss_vvvec[iData][det_id.Atoi()][iMat]->GetNewE(energy, Prm->GetThickness(iMat));
                energy_total += energy - new_energy;
                outData->PushEnergyArray(energy - new_energy);
                energy = new_energy;
            } else {
                outData->PushEnergyArray(0.0);
            }
        }
        outData->SetEtotal(gRandom->Gaus(energy_total, energy_total * fResolution[det_id.Atoi()]));

        itr++;
    }
}

Int_t TDetectParticleProcessor::GetStripID(Double_t pos, Int_t max_strip, Double_t size) {
    Int_t result = -1;
    for (Int_t i = 0; i < max_strip; i++) {
        if (size * ((Double_t)i / max_strip - 0.5) < pos && pos < size * ((Double_t)(i + 1) / max_strip - 0.5)) {
            result = i;
            break;
        }
    }
    // if(result == -1) std::cout << "warning, stripID is incorrect" << std::endl;
    return result;
}
