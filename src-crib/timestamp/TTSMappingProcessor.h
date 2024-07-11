// K.Okawa modified <okawa@cns.s.u-tokyo.ac.jp>

#ifndef _TTSMAPPINGPROCESSOR_H_
#define _TTSMAPPINGPROCESSOR_H_

#include <TProcessor.h>

namespace art {
  class TTSMappingProcessor;
  class TCategorizedData;
}

class art::TTSMappingProcessor : public TProcessor {
 public:
  TTSMappingProcessor();
  virtual ~TTSMappingProcessor();

  virtual void Init(TEventCollection *col);
  virtual void Process();

 protected:
  TString            fInputColName;
  TString            fOutputColName;
  TCategorizedData **fCategorizedData;
  TClonesArray      *fOutputArray;

  Int_t fCatID;       // category id
  Int_t fDataTypeID;  // typeid for data

 private:
  TTSMappingProcessor(const TTSMappingProcessor&);
  TTSMappingProcessor& operator=(const TTSMappingProcessor&);

  ClassDef(TTSMappingProcessor,0) // simple data mapper
};

#endif // _TTSMAPPINGPROCESSOR_H_
