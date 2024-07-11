/*
   @File name     : TCmdDraw.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 17:52:08
   @Last modified : 2023-06-13 17:52:42
*/

#ifndef _TCMDDRAW_H_
#define _TCMDDRAW_H_

#include "TCatCmd.h"

namespace art {
class TCmdDraw;
}

class TF1;

class art::TCmdDraw : public TCatCmd {
public:
   TCmdDraw();
   ~TCmdDraw();

   static const char *kFuncNameBase;
   static const char *kLabelNameBase;
   Long_t Cmd(vector<TString>);
   void Help();

private:
   TCmdDraw(const TCmdDraw &);            // undefined
   TCmdDraw &operator=(const TCmdDraw &); // undefined

   ClassDef(TCmdDraw, 1);                  // Obtain slope graphically
};

#endif // end of #ifndef _TCMDDRAW_H_
