/*
   @File name     : TBranchCopyProcessor.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 16:06:58
   @Last modified : 2023-06-13 16:06:58
*/

#ifndef _TBRANCHCOPYPROCESSOR_H_
#define _TBRANCHCOPYPROCESSOR_H_

#include <TProcessor.h>

namespace art {
class TBranchCopyProcessor;
}

class TClonesArray;

class art::TBranchCopyProcessor : public TProcessor {
public:
   TBranchCopyProcessor();
   virtual ~TBranchCopyProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputColName;
   TString fOutputColName;
   TClonesArray **fInData; //!
   TClonesArray *fOutData; //!

private:
   TBranchCopyProcessor(const TBranchCopyProcessor &);
   TBranchCopyProcessor &operator=(const TBranchCopyProcessor &);

   ClassDef(TBranchCopyProcessor, 0); // simple data mapper
};

#endif // end of #ifndef _TBRANCHCOPYPROCESSOR_H_
