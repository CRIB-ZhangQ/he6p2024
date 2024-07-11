/**
 * @file   TF1PPACProcessor.h
 * @brief  Created from TPPACProcessor.h
 *
 * @date   Created:       2013-10-30 14:01:27
 *         Last Modified : 2022-08-08 17:27:59 JST (K.Okawa)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>, OTA Shinsuke <ota@cns.s.u-tokyo.ac.jp>
 *         OKAWA Kodai <okawa@cns.s.u-tokyo.ac.jp>
 *    Copyright (C) 2013-2014 KAWASE Shoichiro, OTA Shinsuke  All rights reserved
 */

#ifndef TF1PPACPROCESSOR_H
#define TF1PPACPROCESSOR_H

#include <TProcessor.h>

#include <vector>

namespace art {
   class TF1PPACProcessor;
   class TCategorizedData;
   class TConverterBase;
   class TPPACParameter;
}

class TClonesArray;

class art::TF1PPACProcessor : public TProcessor {

public:
   // Default constructor
   TF1PPACProcessor();
   virtual ~TF1PPACProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t		  fInputColName;
   TString		  fOutputColName;
   TCategorizedData	**fCategorizedData; //!
   TClonesArray		 *fPPACOut;

   Int_t fCatID; // default: 1 (ppac in anapaw)

   static const Int_t DETID_ORIGIN = 0;

   TString fTimingConverterArrayName;
   TString fChargeConverterArrayName;
   TString fXConverterArrayName;
   TString fYConverterArrayName;

   std::vector<TConverterBase*> *fTimingConverterArray; //!
   std::vector<TConverterBase*> *fChargeConverterArray; //!
   std::vector<TConverterBase*> *fXConverterArray;      //!
   std::vector<TConverterBase*> *fYConverterArray;      //!


   // using ppac parameter set
   TString fParameterName; // name of parameter set 
   TClonesArray **fParameterSet; //! parameter set
   StringVec_t fListOfParameterNames; // list of parameter name for each ppac
   std::vector<TPPACParameter*> fPPACParameter; //! list of parmaeter for each ppac
   // output separately
   StringVec_t fListOfPPACNames; // list of ppac name for each ppac
   std::vector<TClonesArray*> fPPACArray; //! list of ppacs
   Bool_t fDoSeparatePPACs; // flag if separte the output of PPACs
   Bool_t fHasEachChConverter; // flag if the converter for each channel is prepared
   Bool_t fDoInsideOffset; // flag to calibrate inside offset
   Bool_t fDoOutsideOffset; // flag to calibrate outside offset
   Bool_t fDoGeometryOffset; // flag to calibrate geometry offset
   

private:
   // Copy constructor (prohibited)
   TF1PPACProcessor(const TF1PPACProcessor& rhs);
   // Assignment operator (prohibited)
   TF1PPACProcessor& operator=(const TF1PPACProcessor& rhs);

   ClassDef(TF1PPACProcessor,0) // processor for PPAC analysis
};

#endif // TF1PPACPROCESSOR_H
