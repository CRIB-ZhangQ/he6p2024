/*
   @File name     : TSolidAngleProcessor.cc
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-08-01 11:07:56
   @Last modified : 2023-08-01 11:08:16
*/

#include "TSolidAngleProcessor.h"
#include "TReactionInfo.h"
#include "../telescope/TTelescopeData.h"

#include <TDataObject.h>
#include <constant.h>
#include <TArtemisUtil.h>

#include <TClonesArray.h>
#include <TClass.h>
#include <TFile.h>
#include <TMath.h>

using art::TSolidAngleProcessor;

ClassImp(TSolidAngleProcessor);

TSolidAngleProcessor::TSolidAngleProcessor() : fInData_cm(NULL), fInData_det(NULL)
{
   RegisterInputCollection("InputCollection_cm", "input branch (collection) name", fInputColName_cm, TString("input1"));
   RegisterInputCollection("InputCollection_det", "input branch (collection) name", fInputColName_det,
                           TString("input2"));

   DoubleVec_t init_d_vec;
   RegisterProcessorParameter("Nbin_angle", "Nbin of the angle histogram", fNbin_angle, 0);
   RegisterProcessorParameter("range_angle", "range of the angle histogram", fRange_angle, init_d_vec);
   RegisterProcessorParameter("Nbin_energy", "Nbin of the energy histogram", fNbin_energy, 0);
   RegisterProcessorParameter("range_energy", "range of the energy histogram", fRange_energy, init_d_vec);
   RegisterProcessorParameter("IsInverseKinematics", "inverse kinematics true/false", fIsIK, false);
   RegisterProcessorParameter("CenterOfMass", "inverse kinematics true/false", fCenterOfMass, false);

   RegisterProcessorParameter("HistFile", "name of output histogram rootfile", fFileName, TString(""));
}

TSolidAngleProcessor::~TSolidAngleProcessor()
{
   delete h1A;
   delete h2EA;
   delete h1_all;
   delete h2_all;
   delete h1;
   delete h2;
   delete h1_0;
   delete h1_1;
   delete h1_2;
   delete h1_3;
   delete h1_4;
   delete h1_5;
   delete h1_lab;
   h1A = NULL;
   h2EA = NULL;
   h1_all = NULL;
   h2_all = NULL;
   h1 = NULL;
   h2 = NULL;
   h1_0 = NULL;
   h1_1 = NULL;
   h1_2 = NULL;
   h1_3 = NULL;
   h1_4 = NULL;
   h1_5 = NULL;
   h1_lab = NULL;
}

