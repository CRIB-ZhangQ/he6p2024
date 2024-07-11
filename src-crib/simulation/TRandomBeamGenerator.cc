/*
   @File name     : TRandomBeamGenerator.cc
   @description   : position and angle random beam generator
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-09 15:57:01
   @Last modified : 2023-08-01 11:06:31
*/

#include "TRandomBeamGenerator.h"
#include <TClonesArray.h>
#include <TMath.h>
#include <TRandom.h>
#include <TArtAtomicMassTable.h>

#include "TParticleInfo.h"

using art::TRandomBeamGenerator;

ClassImp(TRandomBeamGenerator);

TRandomBeamGenerator::TRandomBeamGenerator() : fOutData(NULL), fMass(0.0)
{
   RegisterOutputCollection("OutputCollection", "simulation result collection", fOutputColName, TString("result"));
   RegisterOutputCollection("OutputTrackCollection", "simulation tracking information", fOutputTrackColName,
                            TString("track"));

   RegisterProcessorParameter("MassNum", "beam mass number", fMassNum, 0);
   RegisterProcessorParameter("AtomicNum", "beam atomic number", fAtmNum, 0);
   RegisterProcessorParameter("ChargeNum", "beam charge number", fChargeNum, 0);
   RegisterProcessorParameter("Mass", "beam charge number", fMass, 0.);
   RegisterProcessorParameter("IniEnergy", "beam energy (MeV)", fBeamEnergy, 100.0);

   RegisterProcessorParameter("Xsigma", "dispersion of X position (mm)", fXsigma, 1.0);
   RegisterProcessorParameter("Ysigma", "dispersion of Y position (mm)", fYsigma, 1.0);
   RegisterProcessorParameter("Asigma", "dispersion of A angle (deg)", fAsigma, 1.0);
   RegisterProcessorParameter("Bsigma", "dispersion of B angle (deg)", fBsigma, 1.0);
   RegisterProcessorParameter("Esigma", "dispersion of beam energy (MeV)", fEsigma, 1.0);
}

TRandomBeamGenerator::~TRandomBeamGenerator()
{
   delete fOutData;
   fOutData = NULL;
   delete fOutTrackData;
   fOutTrackData = NULL;
}

void TRandomBeamGenerator::Init(TEventCollection *col)
{
   Info("Init", "making => %s, %s", fOutputColName.Data(), fOutputTrackColName.Data());

   Double_t deg2rad = TMath::Pi() / 180.;
   fAsigma *= deg2rad;
   fBsigma *= deg2rad;

   fMass *= TArtAtomicMassTable::kAtomicMassUnit; // amu to MeV

   fOutData = new TClonesArray("art::TParticleInfo");
   fOutData->SetName(fOutputColName);
   col->Add(fOutputColName, fOutData, fOutputIsTransparent);

   fOutTrackData = new TClonesArray("art::TTrack");
   fOutTrackData->SetName(fOutputTrackColName);
   col->Add(fOutputTrackColName, fOutTrackData, fOutputIsTransparent);

   gRandom->SetSeed(time(NULL));
}

void TRandomBeamGenerator::Process()
{
   fOutData->Clear("C");

   TParticleInfo *outData = static_cast<TParticleInfo *>(fOutData->ConstructedAt(0));
   outData->SetMassNumber(fMassNum);
   outData->SetAtomicNumber(fAtmNum);
   outData->SetCharge(fChargeNum);

   Double_t posx = gRandom->Gaus(0., fXsigma);
   Double_t posy = gRandom->Gaus(0., fYsigma);
   Double_t angx = gRandom->Gaus(0., fAsigma);
   Double_t angy = gRandom->Gaus(0., fBsigma);
   Double_t energy = gRandom->Gaus(fBeamEnergy, fEsigma);

   Double_t beta = TMath::Sqrt(1.0 - TMath::Power(fMass / (fMass + energy), 2));                    // kinematics
   Double_t norm =
      TMath::Sqrt(TMath::Tan(angx) * TMath::Tan(angx) + TMath::Tan(angy) * TMath::Tan(angy) + 1.0); // kinematics
   Double_t beta_x = beta * TMath::Tan(angx) / norm;
   Double_t beta_y = beta * TMath::Tan(angy) / norm;
   Double_t beta_z = beta * 1.0 / norm;

   TLorentzVector beam(0., 0., 0., fMass);
   beam.Boost(beta_x, beta_y, beta_z);

   outData->SetID(0);
   outData->SetLorentzVector(beam);
   outData->SetEnergy(energy);
   outData->SetCurrentZ(0.);
   outData->SetTrack(posx, posy, 0., angx, angy);

   TTrack *outTrackData = static_cast<TTrack *>(fOutTrackData->ConstructedAt(0));
   outTrackData->SetID(0);
   outTrackData->SetPos(posx, posy, 0.);
   outTrackData->SetAngle(angx, angy);
}
