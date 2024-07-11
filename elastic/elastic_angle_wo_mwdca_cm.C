#include <iostream>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH2D.h>

// parameters for calculating the cross section
const Double_t ch2_thickness = 4.65;// 50 um -> 4.65 mg/cm2 
const Double_t NA = 6.023e23;
const Double_t z_carbon = 1;
const Double_t z_hydrogen = 1;
const Double_t m_carbon = 12;
const Double_t m_hydrogen = 1;
const Double_t ch2_density = 0.93; //solid target: g/cm3
const Double_t cm2mb = 1.0e27; //mb
const Double_t downscale = 1.0e4; //mb



void elastic_angle_wo_mwdca_cm()
{
  TCanvas *canvas = new TCanvas("canvas","canvas",800,600);

  const Int_t numofTel = 6;
  TFile *fphy = new TFile("elastic_phy_test.root");
  TFile *fsolidangle = new TFile("output/sim/solidangle.hist.root");
  TFile *fsinglebeam = new TFile("output/single/gate/phyrun/run0099.root");

  if(!fphy && !fsolidangle && !fsinglebeam){
    std::cerr<<"Error: could not open the root file"<<std::endl;
    return;
  }


  TTree *tree = (TTree*)fsinglebeam->Get("tree"); 

  TDirectoryFile *dirfile[numofTel];
  for(Int_t tel = 0; tel < numofTel; tel++){
    dirfile[tel] = (TDirectoryFile*)fphy->Get(Form("tel%d",tel+1));
    if(!dirfile[tel]){
      std::cerr<<"Error: Could not find directory in file"<<std::endl;
      fphy->Close();
      return;
    }
  }

  TH1D *telA[numofTel];
  for(Int_t tel = 0; tel < numofTel; tel++){
    telA[tel] = (TH1D*)(dirfile[tel]->Get(Form("tel%d_theta_cm",tel+1)));
    if(!telA[tel]){
      std::cerr<<"Error: Could not find directory in file"<<std::endl;
      fphy->Close();
      return;
    }
  }

  TH1D* telAll = telA[0];
  for(Int_t tel = 0; tel < numofTel; tel++){
    telAll->Add(telA[tel]);
  }

  Int_t min_angle = telAll->GetXaxis()->FindBin(35);
  Int_t max_angle = telAll->GetXaxis()->FindBin(170);
  telAll->GetXaxis()->SetRange(min_angle, max_angle);

  TH1D* telsolidangle = (TH1D*)fsolidangle->Get("SolidAngle_1D");

  telsolidangle->Draw("hist");
  canvas->SaveAs("elastic/solidangle_cm.png");

  Double_t Ntarget = ch2_thickness *1.0e-3 * NA * z_hydrogen / (m_carbon * z_carbon + m_hydrogen * z_hydrogen);
  Double_t Nbeam = tree->GetEntries();
  Double_t cs_factor = Ntarget * Nbeam * downscale / cm2mb; 

  telAll->Divide(telsolidangle);
  telAll->Scale(1.0/cs_factor);

  canvas->SetLogy();
  gStyle->SetOptStat(0);
  telAll->Draw("");
  canvas->SaveAs("elastic/CS_cm_test.png");

  fphy->Close();
  delete fphy;
  delete canvas;
}
