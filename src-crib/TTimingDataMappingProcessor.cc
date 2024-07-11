/**
 * @file   TTimingDataMappingProcessor.cc
 * @brief  simple data mapper
 *
 * @date   Created       : 2014-05-19 00:26:01 JST
 *         Last Modified : Jul 19, 2022 18:36:37 JST (okawa)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *  edit   OKAWA Kodai <okawa@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TTimingDataMappingProcessor.h"

#include <TRawDataObject.h>
#include <TCategorizedData.h>
#include <TTimingData.h>

#include "constant.h"

using art::TTimingDataMappingProcessor;

ClassImp(TTimingDataMappingProcessor)

TTimingDataMappingProcessor::TTimingDataMappingProcessor()
  : fOutputArray(NULL) {
  RegisterInputCollection("InputCollection","rawdata object returned by TRIDFEventStore",
			  fInputColName,TString("catdata"),
			  reinterpret_cast<void***>(&fCategorizedData),
			  "art::TCategorizedData");
  RegisterOutputCollection("OutputCollection","output collection name",
			   fOutputColName,TString("simple"),
			   reinterpret_cast<void**>(&fOutputArray),
			   "TClonesArray","art::TTimingData");
  RegisterProcessorParameter("CatID","Category ID",fCatID,0);
  RegisterProcessorParameter("DataTypeID","data type id for input",
			     fDataTypeID,0);
  //  RegisterProcessorParameter("OutputType","not yet implemented (always output TTimingData)"
  //			     fOutputType,2);
  RegisterProcessorParameter("Sparse","hit will be inserted at the index corresponding to its id if sparse is 0, otherwise added ordinaly", fIsSparse,kTRUE);
}

TTimingDataMappingProcessor::~TTimingDataMappingProcessor()
{
  delete fOutputArray;
  fOutputArray = NULL;
}

void TTimingDataMappingProcessor::Init(TEventCollection *col)
{
  Info("Init","CatID: %d, DataTypeID: %d => %s",
       fCatID, fDataTypeID, fOutputColName.Data());
  if (!fIsSparse) Info("Init","Not sparse.");
}

void TTimingDataMappingProcessor::Process()
{
  if (!fCategorizedData) return;

  fOutputArray->Clear("C");

  const TObjArray *const cat = (*fCategorizedData)->FindCategory(fCatID);
  if (!cat) return;

  const Int_t n = cat->GetEntriesFast();

  for(Int_t i = 0 ; i != n ; ++i) {
    const TObjArray *const det = static_cast<TObjArray*>(cat->At(i));
    const TObjArray *const dataArray = static_cast<TObjArray*>(det->At(fDataTypeID));
    if (!dataArray) continue;

    Double_t measure ;
    
    const TRawDataObject *const hit =
       dynamic_cast<TRawDataObject*>(dataArray->At(0));
    const Int_t detID = hit->GetDetID();
    const Int_t idx = fIsSparse ? fOutputArray->GetEntriesFast() : detID;
    TTimingData *const data =
      static_cast<TTimingData*>(fOutputArray->ConstructedAt(idx));
    if (IsValid(data->GetID())) return; // take only the first hit if not sparse

    data->SetID(detID);
    data->SetTiming(hit->GetValue());
  }

  if(fIsSparse) {
    TTimingData::SetSortType( TTimingData::kTiming);
    TTimingData::SetSortOrder(TTimingData::kASC);
    fOutputArray->UnSort();
    fOutputArray->Sort();
    fOutputArray->Compress();
  } else {
    const Int_t n = fOutputArray->GetEntriesFast();
    for (Int_t i = 0; i!=n; ++i) {
      fOutputArray->ConstructedAt(i);
    }
  }
}

