// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#ifndef _TTGTIKPREPPROCESSOR_H_
#define _TTGTIKPREPPROCESSOR_H_

#include <TProcessor.h>

#include <TVector3.h>

#include <vector>

namespace art {
   class TTGTIKPrepProcessor;
}

class TClonesArray;

class art::TTGTIKPrepProcessor : public TProcessor {
public:
   // Default constructor
   TTGTIKPrepProcessor();
   virtual ~TTGTIKPrepProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

   TVector3 get_DetPosition(Int_t telID, Int_t XID, Int_t YID, TVector3 win_offset, TVector3 win_dir);
   TVector3 get_rawDetPosition(Int_t XID, Int_t YID, Double_t orig_rotate_z, Double_t distance, Double_t angle, Double_t offset, Bool_t x_reflection);

   Bool_t debug_flag;

protected:
   TString        fInputColName_a;
   TString        fInputColName_b;
   TString        fInputColName_tel;
   TString        fOutputColName;

   TClonesArray **fInData_a; //!
   TClonesArray **fInData_b; //!
   TClonesArray **fInData_tel; //!
   TClonesArray  *fOutData; //!

   Double_t fWindowpos;

   Bool_t fIsSparse;

private:
   // Copy constructor (prohibited)
   TTGTIKPrepProcessor(const TTGTIKPrepProcessor& rhs);
   // Assignment operator (prohibited)
   TTGTIKPrepProcessor& operator=(const TTGTIKPrepProcessor& rhs);

   ClassDef(TTGTIKPrepProcessor,1) // processor for calibration of timing and charge data
};

#endif // _TTGTIKPREPPROCESSOR_H_
