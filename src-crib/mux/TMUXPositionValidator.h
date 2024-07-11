// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#ifndef _TMUXPOSITIONVALIDATOR_H_ 
#define _TMUXPOSITIONVALIDATOR_H_

#include <TProcessor.h>

namespace art {
   class TMUXPositionValidator;
}
class TClonesArray;

class art::TMUXPositionValidator : public TProcessor {
public:
   TMUXPositionValidator();
   virtual ~TMUXPositionValidator();

   TMUXPositionValidator(const TMUXPositionValidator& rhs);
   TMUXPositionValidator& operator=(const TMUXPositionValidator& rhs);

protected:
   virtual void Init(TEventCollection *col);
   virtual void Process();
private:
   TString fInputName; // name of input collection
   TString fOutputName; // name of output collection
   TClonesArray **fInput; //! input
   TClonesArray  *fOutput; //! output

//   Int_t fTimeVariable; // 0 time 1 timestamp
   FloatVec_t fValidPositionRange;
   Float_t fValidPositionMin;
   Float_t fValidPositionMax;
   

   ClassDef(TMUXPositionValidator,1) // validate time in certain window
};

#endif // _TMUXPOSITIONVALIDATOR_H_
