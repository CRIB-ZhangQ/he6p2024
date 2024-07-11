// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#ifndef _TTGTIKPREPDATA_H_
#define _TTGTIKPREPDATA_H_

#include "TDataObject.h"
#include "constant.h"

namespace art {
   class TTGTIKPrepData;
}

class art::TTGTIKPrepData : public TDataObject {
public:
   typedef enum {kID, kTiming} ESortType;
   typedef enum {kASC, kDESC} ESortOrder;

   TTGTIKPrepData();
   virtual ~TTGTIKPrepData();
   TTGTIKPrepData(const TTGTIKPrepData& rhs);
   TTGTIKPrepData& operator=(const TTGTIKPrepData& rhs);

   Int_t GetReac_flag() const {return fReac_flag;}
   void SetReac_flag(Int_t arg) { fReac_flag = arg; }

   Double_t GetEtotal() const {return fEtotal;}
   void SetEtotal(Double_t arg) { fEtotal = arg; }

   Double_t GetWinx() const {return fWinx;}
   void SetWinx(Double_t arg) { fWinx = arg; }
   Double_t GetWiny() const {return fWiny;}
   void SetWiny(Double_t arg) { fWiny = arg; }
   Double_t GetWinz() const {return fWinz;}
   void SetWinz(Double_t arg) { fWinz = arg; }

   Double_t GetDetx() const {return fDetx;}
   void SetDetx(Double_t arg) { fDetx = arg; }
   Double_t GetDety() const {return fDety;}
   void SetDety(Double_t arg) { fDety = arg; }
   Double_t GetDetz() const {return fDetz;}
   void SetDetz(Double_t arg) { fDetz = arg; }


   virtual void Copy(TObject& dest) const;
   virtual void Clear(Option_t *opt="");

protected:
   Int_t fReac_flag;

   Double_t fEtotal;

   Double_t fWinx;
   Double_t fWiny;
   Double_t fWinz;

   Double_t fDetx;
   Double_t fDety;
   Double_t fDetz;

   ClassDef(TTGTIKPrepData,1)
};

#endif // _TTGTIKPREPDATA_H_