void TSolidAngleProcessor::Init(TEventCollection *col)
{
   Info("Init", "make solid angle histogram X: Ecm, Y: Thetacm");
   if (fRange_angle.size() != 2 || fRange_energy.size() != 2) {
      SetStateError("input yaml format is wrong");
      return;
   }
   Info("Init", "\t h(%d, %.1lf, %.1lf, %d, %.1lf, %.1lf)", fNbin_energy, fRange_energy[0], fRange_energy[1],
        fNbin_angle, fRange_angle[0], fRange_angle[1]);
   Info("Init", "making %s", fFileName.Data());

   fInData_cm = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName_cm.Data()));
   if (!fInData_cm) {
      SetStateError(TString::Format("input not found: %s", fInputColName_cm.Data()));
      return;
   }
   const TClass *const cl_cm = (*fInData_cm)->GetClass();
   if (!cl_cm->InheritsFrom(art::TReactionInfo::Class())) {
      SetStateError("contents of input array must inherit from art::TReactionInfo");
   }

   fInData_det = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName_det.Data()));
   if (!fInData_det) {
      SetStateError(TString::Format("input not found: %s", fInputColName_det.Data()));
      return;
   }
   const TClass *const cl_det = (*fInData_det)->GetClass();
   if (!cl_det->InheritsFrom(art::TTelescopeData::Class())) {
      SetStateError("contents of input array must inherit from art::TTelescopeData");
   }

   h1A = new TH1D("h1A", "h1A", fNbin_angle, fRange_angle[0], fRange_angle[1]);
   h2EA = new TH2D("h2EA", "h2EA", fNbin_angle, fRange_angle[0], fRange_angle[1],
                                  fNbin_energy, fRange_energy[0], fRange_energy[1]);
   h1_all = new TH1D("normalize_1D", "normalize_1D", fNbin_angle, fRange_angle[0], fRange_angle[1]);
   h2_all = new TH2D("normalize_2D", "normalize_2D", fNbin_angle, fRange_angle[0], fRange_angle[1],
                                                     fNbin_energy, fRange_energy[0], fRange_energy[1]);
   h1 = new TH1D("SolidAngle_1D", "SolidAngle_1D", fNbin_angle, fRange_angle[0], fRange_angle[1]);
   h2 = new TH2D("SolidAngle_2D", "SolidAngle_2D", fNbin_angle, fRange_angle[0], fRange_angle[1],
                                                   fNbin_energy, fRange_energy[0], fRange_energy[1]);
   h1_0 = new TH1D("tel1", "tel1", fNbin_angle, fRange_angle[0], fRange_angle[1]);
   h1_1 = new TH1D("tel2", "tel2", fNbin_angle, fRange_angle[0], fRange_angle[1]);
   h1_2 = new TH1D("tel3", "tel3", fNbin_angle, fRange_angle[0], fRange_angle[1]);
   h1_3 = new TH1D("tel4", "tel4", fNbin_angle, fRange_angle[0], fRange_angle[1]);
   h1_4 = new TH1D("tel5", "tel5", fNbin_angle, fRange_angle[0], fRange_angle[1]);
   h1_5 = new TH1D("tel6", "tel6", fNbin_angle, fRange_angle[0], fRange_angle[1]);

   //for check the solid angle after converting the Omega_cm t0 Omega_lab
   h1_lab = new TH1D("h1_lab", "h1_lab", fNbin_angle / 2, fRange_angle[0] / 2, fRange_angle[1] / 2);


   for(Int_t Ebin = 0; Ebin < fNbin_energy; Ebin++){
     Double_t energy_center = fRange_energy[0] + (fRange_energy[1] - fRange_energy[0]) / (2.0 * fNbin_energy) + (Double_t)Ebin * (fRange_energy[1] - fRange_energy[0]) / fNbin_energy;
     for (Int_t Abin = 0; Abin < fNbin_angle; Abin++) {
       // Calculate the bin center of the angle in the center of mass system
       Double_t angle_center = fRange_angle[0] + (fRange_angle[1] - fRange_angle[0]) / (2.0 * fNbin_angle) + (Double_t)Abin * (fRange_angle[1] - fRange_angle[0]) / fNbin_angle;
       //conversion factor: convert solid angle from the center of mass system to the laboratory system 
       Double_t fOmegaCm2Lab = OmegaCM2LAB(angle_center);

       // in the c.m. system
       h1A->Fill(angle_center, cos(Abin*TMath::DegToRad())-cos((Abin+1)*TMath::DegToRad()));
       h2EA->Fill(angle_center, energy_center, cos(Abin*TMath::DegToRad())-cos((Abin+1)*TMath::DegToRad()));

       //check the output by converting to the lab system
       //h1A->Fill(angle_center, fOmegaCm2Lab * (cos(Abin*TMath::DegToRad())-cos((Abin+1)*TMath::DegToRad())));
       //h2EA->Fill(angle_center, energy_center, fOmegaCm2Lab * (cos(Abin*TMath::DegToRad())-cos((Abin+1)*TMath::DegToRad())));
     }
   }

   h1A->Scale(1.0/fNbin_energy);
}

void TSolidAngleProcessor::Process()
{
   if ((*fInData_cm)->GetEntriesFast() != 1) {
      SetStateError("input reaction data branch entry is not 1");
      return;
   }

   const TDataObject *const inData = static_cast<TDataObject *>((*fInData_cm)->At(0));
   const TReactionInfo *const Data = dynamic_cast<const TReactionInfo *>(inData);
   const TDataObject *const inData_det = static_cast<TDataObject *>((*fInData_det)->At(0));
   const TTelescopeData *const ftel = dynamic_cast<const TTelescopeData *>(inData_det);

   if(fCenterOfMass){
     h1_all->Fill(Data->GetTheta());
     h2_all->Fill(Data->GetTheta(), Data->GetEnergy());
   }else{
     for (Int_t i = 0; i < fNbin_angle; i++) {
        Double_t angle_center = fRange_angle[0] + (fRange_angle[1] - fRange_angle[0]) / (2.0 * fNbin_angle) +
                                (Double_t)i * (fRange_angle[1] - fRange_angle[0]) / fNbin_angle;
        h1_all->Fill(angle_center);
        //h2_all->Fill(angle_center, Data->GetEnergy());
     }
   }

   if ((*fInData_det)->GetEntriesFast() > 0) {
      if ((*fInData_det)->GetEntriesFast() > 1) {
         std::cerr << "not single particle detected" << std::endl;
         return;
      }

      // for each telescope
      Int_t det_ID = ftel->GetID();
      if(det_ID==1) h1_0->Fill(Data->GetTheta());
      else if(det_ID==2) h1_1->Fill(Data->GetTheta());
      else if(det_ID==3) h1_2->Fill(Data->GetTheta());
      else if(det_ID==4) h1_3->Fill(Data->GetTheta());
      else if(det_ID==5) h1_4->Fill(Data->GetTheta());
      else h1_5->Fill(Data->GetTheta());


      //for total solidangle
      h1->Fill(Data->GetTheta());
      h2->Fill(Data->GetTheta(), Data->GetEnergy());
   }
}

