/*
   @File name     : TCmdXfitg.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 17:52:08
   @Last modified : 2023-06-13 17:52:42
*/

#ifndef _TCMDXFITG_H_
#define _TCMDXFITG_H_

#include "TCatCmd.h"

namespace art {
class TCmdXfitg;
}

class TF1;

class art::TCmdXfitg : public TCatCmd {
public:
   TCmdXfitg();
   ~TCmdXfitg();

   static const char *kFuncNameBase;
   static const char *kLabelNameBase;
   Long_t Cmd(vector<TString>);
   void Help();

private:
   TCmdXfitg(const TCmdXfitg &);            // undefined
   TCmdXfitg &operator=(const TCmdXfitg &); // undefined

   ClassDef(TCmdXfitg, 1);                  // Obtain slope graphically
};

#endif // end of #ifndef _TCMDXFITG_H_