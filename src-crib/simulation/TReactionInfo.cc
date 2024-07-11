/*
   @File name     : TReactionInfo.cc
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-08-01 11:06:53
   @Last modified : 2023-08-01 11:07:15
*/

#include "TReactionInfo.h"
#include <TConverterBase.h>
#include <constant.h>

using art::TReactionInfo;

ClassImp(art::TReactionInfo);

TReactionInfo::TReactionInfo()
   : fEnergy(kInvalidD),
     fTheta(kInvalidD),
     fPhi(kInvalidD),
     fX(kInvalidD),
     fY(kInvalidD),
     fZ(kInvalidD),
     fExcitedID(kInvalidI),
     fTheta_SA(kInvalidD)
{
   TDataObject::SetID(kInvalidI);
   SetTrack(0., 0., 0., 0., 0.);
   fVec.SetXYZT(0., 0., 0., 0.);
}

TReactionInfo::~TReactionInfo() {}

TReactionInfo::TReactionInfo(const TReactionInfo &rhs)
   : TDataObject(rhs),
     fTrack(rhs.fTrack),
     fVec(rhs.fVec),
     fEnergy(rhs.fEnergy),
     fTheta(rhs.fTheta),
     fPhi(rhs.fPhi),
     fX(rhs.fX),
     fY(rhs.fY),
     fZ(rhs.fZ),
     fExcitedID(rhs.fExcitedID),
     fTheta_SA(rhs.fTheta_SA)
{
}

TReactionInfo &TReactionInfo::operator=(const TReactionInfo &rhs)
{
   if (this != &rhs) {
      ((TReactionInfo &)rhs).Copy(*this);
   }
   return *this;
}

void TReactionInfo::Copy(TObject &dest) const
{
   TDataObject::Copy(dest);
   TReactionInfo &cobj = *(TReactionInfo *)&dest;
   cobj.fEnergy = this->GetEnergy();
   cobj.fTheta = this->GetTheta();
   cobj.fPhi = this->GetPhi();
   cobj.fX = this->GetX();
   cobj.fY = this->GetY();
   cobj.fZ = this->GetZ();
   cobj.fExcitedID = this->GetExID();
   cobj.fTheta_SA = this->GetTheta_SA();
   cobj.fTrack = this->GetTrack();
   cobj.fVec = this->GetLorentzVector();
}

void TReactionInfo::Clear(Option_t *opt)
{
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);
   fEnergy = kInvalidD;
   fTheta = kInvalidD;
   fPhi = kInvalidD;
   fX = kInvalidD;
   fY = kInvalidD;
   fZ = kInvalidD;
   fExcitedID = kInvalidI;
   fTheta_SA = kInvalidD;

   SetTrack(0., 0., 0., 0., 0.);
   fVec.SetXYZT(0., 0., 0., 0.);
}
