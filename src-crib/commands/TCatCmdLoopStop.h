/*
   @File name     : TCatCmdLoopStop.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 17:28:09
   @Last modified : 2023-06-13 17:28:09
*/

#ifndef _TCATCMDLOOPSTOP_H_
#define _TCATCMDLOOPSTOP_H_

#include <TCatCmd.h>

class TCatCmdLoopStop : public TCatCmd {
protected:
   TCatCmdLoopStop();

public:
   ~TCatCmdLoopStop();

   static TCatCmdLoopStop *Instance();

   virtual Long_t Cmd(vector<TString> args);

   virtual void Help();

   ClassDef(TCatCmdLoopStop, 1);
};

#endif // end of #ifndef _TCATCMDLOOPSTOP_H_