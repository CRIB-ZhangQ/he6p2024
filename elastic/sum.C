#include <iostream>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH2D.h>

// parameters for calculating the cross section
const Double_t ch2_thickness = 4.65;// 50 um -> 4.65 mg/cm2 
const Double_t NA = 6.023e23;
const Double_t z_carbon = 5;
const Double_t z_hydrogen = 1;
const Double_t m_carbon = 12;
const Double_t m_hydrogen = 1;
const Double_t ch2_density = 0.93; //solid target: g/cm3
const Double_t cm2mb = 1.0e27; //mb
const Double_t downscale = 1.0e4;



void sum(){
  gROOT->ProcessLine("zone");
  TCanvas *canvas = new TCanvas("canvas","canvas",800,600);
  canvas->Divide(2, 3);

  const Int_t numofTel = 6;
  TFile *fphy = new TFile("elastic_phy_0099.root");
  TFile *fbg = new TFile("elastic_phy_141_wo_mwdca.root");
  TFile *fsinglebeam_phy = new TFile("output/single/gate/phyrun/run0099.root");
  TFile *fsinglebeam_bg = new TFile("output/single/gate/phyrun/run0141.root");
  TFile *fsolidangle = new TFile("output/sim/solidangle.hist.root");

  if(!fphy && !fsolidangle && !fsinglebeam_phy && !fbg && fsinglebeam_bg){
    std::cerr<<"Error: could not open the root file"<<std::endl;
    exit(0);
  }

  TTree *tree_phy = (TTree*)fsinglebeam_phy->Get("tree"); 
  TTree *tree_bg = (TTree*)fsinglebeam_bg->Get("tree"); 

  TDirectoryFile *dirfile_phy[numofTel];
  TDirectoryFile *dirfile_bg[numofTel];
  TH1D *telA_phy[numofTel];
  TH1D *telA_bg[numofTel];

  for(Int_t tel = 0; tel < numofTel; tel++){
    dirfile_phy[tel] = (TDirectoryFile*)fphy->Get(Form("tel%d",tel+1));
    dirfile_bg[tel] = (TDirectoryFile*)fbg->Get(Form("tel%d",tel+1));
    if(!dirfile_phy[tel] && !dirfile_bg[tel]){
      std::cerr<<"Error: Could not find directory in file"<<std::endl;
      fphy->Close();
      fbg->Close();
      exit(0);
    }
    telA_phy[tel] = (TH1D*)(dirfile_phy[tel]->Get(Form("tel%d_theta_cm",tel+1)));
    telA_bg[tel] = (TH1D*)(dirfile_bg[tel]->Get(Form("tel%d_theta_cm",tel+1)));
    if(!telA_phy[tel] && !telA_bg[tel]){
      std::cerr<<"Error: Could not find directory in file"<<std::endl;
      fphy->Close();
      exit(0);
    }
  }

  TH1D* telAll_phy = new TH1D("telAll_phy","telAll_phy",180,0,180);
  TH1D* telAll_bg = new TH1D("telAll_bg","telAll_bg",180,0,180);

  for(Int_t tel = 0; tel < numofTel; tel++){
    telAll_phy->Add(telA_phy[tel]);
    telAll_bg->Add(telA_bg[tel]);
  }

  telAll_phy->SetLineColor(kRed);;
  canvas->cd(1);
  gStyle->SetOptStat(0);
  telAll_phy->SetTitle("with mwdca and mwdcb");
  telAll_phy->Draw("hist");
  canvas->Update();
  canvas->cd(2);
  gStyle->SetOptStat(0);
  telAll_phy->SetTitle("with mwdcb (Blue curve)");
  telAll_phy->Draw("hist");
  canvas->Update();
  canvas->cd(3);
  gStyle->SetOptStat(0);
  telAll_bg->SetTitle("Background without normalization");
  telAll_bg->Draw("hist");
  canvas->Update();

  //telAll_phy->SetFillColor(kRed);
  


  Double_t Nbeam_phy = tree_phy->GetEntries("") * downscale;
  Double_t Nbeam_bg = tree_bg->GetEntries("") * downscale;
  Double_t beam_factor = Nbeam_phy / Nbeam_bg;

  telAll_bg->Scale(beam_factor);

  canvas->cd(4);
  gStyle->SetOptStat(0);
  telAll_bg->SetTitle("Background with normalization");
  telAll_bg->Draw("hist");
  canvas->Update();
  


  TH1D* telAll = new TH1D("telAll","telAll",180,0,180);

  telAll->Sumw2();
  telAll->Add(telAll_phy, telAll_bg, 1.0, -1.0 * beam_factor);


  Int_t min_angle = telAll->GetXaxis()->FindBin(35);
  Int_t max_angle = telAll->GetXaxis()->FindBin(170);
  //telAll->GetXaxis()->SetRange(min_angle, max_angle);

  TH1D* telsolidangle = (TH1D*)fsolidangle->Get("SolidAngle_1D");

  canvas->cd(5);
  telsolidangle->Draw("hist");
  canvas->Update();

  Double_t Ntarget = ch2_thickness *1.0e-3 * NA * z_hydrogen / (m_carbon * z_carbon + m_hydrogen * z_hydrogen);

  Double_t cs_factor = Ntarget * Nbeam_phy / cm2mb; 

  telAll->Divide(telsolidangle);
  telAll->Scale(1.0/cs_factor);



  canvas->cd(6);
  gPad->SetLogy();
  canvas->SetLogy();
  gStyle->SetOptStat(0);
  telAll->SetTitle("Cross section (mb/sr)");
  telAll->SetLineColor(kRed);;
  //telAll->SetFillColor(kRed);

  telAll->Draw("");
  canvas->SaveAs("elastic/CS_cm.png");

  //fphy->Close();
  //delete fphy;
  //delete canvas;
  //return 0;

}
