/**
 * @file    TTelescopeProcessor.cc
 * @brief   gather the information to the one object
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2024-01-17 16:51:53
 * @note    treat the largest value of each layor
 *          the data of X side is used for DSSSD
 *          assume beam position (0, 0) and direction (0, 0, 1)
 */

#include "TReacAnalysisProcessor.h"
#include "TReacAnalysisData.h"

#include "TDataObject.h"
#include "../telescope/TTelescopeData.h"
#include "TMWDCTrackingResult.h"
#include "constant.h"

#include <TClass.h>
#include <TClonesArray.h>
#include <TMath.h>

//#include "../geo/TDetectorParameter.h"
//#include "../geo/TTargetParameter.h"
//#include <TRandom.h>

using art::TReacAnalysisProcessor;

ClassImp(art::TReacAnalysisProcessor);

// Default constructor
TReacAnalysisProcessor::TReacAnalysisProcessor()
    : fInData1(NULL), fInData2(NULL), fInData3(NULL), fOutData(NULL) {
    //RegisterInputCollection("InputCollection1",
    //                        "array of objects inheriting from art::TMWDCTrackingResult",
    //                        fInputColName1, TString("fmwdca"));
    RegisterInputCollection("InputCollection2",
                            "array of objects inheriting from art::TMWDCTrackingResult",
                            fInputColName2, TString("fmwdcb"));
    RegisterInputCollection("InputCollection3",
                            "array of objects inheriting from art::TMWDCTrackingResult",
                            fInputColName3, TString("ftrack"));
    RegisterInputCollection("InputCollection4",
                            "array of objects inheriting from art::TTelescopeData",
                            fInputColName4, TString("ftel"));
    RegisterOutputCollection("OutputCollection", "output class will be art::TReacAnalysisData",
                             fOutputColName, TString("fPRA"));
    StringVec_t init_s_vec;
    IntVec_t init_i_vec;
    DoubleVec_t init_d_vec;

    RegisterProcessorParameter("Ebeam", "beam energy of 6He", fEbeam, 44.0);
    RegisterProcessorParameter("Mass", "mass number of the A(a,b)B reaction", fMass, init_d_vec);
    RegisterProcessorParameter("Charge", "charge number of the A(a,b)B reaction", fCharge, init_i_vec);
    RegisterProcessorParameter("Qvalue", "reaction energy of the A(a,b)B reaction", fQvalue, 0.0);
    RegisterProcessorParameter("Exci", "the excited state of compound nucleus:a+A", fExci, -1.8);
    RegisterProcessorParameter("Particle", "the emitted particle of A(a,b)B reaction: b or B", fParticle, 0);//0: light particle, 1: heavy particle
}

TReacAnalysisProcessor::~TReacAnalysisProcessor() {
    delete fOutData;
    fOutData = NULL;
}

void TReacAnalysisProcessor::Init(TEventCollection *col) {
    Info("Init", "%s %s %s %s => %s", fInputColName1.Data(), fInputColName2.Data(), fInputColName3.Data(), fInputColName4.Data(), fOutputColName.Data());

    // check if mwdca object exist or not
    //fInData1 = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName1.Data()));
    //if (!fInData1) {
    //    SetStateError(TString::Format("input not found: %s", fInputColName1.Data()));
    //    return;
    //}

    // check if mwdcb object exist or not
    fInData2 = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName2.Data()));
    if (!fInData2) {
        SetStateError(TString::Format("input not found: %s", fInputColName2.Data()));
        return;
    }

    // check if f3mwdc object exist or not
    fInData3 = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName3.Data()));
    if (!fInData3) {
        SetStateError(TString::Format("input not found: %s", fInputColName3.Data()));
        return;
    }

    // check if telescope object exist or not
    fInData4 = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName4.Data()));
    if (!fInData4) {
        SetStateError(TString::Format("input not found: %s", fInputColName4.Data()));
        return;
    }

    // check if input collection is valid or not
    //const TClass *const cl1 = (*fInData1)->GetClass();
    const TClass *const cl2 = (*fInData2)->GetClass();
    const TClass *const cl3 = (*fInData3)->GetClass();
    const TClass *const cl4 = (*fInData4)->GetClass();
    //fInputHasData = (cl1->InheritsFrom(art::TMWDCTrackingResult::Class())) && (cl2->InheritsFrom(art::TMWDCTrackingResult::Class())) && (cl3->InheritsFrom(art::TMWDCTrackingResult::Class())) && (cl4->InheritsFrom(art::TTelescopeData::Class()));
    fInputHasData = (cl2->InheritsFrom(art::TMWDCTrackingResult::Class())) && (cl3->InheritsFrom(art::TMWDCTrackingResult::Class())) && (cl4->InheritsFrom(art::TTelescopeData::Class()));

    if (!fInputHasData) {
        SetStateError("contents of input array must inherit from art::TTimingChargeData");
        return;
    }

    fOutData = new TClonesArray("art::TReacAnalysisData");
    fOutData->SetName(fOutputColName);
    col->Add(fOutputColName, fOutData, fOutputIsTransparent);

    //gRandom->SetSeed();
    //gRandom->SetSeed(time(NULL));
}

