/*
   @File name     : TSolidAngleProcessor.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-08-01 11:08:17
   @Last modified : 2023-08-01 11:08:32
*/

#ifndef _TSOLIDANGLEPROCESSOR_H_
#define _TSOLIDANGLEPROCESSOR_H_

#include <TProcessor.h>

#include <TH1.h>
#include <TH2.h>

namespace art {
class TSolidAngleProcessor;
}

class TClonesArray;

class art::TSolidAngleProcessor : public TProcessor {
public:
   TSolidAngleProcessor();
   virtual ~TSolidAngleProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void PostLoop();

protected:
   TString fInputColName_cm;
   TString fInputColName_det;
   TClonesArray **fInData_cm;  //!
   TClonesArray **fInData_det; //!

   Int_t fNbin_angle;
   DoubleVec_t fRange_angle;
   Int_t fNbin_energy;
   DoubleVec_t fRange_energy;
   TString fFileName;
   Bool_t fIsIK;
   Bool_t fCenterOfMass;

   TH1D *h1A;     // for normalization (x: Ecm)
   TH2D *h2EA;     // for normalization (x: Ecm)
   TH1D *h1_all;   // for normalize (x: Ecm)
   TH2D *h2_all;   // for normalize (x: Ecm, y: Thetacm)
   TH1D *h1;       //total solid angle (x: Ecm)
   TH2D *h2;       //total solid angle (x: Ecm, y: Thetacm)
   TH1D *h1_0;     //telescope 1: solid angle (x: Ecm)
   TH1D *h1_1;     //telescope 2: solid angle (x: Ecm)
   TH1D *h1_2;     //telescope 3: solid angle (x: Ecm)
   TH1D *h1_3;     //telescope 4: solid angle (x: Ecm)
   TH1D *h1_4;     //telescope 5: solid angle (x: Ecm)
   TH1D *h1_5;     //telescope 6: solid angle (x: Ecm)
   TH1D *h1_lab;     //total solid angle in the lab system(x: Ecm)

private:
   TSolidAngleProcessor(const TSolidAngleProcessor &rhs);
   TSolidAngleProcessor &operator=(const TSolidAngleProcessor &rhs);
   Double_t OmegaCM2LAB(Double_t thetacm);
   Double_t fGamma();

   ClassDef(TSolidAngleProcessor, 1)
};

#endif // end of #ifndef _TSOLIDANGLEPROCESSOR_H_
