/**
 * @file   TTimingDataMappingProcessor.h
 * @brief  simple data mapper
 *
 * @date   Created       : 2014-05-19 00:22:01 JST
 *         Last Modified : Jul 19, 2022 18:36:37 JST (okawa)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *  edit   OKAWA Kodai <okawa@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef _TTIMINGDATAMAPPINGPROCESSOR_H_
#define _TTIMINGDATAMAPPINGPROCESSOR_H_

#include <TProcessor.h>

namespace art {
  class TTimingDataMappingProcessor;
  class TCategorizedData;
}

class art::TTimingDataMappingProcessor : public TProcessor {
 public:
  TTimingDataMappingProcessor();
  virtual ~TTimingDataMappingProcessor();

  virtual void Init(TEventCollection *col);
  virtual void Process();

 protected:
  TString            fInputColName;
  TString            fOutputColName;
  TCategorizedData **fCategorizedData;
  TClonesArray      *fOutputArray;

  Int_t fCatID;       // category id
  Int_t fDataTypeID;  // typeid for data

  Bool_t fIsSparse;

 private:
  TTimingDataMappingProcessor(const TTimingDataMappingProcessor&);
  TTimingDataMappingProcessor& operator=(const TTimingDataMappingProcessor&);

  ClassDef(TTimingDataMappingProcessor,0) // simple data mapper
};

#endif // _TTIMINGDATAMAPPINGPROCESSOR_H_

