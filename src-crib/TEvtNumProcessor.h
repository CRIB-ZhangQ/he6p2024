/**
 * @file   TEvtNumProcessor.h
 * @date   Created : Jun 22, 2012 21:22:45 JST
 *   Last Modified : Feb 04, 2014 16:33:20 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2012
 */

#ifndef _TEVTNUMPROCESSOR_H_
#define _TEVTNUMPROCESSOR_H_

#include "TProcessor.h"
#include <TStopwatch.h>

namespace art {
   class TEvtNumProcessor;
}



class art::TEvtNumProcessor  : public TProcessor {

 public:
  TEvtNumProcessor();
  ~TEvtNumProcessor();

  virtual void Init(TEventCollection*col);
  virtual void EndOfRun();
  virtual void Process();

 private:
  Int_t fCurNum;
  Int_t fLimEvNum;
  Bool_t fPriEv;
  Int_t fPriEvNum;

 protected:
  ClassDef(TEvtNumProcessor,1);
};
#endif // end of #ifdef _TEVTNUMPROCESSOR_H_
