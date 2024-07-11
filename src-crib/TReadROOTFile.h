/*
   @File name     : TReadROOTFile.h
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 16:06:58
   @Last modified : 2023-06-13 16:06:58
*/

#ifndef _TREADROOTFILE_H_
#define _TREADROOTFILE_H_

#include <TProcessor.h>
#include <TFile.h>

namespace art {
class TReadROOTFile;
}

class TClonesArray;

class art::TReadROOTFile : public TProcessor {
public:
   TReadROOTFile();
   virtual ~TReadROOTFile();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputName;

   TFile* fFile; //!

private:
   TReadROOTFile(const TReadROOTFile &);
   TReadROOTFile &operator=(const TReadROOTFile &);

   ClassDef(TReadROOTFile, 0); // simple data mapper
};

#endif // end of #ifndef _TREADROOTFILE_H_
