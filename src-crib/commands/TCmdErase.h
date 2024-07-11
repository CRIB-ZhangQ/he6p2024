/*
   @File name     : TCmdErase.h
   @description   :
   @Author        :
   @Created date  : 2023-06-13 17:34:58
   @Last modified : 2023-06-13 17:34:58
*/

#ifndef _TCMDERASE_H_
#define _TCMDERASE_H_

#include <TCatCmd.h>

namespace art {
class TCmdErase;
}

class TDirectory;
class TList;

class art::TCmdErase : public TCatCmd {

public:
   TCmdErase();
   ~TCmdErase();

   // static TCmdErase* Instance();
   virtual Long_t Cmd(vector<TString> args);
   virtual Long_t Run();

protected:
   virtual void EraseRecursive(TList *list);

private:
   TCmdErase(const TCmdErase &);            // undefined
   TCmdErase &operator=(const TCmdErase &); // undefined

   ClassDef(TCmdErase, 1);
};
#endif // end of #ifndef _TCMDERASE_H_