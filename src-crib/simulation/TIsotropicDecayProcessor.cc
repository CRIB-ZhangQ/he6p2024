/*
   @File name     : TNBodyReactionProcessor.cc
   @description   :
   @Author        : Zhang <zhangq22@cns.s.u-tokyo.ac.jp>
   @Created date  : 2024-05-06 18:56:00
   @Last modified : 2024-05-06 18:56:00
*/
// NOTE: for isotropic decay 

#include "TIsotropicDecayProcessor.h"
#include "TReactionInfo.h"
#include "TParticleInfo.h"

#include <TDataObject.h>
#include <constant.h>
#include <TArtAtomicMassTable.h>

#include <TRandom.h>
#include <time.h>
#include <TClonesArray.h>
#include <TClass.h>
#include <TLorentzVector.h>

using art::TIsotropicDecayProcessor;

ClassImp(TIsotropicDecayProcessor);

TIsotropicDecayProcessor::TIsotropicDecayProcessor() : fInData(NULL), fOutData(NULL)
{
   RegisterInputCollection("InputCollection", "input branch (collection) name", fInputColName, TString("input"));
   RegisterOutputCollection("OutputCollection", "output branch (collection) name", fOutputColName,
                            TString("reaction_particles"));

   // Target information
   RegisterProcessorParameter("BeamMass", "beam mass", fBeamMass, 0.);
   RegisterProcessorParameter("TargetMass", "Target mass", fTargetMass, 0.);
   RegisterProcessorParameter("TargetThickness", "Target mass", fTargetThickness, 0.);

}

TIsotropicDecayProcessor::~TIsotropicDecayProcessor()
{
   delete fOutData;
   fOutData = NULL;
}

void TIsotropicDecayProcessor::Init(TEventCollection *col)
{
   Info("Init", "\033[1mNOTE\033[m: the isotope will decay isotropicically");
   Info("Init", "%s, reaction info => %s", fInputColName.Data(),
        fOutputColName.Data());


   fInData = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName.Data()));
   if (!fInData) {
      SetStateError(TString::Format("input not found: %s", fInputColName.Data()));
      return;
   }
   const TClass *const cl = (*fInData)->GetClass();
   if (!cl->InheritsFrom(art::TParticleInfo::Class())) {
      SetStateError("contents of input array must inherit from art::TParticleInfo");
   }

   fOutData = new TClonesArray("art::TReactionInfo");
   fOutData->SetName(fOutputColName);
   col->Add(fOutputColName, fOutData, fOutputIsTransparent);

   fTargetMass *= TArtAtomicMassTable::kAtomicMassUnit;     // amu to MeV
   fBeamMass *= TArtAtomicMassTable::kAtomicMassUnit;     // amu to MeV

   gRandom->SetSeed(time(NULL));
}

void TIsotropicDecayProcessor::Process()
{
   fOutData->Clear("C");

   if ((*fInData)->GetEntriesFast() != 1) {
      SetStateError("input branch entry is not 1");
      return;
   }

   const TDataObject *const inData = static_cast<TDataObject *>((*fInData)->At(0));
   const TParticleInfo *const Data = dynamic_cast<const TParticleInfo *>(inData);

   TLorentzVector target_vec(0., 0., 0., fTargetMass);
   TLorentzVector beam_vec = Data->GetLorentzVector();
   Double_t beam_energy = Data->GetEnergy();

   Double_t reac_posz;
   // the axia is at the target center
   reac_posz = fTargetThickness * (gRandom->Uniform() -0.5);
   Double_t reac_posx = Data->GetTrack().GetX(reac_posz);
   Double_t reac_posy = Data->GetTrack().GetY(reac_posz);

   Double_t thickness =
      TVector3(reac_posx - Data->GetTrack().GetX(0.), reac_posy - Data->GetTrack().GetY(0.), reac_posz).Mag();


   //solid calculation
   //beam velocity
   TVector3 beam_vel= beam_vec.Vect();
   // need to change MeV to GeV
   fTargetMass = fTargetMass*0.001;

   //for solid angle calculation
   Double_t costheta = 2.0 * (gRandom->Uniform())-1.0; 
   Double_t theta = acos(costheta) * TMath::RadToDeg();
   Double_t phi = 2.0 * TMath::Pi() * (gRandom->Uniform()) * TMath::RadToDeg();
   Double_t decay_u = sin(theta * TMath::DegToRad()) * cos(phi * TMath::DegToRad());
   Double_t decay_v = sin(theta * TMath::DegToRad()) * sin(phi * TMath::DegToRad());
   Double_t decay_w = cos(theta * TMath::DegToRad());
   TVector3 Decay_vel(decay_u, decay_v, decay_w);
   Double_t emitAngle = beam_vel.Angle(Decay_vel) * TMath::RadToDeg();
   //std::cout<<"emitAngle = "<<emitAngle<<std::endl;


   TReactionInfo *outData = static_cast<TReactionInfo *>(fOutData->ConstructedAt(0));
   outData->SetXYZ(reac_posx, reac_posy, reac_posz);

   outData->SetLorentzVector(decay_u, decay_v, decay_w,fTargetMass);

   outData->SetTheta(emitAngle);
   outData->SetPhi(phi);
   outData->SetXYZ(reac_posx, reac_posy, reac_posz);
   outData->SetEnergy(beam_energy);
}