void TReacAnalysisProcessor::Process() {
    fOutData->Clear("C");

    //Int_t nData1 = (*fInData1)->GetEntriesFast();
    Int_t nData2 = (*fInData2)->GetEntriesFast();
    Int_t nData3 = (*fInData3)->GetEntriesFast();
    Int_t nData4 = (*fInData4)->GetEntriesFast();

    // if no hit, do nothing
    //if (nData1 == 0 || nData2 == 0 || nData3 == 0 || nData4 == 0) {
    if (nData2 == 0 || nData3 == 0 || nData4 == 0) {
        return;
    }


    TReacAnalysisData *outData = static_cast<TReacAnalysisData *>(fOutData->ConstructedAt(0));
    outData->Clear();

    //outData->SetID(fTelID);
    //if (fHasDetPrm && fHasTargetPrm) {
    //    outData->SetN(fDetParameter->GetN());
    //} else {
    //    outData->SetN(DEFAULT_SSD_MAX_NUMBER);
    //}

    //const TDataObject *const inData1 = static_cast<TDataObject *>((*fInData1)->At(0));
    //const TMWDCTrackingResult *const fmwdca = dynamic_cast<const TMWDCTrackingResult *>(inData1);
    const TDataObject *const inData2 = static_cast<TDataObject *>((*fInData2)->At(0));
    const TMWDCTrackingResult *const fmwdcb = dynamic_cast<const TMWDCTrackingResult *>(inData2);
    const TDataObject *const inData3 = static_cast<TDataObject *>((*fInData3)->At(0));
    const TMWDCTrackingResult *const f3mwdc = dynamic_cast<const TMWDCTrackingResult *>(inData3);
    const TDataObject *const inData4 = static_cast<TDataObject *>((*fInData4)->At(0));
    const TTelescopeData *const ftel = dynamic_cast<const TTelescopeData *>(inData4);


    // mwdca process
    //Double_t dca_x = fmwdca->GetTrack()->GetY();//fmwdc->GetY(), directly
    //Double_t dca_y = fmwdca->GetTrack()->GetY();
    //Double_t dca_z = -462.0;
    //TVector3 dca_p(dca_x, dca_y, -462.0);

    // mwdcb process
    Double_t dcb_x = fmwdcb->GetX();
    Double_t dcb_y = fmwdcb->GetY();
    //Double_t dcb_z = -171.5;
    //TVector3 dcb_p(dcb_x, dcb_y, -171.5);

    // f3mwdc: CH2 target
    Double_t tar_x = f3mwdc->GetX();
    Double_t tar_y = f3mwdc->GetY();
    Double_t tar_z = 0.0;
    Double_t beam_fA = f3mwdc->GetA();
    Double_t beam_fB = f3mwdc->GetB();
    TVector3 tar_p(tar_x, tar_y, tar_z);

    Double_t beam_fX = TMath::Tan(beam_fA);
    Double_t beam_fY = TMath::Tan(beam_fB);
    Double_t beam_fZ = 1.0;
    Double_t norm = TMath::Sqrt(beam_fX*beam_fX + beam_fY*beam_fY + 1.0);
    Double_t beam_dir_x = beam_fX/norm;
    Double_t beam_dir_y = beam_fY/norm;
    Double_t beam_dir_z = beam_fZ/norm;



    //telescope
    Int_t telXID = ftel->GetXID();
    Int_t telYID = ftel->GetYID();
    Double_t teldEX = ftel->GetdE();
    Double_t telE_re = ftel->GetE();
    Double_t telEtotal = ftel->GetEtotal();
    Double_t teldEXTiming = ftel->GetTelTiming();
    Double_t telEbTiming = ftel->GetTimingArray(1);
    Double_t px = ftel->X(); 
    Double_t py = ftel->Y(); 
    Double_t pz = ftel->Z(); 
    TVector3 det_p(px, py, pz);

    //check the timing valid
    if(!IsValid(teldEXTiming))return;

    //the direction of 6He beam
    //emit_dir
    TVector3 emit_dir;
    TVector3 beam_dir(beam_dir_x, beam_dir_y, beam_dir_z);
    Double_t angle_lab, angle_cm;

    //from mwdca and mwdcb
    //beam_dir = dcb_p - dca_p;
    emit_dir = det_p - tar_p; 
    angle_lab = beam_dir.Angle(emit_dir) * TMath::RadToDeg();
    angle_cm = 180.0 - (angle_lab + asin(fgamma()*sin(angle_lab*TMath::DegToRad()))*TMath::RadToDeg());
    //std::cout<<"theta_lab1 = "<<angle_lab<<", theta_test = "<<angle_lab_test<<std::endl;


    outData->SetMWDCb_X(dcb_x);
    outData->SetMWDCb_Y(dcb_y);
    outData->Setf3MWDC_X(tar_x);
    outData->Setf3MWDC_Y(tar_y);
    outData->SetBeamAngle_X(beam_fA*TMath::RadToDeg());
    outData->SetBeamAngle_Y(beam_fB*TMath::RadToDeg());
    outData->SetXID(telXID);
    outData->SetYID(telYID);
    outData->SetdEX(teldEX);
    outData->SetdEXTiming(teldEXTiming);
    outData->SetE(telE_re);
    outData->SetEbTiming(telEbTiming);
    outData->SetEtotal(telEtotal);
    outData->SetTheta_lab(angle_lab);
    outData->SetTheta_cm(angle_cm);
}


Double_t TReacAnalysisProcessor::fgamma() {
  Double_t mass_a = fMass[1], mass_A = fMass[0] , mass_b = fMass[2] , mass_B = fMass[3];
  Double_t Ecm = fEbeam * mass_A / (mass_a + mass_A);
  Double_t gamma;

  if(fParticle == Light_b)
    gamma = mass_a*mass_b*(mass_b+mass_B)*Ecm / (mass_A*mass_B*(mass_a+mass_A)*(Ecm+fQvalue));
  else
    gamma = mass_a*mass_B*(mass_B+mass_b)*Ecm / (mass_A*mass_b*(mass_a+mass_A)*(Ecm+fQvalue));
  return TMath::Sqrt(gamma);
}
