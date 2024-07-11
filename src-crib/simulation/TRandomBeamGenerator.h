/*
   @File name     : TRandomBeamGenerator.h
   @description   : position and angle random beam generator
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-09 15:57:38
   @Last modified : 2023-08-01 11:06:44
*/

#ifndef _TRANDOMBEAMGENERATOR_H_
#define _TRANDOMBEAMGENERATOR_H_

#include <TProcessor.h>

namespace art {
class TRandomBeamGenerator;
}

class TClonesArray;

class art::TRandomBeamGenerator : public TProcessor {
public:
   TRandomBeamGenerator();
   virtual ~TRandomBeamGenerator();

   void Init(TEventCollection *);
   void Process();

protected:
   TString fOutputColName;
   TString fOutputTrackColName;

   TClonesArray *fOutData;      //!
   TClonesArray *fOutTrackData; //!

   Int_t fMassNum;
   Int_t fAtmNum;
   Int_t fChargeNum;
   Double_t fBeamEnergy;

   Double_t fXsigma;
   Double_t fYsigma;
   Double_t fAsigma;
   Double_t fBsigma;
   Double_t fEsigma;

   Double_t fMass;

private:
   // Copy constructor (prohibited)
   TRandomBeamGenerator(const TRandomBeamGenerator &rhs);
   // Assignment operator (prohibited)
   TRandomBeamGenerator &operator=(const TRandomBeamGenerator &rhs);

   ClassDef(TRandomBeamGenerator, 1);
};

#endif // end of #ifndef _TRANDOMBEAMGENERATOR_H_
