// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#ifndef _TTGTIKDATA_H_
#define _TTGTIKDATA_H_

#include "TDataObject.h"
#include "constant.h"

namespace art {
   class TTGTIKData;
}

class art::TTGTIKData : public TDataObject {
public:
   typedef enum {kID, kTiming} ESortType;
   typedef enum {kASC, kDESC} ESortOrder;

   TTGTIKData();
   virtual ~TTGTIKData();
   TTGTIKData(const TTGTIKData& rhs);
   TTGTIKData& operator=(const TTGTIKData& rhs);

   Int_t GetFlag() const {return fFlag;}
   void SetFlag(Int_t arg) { fFlag = arg; }

   Double_t GetReacZ() const {return fReacZ;}
   void SetReacZ(Double_t arg) { fReacZ = arg; }
   Double_t GetEcm() const {return fEcm;}
   void SetEcm(Double_t arg) { fEcm = arg; }
   Double_t GetThetaL() const {return fThetaL;}
   void SetThetaL(Double_t arg) { fThetaL = arg; }
   Double_t GetThetaCM() const {return fThetaCM;}
   void SetThetaCM(Double_t arg) { fThetaCM = arg; }


   virtual void Copy(TObject& dest) const;
   virtual void Clear(Option_t *opt="");

protected:
   Int_t fFlag;

   Double_t fReacZ;
   Double_t fEcm;
   Double_t fThetaL;
   Double_t fThetaCM;

   ClassDef(TTGTIKData,1)
};

#endif // _TTGTIKDATA_H_
