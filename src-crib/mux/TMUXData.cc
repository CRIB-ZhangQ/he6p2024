// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "TMUXData.h"
#include "TConverterBase.h"

using art::TMUXData;

ClassImp(art::TMUXData)

TMUXData::TMUXData()
   : fE1(kInvalidD), fE2(kInvalidD), fP1(kInvalidD), fP2(kInvalidD), fTiming(kInvalidD)
{
   TDataObject::SetID(kInvalidI);
}

TMUXData::~TMUXData() {}

TMUXData::TMUXData(const TMUXData& rhs)
   : TDataObject(rhs),
     fE1(rhs.fE1),
     fE2(rhs.fE2),
     fP1(rhs.fP1),
     fP2(rhs.fP2),
     fTiming(rhs.fTiming)
{}

TMUXData& TMUXData::operator=(const TMUXData& rhs)
{
   if (this != &rhs) {
      ((TMUXData&)rhs).Copy(*this);
   }
   return *this;
}

void TMUXData::Copy(TObject& dest) const
{
   TDataObject::Copy(dest);
   TMUXData &cobj = *(TMUXData*)&dest;
   cobj.fE1 = this->GetE1();
   cobj.fE2 = this->GetE2();
   cobj.fP1 = this->GetP1();
   cobj.fP2 = this->GetP2();
   cobj.fTiming = this->GetTrig();
}

void TMUXData::Clear(Option_t *opt)
{
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);
   fE1 = kInvalidD;
   fE2 = kInvalidD;
   fP1 = kInvalidD;
   fP2 = kInvalidD;
   fTiming = kInvalidD;
}
