/*
   @File name     : TTGTIKProcessor.cc
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-08-01 11:11:20
   @Last modified : 2023-08-03 12:10:37
*/
// bisection method (not Newton method)

#include "TTGTIKProcessor.h"

#include <ITiming.h>
#include <ICharge.h>
#include <constant.h>
#include <TDataObject.h>
#include <TArtAtomicMassTable.h>

#include "../simulation/TReactionInfo.h"

#include <TClonesArray.h>
#include <TClass.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TRandom.h>

using art::TTGTIKProcessor;

ClassImp(art::TTGTIKProcessor);

TTGTIKProcessor::TTGTIKProcessor() : fInData(NULL), fInTrackData(NULL), fOutData(NULL)
{
   RegisterInputCollection("InputCollection", "telescope data inherit from TTelescopeData", fInputColName,
                           TString("tel"));
   RegisterInputCollection("InputTrackCollection", "tracking data inherit from TTrack", fInputTrackColName,
                           TString("track"));
   RegisterOutputCollection("OutputCollection", "reconstracted reaction information using TGTIK method", fOutputColName,
                            TString("result"));

   StringVec_t init_s_vec;
   IntVec_t init_i_vec;
   DoubleVec_t init_d_vec;
   RegisterProcessorParameter("InitialBeamEnergy", "beam energy just after the window", fInitialBeamEnergy, 0.0);
   RegisterProcessorParameter("EnergyThreshold", "detected energy threshold", fEnergyThreshold, 0.0);
   RegisterProcessorParameter("TargetGasName", "target gas name", fTargetName, TString(""));
   RegisterProcessorParameter("ParticleName", "particle names of the reaction", fParticleName, init_s_vec);
   RegisterProcessorParameter("ParticleMass", "particle masses of the reaction", fParticleMass, init_d_vec);
   RegisterProcessorParameter("ParticleAtomicNum", "particle atomic numbers of the reaction", fParticleAtomicNum,
                              init_i_vec);

   RegisterOptionalInputInfo("DetectorParameter", "name of telescope parameter", fParameterName,
                             TString("prm:detector"), &fDetectorPrm, "TClonesArray", "art::TDetectorParameter");
}

TTGTIKProcessor::~TTGTIKProcessor()
{
   delete fOutData;
   fOutData = NULL;
   delete fElossTable;
   fElossTable = NULL;
}

void TTGTIKProcessor::Init(TEventCollection *col)
{
   Info("Init", "%s, %s => %s", fInputColName.Data(), fInputTrackColName.Data(), fOutputColName.Data());

   fInData = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName.Data()));
   if (!fInData) {
      SetStateError(TString::Format("input not found: %s", fInputColName.Data()));
      return;
   }

   fInTrackData = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputTrackColName.Data()));
   if (!fInTrackData) {
      SetStateError(TString::Format("input not found: %s", fInputTrackColName.Data()));
      return;
   }

   const TClass *const cl1 = (*fInData)->GetClass();
   const TClass *const cl2 = (*fInTrackData)->GetClass();
   if (!(cl1->InheritsFrom(art::TTelescopeData::Class()))) {
      SetStateError(TString::Format("%s need to inherit from TTelescopeData", fInputColName.Data()));
      return;
   }
   if (!(cl2->InheritsFrom(art::TTrack::Class()))) {
      SetStateError(TString::Format("%s need to inherit from TTrack", fInputTrackColName.Data()));
      return;
   }

   if (!fDetectorPrm) {
      SetStateError(TString::Format("input not found: %s", fParameterName.Data()));
      return;
   }

   if (fParticleName.size() != 4 || fParticleMass.size() != 4 || fParticleAtomicNum.size() != 4) {
      SetStateError("not correct input yaml");
      return;
   }

   Info("Init", "reconstract the reaction: %s + %s -> %s + %s", fParticleName[0].Data(), fParticleName[1].Data(),
        fParticleName[2].Data(), fParticleName[3].Data());
   for (Int_t iPart = 0; iPart < 4; iPart++) {
      Info("Init", "\tid=%d: mass=%lf,\tZ=%d", iPart, fParticleMass[iPart], fParticleAtomicNum[iPart]);
   }
   Info("Init", "\tdetected particle id=3: %s, mass=%lf, Z=%d", fParticleName[3].Data(), fParticleMass[3],
        fParticleAtomicNum[3]);

   for (Int_t iPart = 0; iPart < fParticleMass.size(); iPart++) {
      fParticleMass[iPart] *= TArtAtomicMassTable::kAtomicMassUnit; // amu to MeV
   }

   Info("Init", "\tQ-value: %lf MeV", (fParticleMass[0] + fParticleMass[1]) - (fParticleMass[2] + fParticleMass[3]));

   Info("Init", "preparing %s->%s and %s->%s energy loss table", fParticleName[0].Data(), fTargetName.Data(),
        fParticleName[3].Data(), fTargetName.Data());
   fElossTable = new SRIMData();
   fElossBeam = fElossTable->GetTable(fParticleName[0], fTargetName);
   fElossDetectParticle = fElossTable->GetTable(fParticleName[3], fTargetName);
   fMaxLength = fElossBeam->GetRange(fElossBeam->GetOldE(fInitialBeamEnergy, kWindowUncertainty));

   fOutData = new TClonesArray("art::TReactionInfo");
   fOutData->SetName(fOutputColName);
   col->Add(fOutputColName, fOutData, fOutputIsTransparent);

   gRandom->SetSeed(time(NULL));
}

