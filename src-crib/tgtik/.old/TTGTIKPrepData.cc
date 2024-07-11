// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "TTGTIKPrepData.h"
#include "TConverterBase.h"

using art::TTGTIKPrepData;

ClassImp(art::TTGTIKPrepData)

TTGTIKPrepData::TTGTIKPrepData()
   : fReac_flag(kInvalidI), fEtotal(kInvalidD), 
     fWinx(kInvalidD), fWiny(kInvalidD), fWinz(kInvalidD),
     fDetx(kInvalidD), fDety(kInvalidD), fDetz(kInvalidD)
{
   TDataObject::SetID(kInvalidI);
}

TTGTIKPrepData::~TTGTIKPrepData() {}

TTGTIKPrepData::TTGTIKPrepData(const TTGTIKPrepData& rhs)
   : TDataObject(rhs),
     fReac_flag(rhs.fReac_flag),
     fEtotal(rhs.fEtotal),
     fWinx(rhs.fWinx),
     fWiny(rhs.fWiny),
     fWinz(rhs.fWinz),
     fDetx(rhs.fDetx),
     fDety(rhs.fDety),
     fDetz(rhs.fDetz)
{}

TTGTIKPrepData& TTGTIKPrepData::operator=(const TTGTIKPrepData& rhs)
{
   if (this != &rhs) {
      ((TTGTIKPrepData&)rhs).Copy(*this);
   }
   return *this;
}

void TTGTIKPrepData::Copy(TObject& dest) const
{
   TDataObject::Copy(dest);
   TTGTIKPrepData &cobj = *(TTGTIKPrepData*)&dest;

   cobj.fReac_flag = this->GetReac_flag();
   cobj.fEtotal = this->GetEtotal();
   cobj.fWinx = this->GetWinx();
   cobj.fWiny = this->GetWiny();
   cobj.fWinz = this->GetWinz();
   cobj.fDetx = this->GetDetx();
   cobj.fDety = this->GetDety();
   cobj.fDetz = this->GetDetz();
}

void TTGTIKPrepData::Clear(Option_t *opt)
{
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);

   fReac_flag = kInvalidI;
   fEtotal = kInvalidD;
   fWinx = kInvalidD;
   fWiny = kInvalidD;
   fWinz = kInvalidD;
   fDetx = kInvalidD;
   fDety = kInvalidD;
   fDetz = kInvalidD;
}
