/*
   @File name     : TNBodyReactionProcessor.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-08-01 11:05:46
   @Last modified : 2023-08-01 11:05:59
*/

#ifndef _TNBODYREACTIONPROCESSOR_H_
#define _TNBODYREACTIONPROCESSOR_H_

#include <TMath.h>
#include <TLorentzVector.h>
#include <TGenPhaseSpace.h>

#include <TProcessor.h>
#include <SRIMData.h>
#include <SRIMtable.h>

namespace art {
class TNBodyReactionProcessor;
}

class TClonesArray;

class art::TNBodyReactionProcessor : public TProcessor {
public:
   TNBodyReactionProcessor();
   virtual ~TNBodyReactionProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputColName;
   TString fOutputColName;
   TClonesArray **fInData;     //!
   TClonesArray *fOutData;     //!

   Double_t fTargetThickness;

private:

   TNBodyReactionProcessor(const TNBodyReactionProcessor &rhs);
   TNBodyReactionProcessor &operator=(const TNBodyReactionProcessor &rhs);

   ClassDef(TNBodyReactionProcessor, 1)
};

#endif // end of #ifndef _TNBODYREACTIONPROCESSOR_H_
