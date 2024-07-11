// template .cc Processor file for artemis
// this is example file that create a new branch from the created branch
// (not mapping processor)
// example) input and output Data type is TTimingChargeData

#include "TTemplateProcessor.h"

#include "TDataObject.h"
#include "TTimingChargeData.h"
#include "constant.h"

#include <TClonesArray.h>
#include <TClass.h>

using art::TTemplateProcessor;

ClassImp(TTemplateProcessor);

TTemplateProcessor::TTemplateProcessor()
    : fInData(NULL), fOutData(NULL)
{
    RegisterInputCollection("InputCollection", "input branch (collection) name",
                            fInputColName, TString("input"));
    RegisterOutputCollection("OutputCollection", "output branch (collection) name",
                             fOutputColName, TString("simple"));

    // input parameter
    RegisterProcessorParameter("Parameter", "input parameter from steering file",
                               fParameter, 0.);
}

TTemplateProcessor::~TTemplateProcessor()
{
    delete fOutData;
    fOutData = NULL;
}

void TTemplateProcessor::Init(TEventCollection *col)
{
    Info("Init", "%s => %s",
         fInputColName.Data(), fOutputColName.Data());

    fInData = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName.Data()));
    if (!fInData)
    {
        SetStateError(TString::Format("input not found: %s", fInputColName.Data()));
        return;
    }
    const TClass *const cl = (*fInData)->GetClass();
    if (!cl->InheritsFrom(art::TTimingChargeData::Class()))
    {
        SetStateError("contents of input array must inherit from art::TTimingChargeData");
    }

    fOutData = new TClonesArray("art::TTelescopeData");
    fOutData->SetName(fOutputColName);
    col->Add(fOutputColName, fOutData, fOutputIsTransparent);
}

void TTemplateProcessor::Process()
{
    fOutData->Clear("C");

    // get the value from input branch
    for (Int_t iData = 0; iData < (*fInData)->GetEntriesFast(); ++iData)
    {
        const TDataObject *const inData = static_cast<TDataObject *>((*fInData)->At(iData));
        const TTimingChargeData *const Data = dynamic_cast<const TTimingChargeData *>(inData);

        // Data is input branch object
        // ex) Double_t in_charge = Data->GetCharge();
    }

    // prepare output branch
    TTimingChargeData *outData = static_cast<TTimingChargeData *>(fOutData->ConstructedAt(0)); // 0 is array index

    // outData is output branch object
    // ex) outData->SetID(0);
}
