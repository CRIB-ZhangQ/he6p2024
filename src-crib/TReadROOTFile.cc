/*
   @File name     : TReadROOTFile.cc
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 16:06:15
   @Last modified : 2023-06-13 16:06:15
*/

#include "TReadROOTFile.h"

#include <TClass.h>

#include "constant.h"

using art::TReadROOTFile;

ClassImp(TReadROOTFile);

TReadROOTFile::TReadROOTFile()
{
   RegisterProcessorParameter("FileName","name of the ROOT file", fInputName, TString(""));
}

TReadROOTFile::~TReadROOTFile()
{
  fFile->Close();
  delete fFile;
}

void TReadROOTFile::Init(TEventCollection *col)
{
   Info("Init", "read %s", fInputName.Data());

   fFile = new TFile(fInputName.Data(), "read");
   if (!fFile) {
      std::cerr << "ERROR : firstly update command needed" << std::endl;
      exit(1);
   }
}

void TReadROOTFile::Process()
{
}
