#include <iostream>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH2D.h>
void elastic_wo_mwdca()
{
  TCanvas *canvas = new TCanvas("canvas","canvas",800,600);
  const Int_t numofTel = 6;
  TFile *fphy = new TFile("elastic_phy_test.root");
  if(!fphy){
    std::cerr<<"Error: could not open the root file"<<std::endl;
  }
  TDirectoryFile *dirfile[numofTel];
  //TDirectoryFile *dirfile = (TDirectoryFile*)fphy->Get(Form("tel1"));
  for(Int_t tel = 0; tel < numofTel; tel++){
    dirfile[tel] = (TDirectoryFile*)fphy->Get(Form("tel%d",tel+1));
    if(!dirfile[tel]){
      std::cerr<<"Error: Could not find directory in file"<<std::endl;
      fphy->Close();
      return;
    }
  }

  TH2D *telEA[numofTel];
  //TH2D *telEA = (TH2D*)dirfile->Get(Form("tel1_energy_theta_lab"));
  for(Int_t tel = 0; tel < numofTel; tel++){
    telEA[tel] = (TH2D*)(dirfile[tel]->Get(Form("tel%d_energy_theta_cm",tel+1)));
    if(!telEA[tel]){
      std::cerr<<"Error: Could not find directory in file"<<std::endl;
      fphy->Close();
      return;
    }
  }

  TH2D* telAll = telEA[0];
  for(Int_t tel = 0; tel < numofTel; tel++){
    telAll->Add(telEA[tel]);
  }
  telAll->Draw("colz");
  canvas->SaveAs("telAllEA_cm_raw_wo_mwdca.png");

  fphy->Close();
  delete fphy;
  delete canvas;
}
