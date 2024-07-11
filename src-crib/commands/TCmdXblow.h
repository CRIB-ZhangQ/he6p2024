/*
   @File name     : TCmdXblow.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 17:47:20
   @Last modified : 2023-06-13 17:49:32
*/

#ifndef _TCMDXBLOW_H_
#define _TCMDXBLOW_H_

#include <TCatCmd.h>

namespace art {
class TCmdXblow;
}

class TH1;

class art::TCmdXblow : public TCatCmd {
public:
   TCmdXblow();
   ~TCmdXblow();

   static const char *kFuncNameBase;
   static const char *kLabelNameBase;
   Long_t Cmd(vector<TString>);
   TH1 *Run(TH1 *h1, Option_t *opt = "");
   void Help();

private:
   TCmdXblow(const TCmdXblow &);            // undefined
   TCmdXblow &operator=(const TCmdXblow &); // undefined

   ClassDef(TCmdXblow, 1);
};

#endif // end of #ifndef _TCMDXBLOW_H_