void TTGTIKProcessor::Process()
{
   fOutData->Clear("C");

   Int_t nData = (*fInData)->GetEntriesFast();
   if (nData == 0)
      return;
   else if (nData != 1) {
      std::cerr << "warning: the input " << fInputColName << " entry number is not 1, but " << nData << std::endl;
   }

   Int_t nTrackData = (*fInTrackData)->GetEntriesFast();
   if (nTrackData == 0)
      return;
   if (nTrackData != 1) {
      std::cerr << "warning: the input " << fInputTrackColName << " entry number is not 1, but " << nTrackData
                << std::endl;
   }

   const TDataObject *const inData = static_cast<TDataObject *>((*fInData)->At(0));
   const TTelescopeData *const Data = dynamic_cast<const TTelescopeData *>(inData);
   const TDataObject *const inTrackData = static_cast<TDataObject *>((*fInTrackData)->At(0));
   const TTrack *const TrackData = dynamic_cast<const TTrack *>(inTrackData);

   if (Data->GetEtotal() < fEnergyThreshold)
      return;

   Double_t reac_z = GetReactionPosition(TrackData, Data);

   if (!IsValid(reac_z))
      return;

   TReactionInfo *outData = static_cast<TReactionInfo *>(fOutData->ConstructedAt(0));
   outData->SetID(0);
   outData->SetXYZ(TrackData->GetX(reac_z), TrackData->GetY(reac_z), reac_z);
   outData->SetEnergy(GetEcmFromBeam(reac_z, TrackData));
}

Double_t TTGTIKProcessor::GetReactionPosition(const TTrack *track, const TTelescopeData *data)
{
   // return newton(track, data);
   return bisection(track, data);
}

Double_t TTGTIKProcessor::newton(const TTrack *track, const TTelescopeData *data)
{
   return kInvalidD;
}

Double_t TTGTIKProcessor::bisection(const TTrack *track, const TTelescopeData *data)
{
   Double_t flow = TargetFunction(-kWindowUncertainty, track, data);
   Double_t fhigh = TargetFunction(fMaxLength, track, data);
   Double_t low = -kWindowUncertainty;
   Double_t high = fMaxLength;
   Double_t ftmp = TargetFunction(-kWindowUncertainty, track, data);
   Int_t sign = 1;
   Bool_t isFirst = true;

   for (Int_t i = 0; i < 10; i++) {
      Double_t tmpz = -kWindowUncertainty + (Double_t)i * (fMaxLength + kWindowUncertainty);
      Double_t f = TargetFunction(tmpz, track, data);
      if (!IsValid(f))
         continue;

      if (isFirst) {
         sign = f >= 0 ? 1 : -1;
         isFirst = false;
      }

      if (sign == f >= 0 ? 1 : -1) {
         ftmp = f;
      } else {
         flow = ftmp;
         fhigh = f;
         low = -kWindowUncertainty + (Double_t)(i - 1) * (fMaxLength + kWindowUncertainty);
         high = tmpz;
         break;
      }
   }
   if (!IsValid(flow) || !IsValid(fhigh))
      return kInvalidD;
   if (flow * fhigh > 0) {
      std::cerr << "[Warning in bisect] f(low) * f(high) > 0, f(low): " << flow << ", f(high): " << fhigh << std::endl;
      return kInvalidD;
   }

   Double_t left = low;
   Double_t right = high;

   Double_t middle = 0.0;
   Int_t itr = 0;
   while (TMath::Abs(right - left) > kEpsilon) {
      middle = (left + right) / 2.0;

      Double_t fleft = TargetFunction(left, track, data);
      Double_t fmiddle = TargetFunction(middle, track, data);
      if (!IsValid(fleft) || !IsValid(fmiddle))
         return kInvalidD;

      if (fleft * fmiddle < 0) {
         right = middle;
      } else {
         left = middle;
      }

      itr++;
      if (itr == kMaxIteration) {
         std::cerr << "[bisection] not converge!" << std::endl;
         return kInvalidD;
      }
   }
   return middle;
}