void TSolidAngleProcessor::PostLoop()
{
   Util::PrepareDirectoryFor(fFileName);
   TFile *file = new TFile(fFileName, "recreate");
   if (!file) {
      std::cerr << "ERROR : cannot open " << fFileName << std::endl;
      return;
   }

   // for the nuclear reaction
   if(fCenterOfMass){
     h1->Multiply(h1,h1A, 2.0 * TMath::Pi())    , h1->Divide(h1_all);
     //h2->Multiply(h2,h2EA, 2.0 * TMath::Pi())   , h2->Divide(h2_all);
     h1_0->Multiply(h1_0,h1A, 2.0 * TMath::Pi()), h1_0->Divide(h1_all);
     h1_1->Multiply(h1_1,h1A, 2.0 * TMath::Pi()), h1_1->Divide(h1_all);
     h1_2->Multiply(h1_2,h1A, 2.0 * TMath::Pi()), h1_2->Divide(h1_all);
     h1_3->Multiply(h1_3,h1A, 2.0 * TMath::Pi()), h1_3->Divide(h1_all);
     h1_4->Multiply(h1_4,h1A, 2.0 * TMath::Pi()), h1_4->Divide(h1_all);
     h1_5->Multiply(h1_5,h1A, 2.0 * TMath::Pi()), h1_5->Divide(h1_all);
   }else{
     // for the radioactive decay 
     h1->Scale(4.0 * TMath::Pi()) ,  h1->Divide(h1_all);
     h2->Scale(4.0 * TMath::Pi()) ,  h2->Divide(h2_all);
     h1_0->Scale(4.0 * TMath::Pi()), h1_0->Divide(h1_all);
     h1_1->Scale(4.0 * TMath::Pi()), h1_1->Divide(h1_all);
     h1_2->Scale(4.0 * TMath::Pi()), h1_2->Divide(h1_all);
     h1_3->Scale(4.0 * TMath::Pi()), h1_3->Divide(h1_all);
     h1_4->Scale(4.0 * TMath::Pi()), h1_4->Divide(h1_all);
     h1_5->Scale(4.0 * TMath::Pi()), h1_5->Divide(h1_all);
   }

   //check the solid anlge in the lab. system
   //for(Int_t angle_bin = 1; angle_bin < h1->GetNbinsX(); angle_bin++){
   //  double angle_lab = (180.0 - h1->GetXaxis()->GetBinCenter(angle_bin)) / 2.0;
   //  double SolidAngle = h1->GetBinContent(angle_bin);
   //  std::cout<<"angle_bin = "<<angle_bin<<", angle = "<<angle_lab<<", solidangle = "<<SolidAngle<<std::endl;
   //  h1_lab->Fill(angle_lab, SolidAngle);
   //}
     

   h1->Write("SolidAngle_1D");
   h2->Write("SolidAngle_2D");

   file->Close();
}

Double_t TSolidAngleProcessor::OmegaCM2LAB(Double_t thetacm){

 //For the reaction A(a,b)B
  Double_t Mass[4] = {6.01885889, 1.007825031898, 6.01885889, 1.007825031898};
  Double_t E_6He = 45.0;
  Double_t deg2rad = TMath::DegToRad();
  Double_t mass_a = Mass[0], mass_A = Mass[1] , mass_b = Mass[2] , mass_B = Mass[3];
  Double_t Ecm = E_6He * mass_A / (mass_a + mass_A);
  Double_t gamma_beta = TMath::Sqrt(fGamma());
  Double_t fOmegalab2cm = abs((gamma_beta*cos(thetacm*deg2rad) - 1.0) / pow((1.0 + gamma_beta*gamma_beta - 2.0*gamma_beta*cos(thetacm*deg2rad)), 1.5)); 
  return fOmegalab2cm; 
}

Double_t TSolidAngleProcessor::fGamma(){
  Double_t Mass[4] = {6.01885889, 1.007825031898, 6.01885889, 1.007825031898};
  Double_t E_6He = 45.0;
  Double_t Qvalue = 0.0;
  Double_t mass_a = Mass[0], mass_A = Mass[1] , mass_b = Mass[2] , mass_B = Mass[3];
  Double_t Ecm = E_6He * mass_A / (mass_a + mass_A);
  return mass_a*mass_B*(mass_b+mass_B)*Ecm/(mass_A*mass_b*(mass_a+mass_A)*(Ecm+Qvalue));
}
