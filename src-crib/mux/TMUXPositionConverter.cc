// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "TMUXPositionConverter.h"

#include <cstdio>
#include <iostream>

#include <TROOT.h>
#include <TString.h>
#include <TClass.h>
#include "constant.h"

using art::TMUXPositionConverter;

ClassImp(art::TMUXPositionConverter)

TMUXPositionConverter::~TMUXPositionConverter() {
}

Double_t TMUXPositionConverter::Convert(const Double_t val) const
{
   if(fParam[0] < val && val < fParam[1]){ return 1; }
   else{return kInvalidI; }
}

void TMUXPositionConverter::Print(Option_t *) const {
   const TString indent(' ', gROOT->GetDirLevel());
   printf("OBJ: %s\t%s\n", IsA()->GetName(), GetName());
   printf("%s f(x) = %lf + %lf * x\n", indent.Data(), fParam[0], fParam[1]);
}
