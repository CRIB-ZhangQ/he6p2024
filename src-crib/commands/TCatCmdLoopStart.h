/*
   @File name     : TCatCmdLoopStart.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 17:24:46
   @Last modified : 2023-06-13 17:24:46
*/

#ifndef _TCATCMDLOOPSTART_H_
#define _TCATCMDLOOPSTART_H_

#include <TCatCmd.h>

class TCatCmdLoopStart : public TCatCmd {
protected:
   TCatCmdLoopStart();

public:
   ~TCatCmdLoopStart();

   static TCatCmdLoopStart *Instance();

   virtual Long_t Cmd(vector<TString> args);

   virtual void Help();

   ClassDef(TCatCmdLoopStart, 1);
};
#endif // end of #ifdef _TCATCMDLOOPSTART_H_