// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#ifndef _TTSDATA_H_
#define _TTSDATA_H_

#include "TDataObject.h"
#include "constant.h"

namespace art {
   class TTSData;
}

class art::TTSData : public TDataObject {
public:
   typedef enum {kID, kTiming} ESortType;
   typedef enum {kASC, kDESC} ESortOrder;

   TTSData();
   virtual ~TTSData();
   TTSData(const TTSData& rhs);
   TTSData& operator=(const TTSData& rhs);

   ULong64_t GetTS() const {return fTS;}
   void SetTS(ULong64_t arg){ fTS = arg; }
   Double_t GetTScal() const {return fTScal;}
   void SetTScal(Double_t arg){ fTScal = arg; }

   virtual void Copy(TObject& dest) const;
   virtual void Clear(Option_t *opt="");

protected:
   ULong64_t fTS;
   Double_t fTScal;

   ClassDef(TTSData,1)
};

#endif // _TTSDATA_H_
