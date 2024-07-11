#include <iostream>
#include <TFile.h>
#include <TROOT.h>
#include <TTree.h>
#include <TClonesArray.h>

#include "/home/zq/Tools/artemis/artemis-share/src/mwdc/TMWDCTrackingResult.h"
#include "/home/zq/Tools/artemis/artemis-share/src/mwdc/TMWDCTrackingResult.h"
#include "/home/zq/Tools/artemis/artemis-share/src/cont/TTimingChargeData.h"
#include "/home/zq/Tools/artemis/artemis-share/src/cont/TTimingData.h"
#include "/home/zq/art_analysis/he6p/he6p/src-crib/telescope/TTelescopeData.h"
#include "/home/zq/art_analysis/he6p/he6p/src-crib/telescope/TTelescopeData.h"
#include "/home/zq/art_analysis/he6p/he6p/src-crib/telescope/TTelescopeData.h"
int main()
{
  TFile fin("/home/zq/art_analysis/he6p/he6p/output/run/0143/chkf3run0143.root");
  TFile fout("elastic.root","recreate");

  TTree *tree = (TTree*)fin.Get("tree");
  TTree *treeOut = new TTree("tree","tree");

  //rf
  TClonesArray *rf_cal_0 = new TClonesArray("art::TTimingData");
  TClonesArray *coin_raw = new TClonesArray("art::TTimingData");
  
  //mwdc cathod
  
  TClonesArray *catha_cal = new TClonesArray("art::TTimingChargeData");
  TClonesArray *cathb_cal = new TClonesArray("art::TTimingChargeData");

  //mwdca and mwdcb
  TClonesArray *mwdca = new TClonesArray("art::TMWDCTrackingResult");
  TClonesArray *mwdcb = new TClonesArray("art::TMWDCTrackingResult");
  TClonesArray *f3mwdc = new TClonesArray("art::TMWDCTrackingResult");
  
  tree->SetBranchAddress("rf_cal_0", &rf_cal_0);
  tree->SetBranchAddress("coin_raw", &coin_raw);
  tree->SetBranchAddress("catha_cal", &catha_cal);
  tree->SetBranchAddress("cathb_cal", &cathb_cal);
  tree->SetBranchAddress("mwdca", &mwdca);
  tree->SetBranchAddress("mwdcb", &mwdcb);
  tree->SetBranchAddress("f3mwdc", &f3mwdc);



  double rf0, single, coin, cathda, cathdb, mwdca_x, mwdca_y, mwdca_timing, mwdcb_x, mwdcb_y, mwdcb_timing, target_x, target_y, target_z;
  treeOut->Branch("rf0", &rf0, "rf0/D");
  treeOut->Branch("single", &single, "single/D");
  treeOut->Branch("coin", &coin, "coin/D");
  treeOut->Branch("cathda", &cathda,"cathda/D");
  treeOut->Branch("cathdb", &cathdb,"cathdb/D");
  treeOut->Branch("mwdca_x", &mwdca_x, "mwdca_x/D");
  treeOut->Branch("mwdca_y", &mwdca_y, "mwdca_y/D");
  treeOut->Branch("mwdca_timing", &mwdca_timing, "mwdca_timing/D");
  treeOut->Branch("mwdcb_x", &mwdcb_x, "mwdcb_x/D");
  treeOut->Branch("mwdcb_y", &mwdcb_y, "mwdcb_y/D");
  treeOut->Branch("mwdcb_timing", &mwdcb_timing, "mwdcb_timing/D");
  treeOut->Branch("target_x", &target_x, "target_x/D");
  treeOut->Branch("target_y", &target_y, "target_y/D");
  treeOut->Branch("target_z", &target_z, "target_z/D");


  //tel1
  //TClonesArray *tel1dEX_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel1dEY_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel1E_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel1dEX_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel1dEY_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel1E_cal = new TClonesArray("art::TimingChargeData");
  TClonesArray *tel1 = new TClonesArray("art::TTelescopeData");
 
  //tree->SetBranchAddress("tel1dEX_raw", &tel1dEX_raw);
  //tree->SetBranchAddress("tel1dEY_raw", &tel1dEY_raw);
  //tree->SetBranchAddress("tel1E_raw", &tel1E_raw);
  //tree->SetBranchAddress("tel1dEX_cal", &tel1dEX_cal);
  //tree->SetBranchAddress("tel1dEX_cal", &tel1dEY_cal);
  //tree->SetBranchAddress("tel1dEX_cal", &tel1E_cal);
  tree->SetBranchAddress("tel1", &tel1);
  double tel1dEaX,tel1dEaY,tel1Eb,tel1Ec,tel1Ed, tel1Etotal;
  double tel1dEaXTiming; 
  double tel1X, tel1Y, tel1Z;
  treeOut->Branch("tel1dEaXTiming", &tel1dEaXTiming,"tel1dEaXTiming/D");
  treeOut->Branch("tel1dEaX", &tel1dEaX,"tel1dEaX/D");
  treeOut->Branch("tel1dEaY", &tel1dEaY,"tel1dEaY/D");
  treeOut->Branch("tel1Eb", &tel1Eb,"tel1Eb/D");
  treeOut->Branch("tel1Ec", &tel1Ec,"tel1Ec/D");
  treeOut->Branch("tel1Ed", &tel1Ed,"tel1Ed/D");
  treeOut->Branch("tel1Etotal", &tel1Etotal,"tel1Etotal/D");

  //tel2 
  //TClonesArray *tel2dEX_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel2dEY_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel2E_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel2dEX_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel2dEY_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel2E_cal = new TClonesArray("art::TimingChargeData");
  TClonesArray *tel2 = new TClonesArray("art::TTelescopeData");
  //tree->SetBranchAddress("tel2dEX_raw", &tel2dEX_raw);
  //tree->SetBranchAddress("tel2dEY_raw", &tel2dEY_raw);
  //tree->SetBranchAddress("tel2E_raw", &tel2E_raw);
  //tree->SetBranchAddress("tel2dEX_cal", &tel2dEX_cal);
  //tree->SetBranchAddress("tel2dEX_cal", &tel2dEY_cal);
  //tree->SetBranchAddress("tel2dEX_cal", &tel2E_cal);
  tree->SetBranchAddress("tel2", &tel2);
  double tel2dEaX,tel2dEaY,tel2Eb,tel2Ec,tel2Ed, tel2Etotal;
  double tel2dEaXTiming; 
  double tel2X, tel2Y, tel2Z;
  treeOut->Branch("tel2dEaXTiming", &tel2dEaXTiming,"tel2dEaXTiming/D");
  treeOut->Branch("tel2dEaX", &tel2dEaX,"tel2dEaX/D");
  treeOut->Branch("tel2dEaY", &tel2dEaY,"tel2dEaY/D");
  treeOut->Branch("tel2Eb", &tel2Eb,"tel2Eb/D");
  treeOut->Branch("tel2Ec", &tel2Ec,"tel2Ec/D");
  treeOut->Branch("tel2Ed", &tel2Ed,"tel2Ed/D");
  treeOut->Branch("tel2Etotal", &tel2Etotal,"tel2Etotal/D");
  
  //tel3 
  //TClonesArray *tel3dEX_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel3dEY_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel3E_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel3dEX_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel3dEY_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel3E_cal = new TClonesArray("art::TimingChargeData");
  TClonesArray *tel3 = new TClonesArray("art::TTelescopeData");
  //tree->SetBranchAddress("tel3dEX_raw", &tel3dEX_raw);
  //tree->SetBranchAddress("tel3dEY_raw", &tel3dEY_raw);
  //tree->SetBranchAddress("tel3E_raw", &tel3E_raw);
  //tree->SetBranchAddress("tel3dEX_cal", &tel3dEX_cal);
  //tree->SetBranchAddress("tel3dEX_cal", &tel3dEY_cal);
  //tree->SetBranchAddress("tel3dEX_cal", &tel3E_cal);
  tree->SetBranchAddress("tel3", &tel3);
  double tel3dEaX,tel3dEaY,tel3Eb,tel3Ec, tel3Etotal;
  double tel3dEaXTiming; 
  double tel3X, tel3Y, tel3Z;
  treeOut->Branch("tel3dEaXTiming", &tel3dEaXTiming,"tel3dEaXTiming/D");
  treeOut->Branch("tel3dEaX", &tel3dEaX,"tel3dEaX/D");
  treeOut->Branch("tel3dEaY", &tel3dEaY,"tel3dEaY/D");
  treeOut->Branch("tel3Eb", &tel3Eb,"tel3Eb/D");
  treeOut->Branch("tel3Ec", &tel3Ec,"tel3Ec/D");
  treeOut->Branch("tel3Etotal", &tel3Etotal,"tel3Etotal/D");

  //tel4 
  //TClonesArray *tel4dEX_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel4dEY_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel4E_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel4dEX_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel4dEY_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel4E_cal = new TClonesArray("art::TimingChargeData");
  TClonesArray *tel4 = new TClonesArray("art::TTelescopeData");
  //tree->SetBranchAddress("tel4dEX_raw", &tel4dEX_raw);
  //tree->SetBranchAddress("tel4dEY_raw", &tel4dEY_raw);
  //tree->SetBranchAddress("tel4E_raw", &tel4E_raw);
  //tree->SetBranchAddress("tel4dEX_cal", &tel4dEX_cal);
  //tree->SetBranchAddress("tel4dEX_cal", &tel4dEY_cal);
  //tree->SetBranchAddress("tel4dEX_cal", &tel4E_cal);
  tree->SetBranchAddress("tel4", &tel4);
  double tel4dEaX,tel4dEaY,tel4Eb,tel4Etotal;
  double tel4dEaXTiming; 
  double tel4X, tel4Y, tel4Z;
  treeOut->Branch("tel4dEaXTiming", &tel4dEaXTiming,"tel4dEaXTiming/D");
  treeOut->Branch("tel4dEaX", &tel4dEaX,"tel4dEaX/D");
  treeOut->Branch("tel4dEaY", &tel4dEaY,"tel4dEaY/D");
  treeOut->Branch("tel4Eb", &tel4Eb,"tel4Eb/D");
  treeOut->Branch("tel4Etotal", &tel4Etotal,"tel4Etotal/D");

  //tel5 
  //TClonesArray *tel5dEX_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel5dEY_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel5E_raw = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel5dEX_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel5dEY_cal = new TClonesArray("art::TimingChargeData");
  //TClonesArray *tel5E_cal = new TClonesArray("art::TimingChargeData");
  TClonesArray *tel5 = new TClonesArray("art::TTelescopeData");
  //tree->SetBranchAddress("tel5dEX_raw", &tel5dEX_raw);
  //tree->SetBranchAddress("tel5dEY_raw", &tel5dEY_raw);
  //tree->SetBranchAddress("tel5E_raw", &tel5E_raw);
  //tree->SetBranchAddress("tel5dEX_cal", &tel5dEX_cal);
  //tree->SetBranchAddress("tel5dEY_cal", &tel5dEY_cal);
  //tree->SetBranchAddress("tel5E_cal", &tel5E_cal);
  tree->SetBranchAddress("tel5", &tel5);

  double tel5dEaX,tel5dEaY,tel5Eb,tel5Etotal;
  double tel5dEaXTiming; 
  double tel5X, tel5Y, tel5Z;
  treeOut->Branch("tel5dEaXTiming", &tel5dEaXTiming,"tel5dEaXTiming/D");
  treeOut->Branch("tel5dEaX", &tel5dEaX,"tel5dEaX/D");
  treeOut->Branch("tel5dEaY", &tel5dEaY,"tel5dEaY/D");
  treeOut->Branch("tel5Eb", &tel5Eb,"tel5Eb/D");
  treeOut->Branch("tel5Etotal", &tel5Etotal,"tel5Etotal/D");

  //TClonesArray *tel6dEX_raw = new TClonesArray("art::TimingChargeData"); 
  //TClonesArray *tel6dEX_raw= new TClonesArray("art::TimingChargeData"); 
  //TClonesArray *tel6E_raw= new TClonesArray("art::TimingChargeData"); 
  //TClonesArray *tel6dEX_cal = new TClonesArray("art::TimingChargeData"); 
  //TClonesArray *tel6dEX_cal = new TClonesArray("art::TimingChargeData"); 
  //TClonesArray *tel6E_cal = new TClonesArray("art::TimingChargeData"); 
  TClonesArray *tel6 = new TClonesArray("art::TTelescopeData");
  //tree->SetBranchAddress("tel6dEX_cal", &tel6dEX_cal);
  //tree->SetBranchAddress("tel6dEY_cal", &tel6dEY_cal);
  //tree->SetBranchAddress("tel6E_cal", &tel6E_cal);
  tree->SetBranchAddress("tel6", &tel6);
  double tel6dEaX,tel6dEaY,tel6Eb, tel6Etotal;
  double tel6dEaXTiming; 
  double tel6X, tel6Y, tel6Z;
  treeOut->Branch("tel6dEaXTiming", &tel6dEaXTiming,"tel6dEaXTiming/D");
  treeOut->Branch("tel6dEaX", &tel6dEaX,"tel6dEaX/D");
  treeOut->Branch("tel6dEaY", &tel6dEaY,"tel6dEaY/D");
  treeOut->Branch("tel6Eb", &tel6Eb,"tel6Eb/D");
  treeOut->Branch("tel6Etotal", &tel6Etotal,"tel6Etotal/D");


  
  int events = tree->GetEntriesFast();
  for(int event = 0; event < events; event++){
    
    rf0                = TMath::QuietNaN();
    single             = TMath::QuietNaN();
    coin               = TMath::QuietNaN();
    cathda             = TMath::QuietNaN();
    cathdb             = TMath::QuietNaN();
    mwdca_timing       = TMath::QuietNaN();
    mwdca_x            = TMath::QuietNaN();
    mwdca_y            = TMath::QuietNaN();
    mwdcb_timing       = TMath::QuietNaN();
    mwdcb_x            = TMath::QuietNaN();
    mwdcb_y            = TMath::QuietNaN();
    target_x           = TMath::QuietNaN();
    target_y           = TMath::QuietNaN();
    target_z           = TMath::QuietNaN();
    tel1dEaX           = TMath::QuietNaN();
    tel1dEaY           = TMath::QuietNaN();
    tel1dEaXTiming     = TMath::QuietNaN();
    tel1Eb             = TMath::QuietNaN();
    tel1Ec             = TMath::QuietNaN();
    tel1Ed             = TMath::QuietNaN();
    tel1Etotal         = TMath::QuietNaN();
    tel2dEaX           = TMath::QuietNaN();
    tel2dEaY           = TMath::QuietNaN();
    tel2dEaXTiming     = TMath::QuietNaN();
    tel2Eb             = TMath::QuietNaN();
    tel2Ec             = TMath::QuietNaN();
    tel2Ed             = TMath::QuietNaN();
    tel2Etotal         = TMath::QuietNaN();
    tel3dEaX           = TMath::QuietNaN();
    tel3dEaY           = TMath::QuietNaN();
    tel3dEaXTiming     = TMath::QuietNaN();
    tel3Eb             = TMath::QuietNaN();
    tel3Ec             = TMath::QuietNaN();
    tel3Etotal         = TMath::QuietNaN();
    tel4dEaX           = TMath::QuietNaN();
    tel4dEaY           = TMath::QuietNaN();
    tel4dEaXTiming     = TMath::QuietNaN();
    tel4Eb             = TMath::QuietNaN();
    tel4Etotal         = TMath::QuietNaN();
    tel5dEaX           = TMath::QuietNaN();
    tel5dEaY           = TMath::QuietNaN();
    tel5dEaXTiming     = TMath::QuietNaN();
    tel5Eb             = TMath::QuietNaN();
    tel5Etotal         = TMath::QuietNaN();
    tel6dEaX           = TMath::QuietNaN();
    tel6dEaY           = TMath::QuietNaN();
    tel6dEaXTiming     = TMath::QuietNaN();
    tel6Eb             = TMath::QuietNaN();
    tel6Etotal         = TMath::QuietNaN();






    tree->GetEntry(event);
    std::cout<<"rf_cal_0 = "<<rf_cal_0->GetEntriesFast()<<std::endl;
    std::cout<<"coin_raw = "<<coin_raw->GetEntriesFast()<<std::endl;
    std::cout<<"catha_cal = "<<catha_cal->GetEntriesFast()<<std::endl;
    std::cout<<"cathb_cal = "<<cathb_cal->GetEntriesFast()<<std::endl;
    std::cout<<"mwdca = "<<mwdca->GetEntriesFast()<<std::endl;
    std::cout<<"mwdca = "<<mwdca->GetEntriesFast()<<std::endl;

    std::cout<<"mwdcb = "<<mwdcb->GetEntriesFast()<<std::endl;
    std::cout<<"mwdcb = "<<mwdcb->GetEntriesFast()<<std::endl;

    std::cout<<"f3mwdc = "<<f3mwdc->GetEntriesFast()<<std::endl;

    std::cout<<"tel1 = "<<tel1->GetEntriesFast()<<std::endl;
    std::cout<<"tel2 = "<<tel2->GetEntriesFast()<<std::endl;
    std::cout<<"tel3 = "<<tel3->GetEntriesFast()<<std::endl;
    std::cout<<"tel4 = "<<tel4->GetEntriesFast()<<std::endl;
    std::cout<<"tel5 = "<<tel5->GetEntriesFast()<<std::endl;
    std::cout<<"tel6 = "<<tel6->GetEntriesFast()<<std::endl;
  }
 treeOut->Write();
}
