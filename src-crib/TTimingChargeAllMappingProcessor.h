/**
 * @file   TTimingChargeAllMappingProcessor.h
 * @brief
 *
 * @date   Created:       2013-08-12 12:36:40
 *         Last Modified: 2014-06-18 11:03:41 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TTIMINGCHARGEALLMAPPINGPROCESSOR_H
#define TTIMINGCHARGEALLMAPPINGPROCESSOR_H

#include <TProcessor.h>

namespace art{
   class TTimingChargeAllMappingProcessor;
   class TCategorizedData;
}

class TClonesArray;

class art::TTimingChargeAllMappingProcessor : public TProcessor {

public:
   // Default constructor
   TTimingChargeAllMappingProcessor();
   virtual ~TTimingChargeAllMappingProcessor();

   virtual void Init(TEventCollection*);
   virtual void Process();

protected:
   TString            fInputColName;
   TString            fOutputColName;
   TCategorizedData **fCategorizedData; //!
   TClonesArray      *fPlastic; //!

   Int_t  fCatID;        // default: 21 (spla in anapaw)
   Int_t  fChargeType;   // 0: V1190 width (default), 1: QDC (V792 etc)
   Int_t  fChargeTypeID; // used only when fChargeType is QDC
   Int_t  fTimingTypeID; // Typeid for timing (& width when fChargeType is V1190width)
   Bool_t fTrailingComesFirst; // F: TL->TT (default), T: TT->TL (for QTC etc.)

   Bool_t fIsSparse;
   Bool_t fIsSort;

private:
   // Copy constructor (prohibited)
   TTimingChargeAllMappingProcessor(const TTimingChargeAllMappingProcessor&);
   // Assignment operator (prohibited)
   TTimingChargeAllMappingProcessor& operator=(const TTimingChargeAllMappingProcessor&);

   void MapEdgedTime(const TObjArray*);
   void MapPairedTimeCharge(const TObjArray*);
   void MapTimeCharge(const TObjArray*, const TObjArray*);

   enum EChargeTypeID { kWIDTH, kQDC, kHINP,};

   ClassDef(TTimingChargeAllMappingProcessor,0) // processor for mapping timine and charge data
};

#endif // TTIMINGCHARGEALLMAPPINGPROCESSOR_H
