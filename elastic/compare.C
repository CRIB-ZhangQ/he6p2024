#include <iostream>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH2D.h>
#include <TLegend.h>

// parameters for calculating the cross section
const Double_t ch2_thickness = 4.65;// 50 um -> 4.65 mg/cm2 
const Double_t NA = 6.023e23;
const Double_t z_carbon = 5;
const Double_t z_hydrogen = 1;
const Double_t m_carbon = 12;
const Double_t m_hydrogen = 1;
const Double_t ch2_density = 0.93; //solid target: g/cm3
const Double_t cm2mb = 1.0e27; //mb
const Double_t downscale = 1.0e4; //mb



void compare()
{
  TCanvas *canvas = new TCanvas("canvas","canvas",800,600);

  const Int_t numofTel = 6;
  TFile *fphy = new TFile("elastic_phy_0099.root");
  TFile *fphy_wo = new TFile("elastic_phy_0099_wo_mwdca.root");
  TFile *fsolidangle = new TFile("output/sim/solidangle.hist.root");
  TFile *fsinglebeam = new TFile("output/single/gate/phyrun/run0099.root");

  if(!fphy && !fphy_wo && !fsolidangle && !fsinglebeam){
    std::cerr<<"Error: could not open the root file"<<std::endl;
    return;
  }


  TTree *tree = (TTree*)fsinglebeam->Get("tree"); 

  TDirectoryFile *dirfile[numofTel];
  TDirectoryFile *dirfile_wo[numofTel];
  for(Int_t tel = 0; tel < numofTel; tel++){
    dirfile[tel] = (TDirectoryFile*)fphy->Get(Form("tel%d",tel+1));
    dirfile_wo[tel] = (TDirectoryFile*)fphy_wo->Get(Form("tel%d",tel+1));
    //if(!dirfile[tel]){
    if(!dirfile[tel] && !dirfile[tel]){
      std::cerr<<"Error: Could not find directory in file"<<std::endl;
      fphy->Close();
      fphy_wo->Close();
      return;
    }
  }

  TH1D *telA[numofTel];
  TH1D *telA_wo[numofTel];
  for(Int_t tel = 0; tel < numofTel; tel++){
    telA[tel] = (TH1D*)(dirfile[tel]->Get(Form("tel%d_theta_cm",tel+1)));
    telA_wo[tel] = (TH1D*)(dirfile_wo[tel]->Get(Form("tel%d_theta_cm",tel+1)));
    if(!telA[tel] && !telA_wo[tel]){
      std::cerr<<"Error: Could not find directory in file"<<std::endl;
      fphy->Close();
      fphy_wo->Close();
      return;
    }
  }

  TH1D* telAll = new TH1D("telAll","telAll",180,0,180);
  TH1D* telAll_wo = new TH1D("telAll_wo_mwdca","telAll_wo_mwdca",180,0,180);
  //TH1D* telAll = telA[0];
  //TH1D* telAll_wo = telA_wo[0];
  for(Int_t tel = 0; tel < numofTel; tel++){
    telAll->Add(telA[tel]);
    telAll_wo->Add(telA_wo[tel]);
  }

  Int_t min_angle = telAll->GetXaxis()->FindBin(35);
  Int_t max_angle = telAll->GetXaxis()->FindBin(170);

  telAll->GetXaxis()->SetRange(min_angle, max_angle);
  telAll_wo->GetXaxis()->SetRange(min_angle, max_angle);

  TH1D* telsolidangle = (TH1D*)fsolidangle->Get("SolidAngle_1D");

  telsolidangle->Draw("hist");
  canvas->SaveAs("elastic/solidangle_cm.png");

  Double_t Ntarget = ch2_thickness *1.0e-3 * NA * z_hydrogen / (m_carbon * z_carbon + m_hydrogen * z_hydrogen);
  Double_t Nbeam = tree->GetEntries();
  Double_t cs_factor = Ntarget * Nbeam * downscale / cm2mb; 

  telAll->Divide(telsolidangle);
  telAll->Scale(1.0/cs_factor);


  telAll_wo->Divide(telsolidangle);
  telAll_wo->Scale(1.0/cs_factor);

  //canvas->SetLogy();
  gStyle->SetOptStat(0);

  telAll->SetLineColor(kRed);;
  telAll_wo->SetLineColor(kBlue);;

  telAll->SetFillColor(kRed);
  //telAll_wo->SetFillColor(kBlue);

  telAll->Draw("hist");
  telAll_wo->Draw("histsame");



  TLegend *legend = new TLegend(0.4328321,0.7256944,0.887494,0.8813056,NULL,"brNDC");
  legend->SetTextSize(0.035);
  legend->SetTextFont(132);
  legend->SetBorderSize(0);
  legend->AddEntry(telAll,"With both mwdca and mwdcb","l");
  legend->AddEntry(telAll_wo,"With only mwdcb","l");
  legend->Draw("same");

  telAll->SetTitle("Cross section of the ^{6}He(p,p)^{6}He reaction without B.G.");
  telAll_wo->SetTitle("Cross section of the ^{6}He(p,p)^{6}He reaction without B.G.");
  canvas->SaveAs("elastic/compare.png");


  //fphy->Close();
  //fphy_wo->Close();
  //delete fphy;
  //delete fphy_wo;
  //delete canvas;
}
