/*
   @File name     : TParticleInfo.cc
   @description   : particle information class
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-09 17:56:50
   @Last modified : 2023-08-01 11:06:09
*/

#include "TParticleInfo.h"
#include <constant.h>

using art::TParticleInfo;

ClassImp(art::TParticleInfo);

TParticleInfo::TParticleInfo()
   : fMassNumber(kInvalidI), fAtomicNumber(kInvalidI), fCharge(kInvalidI), fEnergy(kInvalidD), fCurrentZ(kInvalidD)
{
   TDataObject::SetID(kInvalidI);
   SetTrack(0., 0., 0., 0., 0.);
   fVec.SetXYZT(0., 0., 0., 0.);
}

TParticleInfo::~TParticleInfo() {}

TParticleInfo::TParticleInfo(const TParticleInfo &rhs)
   : TDataObject(rhs),
     fTrack(rhs.fTrack),
     fVec(rhs.fVec),
     fMassNumber(rhs.fMassNumber),
     fAtomicNumber(rhs.fAtomicNumber),
     fCharge(rhs.fCharge),
     fEnergy(rhs.fEnergy),
     fCurrentZ(rhs.fCurrentZ)
{
}

TParticleInfo &TParticleInfo::operator=(const TParticleInfo &rhs)
{
   if (this != &rhs) {
      ((TParticleInfo &)rhs).Copy(*this);
   }
   return *this;
}

void TParticleInfo::Copy(TObject &dest) const
{
   TDataObject::Copy(dest);
   TParticleInfo &cobj = *(TParticleInfo *)&dest;

   cobj.fMassNumber = this->GetMassNumber();
   cobj.fAtomicNumber = this->GetAtomicNumber();
   cobj.fCharge = this->GetCharge();
   cobj.fEnergy = this->GetEnergy();
   cobj.fCurrentZ = this->GetCurrentZ();
   cobj.fTrack = this->GetTrack();
   cobj.fVec = this->GetLorentzVector();
}

void TParticleInfo::Clear(Option_t *opt)
{
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);

   fMassNumber = kInvalidI;
   fAtomicNumber = kInvalidI;
   fCharge = kInvalidI;
   fEnergy = kInvalidD;
   fCurrentZ = kInvalidD;

   SetTrack(0., 0., 0., 0., 0.);
   fVec.SetXYZT(0., 0., 0., 0.);
}
