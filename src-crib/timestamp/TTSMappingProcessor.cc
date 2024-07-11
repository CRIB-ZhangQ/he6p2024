// K.Okawa modified <okawa@cns.s.u-tokyo.ac.jp>

#include "TTSMappingProcessor.h"

#include "TTSData.h"
#include <TCategorizedData.h>
#include <TRawDataObject.h>

#include "constant.h"

using art::TTSMappingProcessor;

ClassImp(TTSMappingProcessor);

TTSMappingProcessor::TTSMappingProcessor()
    : fOutputArray(NULL) {
    RegisterInputCollection("InputCollection", "rawdata object returned by TRIDFEventStore",
                            fInputColName, TString("catdata"),
                            reinterpret_cast<void ***>(&fCategorizedData),
                            "art::TCategorizedData");
    RegisterOutputCollection("OutputCollection", "output collection name",
                             fOutputColName, TString("simple"),
                             reinterpret_cast<void **>(&fOutputArray),
                             "TClonesArray", "art::TTSData");
    RegisterProcessorParameter("CatID", "Category ID", fCatID, 0);
    RegisterProcessorParameter("DataTypeID", "data type id for input",
                               fDataTypeID, 0);
}

TTSMappingProcessor::~TTSMappingProcessor() {
    delete fOutputArray;
    fOutputArray = NULL;
}

void TTSMappingProcessor::Init(TEventCollection *col) {
    Info("Init", "CatID: %d, DataTypeID: %d => %s",
         fCatID, fDataTypeID, fOutputColName.Data());
}

void TTSMappingProcessor::Process() {
    if (!fCategorizedData)
        return;

    fOutputArray->Clear("C");

    const TObjArray *const cat = (*fCategorizedData)->FindCategory(fCatID);
    if (!cat)
        return;

    const Int_t n = cat->GetEntriesFast();

    for (Int_t i = 0; i != n; ++i) {
        const TObjArray *const det = static_cast<TObjArray *>(cat->At(i));
        const TObjArray *const dataArray = static_cast<TObjArray *>(det->At(fDataTypeID));
        if (!dataArray)
            continue;

        const TRawDataObject *const hit = dynamic_cast<TRawDataObject *>(dataArray->At(0));
        if (!hit)
            continue;
        const Int_t detID = hit->GetDetID();
        const Int_t idx = detID;
        TTSData *const data = static_cast<TTSData *>(fOutputArray->ConstructedAt(idx));

        data->SetID(detID);
        data->SetTS(hit->GetValue());
        data->SetTScal(hit->GetValue() * 0.01);
    }
}
