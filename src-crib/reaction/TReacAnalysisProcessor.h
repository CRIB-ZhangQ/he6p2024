/*
   @File name     : TTGTIKProcessor.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2024-04-25 22:58
   @last modified  : 2024-04-25 22:58
*/

#ifndef _TREACANALYSISPROCESSOR_H_
#define _TREACANALYSISPROCESSOR_H_

#include <TProcessor.h>
#include <TTrack.h>
#include <SRIMData.h>
#include <SRIMtable.h>
#include <TEnum.h>
//#include "../geo/TDetectorParameter.h"
//#include "../telescope/TTelescopeData.h"

namespace art {
class TReacAnalysisProcessor;
}

class TClonesArray;

class art::TReacAnalysisProcessor : public TProcessor {

typedef enum {Light_b, Heavy_B} Particle; 

public:
   // Default constructor
   TReacAnalysisProcessor();
   virtual ~TReacAnalysisProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputColName1;
   TString fInputColName2;
   TString fInputColName3;
   TString fInputColName4;
   TString fOutputColName;
   //TString fParameterName;

   TClonesArray **fInData1;      //!
   TClonesArray **fInData2;      //!
   TClonesArray **fInData3;      //!
   TClonesArray **fInData4; //!
   TClonesArray *fOutData;      //!

   //reaction information
   DoubleVec_t fMass;
   IntVec_t fCharge;
   Double_t fQvalue;
   Double_t fEbeam;
   Double_t fExci;
   Int_t fParticle;

   Bool_t fInputHasData;

private:

   Double_t fgamma();
   // Copy constructor (prohibited)
   TReacAnalysisProcessor(const TReacAnalysisProcessor &rhs);
   // Assignment operator (prohibited)
   TReacAnalysisProcessor &operator=(const TReacAnalysisProcessor &rhs);

   ClassDef(TReacAnalysisProcessor, 0)
};

#endif // end of #ifndef _TREACANALYSISPROCESSOR_H_
