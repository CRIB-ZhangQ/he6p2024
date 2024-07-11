/*
   @File name     : TCatCmdLoopStop.cc
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 17:29:21
   @Last modified : 2023-06-13 17:29:21
*/

#include "TCatCmdLoopStop.h"
#include <TLoopManager.h>

ClassImp(TCatCmdLoopStop);

TCatCmdLoopStop::TCatCmdLoopStop()
{
   SetName("stop");
   SetTitle("stop loop (same with suspend)");
}
TCatCmdLoopStop::~TCatCmdLoopStop() {}

TCatCmdLoopStop *TCatCmdLoopStop::Instance()
{
   static TCatCmdLoopStop instance;
   return &instance;
}

Long_t TCatCmdLoopStop::Cmd(vector<TString> args)
{
   art::TLoopManager *lm = art::TLoopManager::Instance();
   switch (args.size()) {
   case 1: lm->Suspend(0); break;
   default: break;
   }
   return 1;
}

void TCatCmdLoopStop::Help() {}