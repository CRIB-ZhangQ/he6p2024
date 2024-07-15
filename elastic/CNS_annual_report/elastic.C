#include <iostream>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TMath.h>
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TColor.h"
#include <TLatex.h>

const Double_t Pi = 4.0 * atan(1);
const Double_t deg2rad = Pi / 180.0;
const Double_t  u = 931.49432;
Double_t Ebeam = 45.0;
Double_t Qvalue = -1.8;


const Double_t mass_p0[4] = {6.01885889,    1.007825031898, 6.01885889,    1.007825031898};
const Double_t mass_p1[4] = {6.01885889,    1.007825031898, 6.01885889,    1.007825031898}; 
const Double_t mass_tp[4]{3.01604927767, 1.007825031898, 3.01604927767, 1.007825031898};
const Int_t charge_p0[4]  = {2, 1, 2, 1}; 
const Int_t charge_p1[4]  = {2, 1, 2, 1}; 
const Int_t charge_tp[4]  = {1, 1, 1, 1}; 

void kinematical(Double_t Ebeam,const Double_t mass[],const Int_t charge[], Double_t Qvalue, Double_t angle[], Double_t energy[]);


void elastic()
{
  TCanvas *canvas = new TCanvas("canvas","canvas",800,600);
  const Int_t numofTel = 6;
  TFile *fphy = new TFile("elastic_phy.root");
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
  for(Int_t tel = 0; tel < numofTel; tel++){
    telEA[tel] = (TH2D*)(dirfile[tel]->Get(Form("tel%d_energy_theta_lab",tel+1)));
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

  //telAll->GetXaxis()->SetTitle("#bf {#it{#theta}_{lab} (deg.)}");
  telAll->GetXaxis()->SetTitle("#bf{#it{#theta}_{lab} (deg.)}");
  telAll->GetXaxis()->CenterTitle(true);
  telAll->GetXaxis()->SetTitleOffset(0.9);
  telAll->GetXaxis()->SetLabelFont(62);
  telAll->GetXaxis()->SetTitleSize(0.045);
  //telAll->GetYaxis()->SetTitle("#bf {"Proton energy (MeV)"});
  telAll->GetYaxis()->SetTitle("#bf{Proton energy (MeV)}");
  telAll->GetYaxis()->CenterTitle(true);
  telAll->GetYaxis()->SetTitleOffset(0.8);
  telAll->GetYaxis()->SetLabelFont(62);
  telAll->GetYaxis()->SetTitleSize(0.045);
  telAll->GetZaxis()->SetLabelFont(42);
  telAll->GetZaxis()->SetTitleOffset(1);
  telAll->GetZaxis()->SetTitleFont(42);
  telAll->Draw("colz");

  gStyle->SetOptStat(0);

  telAll->SetTitle("");
  telAll->Draw("colz");

  const Int_t numOfBins = 179;
  Double_t angle[numOfBins] {0.0};
  Double_t energy[numOfBins] {0.0};

  //6He(p,p0)6He reaction, Ebeam = 43.44, Qvalue = 0.0;
  Ebeam = 45.0, Qvalue = 0.0;
  kinematical(Ebeam, mass_p0, charge_p0, Qvalue, angle, energy);
  TGraph *kine_p0 = new TGraph(numOfBins, angle, energy);

  //6He(p,p1)6He reaction, Ebeam = 43.44, Qvalue = 0.0;
  Ebeam = 45.0, Qvalue = -1.8;
  kinematical(Ebeam, mass_p0, charge_p0, Qvalue, angle, energy);
  TGraph *kine_p1 = new TGraph(numOfBins, angle, energy);


  //3H(p,p0)3H reaction, Ebeam = 21.673, Qvalue = 0.0;
  Ebeam = 22.45, Qvalue = 0.0;
  kinematical(Ebeam, mass_tp, charge_tp, Qvalue, angle, energy);
  TGraph *kine_tp = new TGraph(numOfBins, angle, energy);

  kine_p0->SetLineWidth(3);
  kine_p1->SetLineWidth(3);
  kine_tp->SetLineWidth(3);


  kine_p0->Draw("same");
  kine_p1->Draw("same");
  kine_tp->Draw("same");

  kine_p0->SetLineStyle(1);
  kine_p1->SetLineStyle(2);
  kine_tp->SetLineStyle(4);
  kine_p0->SetLineColor(2);
  kine_p1->SetLineColor(1);
  kine_tp->SetLineColor(4);


  TLegend *legend = new TLegend(0.6328321,0.7256944,0.887494,0.8813056,NULL,"brNDC");
  legend->SetTextSize(0.035);
  legend->SetTextFont(132);
  legend->SetBorderSize(0);
  legend->AddEntry(kine_p0,"#bf{^{6}He(#it{p}, #it{p}_{0})^{6}He}","l");
  legend->AddEntry(kine_p1,"#bf{^{6}He(#it{p}, #it{p}_{1})^{6}He}","l");
  legend->AddEntry(kine_tp,"#bf{^{3}H(#it{p}, #it{p}_{0})^{3}H}","l");
  legend->Draw("same");

  canvas->SaveAs("telAllEA_lab_raw.png");

  //fphy->Close();
  //delete fphy;
  //delete canvas;
}


void kinematical(Double_t Ebeam,const Double_t mass[],const Int_t charge[], Double_t Qvalue, Double_t angle[], Double_t energy[]){
  Double_t mass_a = mass[0], mass_A = mass[1], mass_b = mass[2], mass_B = mass[3];
  Double_t charge_a = charge[0], charge_A = charge[1], charge_b = charge[2], charge_B = charge[3];
  Double_t pa2 = 2.0 * Ebeam * mass_a; 
  Double_t velocity_a = sqrt(pa2 / (mass_a * mass_a));
  Double_t velocity_cm = velocity_a * mass_a / (mass_a + mass_A);
  Double_t pb_cm2 = (pa2 * mass_A / (2.0 * mass_a * (mass_A + mass_a)) + Qvalue) * 2.0 * mass_b * mass_B / (mass_b+mass_B);
  Double_t pb_cm = sqrt(pb_cm2);
  Double_t velocity_b = pb_cm / mass_b;

  for(int i = 1; i<180; i++){
    Double_t alpha = atan(sin(i * deg2rad) / (mass_b * velocity_cm / pb_cm + cos(i * deg2rad)));
    Double_t beta  = atan(sin(i * deg2rad) / (mass_B * velocity_cm / pb_cm - cos(i * deg2rad)));

    alpha = alpha / deg2rad;
    beta  = beta  / deg2rad;

    Double_t energy_b = 0.5 * mass_b * (velocity_cm * velocity_cm + velocity_b * velocity_b + 2.0 * velocity_b * velocity_cm * cos(i * deg2rad)); 
    Double_t energy_B = Ebeam + Qvalue - energy_b;

    angle[i-1] = beta;
    energy[i-1] = energy_B;
  }
}




  
