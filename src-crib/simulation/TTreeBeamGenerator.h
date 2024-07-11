/*
   @File name     : TTreeBeamGenerator.h
   @description   : position and angle random beam generator
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-09 15:57:38
   @Last modified : 2023-08-01 11:06:44
*/

#ifndef _TTREEBEAMGENERATOR_H_
#define _TTREEBEAMGENERATOR_H_

#include <TProcessor.h>

namespace art {
class TTreeBeamGenerator;
}

class TClonesArray;

class art::TTreeBeamGenerator : public TProcessor {
public:
   TTreeBeamGenerator();
   virtual ~TTreeBeamGenerator();

   void Init(TEventCollection *);
   void Process();

protected:
   TString fInputColName;
   TString fOutputColName;

   TClonesArray **fInData;     //!
   TClonesArray *fOutData;     //!

   Int_t fMassNum;
   Int_t fAtmNum;
   Int_t fChargeNum;
   Double_t fMass;
   Double_t fBeamEnergy;

   Double_t fEsigma;


private:
   // Copy constructor (prohibited)
   TTreeBeamGenerator(const TTreeBeamGenerator &rhs);
   // Assignment operator (prohibited)
   TTreeBeamGenerator &operator=(const TTreeBeamGenerator &rhs);

   ClassDef(TTreeBeamGenerator, 1);
};

#endif // end of #ifndef _TTREEBEAMGENERATOR_H_
