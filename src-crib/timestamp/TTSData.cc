// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "TTSData.h"
#include "TConverterBase.h"

using art::TTSData;

ClassImp(art::TTSData)

TTSData::TTSData()
   : fTS(kInvalidI), fTScal(kInvalidD)
{
   TDataObject::SetID(kInvalidI);
}

TTSData::~TTSData() {}

TTSData::TTSData(const TTSData& rhs)
   : TDataObject(rhs),
     fTS(rhs.fTS),
     fTScal(rhs.fTScal)
{}

TTSData& TTSData::operator=(const TTSData& rhs)
{
   if (this != &rhs) {
      ((TTSData&)rhs).Copy(*this);
   }
   return *this;
}

void TTSData::Copy(TObject& dest) const
{
   TDataObject::Copy(dest);
   TTSData &cobj = *(TTSData*)&dest;
   cobj.fTS = this->GetTS();
   cobj.fTScal  = this->GetTScal();
}

void TTSData::Clear(Option_t *opt)
{
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);
   fTS = kInvalidI;
   fTScal = kInvalidD;
}
