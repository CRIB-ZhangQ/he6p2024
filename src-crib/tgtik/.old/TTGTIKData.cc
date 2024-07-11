// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "TTGTIKData.h"
#include "TConverterBase.h"

using art::TTGTIKData;

ClassImp(art::TTGTIKData)

TTGTIKData::TTGTIKData()
   : fFlag(kInvalidI),
     fReacZ(kInvalidD), fEcm(kInvalidD), fThetaL(kInvalidD), fThetaCM(kInvalidD)
{
   TDataObject::SetID(kInvalidI);
}

TTGTIKData::~TTGTIKData() {}

TTGTIKData::TTGTIKData(const TTGTIKData& rhs)
   : TDataObject(rhs),
     fFlag(rhs.fFlag),
     fReacZ(rhs.fReacZ),
     fEcm(rhs.fEcm),
     fThetaL(rhs.fThetaL),
     fThetaCM(rhs.fThetaCM)
{}

TTGTIKData& TTGTIKData::operator=(const TTGTIKData& rhs)
{
   if (this != &rhs) {
      ((TTGTIKData&)rhs).Copy(*this);
   }
   return *this;
}

void TTGTIKData::Copy(TObject& dest) const
{
   TDataObject::Copy(dest);
   TTGTIKData &cobj = *(TTGTIKData*)&dest;

   cobj.fFlag = this->GetFlag();
   cobj.fReacZ = this->GetReacZ();
   cobj.fEcm = this->GetEcm();
   cobj.fThetaL = this->GetThetaL();
   cobj.fThetaCM = this->GetThetaCM();
}

void TTGTIKData::Clear(Option_t *opt)
{
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);

   fFlag = kInvalidI;
   fReacZ = kInvalidD;
   fEcm = kInvalidD;
   fThetaL = kInvalidD;
   fThetaCM = kInvalidD;
}
