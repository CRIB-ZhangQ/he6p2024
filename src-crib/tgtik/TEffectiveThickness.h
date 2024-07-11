/*
   @File name     : TEffectiveThickness.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 16:06:58
   @Last modified : 2023-08-01 11:09:05
*/

#ifndef _TEFFECTIVETHICKNESS_H_
#define _TEFFECTIVETHICKNESS_H_

#include <TProcessor.h>
#include <SRIMData.h>
#include <SRIMtable.h>

namespace art {
class TEffectiveThickness;
}

class TClonesArray;

class art::TEffectiveThickness : public TProcessor {
public:
   TEffectiveThickness();
   virtual ~TEffectiveThickness();

   virtual void Init(TEventCollection *col);
   virtual void Process();

   const Double_t kAvogadro = 6.02214076e+23;

protected:
   TString fOutputName;
   StringVec_t fParticleNames;
   DoubleVec_t fParticleMass;
   TString fTarget;
   Double_t fTargetA;

   Int_t fNbin_Energy;
   DoubleVec_t fRange;
   Double_t fDeltaE;
   Double_t fDensity;

   SRIMData *fElossTable; //!
   SRIMtable *fElossBeam; //!

private:
   TEffectiveThickness(const TEffectiveThickness &);
   TEffectiveThickness &operator=(const TEffectiveThickness &);

   Double_t Ecm2Elab(Double_t energy);

   ClassDef(TEffectiveThickness, 0);
};

#endif // end of #ifndef _TEFFECTIVETHICKNESS_H_
