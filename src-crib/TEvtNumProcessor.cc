/**
 * @file   TEvtNumProcessor.cc
 * @date   Created : Jun 22, 2012 21:22:04 JST
 *   Last Modified : Feb 04, 2014 16:34:12 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2012
 */
#include <cstdlib>
#include "TEvtNumProcessor.h"
#include <iostream>

#include <TBenchmark.h>
#include <TStopwatch.h>

ClassImp(art::TEvtNumProcessor);

art::TEvtNumProcessor::TEvtNumProcessor():
  fCurNum(0), fLimEvNum(0), fPriEv(kFALSE), fPriEvNum(100) {
  RegisterProcessorParameter("EventNumLimit","the limit of the event number",
			     fLimEvNum, 0);
  RegisterOptionalParameter("PrintEvent","print the current event number",
			    fPriEv, kFALSE);
  RegisterProcessorParameter("PrintEventNum","event number of frequency",
			     fPriEvNum, 100);}

art::TEvtNumProcessor::~TEvtNumProcessor() {}

void art::TEvtNumProcessor::Init(TEventCollection *) {}

void art::TEvtNumProcessor::EndOfRun(){
  std::cout << "  Event Number: " << fCurNum << std::endl;
}

void art::TEvtNumProcessor::Process() {
  fCurNum++;
  if (fPriEv && !(fCurNum % fPriEvNum)) {
    std::cout << "\r  Event Number: " << fCurNum;
    std::cout.flush();
  }

  if (fLimEvNum && fCurNum >= fLimEvNum) {
    std::cout << "\r  Event Number: " << fCurNum << std::endl;
    Info("Process","The number of event is limited to %d.",fCurNum);
    SetStopLoop();
  }

}