Double_t TTGTIKProcessor::TargetFunction(Double_t z, const TTrack *track, const TTelescopeData *data)
{
   Double_t Ecm_beam = GetEcmFromBeam(z, track);
   Double_t Ecm_detect = GetEcmFromDetectParticle(z, track, data);
   if (!IsValid(Ecm_beam)) {
      // std::cerr << "Ecm_beam is invalid" << std::endl;
      return kInvalidD;
   }
   if (!IsValid(Ecm_detect)) {
      // std::cerr << "Ecm_detect is invalid" << std::endl;
      return kInvalidD;
   }
   return Ecm_beam - Ecm_detect;
}

// kinematics
Double_t TTGTIKProcessor::GetEcmFromBeam(Double_t z, const TTrack *track)
{
   Double_t energy = 0.0;
   TVector3 beam_flight(track->GetX(z) - track->GetX(0), track->GetY(z) - track->GetY(0), z);
   if (z > 0) {
      energy = fElossBeam->GetNewE(fInitialBeamEnergy, beam_flight.Mag());
      if (energy < 0.01)
         return 0.0;
   } else if (z < 0) {
      energy = fElossBeam->GetOldE(fInitialBeamEnergy, beam_flight.Mag());
   }

   Double_t beta = TMath::Sqrt(1.0 - TMath::Power(fParticleMass[0] / (fParticleMass[0] + energy), 2));
   Double_t ang[2] = {track->GetA(), track->GetB()};
   Double_t norm = TMath::Sqrt(TMath::Tan(ang[0]) * TMath::Tan(ang[0]) + TMath::Tan(ang[1]) * TMath::Tan(ang[1]) + 1.0);
   Double_t beta_array[3] = {beta * TMath::Tan(ang[0]) / norm, beta * TMath::Tan(ang[1]) / norm, beta * 1.0 / norm};

   TLorentzVector beam(0., 0., 0., fParticleMass[0]);
   beam.Boost(beta_array[0], beta_array[1], beta_array[2]);

   TLorentzVector target(0., 0., 0., fParticleMass[1]);

   TLorentzVector cm_vec = beam + target;
   TVector3 beta_cm = cm_vec.BoostVector();
   beam.Boost(-beta_cm);
   target.Boost(-beta_cm);

   return (beam.E() - beam.M()) + (target.E() - target.M());
}

Double_t TTGTIKProcessor::GetEcmFromDetectParticle(Double_t z, const TTrack *track, const TTelescopeData *data)
{
   Int_t tel_id = data->GetID();
   const TParameterObject *const inPrm = static_cast<TParameterObject *>((*fDetectorPrm)->At(tel_id - 1));
   const TDetectorParameter *Prm = dynamic_cast<const TDetectorParameter *>(inPrm);
   if (!Prm) {
      std::cerr << "parameter is not found" << std::endl;
      return kInvalidD;
   }

   Int_t stripNum[2] = {Prm->GetStripNum(0), Prm->GetStripNum(1)};
   Int_t stripID[2] = {data->GetXID(), data->GetYID()};
   Double_t stripSize[2] = {Prm->GetSize(0) / (Double_t)stripNum[0], Prm->GetSize(1) / (Double_t)stripNum[1]};

   Double_t det_x, det_y;
   if (stripID[0] >= 0 && stripID[0] < stripNum[0]){
     det_x = -Prm->GetSize(0) / 2.0 + stripSize[0] * ((Double_t)stripID[0] + gRandom->Uniform());
   } else{
     det_x = -Prm->GetSize(0) / 2.0 + Prm->GetSize(0) * gRandom->Uniform();
   }

   if (stripID[1] >= 0 && stripID[1] < stripNum[1]){
     det_y = -Prm->GetSize(1) / 2.0 + stripSize[1] * ((Double_t)stripID[1] + gRandom->Uniform());
   } else{
     det_y = -Prm->GetSize(1) / 2.0 + Prm->GetSize(1) * gRandom->Uniform();
   }

   TVector3 detect_position(det_x, det_y, Prm->GetDistance());
   detect_position.RotateY(Prm->GetAngle()); // rad
   detect_position += TVector3(Prm->GetCenterRotPos(0), Prm->GetCenterRotPos(1), Prm->GetCenterRotPos(2));

   TVector3 reaction_position(track->GetX(z), track->GetY(z), z);
   TVector3 track_direction(track->GetX(1.) - track->GetX(0.), track->GetY(1.) - track->GetY(0.), 1.0);

   Double_t theta = track_direction.Angle(detect_position - reaction_position); // LAB, rad
   Double_t energy = fElossDetectParticle->GetOldE(data->GetEtotal(), (detect_position - reaction_position).Mag());

#if 0
  std::cout << "x_det: " << detect_position.X() << ", x_real: " << data->GetPosition().X() << std::endl;  
  std::cout << "y_det: " << detect_position.Y() << ", y_real: " << data->GetPosition().Y() << std::endl;  
  std::cout << "z_det: " << detect_position.Z() << ", z_real: " << data->GetPosition().Z() << std::endl;  
  std::cout << "--" << std::endl;

  for(Double_t ecm=0.01; ecm<1.0e+4; ecm *= 1.1){
    Double_t energy_L = energy + fParticleMass[3];
    Double_t energy_cm_all = ecm + fParticleMass[0] + fParticleMass[1];
    Double_t energy_4cm = (energy_cm_all - (fParticleMass[2]*fParticleMass[2] - fParticleMass[3]*fParticleMass[3])
                          /energy_cm_all)/2.0;
    Double_t energy1 = (ecm * ecm + 2.0*ecm*(fParticleMass[0]+fParticleMass[1]) 
                        + 2.0*fParticleMass[0]*fParticleMass[1])/(2.0 * fParticleMass[1]);
    Double_t beta = TMath::Sqrt(energy1 * energy1 - fParticleMass[0]*fParticleMass[0])
                    /(energy1 + fParticleMass[1]);

    Double_t arg = (energy_L - energy_4cm*TMath::Sqrt(1.0 - beta*beta))
                        /(beta * TMath::Sqrt(energy_L * energy_L - fParticleMass[3]*fParticleMass[3]));
    std::cout << "ecm: " << ecm << " , costheta: " << arg << " , theta: " << TMath::ACos(arg)*TMath::RadToDeg() << std::endl;
  }
  std::cout << "==" << std::endl;
#endif

   // kinematics (using bisection method) detected particle id=3
   // return GetEcm_kinematics(energy, theta, 0.01, 1.0e+4);

   // classic kinematics
   return GetEcm_classic_kinematics(energy, theta);
}

