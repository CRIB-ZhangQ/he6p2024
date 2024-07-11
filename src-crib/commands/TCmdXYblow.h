/*
   @File name     : TCmdXYblow.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 17:37:11
   @Last modified : 2023-06-13 17:45:15
*/

#ifndef _TCMDXYBLOW_H_
#define _TCMDXYBLOW_H_

#include <TCatCmd.h>

namespace art {
class TCmdXYblow;
}

class TH2;

class art::TCmdXYblow : public TCatCmd {
public:
   TCmdXYblow();
   ~TCmdXYblow();

   static const char *kFuncNameBase;
   static const char *kLabelNameBase;
   Long_t Cmd(vector<TString>);
   TH2 *Run(TH2 *h2, Option_t *opt = "");
   void Help();

private:
   TCmdXYblow(const TCmdXYblow &);            // undefined
   TCmdXYblow &operator=(const TCmdXYblow &); // undefined

   ClassDef(TCmdXYblow, 1)
};

#endif // end of #ifndef _TCMDXYBLOW_H_