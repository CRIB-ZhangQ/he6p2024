/*
   @File name     : TBranchCopyProcessor.cc
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 16:06:15
   @Last modified : 2023-06-13 16:06:15
*/

#include "TBranchCopyProcessor.h"

#include <TClonesArray.h>
#include <TClass.h>

#include "TDataObject.h"
#include "constant.h"

using art::TBranchCopyProcessor;

ClassImp(TBranchCopyProcessor);

TBranchCopyProcessor::TBranchCopyProcessor() : fInData(NULL), fOutData(NULL)
{
   RegisterInputCollection("InputCollection", "name of the origin branch", fInputColName, TString("origin"));
   RegisterOutputCollection("OutputCollection", "output collection name", fOutputColName, TString("copy"));
}

TBranchCopyProcessor::~TBranchCopyProcessor()
{
   delete fOutData;
   fOutData = NULL;
}

void TBranchCopyProcessor::Init(TEventCollection *col)
{
   Info("Init", "%s => %s copy", fInputColName.Data(), fOutputColName.Data());

   fInData = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName.Data()));
   if (!fInData) {
      SetStateError(TString::Format("input not found: %s", fInputColName.Data()));
      return;
   }

   const TClass *const cl = (*fInData)->GetClass();
   fOutData = new TClonesArray(cl);
   fOutData->SetName(fOutputColName);
   col->Add(fOutputColName, fOutData, fOutputIsTransparent);
}

void TBranchCopyProcessor::Process()
{
   fOutData->Clear("C");

   for (Int_t iData = 0; iData < (*fInData)->GetEntriesFast(); iData++) {
      const TDataObject *const inData = static_cast<TDataObject *>((*fInData)->At(iData));
      TDataObject *outData = static_cast<TDataObject *>(fOutData->ConstructedAt(iData));

      inData->Copy(*outData);
   }
}
