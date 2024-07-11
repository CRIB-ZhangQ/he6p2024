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
   TString fOutputReacColName;
   TClonesArray **fInData;     //!
   TClonesArray *fOutData;     //!
   TClonesArray *fOutReacData; //!

   TString fBeamName;

   Int_t fTargetMassNum;
   Int_t fTargetAtmNum;
   Int_t fTargetChargeNum;
   Double_t fTargetMass;
   TString fTargetName;
   Double_t fTargetThickness;
   Double_t fMinusPosition;
   Bool_t fTargetisGas;
   Double_t fBeamMass;

   Int_t fDecayNum;
   IntVec_t fReacMassNum;
   IntVec_t fReacAtmNum;
   IntVec_t fReacChargeNum;
   DoubleVec_t fReacMass;
   DoubleVec_t fExciteLevel;
   StringVec_t fReacPartName;

   //SRIMData *fElossTable;                   //!
   //SRIMtable *fElossTarget;                 //!
   //std::vector<SRIMtable *> fElossProducts; //!

   TGenPhaseSpace event;

   Double_t deg2rad = TMath::DegToRad();

private:
   TLorentzVector GetLossEnergyVector(TLorentzVector vec, Double_t eloss);

   TNBodyReactionProcessor(const TNBodyReactionProcessor &rhs);
   TNBodyReactionProcessor &operator=(const TNBodyReactionProcessor &rhs);

   ClassDef(TNBodyReactionProcessor, 1)
};

#endif // end of #ifndef _TNBODYREACTIONPROCESSOR_H_
