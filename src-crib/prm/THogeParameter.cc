#include "THogeParameter.h"

using art::THogeParameter;

ClassImp(THogeParameter)

THogeParameter::THogeParameter() : TFixedNumberParameter(2) {fParam[1] = 1.;}
THogeParameter::THogeParameter(Double_t a0, Double_t a1) :TFixedNumberParameter(2), fgain(a0), foffset(a1) {}


THogeParameter::~THogeParameter() {}

