// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

/* 
 * if no valid converter given, this processor does nothing.
 */

#ifndef _TTELESCOPEPROCESSOR_H_
#define _TTELESCOPEPROCESSOR_H_

#include <TProcessor.h>
#include "TTelescopeParameter.h"

namespace art {
   class TTelescopeProcessor;
   class TTelescopeParameter;
}

class TClonesArray;

class art::TTelescopeProcessor : public TProcessor {
public:
   // Default constructor
   TTelescopeProcessor();
   virtual ~TTelescopeProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

   Bool_t debug_flag;

protected:
   TString        fInputColName1;
   TString        fInputColName2;
   TString        fInputColName3;
   TString        fOutputColName;
   TClonesArray **fInData1; //!
   TClonesArray **fInData2; //!
   TClonesArray **fInData3; //!
   TClonesArray  *fOutData; //!

   Bool_t fInputHasData;

   // from parameter file
   TString fParameterName; 
   TClonesArray **fParameterSet; //!
   TTelescopeParameter *fTelescopeParameter;

private:
   // Copy constructor (prohibited)
   TTelescopeProcessor(const TTelescopeProcessor& rhs);
   // Assignment operator (prohibited)
   TTelescopeProcessor& operator=(const TTelescopeProcessor& rhs);

   ClassDef(TTelescopeProcessor,2) // processor for calibration of timing and charge data
};

#endif // _TTELESCOPEPROCESSOR_H_
