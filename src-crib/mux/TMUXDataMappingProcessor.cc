// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "TMUXDataMappingProcessor.h"

#include <TRawDataObject.h>
#include <TCategorizedData.h>
#include <mux/TMUXData.h>

#include "constant.h"

using art::TMUXDataMappingProcessor;

ClassImp(TMUXDataMappingProcessor)

TMUXDataMappingProcessor::TMUXDataMappingProcessor()
  : fOutputArray(NULL) {
  RegisterInputCollection("InputCollection","rawdata object returned by TRIDFEventStore",
			  fInputColName,TString("catdata"),
			  reinterpret_cast<void***>(&fCategorizedData),
			  "art::TCategorizedData");
  RegisterOutputCollection("OutputCollection","output collection name",
			   fOutputColName,TString("simple"),
			   reinterpret_cast<void**>(&fOutputArray),
			   "TClonesArray","art::TMUXData");
  RegisterProcessorParameter("CatID","Category ID",fCatID,0);
}

TMUXDataMappingProcessor::~TMUXDataMappingProcessor()
{
  delete fOutputArray;
  fOutputArray = NULL;
}

void TMUXDataMappingProcessor::Init(TEventCollection *col)
{
  Info("Init","CatID: %d => %s", fCatID, fOutputColName.Data());
}

void TMUXDataMappingProcessor::Process()
{
  if (!fCategorizedData) return;

  fOutputArray->Clear("C");

  const TObjArray *const cat = (*fCategorizedData)->FindCategory(fCatID);
  if (!cat) return;
  //std::cout << "debug: find category" << std::endl; //debug
  Double_t m[TMUXData::kNRAW];

  const Int_t n = cat->GetEntriesFast();

  for(Int_t i = 0 ; i != n ; ++i) {
    const TObjArray *const det = static_cast<TObjArray*>(cat->At(i));
    //if(det->GetEntriesFast() != 5) continue; // get only adc and tdc data exist
    //std::cout << "debug: find " << det->GetEntriesFast() << " contents" << std::endl; //debug

    Int_t detID = kInvalidI;

    for(Int_t j = 0 ; j != TMUXData::kNRAW ; ++j){
      m[j] = kInvalidD;

      const TObjArray *const dataArray = static_cast<TObjArray*>(det->At(j));

      if(dataArray && dataArray->GetEntriesFast() ){
        const TRawDataObject *const hit = dynamic_cast<TRawDataObject*>(dataArray->At(0));
        if(!hit) continue;

        m[j] = hit->GetValue();
        detID = hit->GetDetID();
      }
    }

    TMUXData *const data = static_cast<TMUXData*>(fOutputArray->ConstructedAt(fOutputArray->GetEntriesFast()));
    data->TDataObject::SetID(detID);
    
    data->SetE1(m[0]);
    data->SetE2(m[1]);
    data->SetP1(m[2]);
    data->SetP2(m[3]);
    data->SetTrig(m[4]);

  }

  //if(fIsSparse) {
  //  TChargeData::SetSortType( TChargeData::kCharge);
  //  TChargeData::SetSortOrder(TChargeData::kASC);
  //  fOutputArray->UnSort();
  //  fOutputArray->Sort();
  //  fOutputArray->Compress();
  //} else {
  //  const Int_t n = fOutputArray->GetEntriesFast();
  //  for (Int_t i = 0; i!=n; ++i) {
  //    fOutputArray->ConstructedAt(i);
  //  }
  //}
}

