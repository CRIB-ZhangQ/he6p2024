// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "TMUXPositionValidator.h"
#include "TMUXData.h"
#include <TClonesArray.h>
//#include <ITiming.h>
#include <TDataObject.h>

using art::TMUXPositionValidator;

ClassImp(TMUXPositionValidator)

TMUXPositionValidator::TMUXPositionValidator()
: fInput(NULL), fOutput(NULL)
{
   RegisterInputCollection("InputCollection","Input collection",
                           fInputName,TString("input"));
   RegisterOutputCollection("OutputCollection","Output collection",fOutputName,TString("validated"));

   const FloatVec_t range(3,0.);
   RegisterProcessorParameter("ValidPositionRange","[min,max,offset] => Pmin = offset + min etc. ignored if min == max",
                              fValidPositionRange,range);
   

}

TMUXPositionValidator::~TMUXPositionValidator()
{
  delete fOutput;
  fOutput = NULL;
}

TMUXPositionValidator::TMUXPositionValidator(const TMUXPositionValidator& rhs)
{
}

TMUXPositionValidator& TMUXPositionValidator::operator=(const TMUXPositionValidator& rhs)
{
   if (this != &rhs) {
   }
   return *this;
}


void TMUXPositionValidator::Init(TEventCollection *col)
{
   fInput = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fInputName.Data()));
   if (!fInput) {
      SetStateError(TString::Format("input not found: %s",fInputName.Data()));
      return;
   }

   if (fValidPositionRange[0] > fValidPositionRange[1]) {
      SetStateError("Position range : min > max");
      return;
   }
   fValidPositionMin = fValidPositionRange[2] + fValidPositionRange[0];
   fValidPositionMax = fValidPositionRange[2] + fValidPositionRange[1];
   
   const TClass* inClass = (*fInput)->GetClass();
   if (!inClass->InheritsFrom(TDataObject::Class())) {
      SetStateError(Form("Input class '%s' does not inherits from TDataObject",inClass->GetName()));
      return;
   }
   fOutput = new TClonesArray(inClass);
   col->Add(fOutputName,fOutput,fOutputIsTransparent);
}

void TMUXPositionValidator::Process()
{
   fOutput->Clear("C");
   Int_t nHits = (*fInput)->GetEntriesFast();
   for (Int_t iHit = 0; iHit != nHits; ++iHit) {
      const TMUXData *const posData = dynamic_cast<const TMUXData*>((*fInput)->At(iHit));
      const TDataObject *const data = static_cast<TDataObject*>((*fInput)->At(iHit));
      if (!posData) continue;
      Double_t pos = posData->GetP1();
      if (pos < fValidPositionMin || fValidPositionMax < pos) {
         continue;
      }
      TObject *const outData = fOutput->ConstructedAt(fOutput->GetEntriesFast());
      data->Copy(*outData);
   }
}
   