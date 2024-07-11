// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#ifndef TMUXPOSITIONCONVERTER_H
#define TMUXPOSITIONCONVERTER_H

#include "TFixedNumberParameter.h"
//#include "../share/src/converter/TFixedNumberParameter.h"

namespace art{
   class TMUXPositionConverter;
}

class TString;

class art::TMUXPositionConverter : public TFixedNumberParameter {
public:
   // Default constructor
   TMUXPositionConverter() : TFixedNumberParameter(2) { fParam[1] = 1.;}
   TMUXPositionConverter(Double_t a0, Double_t a1)
      : TFixedNumberParameter(2) { Set(a0,a1); }

   virtual ~TMUXPositionConverter();

   virtual void Set(Double_t a0, Double_t a1) {fParam[0] = a0; fParam[1] = a1;}
   virtual Double_t Convert(Double_t val) const;
   virtual void Get(Double_t &a0, Double_t &a1) const  {a0 = fParam[0];a1 = fParam[1];}

   virtual void Print(Option_t* opt = "") const;

   ClassDef(TMUXPositionConverter,0) // converter with first-degree polynomial
};

#endif // TMUXPOSITIONCONVERTER_H
