/*
   @File name     : TTGTIKProcessor.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2024-04-25 22:58
   @last modified  : 2024-04-25 22:58
*/

#ifndef _TREACANALYSISPROCESSOR_WO_MWDCA_H_
#define _TREACANALYSISPROCESSOR_WO_MWDCA_H_

#include <TProcessor.h>
#include <TTrack.h>
#include <SRIMData.h>
#include <SRIMtable.h>
#include <TEnum.h>
//#include "../geo/TDetectorParameter.h"
//#include "../telescope/TTelescopeData.h"

namespace art {
class TReacAnalysisProcessor_wo_mwdca;
}

class TClonesArray;

class art::TReacAnalysisProcessor_wo_mwdca : public TProcessor {

typedef enum {Light_b, Heavy_B} Particle; 

public:
   // Default constructor
   TReacAnalysisProcessor_wo_mwdca();
   virtual ~TReacAnalysisProcessor_wo_mwdca();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputColName1;
   TString fInputColName2;
   TString fOutputColName;

   TClonesArray **fInData1;      //!
   TClonesArray **fInData2;      //!
   TClonesArray *fOutData;      //!

   //reaction information
   DoubleVec_t fMass;
   IntVec_t fCharge;
   Double_t fQvalue;
   Double_t fEbeam;
   Double_t fExci;
   Int_t fParticle;
   DoubleVec_t fTargetX_mu;
   DoubleVec_t fTargetX_sigma;
   DoubleVec_t fTargetY_mu;
   DoubleVec_t fTargetY_sigma;

   Bool_t fInputHasData;

private:

   Double_t fgamma();
   // Copy constructor (prohibited)
   TReacAnalysisProcessor_wo_mwdca(const TReacAnalysisProcessor_wo_mwdca &rhs);
   // Assignment operator (prohibited)
   TReacAnalysisProcessor_wo_mwdca &operator=(const TReacAnalysisProcessor_wo_mwdca &rhs);

   ClassDef(TReacAnalysisProcessor_wo_mwdca, 0)
};

#endif // end of #ifndef _TREACANALYSISPROCESSOR_H_
