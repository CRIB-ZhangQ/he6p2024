#ifndef THOGEPARAMETER_H
#define THOGEPARAMETER_H

#include "TFixedNumberParameter.h"

namespace art {
	class THogeParameter;
}

class art::THogeParameter : public TFixedNumberParameter{
public:
	THogeParameter();
	THogeParameter(Double_t a0, Double_t a1);
   

	virtual ~THogeParameter();

  //virtual void Set(Double_t a0, Double_t a1) {fParam[0]= a0; fParam[1] = a1;}
  //virtual Double_t Convert(Double_t val) const {return fParam[0] + fParam[1] * val;}
	virtual Double_t GetGain() const { return fParam[0];}
	virtual Double_t GetOffSet() const { return fParam[1];}
protected:
  Double_t fgain;
  Double_t foffset;

  ClassDef(THogeParameter, 0)
};

#endif
