/*
   @File name     : TCmdErase.cc
   @description   :
   @Author        :
   @Created date  : 2023-06-13 17:34:11
   @Last modified : 2023-06-13 17:34:11
*/

#include "TCmdErase.h"
#include "TArtemisUtil.h"
#include <TFile.h>
#include <TH1.h>
#include <TH1FTreeProj.h>
#include <TH2FTreeProj.h>
#include <TH3FTreeProj.h>

using art::TCmdErase;

ClassImp(TCmdErase);

TCmdErase::TCmdErase()
{
   SetName("erase");
   SetTitle("Erase the histograms in the current directory");
}

TCmdErase::~TCmdErase() {}

// TCmdErase* TCmdErase::Instance()
//{
//    static TCmdErase instance;
//    return &instance;
// }

Long_t TCmdErase::Cmd(vector<TString> args)
{
   /*   if (args.size() == 1) {
       // at least one arguments except command itself is required
       Help();
       return 1;
       }*/
   //   TString opt(args.size()>2 ? args[2] : "create");
   //   return Run(args[1],opt);
   return Run();
}

Long_t TCmdErase::Run()
{
   TList *objects = gDirectory->GetList();
   TDirectory *wkdir = gDirectory;
   // open file
   //   art::Util::PrepareDirectoryFor(filename);
   EraseRecursive(objects);

   wkdir->cd();
   return 1;
}

void TCmdErase::EraseRecursive(TList *list)
{
   if (!list)
      return;
   Int_t nObj = list->GetEntries();
   for (Int_t i = 0; i != nObj; i++) {
      TObject *obj = list->At(i);
      //      parent->cd();
      if (obj->InheritsFrom("TH1")) {

         /*         if (obj->InheritsFrom("art::TH1FTreeProj")) {
                  TH1F(*(art::TH1FTreeProj*)obj).Write();
               } else if (obj->InheritsFrom("art::TH2FTreeProj")) {
                  TH2F(*(art::TH2FTreeProj*)obj).Write();
               } else if (obj->InheritsFrom("art::TH2FTreeProj")) {
                  TH3F(*(art::TH3FTreeProj*)obj).Write();
             } else {*/
         TH1 *hist = (TH1 *)obj;
         hist->Reset();
         //}
      } else if (obj->InheritsFrom("TDirectory")) {
         TDirectory *dir = (TDirectory *)obj;
         EraseRecursive(dir->GetList());
      } else {
         // obj->Write();
      }
   }
}