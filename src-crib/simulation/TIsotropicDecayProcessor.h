/*
   @File name     : TIsotropicDecayProcessor.h
   @description   :
   @Author        : Zhang <zhangq22@cns.s.u-tokyo.ac.jp>
   @Created date  : 2024-05-06 18:56:00
   @Last modified : 2024-05-06 18:56:00
*/

#ifndef _TISOTROPICPROCESSOR_H_
#define _TISOTROPICPROCESSOR_H_

#include <TProcessor.h>

namespace art {
class TIsotropicDecayProcessor;
}

class TClonesArray;

class art::TIsotropicDecayProcessor : public TProcessor {
public:
   TIsotropicDecayProcessor();
   virtual ~TIsotropicDecayProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputColName;
   TString fOutputColName;
   TClonesArray **fInData;     //!
   TClonesArray *fOutData;     //!


   Double_t fTargetMass;
   Double_t fTargetThickness;
   Double_t fBeamMass;


private:
   //TLorentzVector GetLossEnergyVector(TLorentzVector vec, Double_t eloss);

   TIsotropicDecayProcessor (const TIsotropicDecayProcessor &rhs);
   TIsotropicDecayProcessor &operator=(const TIsotropicDecayProcessor &rhs);

   ClassDef(TIsotropicDecayProcessor, 1)
};

#endif // end of #ifndef _TISOTROPICDECAYPROCESSOR_H_
