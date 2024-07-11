/*
   @File name     : TParticleInfo.h
   @description   : particle information class
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-09 17:57:25
   @Last modified : 2023-08-01 11:06:22
*/

#ifndef _TPARTICLEINFO_H_
#define _TPARTICLEINFO_H_

#include <TDataObject.h>
#include <TLorentzVector.h>
#include <TTrack.h>

namespace art {
class TParticleInfo;
}

class art::TParticleInfo : public TDataObject {
public:
   typedef enum { kID, kTiming } ESortType;
   typedef enum { kASC, kDESC } ESortOrder;

   TParticleInfo();
   virtual ~TParticleInfo();
   TParticleInfo(const TParticleInfo &rhs);
   TParticleInfo &operator=(const TParticleInfo &rhs);

   void SetMassNumber(Int_t val) { fMassNumber = val; }
   Int_t GetMassNumber() const { return fMassNumber; }
   void SetAtomicNumber(Int_t val) { fAtomicNumber = val; }
   Int_t GetAtomicNumber() const { return fAtomicNumber; }
   void SetCharge(Int_t val) { fCharge = val; }
   Int_t GetCharge() const { return fCharge; }

   void SetCurrentZ(Double_t val) { fCurrentZ = val; }
   Double_t GetCurrentZ() const { return fCurrentZ; }
   void SetEnergy(Double_t val) { fEnergy = val; }
   Double_t GetEnergy() const { return fEnergy; }

   void SetLorentzVector(Double_t x, Double_t y, Double_t z, Double_t t) { fVec.SetXYZT(x, y, z, t); }
   void SetLorentzVector(TLorentzVector val) { fVec = val; }
   TLorentzVector GetLorentzVector() const { return fVec; }

   void SetTrack(Double_t x, Double_t y, Double_t z, Double_t a, Double_t b)
   {
      fTrack.SetPos(x, y, z);
      fTrack.SetAngle(a, b);
   }
   TTrack GetTrack() const { return fTrack; }

   virtual void Copy(TObject &dest) const;
   virtual void Clear(Option_t *opt = "");

protected:
   Int_t fMassNumber;
   Int_t fAtomicNumber;
   Int_t fCharge;

   Double_t fCurrentZ;
   Double_t fEnergy;    // kinetic energy

   TTrack fTrack;       // tracking information in LAB system
   TLorentzVector fVec; // lorentz vector (px, py, pz, E) of this particle

   ClassDef(TParticleInfo, 1);
};

#endif // end of #ifndef _TPARTICLEINFO_H_
