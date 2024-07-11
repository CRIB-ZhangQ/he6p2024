// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#ifndef _TMUXDATAMAPPINGPROCESSOR_H_
#define _TMUXDATAMAPPINGPROCESSOR_H_

#include <TProcessor.h>

namespace art {
  class TMUXDataMappingProcessor;
  class TCategorizedData;
}

class art::TMUXDataMappingProcessor : public TProcessor {
 public:
  TMUXDataMappingProcessor();
  virtual ~TMUXDataMappingProcessor();

  virtual void Init(TEventCollection *col);
  virtual void Process();

 protected:
  TString            fInputColName;
  TString            fOutputColName;
  TCategorizedData **fCategorizedData;
  TClonesArray      *fOutputArray;

  Int_t fCatID;       // category id

 private:
  TMUXDataMappingProcessor(const TMUXDataMappingProcessor&);
  TMUXDataMappingProcessor& operator=(const TMUXDataMappingProcessor&);

  ClassDef(TMUXDataMappingProcessor,0) // simple data mapper
};

#endif // _TMUXDATAMAPPINGPROCESSOR_H_