// refer from okawa's master thesis
Double_t TTGTIKProcessor::GetEcm_classic_kinematics(Double_t energy, Double_t theta)
{
   // unit = mass (MeV)
   Double_t alpha = (fParticleMass[1] * (fParticleMass[0] + fParticleMass[1])) / (2.0 * fParticleMass[0]);
   Double_t beta = (fParticleMass[3] * (fParticleMass[2] + fParticleMass[3])) / (2.0 * fParticleMass[2]);
   Double_t qvalue = (fParticleMass[0] + fParticleMass[1]) - (fParticleMass[2] + fParticleMass[3]);

   Double_t v4 = TMath::Sqrt(2.0 * energy / fParticleMass[3]);
   Double_t b = (beta * v4 * TMath::Cos(theta)) / (alpha - beta);
   Double_t c = (qvalue - beta * v4 * v4) / (alpha - beta);
   Double_t D = b * b - c;
   if (D < 0) {
      if (TMath::Abs(D) < 1.0e-5) {
         D = 0.0;
      } else {
         std::cout << "b^2 - c = " << D << " < 0, det_energy : " << energy << ", theta : " << theta << std::endl;
         return kInvalidD;
      }
   }

   Double_t vcm = -b + TMath::Sqrt(D);
   if (vcm < 0) {
      std::cout << "vcm < 0! : vcm = " << -b << " + " << TMath::Sqrt(D) << ", det_energy : " << energy
                << ", theta : " << theta << std::endl;
      return kInvalidD;
   }

   return alpha * vcm * vcm;
}

Double_t TTGTIKProcessor::GetEcm_kinematics(Double_t energy, Double_t theta, Double_t low_e, Double_t high_e)
{
   return kInvalidD;
}

// kinematics: assume Ecm, detected energy of particle3 => calculate lab angle
Double_t TTGTIKProcessor::GetLabAngle(Double_t energy, Double_t energy_cm)
{
   Double_t energy_L = energy + fParticleMass[3];
   Double_t energy_cm_all = energy_cm + fParticleMass[0] + fParticleMass[1];
   Double_t energy_4cm =
      (energy_cm_all - (fParticleMass[2] * fParticleMass[2] - fParticleMass[3] * fParticleMass[3]) / energy_cm_all) /
      2.0;
   Double_t energy1 = (energy_cm * energy_cm + 2.0 * energy_cm * (fParticleMass[0] + fParticleMass[1]) +
                       2.0 * fParticleMass[0] * fParticleMass[1]) /
                      (2.0 * fParticleMass[1]);
   Double_t beta = TMath::Sqrt(energy1 * energy1 - fParticleMass[0] * fParticleMass[0]) / (energy1 + fParticleMass[1]);

   Double_t arg = (energy_L - energy_4cm * TMath::Sqrt(1.0 - beta * beta)) /
                  (beta * TMath::Sqrt(energy_L * energy_L - fParticleMass[3] * fParticleMass[3]));

   return TMath::ACos(arg);
}
