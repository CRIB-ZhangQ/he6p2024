/*
   @File name     : TCmdXstatus.h
   @description   : command xstatus
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 10:56:18
   @Last modified : 2023-06-13 11:09:16
*/

#ifndef _TCMDXSTATUS_H_
#define _TCMDXSTATUS_H_

#include <TCatCmd.h>

namespace art {
class TCmdXstatus;
}

class TF1;

class art::TCmdXstatus : public TCatCmd {
public:
   TCmdXstatus();
   ~TCmdXstatus();

   static const char *kFuncNameBase;
   static const char *kLabelNameBase;
   Long_t Cmd(vector<TString>);
   void Help();

private:
   TCmdXstatus(const TCmdXstatus &);            // undefined
   TCmdXstatus &operator=(const TCmdXstatus &); // undefined

   ClassDef(TCmdXstatus, 1);
};

#endif // end of #ifndef _TCMDXSTATUS_H_