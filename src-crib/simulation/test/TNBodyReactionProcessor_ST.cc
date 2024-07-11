/*
   @File name     : TNBodyReactionProcessor.cc
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-08-01 11:04:43
   @Last modified : 2023-08-01 11:05:20
*/
// NOTE: for constant cross section

#include "TNBodyReactionProcessor.h"
#include "TParticleInfo.h"
#include "TReactionInfo.h"

#include <TDataObject.h>
#include <constant.h>
#include <TArtAtomicMassTable.h>

#include <TRandom.h>
//#include <TRandom3.h>
#include <time.h>
#include <TClonesArray.h>
#include <TClass.h>
#include <TLorentzVector.h>

using art::TNBodyReactionProcessor;

ClassImp(TNBodyReactionProcessor);

TNBodyReactionProcessor::TNBodyReactionProcessor() : fInData(NULL), fOutData(NULL), fTargetThickness(50.0)
{
   RegisterInputCollection("InputCollection", "input branch (collection) name", fInputColName, TString("input"));
   RegisterOutputCollection("OutputCollection", "output branch (collection) name", fOutputColName, TString("emitted particles"));

   // Target thickness 
   RegisterProcessorParameter("TargetThickness", "Target mass", fTargetThickness, 50.0);
}

TNBodyReactionProcessor::~TNBodyReactionProcessor()
{
   delete fOutData;
   fOutData = NULL;
}

void TNBodyReactionProcessor::Init(TEventCollection *col)
{
   Info("Init", "\033[1mNOTE\033[m: this is constant cross section and uniform the reaction position");
   Info("Init", "%s => %s", fInputColName.Data(), fOutputColName.Data());

   fInData = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName.Data()));
   if (!fInData) {
      SetStateError(TString::Format("input not found: %s", fInputColName.Data()));
      return;
   }
   const TClass *const cl = (*fInData)->GetClass();
   if (!cl->InheritsFrom(art::TParticleInfo::Class())) {
      SetStateError("contents of input array must inherit from art::TParticleInfo");
   }

   fOutData = new TClonesArray("art::TParticleInfo");
   fOutData->SetName(fOutputColName);
   col->Add(fOutputColName, fOutData, fOutputIsTransparent);


   gRandom->SetSeed(time(NULL));
}

void TNBodyReactionProcessor::Process()
{
   fOutData->Clear("C");

   if ((*fInData)->GetEntriesFast() != 1) {
      SetStateError("input branch entry is not 1");
      return;
   }

   const TDataObject *const inData = static_cast<TDataObject *>((*fInData)->At(0));
   const TParticleInfo *const Data = dynamic_cast<const TParticleInfo *>(inData);


   //decay position on target
   Double_t reac_posx = Data->GetTrack().GetX(reac_posz);
   Double_t reac_posy = Data->GetTrack().GetY(reac_posz);
   Double_t reac_posz = fTargetThickness * gRandom->Uniform();


   //beam velocity
   TVector3 beam_vel= Data->GetLorentzVector().Vect();

   //for solid angle calculation
   Double_t costheta = 2.0 * (gRandom->Uniform())-1.0; 
   Double_t theta = acos(costheta) / deg2rad;
   Double_t phi = 2.0 * TMath::Pi() * (gRandom->Uniform()) * TMath::RadToDeg();
   Double_t dir_decay_u = sin(theta * TMath::DegToRad) * cos(phi * TMath::DegToRad());
   Double_t dir_decay_v = sin(theta * TMath::DegToRad) * sin(phi * TMath::DegToRad());
   Double_t dir_decay_w = cos(theta * TMath::DegToRad);
   TVector3 Decay(dir_decay_u, dir_decay_v, dir_decay_w);
   Double_t emitAngle = beam_vel.Angle(Decay) * TMath::RadToDeg();

   //save branch
   TParticleInfo *outData = static_cast<TParticleInfo *>(fOutData->ConstructedAt(iPart));
   outData->SetTrack(reac_posx, reac_posy, reac_posz, TMath::ATan(Decay.Px() / Decay.Pz()),
                        TMath::ATan(Decay.Py() / Decay.Pz()));
   outData->SetCurrentZ(reac_posz);
   outData->SetTheta(emitAngle);
}
