// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "TTelescopeData.h"
#include "TConverterBase.h"

using art::TTelescopeData;

ClassImp(art::TTelescopeData)

TTelescopeData::TTelescopeData()
   : fX(kInvalidD), fY(kInvalidD), fZ(kInvalidD), fTX(kInvalidD), fTY(kInvalidD), fTZ(kInvalidD), fXID(kInvalidI), fYID(kInvalidI), fNE(0), fdEX(0.0), fdEY(0.0), fdEXTiming(kInvalidD), fdEYTiming(kInvalidD),
      fEvec(kMAXNPARAMETER, kInvalidD), fETimingvec(kMAXNPARAMETER, kInvalidD),
      fdE(0.0), fE(0.0), fEtotal(0.0)
{
   TDataObject::SetID(kInvalidI);
   for(Int_t i=0; i<kMAXNPARAMETER; ++i){
      fEvec[i] = kInvalidD;
      fETimingvec[i] = kInvalidD;
   }
}

TTelescopeData::~TTelescopeData()
{
}

TTelescopeData::TTelescopeData(const TTelescopeData& rhs)
   : TDataObject(rhs),
     fX(rhs.fX),
     fY(rhs.fY),
     fZ(rhs.fZ),
     fTX(rhs.fTX),
     fTY(rhs.fTY),
     fTZ(rhs.fTZ),
     fXID(rhs.fXID),
     fYID(rhs.fYID),
     fNE(rhs.fNE),
     fdEX(rhs.fdEX),
     fdEY(rhs.fdEY),
     fdEXTiming(rhs.fdEXTiming),
     fdEYTiming(rhs.fdEYTiming),
     fEvec(rhs.fEvec),
     fETimingvec(rhs.fETimingvec),
     fdE(rhs.fdE),
     fE(rhs.fE),
     fEtotal(rhs.fEtotal)
{
   for(Int_t i=0; i<kMAXNPARAMETER; ++i){
      fEvec[i] = rhs.fEvec[i];
      fETimingvec[i] = rhs.fEvec[i];
   }
}

TTelescopeData& TTelescopeData::operator=(const TTelescopeData& rhs)
{
   if (this != &rhs) {
      ((TTelescopeData&)rhs).Copy(*this);
   }
   return *this;
}

void TTelescopeData::Init(const Int_t nE)
{
   if(fNE != nE){
      fNE = nE;
      fEvec.resize(fNE);
      fETimingvec.resize(fNE);
   }
   Clear();
}


void TTelescopeData::Copy(TObject& dest) const
{
   TDataObject::Copy(dest);
   TTelescopeData &cobj = *(TTelescopeData*)&dest;

   if (fNE != cobj.fNE) {
      cobj.fNE = fNE;
      cobj.fEvec.resize(fNE);
      cobj.fETimingvec.resize(fNE);
   }

   cobj.fX = this->GetX();
   cobj.fY = this->GetY();
   cobj.fZ = this->GetZ();
   cobj.fTX = this->GetTX();
   cobj.fTY = this->GetTY();
   cobj.fTZ = this->GetTZ();
   cobj.fXID = this->GetXID();
   cobj.fYID = this->GetYID();

   cobj.fdEX = this->GetdEX();
   cobj.fdEY = this->GetdEY();
   cobj.fdEXTiming = this->GetdEXTiming();
   cobj.fdEYTiming = this->GetdEYTiming();

   std::copy(fEvec.begin(),fEvec.end(),cobj.fEvec.begin());
   std::copy(fETimingvec.begin(),fETimingvec.end(),cobj.fETimingvec.begin());
   memcpy(cobj.fEvec, fEvec, sizeof(fEvec));
   memcpy(cobj.fETimingvec, fETimingvec, sizeof(fETimingvec));

   cobj.fdE = this->GetdE();
   cobj.fE = this->GetE();
   cobj.fEtotal = this->GetEtotal();
   
}

void TTelescopeData::Clear(Option_t *opt)
{
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);
   fX = kInvalidD;
   fY = kInvalidD;
   fZ = kInvalidD;

   fTX = kInvalidD;
   fTY = kInvalidD;
   fTZ = kInvalidD;

   fXID = kInvalidI;
   fYID = kInvalidI;

   fdEX = 0.0;
   fdEY = 0.0;
   fdEXTiming = kInvalidD;
   fdEYTiming = kInvalidD;

   std::fill(fEvec.begin(),fEvec.end(),0.0);
   std::fill(fETimingvec.begin(),fETimingvec.end(),kInvalidD);
   for(Int_t i=0; i<kMAXNPARAMETER; ++i){
      fEvec[i] = 0.0;
      fETimingvec[i] = kInvalidD;
   }

   fdE     = 0.0;
   fE      = 0.0;
   fEtotal = 0.0;
}
