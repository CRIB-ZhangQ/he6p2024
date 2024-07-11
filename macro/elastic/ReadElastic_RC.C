#include <iostream>
#include <TFile.h>
#include <TROOT.h>
#include <TString.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TMath.h>
#include <TRandom3.h>
#include <time.h>
#include <vector>

#include "/home/zq/Tools/artemis/artemis-share/src/cont/TTimingChargeData.h"
#include "/home/zq/Tools/artemis/artemis-share/src/cont/TTimingData.h"
#include "/home/zq/Tools/artemis/artemis-share/src/mwdc/TMWDCTrackingResult.h"
#include "/home/zq/Tools/artemis/artemis-share/src/mwdc/TMWDCHitData.h"
#include "/home/zq/art_analysis/he6p2024/he6p2024/src-crib/mux/TMUXData.h"
#include "/home/zq/art_analysis/he6p2024/he6p2024/src-crib/telescope/TTelescopeData.h"
#include "/home/zq/art_analysis/he6p2024/he6p2024/macro/elastic/constant.h"

const int flag_mwdca = 1;//1: no use, 0: use
const int flag_mwdcb = 1;

void ReadElastic_RC(TString runnumber = "")
{
  TString run_name = "chkf3run";
  int run_number = atoi(runnumber);
  TString InputPath, OutputPath, infile, outfile;
  InputPath = "/home/zq/art_analysis/he6p2024/he6p2024/output/run/";
  OutputPath = "/data/he6p/he6p2024/ana_elastic/";
  infile.Form("%s%s%04d%s%s%04d.root", InputPath.Data(), "/", run_number, "/", run_name.Data(), run_number);
  outfile.Form("%s%s%04d.root", OutputPath.Data(), "run", run_number);
  TFile fin(infile);
  TFile fout(outfile, "recreate");

  TTree *tree = (TTree*)fin.Get("tree");
  TTree *treeOut = new TTree("tree","tree");

  //TH2D *Energy_Angle = new TH2D("energy_angle","kinematic",180,0,90,300,0,30);

  //rf
  TClonesArray *rf_cal_0 = new TClonesArray("art::TTimingData");
  TClonesArray *single = new TClonesArray("art::TTimingData");
  TClonesArray *coin = new TClonesArray("art::TTimingData");
  TClonesArray *coin_raw = new TClonesArray("art::TTimingData");

  //mwdc cathod
  std::vector<double> cathbTiming;
  std::vector<double> cathaTiming;
  std::vector<double> mwdcaTiming;
  std::vector<double> mwdcbTiming;
  TClonesArray *cathb_cal = new TClonesArray("art::TTimingChargeData");
  TClonesArray *mwdca = new TClonesArray("art::TMWDCTrackingResult");
  TClonesArray *mwdcb = new TClonesArray("art::TMWDCTrackingResult");
  TClonesArray *f3mwdc = new TClonesArray("art::TMWDCTrackingResult");

  tree->SetBranchAddress("rf_cal_0", &rf_cal_0);
  tree->SetBranchAddress("single", &single);
  tree->SetBranchAddress("coin", &coin);
  tree->SetBranchAddress("coin_raw", &coin_raw);
  tree->SetBranchAddress("cathb_cal", &cathb_cal);
  tree->SetBranchAddress("mwdca", &mwdca);
  tree->SetBranchAddress("mwdcb", &mwdcb);
  tree->SetBranchAddress("f3mwdc", &f3mwdc);

  double rf0;
  //std::vector<double> cathbTiming;
  int single_fID, coin_fID, pileup;
  Double_t mwdca_x, mwdca_y, mwdca_z;
  Double_t mwdcb_x, mwdcb_y, mwdcb_z;
  double fA, fB;
  double targetX, targetY, targetZ;
  double targetX_L,targetX_R, targetY_U,targetY_D;
  double deltaX, deltaY, deltaZ, deltaRange, cosinInX, cosinInY, cosinInZ;
  treeOut->Branch("rf0", &rf0, "rf0/D");
  treeOut->Branch("single_fID", &single_fID, "single_fID/I");
  treeOut->Branch("coin_fID", &coin_fID, "coin_fID/I");
  treeOut->Branch("pileup", &pileup, "pileup/I");
  treeOut->Branch("cathbTiming", &cathbTiming);
  treeOut->Branch("targetX", &targetX, "targetX/D");
  treeOut->Branch("targetY", &targetY, "targetY/D");
  treeOut->Branch("targetZ", &targetZ, "targetZ/D");

  double disTarTel, cosinOutX, cosinOutY, cosinOutZ, emitangle;
  treeOut->Branch("emitangle", &emitangle,"emitangle/D");

  //tel1
  TClonesArray *tel1 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel1", &tel1);
  //TClonesArray *tel1dEX_raw = new TClonesArray("art::TTimingChargeData");
  //TClonesArray *tel1dEY_raw = new TClonesArray("art::TTimingChargeData");
  //TClonesArray *tel1E_raw = new TClonesArray("art::TTimingChargeData");
  //tree->SetBranchAddress("tel1dEX_raw", &tel1dEX_raw);
  //tree->SetBranchAddress("tel1dEY_raw", &tel1dEY_raw);
  //tree->SetBranchAddress("tel1E_raw", &tel1E_raw);
  double tel1dEaX,tel1dEaY,tel1Eb,tel1Ec,tel1Ed, tel1Etotal;
  double tel1dEaX_raw,tel1dEaY_raw,tel1En_raw[3];
  double tel1dEaXTiming, tel1EbTiming, tel1EcTiming, tel1EdTiming; 
  double tel1X, tel1Y, tel1Z;
  //double tel1SolidAngle;
  int    tel1XID, tel1YID;
  double tel1Angle;
  //treeOut->Branch("tel1dEaXTiming", &tel1dEaXTiming,"tel1dEaXTiming/D");
  //treeOut->Branch("tel1EbTiming", &tel1EbTiming,"tel1EbTiming/D");
  //treeOut->Branch("tel1EcTiming", &tel1EcTiming,"tel1EcTiming/D");
  //treeOut->Branch("tel1EdTiming", &tel1EdTiming,"tel1EdTiming/D");
  treeOut->Branch("tel1dEaX", &tel1dEaX,"tel1dEaX/D");
  treeOut->Branch("tel1dEaY", &tel1dEaY,"tel1dEaY/D");
  treeOut->Branch("tel1Eb", &tel1Eb,"tel1Eb/D");
  treeOut->Branch("tel1Ec", &tel1Ec,"tel1Ec/D");
  treeOut->Branch("tel1Ed", &tel1Ed,"tel1Ed/D");
  treeOut->Branch("tel1Etotal", &tel1Etotal,"tel1Etotal/D");
  treeOut->Branch("tel1XID", &tel1XID,"tel1XID/I");
  treeOut->Branch("tel1YID", &tel1YID,"tel1YID/I");
  treeOut->Branch("tel1X", &tel1X,"tel1X/D");
  treeOut->Branch("tel1Y", &tel1Y,"tel1Y/D");
  treeOut->Branch("tel1Z", &tel1Z,"tel1Z/D");
  //treeOut->Branch("tel1SolidAngle", &tel1SolidAngle,"tel1SolidAngle/D");
  treeOut->Branch("tel1Angle", &tel1Angle,"tel1Angle/D");
  //get the raw data of energy
  //treeOut->Branch("tel1dEaX_raw", &tel1dEaX_raw,"tel1dEaX_raw/D");
  //treeOut->Branch("tel1dEaY_raw", &tel1dEaY_raw,"tel1dEaY_raw/D");
  //treeOut->Branch("tel1En_raw", tel1En_raw,Form("tel1En_raw[%d]/D",3));
  //treeOut->Branch("tel1Eb_raw", &tel1Eb_raw,"tel1Eb_raw/D");
  //treeOut->Branch("tel1Ec_raw", &tel1Ec_raw,"tel1Ec_raw/D");
  //treeOut->Branch("tel1Ed_raw", &tel1Ed_raw,"tel1Ed_raw/D");

  //tel2 
  TClonesArray *tel2 = new TClonesArray("art::TTelescopeData");
  TClonesArray *tel2dEX_raw = new TClonesArray("art::TTimingChargeData");
  //TClonesArray *tel2dEY_raw = new TClonesArray("art::TTimingChargeData");
  //TClonesArray *tel2E_raw = new TClonesArray("art::TTimingChargeData");
  tree->SetBranchAddress("tel2", &tel2);
  tree->SetBranchAddress("tel2dEX_raw", &tel2dEX_raw);
  //tree->SetBranchAddress("tel2dEY_raw", &tel2dEY_raw);
  //tree->SetBranchAddress("tel2E_raw", &tel2E_raw);
  double tel2dEaX,tel2dEaY,tel2Eb,tel2Ec,tel2Ed, tel2Etotal;
  double tel2dEaX_raw,tel2dEaY_raw,tel2En_raw[3];
  double tel2dEaXTiming, tel2EbTiming, tel2EcTiming, tel2EdTiming;
  double tel2X, tel2Y, tel2Z;
  //double tel2SolidAngle;
  int    tel2XID, tel2YID;
  int    tel2rawXID;
  double tel2Angle;
  //treeOut->Branch("tel2dEaXTiming", &tel2dEaXTiming,"tel2dEaXTiming/D");
  //treeOut->Branch("tel2EbTiming", &tel2EbTiming,"tel2EbTiming/D");
  //treeOut->Branch("tel2EcTiming", &tel2EcTiming,"tel2EcTiming/D");
  //treeOut->Branch("tel2EdTiming", &tel2EdTiming,"tel2EdTiming/D");
  treeOut->Branch("tel2dEaX_raw", &tel2dEaX_raw,"tel2dEaX_raw/D");
  treeOut->Branch("tel2dEaX", &tel2dEaX,"tel2dEaX/D");
  treeOut->Branch("tel2dEaY", &tel2dEaY,"tel2dEaY/D");
  treeOut->Branch("tel2Eb", &tel2Eb,"tel2Eb/D");
  treeOut->Branch("tel2Ec", &tel2Ec,"tel2Ec/D");
  treeOut->Branch("tel2Ed", &tel2Ed,"tel2Ed/D");
  treeOut->Branch("tel2Etotal", &tel2Etotal,"tel2Etotal/D");
  treeOut->Branch("tel2XID", &tel2XID,"tel2XID/I");
  treeOut->Branch("tel2YID", &tel2YID,"tel2YID/I");
  treeOut->Branch("tel2rawXID", &tel2rawXID,"tel2rawXID/I");
  treeOut->Branch("tel2X", &tel2X,"tel2X/D");
  treeOut->Branch("tel2Y", &tel2Y,"tel2Y/D");
  treeOut->Branch("tel2Z", &tel2Z,"tel2Z/D");
  //treeOut->Branch("tel2SolidAngle", &tel2SolidAngle,"tel2SolidAngle/D");
  treeOut->Branch("tel2Angle", &tel2Angle,"tel2Angle/D");
  //get the raw data of energy
  //treeOut->Branch("tel2dEaX_raw", &tel2dEaX_raw,"tel2dEaX_raw/D");
  //treeOut->Branch("tel2dEaY_raw", &tel2dEaY_raw,"tel2dEaY_raw/D");
  //treeOut->Branch("tel2En_raw", tel2En_raw,Form("tel2En_raw[%d]/D",3));
  
  //tel3 
  TClonesArray *tel3 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel3", &tel3);
  double tel3dEaX,tel3dEaY,tel3Eb,tel3Ec, tel3Etotal;
  double tel3dEaXTiming, tel3EbTiming, tel3EcTiming; 
  double tel3X, tel3Y, tel3Z;
  int    tel3XID, tel3YID;
  double tel3SolidAngle;
  double tel3Angle;
  //treeOut->Branch("tel3dEaXTiming", &tel3dEaXTiming,"tel3dEaXTiming/D");
  //treeOut->Branch("tel3EbTiming", &tel3EbTiming,"tel3EbTiming/D");
  //treeOut->Branch("tel3EcTiming", &tel3EcTiming,"tel3EcTiming/D");
  treeOut->Branch("tel3dEaX", &tel3dEaX,"tel3dEaX/D");
  treeOut->Branch("tel3dEaY", &tel3dEaY,"tel3dEaY/D");
  treeOut->Branch("tel3Eb", &tel3Eb,"tel3Eb/D");
  treeOut->Branch("tel3Ec", &tel3Ec,"tel3Ec/D");
  treeOut->Branch("tel3Etotal", &tel3Etotal,"tel3Etotal/D");
  treeOut->Branch("tel3XID", &tel3XID,"tel3XID/I");
  treeOut->Branch("tel3YID", &tel3YID,"tel3YID/I");
  treeOut->Branch("tel3X", &tel3X,"tel3X/D");
  treeOut->Branch("tel3Y", &tel3Y,"tel3Y/D");
  treeOut->Branch("tel3Z", &tel3Z,"tel3Z/D");
  //treeOut->Branch("tel3SolidAngle", &tel3SolidAngle,"tel3SolidAngle/D");
  treeOut->Branch("tel3Angle", &tel3Angle,"tel3Angle/D");

  //tel4 
  TClonesArray *tel4 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel4", &tel4);
  double tel4dEaX,tel4dEaY,tel4Eb,tel4Etotal;
  double tel4dEaXTiming, tel4EbTiming; 
  double tel4X, tel4Y, tel4Z;
  int    tel4XID, tel4YID;
  double tel4SolidAngle;
  double tel4Angle;
  //treeOut->Branch("tel4dEaXTiming", &tel4dEaXTiming,"tel4dEaXTiming/D");
  //treeOut->Branch("tel4EbTiming", &tel4EbTiming,"tel4EbTiming/D");
  treeOut->Branch("tel4dEaX", &tel4dEaX,"tel4dEaX/D");
  treeOut->Branch("tel4dEaY", &tel4dEaY,"tel4dEaY/D");
  treeOut->Branch("tel4Eb", &tel4Eb,"tel4Eb/D");
  treeOut->Branch("tel4Etotal", &tel4Etotal,"tel4Etotal/D");
  treeOut->Branch("tel4XID", &tel4XID,"tel4XID/I");
  treeOut->Branch("tel4YID", &tel4YID,"tel4YID/I");
  treeOut->Branch("tel4X", &tel4X,"tel4X/D");
  treeOut->Branch("tel4Y", &tel4Y,"tel4Y/D");
  treeOut->Branch("tel4Z", &tel4Z,"tel4Z/D");
  //treeOut->Branch("tel4SolidAngle", &tel4SolidAngle,"tel4SolidAngle/D");
  treeOut->Branch("tel4Angle", &tel4Angle,"tel4Angle/D");

  //tel5 
  TClonesArray *tel5 = new TClonesArray("art::TTelescopeData");
  //TClonesArray *tel5dEX_raw = new TClonesArray("art::TMUXData");
  //TClonesArray *tel5dEY_raw = new TClonesArray("art::TMUXData");
  //TClonesArray *tel5E_raw = new TClonesArray("art::TTimingChargeData");
  tree->SetBranchAddress("tel5", &tel5);
  //tree->SetBranchAddress("tel5dEX_raw", &tel5dEX_raw);
  //tree->SetBranchAddress("tel5dEY_raw", &tel5dEY_raw);
  //tree->SetBranchAddress("tel5E_raw", &tel5E_raw);

  double tel5dEaX,tel5dEaY,tel5Eb,tel5Etotal;
  double tel5dEaXTiming, tel5EbTiming; 
  double tel5X, tel5Y, tel5Z;
  int    tel5XID, tel5YID;
  double tel5SolidAngle;
  double tel5Angle;
  //treeOut->Branch("tel5dEaXTiming", &tel5dEaXTiming,"tel5dEaXTiming/D");
  //treeOut->Branch("tel5EbTiming", &tel5EbTiming,"tel5EbTiming/D");
  treeOut->Branch("tel5dEaX", &tel5dEaX,"tel5dEaX/D");
  treeOut->Branch("tel5dEaY", &tel5dEaY,"tel5dEaY/D");
  treeOut->Branch("tel5Eb", &tel5Eb,"tel5Eb/D");
  treeOut->Branch("tel5Etotal", &tel5Etotal,"tel5Etotal/D");
  treeOut->Branch("tel5XID", &tel5XID,"tel5XID/I");
  treeOut->Branch("tel5YID", &tel5YID,"tel5YID/I");
  treeOut->Branch("tel5X", &tel5X,"tel5X/D");
  treeOut->Branch("tel5Y", &tel5Y,"tel5Y/D");
  treeOut->Branch("tel5Z", &tel5Z,"tel5Z/D");
  treeOut->Branch("tel5SolidAngle", &tel5SolidAngle,"tel5SolidAngle/D");
  treeOut->Branch("tel5Angle", &tel5Angle,"tel5Angle/D");
  //get the raw data of energy
  //treeOut->Branch("tel5dEaX_raw", &tel5dEaX_raw,"tel5dEaX_raw/D");
  //treeOut->Branch("tel5dEaY_raw", &tel5dEaY_raw,"tel5dEaY_raw/D");
  //treeOut->Branch("tel5Eb_raw", &tel5Eb_raw,"tel5Eb_raw/D");

  //tel6
  TClonesArray *tel6 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel6", &tel6);
  double tel6dEaX,tel6dEaY,tel6Eb, tel6Etotal;
  double tel6dEaXTiming, tel6EbTiming; 
  double tel6X, tel6Y, tel6Z;
  int    tel6XID, tel6YID;
  double tel6SolidAngle;
  double tel6Angle;
  //treeOut->Branch("tel6dEaXTiming", &tel6dEaXTiming,"tel6dEaXTiming/D");
  //treeOut->Branch("tel6EbTiming", &tel6EbTiming,"tel6EbTiming/D");
  treeOut->Branch("tel6dEaX", &tel6dEaX,"tel6dEaX/D");
  treeOut->Branch("tel6dEaY", &tel6dEaY,"tel6dEaY/D");
  treeOut->Branch("tel6Eb", &tel6Eb,"tel6Eb/D");
  treeOut->Branch("tel6Etotal", &tel6Etotal,"tel6Etotal/D");
  treeOut->Branch("tel6XID", &tel6XID,"tel6XID/I");
  treeOut->Branch("tel6YID", &tel6YID,"tel6YID/I");
  treeOut->Branch("tel6X", &tel6X,"tel6X/D");
  treeOut->Branch("tel6Y", &tel6Y,"tel6Y/D");
  treeOut->Branch("tel6Z", &tel6Z,"tel6Z/D");
  treeOut->Branch("tel6SolidAngle", &tel6SolidAngle,"tel6SolidAngle/D");
  treeOut->Branch("tel6Angle", &tel6Angle,"tel6Angle/D");


  int ID =TMath::QuietNaN();

  int scale = 10;
  int events = tree->GetEntriesFast();
  for(int event = 0; event < events; event++){
    tree->GetEntry(event);
    if(event%(int(events/scale))==0)
      std::cout<<"Processed: "<<(int)(event*100.0/events+1)<<"%";
    std::cout<<"\r";

    TRandom3* ran = new TRandom3(time(NULL));

    rf0                = TMath::QuietNaN();
    single_fID         = TMath::QuietNaN();
    coin_fID           = TMath::QuietNaN();
    pileup             = TMath::QuietNaN();
    cathbTiming.clear();//        = TMath::QuietNaN();
    mwdca_x            = TMath::QuietNaN();
    mwdca_y            = TMath::QuietNaN();
    mwdca_z            = TMath::QuietNaN();
    mwdcb_x            = TMath::QuietNaN();
    mwdcb_y            = TMath::QuietNaN();
    mwdcb_z            = TMath::QuietNaN();
    targetX            = TMath::QuietNaN();
    targetY            = TMath::QuietNaN();
    targetZ            = TMath::QuietNaN();
    tel1dEaX           = TMath::QuietNaN();
    tel1dEaY           = TMath::QuietNaN();
    tel1Eb             = TMath::QuietNaN();
    tel1Ec             = TMath::QuietNaN();
    tel1Ed             = TMath::QuietNaN();
    tel1Etotal         = TMath::QuietNaN();
    tel1dEaXTiming     = TMath::QuietNaN();
    tel1EbTiming       = TMath::QuietNaN();
    tel1EcTiming       = TMath::QuietNaN();
    tel1EdTiming       = TMath::QuietNaN();
    tel1XID            = TMath::QuietNaN();
    tel1YID            = TMath::QuietNaN();
    tel1X              = TMath::QuietNaN();
    tel1Y              = TMath::QuietNaN();
    tel1Z              = TMath::QuietNaN();
    //tel1SolidAngle     = TMath::QuietNaN();
    tel1Angle          = TMath::QuietNaN();
    tel2dEaX           = TMath::QuietNaN();
    tel2dEaY           = TMath::QuietNaN();
    tel2Eb             = TMath::QuietNaN();
    tel2Ec             = TMath::QuietNaN();
    tel2Ed             = TMath::QuietNaN();
    tel2Etotal         = TMath::QuietNaN();
    tel2dEaXTiming     = TMath::QuietNaN();
    tel2EbTiming       = TMath::QuietNaN();
    tel2EcTiming       = TMath::QuietNaN();
    tel2EdTiming       = TMath::QuietNaN();
    tel2XID            = TMath::QuietNaN();
    tel2YID            = TMath::QuietNaN();
    tel2rawXID         = TMath::QuietNaN();
    tel2X              = TMath::QuietNaN();
    tel2Y              = TMath::QuietNaN();
    tel2Z              = TMath::QuietNaN();
    //tel2SolidAngle     = TMath::QuietNaN();
    tel2Angle          = TMath::QuietNaN();
    tel3dEaX           = TMath::QuietNaN();
    tel3dEaY           = TMath::QuietNaN();
    tel3Eb             = TMath::QuietNaN();
    tel3Ec             = TMath::QuietNaN();
    tel3Etotal         = TMath::QuietNaN();
    tel3dEaXTiming     = TMath::QuietNaN();
    tel3EbTiming       = TMath::QuietNaN();
    tel3EcTiming       = TMath::QuietNaN();
    tel3XID            = TMath::QuietNaN();
    tel3YID            = TMath::QuietNaN();
    tel3X              = TMath::QuietNaN();
    tel3Y              = TMath::QuietNaN();
    tel3Z              = TMath::QuietNaN();
    //tel3SolidAngle     = TMath::QuietNaN();
    tel3Angle          = TMath::QuietNaN();
    tel4dEaX           = TMath::QuietNaN();
    tel4dEaY           = TMath::QuietNaN();
    tel4Eb             = TMath::QuietNaN();
    tel4Etotal         = TMath::QuietNaN();
    tel4dEaXTiming     = TMath::QuietNaN();
    tel4EbTiming       = TMath::QuietNaN();
    tel4XID            = TMath::QuietNaN();
    tel4YID            = TMath::QuietNaN();
    tel4X              = TMath::QuietNaN();
    tel4Y              = TMath::QuietNaN();
    tel4Z              = TMath::QuietNaN();
    //tel4SolidAngle     = TMath::QuietNaN();
    tel4Angle          = TMath::QuietNaN();
    tel5dEaX           = TMath::QuietNaN();
    tel5dEaY           = TMath::QuietNaN();
    tel5Eb             = TMath::QuietNaN();
    tel5Etotal         = TMath::QuietNaN();
    tel5dEaXTiming     = TMath::QuietNaN();
    tel5EbTiming       = TMath::QuietNaN();
    tel5XID            = TMath::QuietNaN();
    tel5YID            = TMath::QuietNaN();
    tel5X              = TMath::QuietNaN();
    tel5Y              = TMath::QuietNaN();
    tel5Z              = TMath::QuietNaN();
    //tel5SolidAngle     = TMath::QuietNaN();
    tel5Angle          = TMath::QuietNaN();
    tel6dEaX           = TMath::QuietNaN();
    tel6dEaY           = TMath::QuietNaN();
    tel6Eb             = TMath::QuietNaN();
    tel6Etotal         = TMath::QuietNaN();
    tel6dEaXTiming     = TMath::QuietNaN();
    tel6EbTiming       = TMath::QuietNaN();
    tel6XID            = TMath::QuietNaN();
    tel6YID            = TMath::QuietNaN();
    tel6X              = TMath::QuietNaN();
    tel6Y              = TMath::QuietNaN();
    tel6Z              = TMath::QuietNaN();
    //tel6SolidAngle     = TMath::QuietNaN();
    tel6Angle          = TMath::QuietNaN();
    tel2dEaX_raw       = TMath::QuietNaN(); 

    //check the entries for all branches
    //std::cout<<"start:--------------------------------"<<std::endl;
    //std::cout<<"rf_cal_0 = "<<rf_cal_0->GetEntriesFast()<<std::endl;
    //std::cout<<"coin_raw = "<<coin_raw->GetEntriesFast()<<std::endl;
    //std::cout<<"tel1 = "<<tel1->GetEntriesFast()<<std::endl;
    //std::cout<<"tel2 = "<<tel2->GetEntriesFast()<<std::endl;
    //std::cout<<"tel3 = "<<tel3->GetEntriesFast()<<std::endl;
    //std::cout<<"tel4 = "<<tel4->GetEntriesFast()<<std::endl;
    //std::cout<<"tel5 = "<<tel5->GetEntriesFast()<<std::endl;
    //std::cout<<"tel6 = "<<tel6->GetEntriesFast()<<std::endl;
    //std::cout<<"end:--------------------------------"<<std::endl;
    //std::cout<<std::endl;
    //**************************************************************

    //rf
    if(rf_cal_0->GetEntriesFast()==1){
      rf0 = static_cast<const art::TTimingData *>(rf_cal_0->At(0))->GetTiming();
    }
    //single
    if(single->GetEntriesFast()==1){
      single_fID = static_cast<const art::TTimingData *>(single->At(0))->GetDetID();
    }
    //coin
    if(coin->GetEntriesFast()==1){
      coin_fID = static_cast<const art::TTimingData *>(coin->At(0))->GetDetID();
    }
    // coin_raw
    if(coin_raw->GetEntriesFast() == 1 || coin_raw->GetEntriesFast() == 2 || coin_raw->GetEntriesFast() == 3  ){
      ID = static_cast<const art::TTimingData *>(coin_raw->At(0))->GetDetID(); 
      //if(ID == 0)
      //  single = ID;
      //else if(ID == 1)
      //  coin = ID;
      //else pileup = ID;
      if(ID==2) pileup = ID;
    }

    //cathb Timing
    for(int entry = 0; entry < cathb_cal -> GetEntriesFast(); entry++){
        cathbTiming.push_back(static_cast<const art::TTimingChargeData *>(cathb_cal->At(entry))->GetTiming());
    }
    // mwdca

    TVector3 dca_vec(mwdca_x, mwdca_y, mwdca_z);
    if(flag_mwdca == 0){
      Int_t dc_a = mwdca->GetEntriesFast();
      for(int j = 0; j < dc_a; j++){
          const art::TMWDCTrackingResult *read = static_cast<const art::TMWDCTrackingResult *>(mwdca->At(j));
          mwdca_x = read->GetTrack()->GetX();
          mwdca_y = read->GetTrack()->GetY();
          mwdca_z = -462.0;
          //mwdca_y = read->GetTrack()->GetZ();
      }
      dca_vec.SetXYZ(mwdca_x,mwdca_y,mwdca_z);
      //cout<<" x = "<<dca_vec.X()<<", y= "<<dca_vec.Y()<<", z = "<<dca_vec.Z()<<std::endl;
      //cout<<" x = "<<mwdca_x<<", y= "<<mwdca_y<<", z = "<<mwdca_z<<std::endl;
    }
      // mwdcb
    TVector3 dcb_vec(mwdcb_x, mwdcb_y, mwdcb_z);
    if(flag_mwdcb == 0){
      Int_t dc_b = mwdcb->GetEntriesFast();
      for(int j = 0; j < dc_b; j++){
          const art::TMWDCTrackingResult *read = static_cast<const art::TMWDCTrackingResult *>(mwdcb->At(j));
          mwdcb_x = read->GetTrack()->GetX();
          mwdcb_y = read->GetTrack()->GetY();
          mwdcb_z = -171.5;  
          //mwdcb_z = read->GetTrack()->GetZ();  
      }
      dcb_vec.SetXYZ(mwdcb_x,mwdcb_y,mwdcb_z);
    }
    //cout<<" mwdcbx = "<<dcb_vec.X()<<", mwdcby= "<<dcb_vec.Y()<<", mwdcbz = "<<dcb_vec.Z()<<std::endl;


    //f3 Target
    Int_t target_t = f3mwdc->GetEntriesFast();
    for(int j = 0; j < target_t; j++){
        const art::TMWDCTrackingResult *read = static_cast<const art::TMWDCTrackingResult *>(f3mwdc->At(j));
        targetX = read->GetTrack()->GetX();
        targetY = read->GetTrack()->GetY();
        targetZ = read->GetTrack()->GetZ();
        //fA = read->GetTrack()->GetA();
        //fB = read->GetTrack()->GetB();
    }
    //run0099
    //mwdcb-triton
    //TVector3 mwdcb(-1.55252, 1.70021,-171.5);
    ////f3target-triton
    //TVector3 tarpos(-0.477088, 1.19911,0.0 );
    //mwdcb-he6
    //TVector3 mwdcb(-1.02655, 1.48615, -171.5);
    //TVector3 mwdcb(-1.02655, 1.48615, -171.5);
    //////f3target-he6
    //TVector3 tarpos(0.317735, 0.93487,0.0 );

    //TVector3 tarpos(targetX, targetY, targetZ);

    //run0166
    //mwdcb-triton
    //TVector3 mwdcb(-1.09261, 1.9637,-171.5);
    ////f3target-triton
    //TVector3 tarpos(-0.017178, 1.4626, 0.0 );
    //mwdcb-he6
    TVector3 mwdcb(-2.12006, 1.70327, -171.5);
    ////f3target-he6
    TVector3 tarpos(-0.775775, 1.15199, 0.0 );

    //beam direction
    TVector3 beamdir;
    if(flag_mwdca == 0 && flag_mwdcb ==0) beamdir = dcb_vec - dca_vec;
    else if(flag_mwdca == 1 && flag_mwdcb == 0) beamdir = tarpos - dcb_vec;
    else{// Assuming the beam is at the target center
      //tarpos.SetXYZ(0., 0., 0.);
      beamdir = tarpos - mwdcb; 
    }
    
    //cout<<" tarx = "<<tarpos.X()<<", tary= "<<tarpos.Y()<<", tarz = "<<tarpos.Z()<<std::endl;
    //cout<<" beamx = "<<beamdir.X()<<", beamy= "<<beamdir.Y()<<", beamz = "<<beamdir.Z()<<std::endl;
    
    


    //assume the beam at the center of the target
    //if( tel1->GetEntriesFast()==1 || tel2->GetEntriesFast()==1 || tel3->GetEntriesFast()==1 || tel4->GetEntriesFast()==1 || tel5->GetEntriesFast()==1 || tel6->GetEntriesFast()==1){

    //  //assume the direction of the incident particle is (0,0,1) 
    //  //std::cout<<"event = "<<event+1<<", single = "<<single<<", coin = "<<coin<<", single event = "<<event + 1<<", dirX = "<<cosinInX<<", dirY = "<<cosinInY<<", dirZ = "<<cosinInZ<<std::endl;
    //}
    //if(single == 0)    
    //std::cout<<"single = "<<single<<", coin = "<<coin<<", single event = "<<event + 1<<", dirX = "<<cosinInX<<", dirY = "<<cosinInY<<", dirZ = "<<cosinInZ<<std::endl;
    //else 
    //std::cout<<"single = "<<single<<", coin = "<<coin<<", coin event = "<<event + 1<<", dirX = "<<cosinInX<<", dirY = "<<cosinInY<<", dirZ = "<<cosinInZ<<std::endl;

    //tel1-6
    //tel1 rawdata
    if(tel1->GetEntriesFast()==1){
      //tel1dEaXTiming = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetdEXTiming();
      //tel1EbTiming   = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetETimingvec(0);
      //tel1EcTiming   = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetETimingvec(1);
      //tel1EdTiming   = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetETimingvec(2);
      tel1dEaX       = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetdEX();
      tel1dEaY       = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetdEY();
      tel1Eb         = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetEnergyArray(1);
      tel1Ec         = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetEnergyArray(2);
      tel1Ed         = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetEnergyArray(3);
      tel1XID        = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetXID();
      tel1YID        = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetYID();
      tel1X          = static_cast<const art::TTelescopeData *>(tel1->At(0))->X();
      tel1Y          = static_cast<const art::TTelescopeData *>(tel1->At(0))->Y();
      tel1Z          = static_cast<const art::TTelescopeData *>(tel1->At(0))->Z();
      tel1Etotal     = tel1dEaX + tel1Eb + tel1Ec + tel1Ed;
      TVector3 tel1pos(tel1X, tel1Y, tel1Z);
      //raw data of energy channel

      //the direction of the emitted particle 
      //the emitted angle respect to the incident beam
      TVector3 emitdir = tel1pos - tarpos; 
      tel1Angle  = beamdir.Angle(emitdir);
      tel1Angle  = tel1Angle * rad2deg;
      //cout<<"tel1Angle = "<<tel1Angle<<std::endl;

      ////calculate the solid angle
      //double dTX = detX-targetX;
      //double dTY = detY-targetY;
      //double dTZ = detZ-targetZ;
      ////double dX  = tel1X-targetX;
      ////double dY  = tel1Y-targetY;
      ////double dZ  = tel1Z-targetZ;
      //double dX  = X_cent-targetX;
      //double dY  = Y_cent-targetY;
      //double dZ  = Z_cent-targetZ;
      //double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      //costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      //if(costheta > 0) tel1SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
      //std::cout<<"angle = "<<tel1Angle<<", solidAngle = "<<tel1SolidAngle<<std::endl;
      //std::cout<<"x = "<<tel1X<<", y = "<<tel1Y<<", z = "<<tel1Z<<std::endl;
    }

    //tel2 raw data
    int tel2nData1 = tel2dEX_raw->GetEntriesFast();
    for(int idata1 = 0; idata1 < tel2nData1; idata1++){
      int XID = static_cast<const art::TTimingChargeData *>(tel2dEX_raw->At(idata1))->GetDetID();
      if(XID != -1){
        tel2dEaX_raw = static_cast<const art::TTimingChargeData *>(tel2dEX_raw->At(idata1))->GetCharge();
        tel2rawXID = XID;
        break;
      }
    }

    



    //tel2
    if(tel2->GetEntriesFast()==1){
      //tel2dEaXTiming = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetdEXTiming();
      //tel2EbTiming   = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetETimingvec(0);
      //tel2EcTiming   = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetETimingvec(1);
      //tel2EdTiming   = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetETimingvec(2);
      tel2dEaX       = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetdEX();
      tel2dEaY       = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetdEY();
      tel2Eb         = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetEnergyArray(1);
      tel2Ec         = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetEnergyArray(2);
      tel2Ed         = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetEnergyArray(3);
      tel2XID        = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetXID();
      tel2YID        = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetYID();
      tel2X          = static_cast<const art::TTelescopeData *>(tel2->At(0))->X();
      tel2Y          = static_cast<const art::TTelescopeData *>(tel2->At(0))->Y();
      tel2Z          = static_cast<const art::TTelescopeData *>(tel2->At(0))->Z();
      tel2Etotal     = tel2dEaX + tel2Eb + tel2Ec + tel2Ed;
      TVector3 tel2pos(tel2X, tel2Y, tel2Z);
      //raw data of energy channel

      //the direction of the emitted particle 
      //the emitted angle respect to the incident beam
      TVector3 emitdir = tel2pos - tarpos; 
      tel2Angle  = beamdir.Angle(emitdir);
      tel2Angle  = tel2Angle * rad2deg;
      
      //the emitted angle respect to the incident beam
      ////calculate the solid angle
      //double dTX = detX-targetX;
      //double dTY = detY-targetY;
      //double dTZ = detZ-targetZ;
      ////double dX  = tel2X-targetX;
      ////double dY  = tel2Y-targetY;
      ////double dZ  = tel2Z-targetZ;
      //double dX  = X_cent-targetX;
      //double dY  = Y_cent-targetY;
      //double dZ  = Z_cent-targetZ;
      //double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      //costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      //if(costheta > 0) tel2SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
    }
    
    //tel3
    if(tel3->GetEntriesFast()==1){
      //tel3dEaXTiming = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetdEXTiming();
      //tel3EbTiming   = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetETimingvec(0);
      //tel3EcTiming   = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetETimingvec(1);
      tel3dEaX       = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetdEX();
      tel3dEaY       = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetdEY();
      tel3Eb         = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetEnergyArray(1);
      tel3Ec         = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetEnergyArray(2);
      tel3XID        = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetXID();
      tel3YID        = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetYID();
      tel3X          = static_cast<const art::TTelescopeData *>(tel3->At(0))->X();
      tel3Y          = static_cast<const art::TTelescopeData *>(tel3->At(0))->Y();
      tel3Z          = static_cast<const art::TTelescopeData *>(tel3->At(0))->Z();
      tel3Etotal     = tel3dEaX + tel3Eb + tel3Ec;
      TVector3 tel3pos(tel3X, tel3Y, tel3Z);

      //the direction of the emitted particle 
      //the emitted angle respect to the incident beam
      TVector3 emitdir = tel3pos - tarpos; 
      tel3Angle  = beamdir.Angle(emitdir);
      tel3Angle  = tel3Angle * rad2deg;


      ////calculate the solid angle
      //double dTX = detX-targetX;
      //double dTY = detY-targetY;
      //double dTZ = detZ-targetZ;
      ////double dX  = tel3X-targetX;
      ////double dY  = tel3Y-targetY;
      ////double dZ  = tel3Z-targetZ;
      //double dX  = X_cent-targetX;
      //double dY  = Y_cent-targetY;
      //double dZ  = Z_cent-targetZ;
      //double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      //costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      //if(costheta > 0) tel3SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ);
    }

    //tel4
    if(tel4->GetEntriesFast()==1){
      //tel4dEaXTiming = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetdEXTiming();
      //tel4EbTiming   = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetETimingvec(0);
      tel4dEaX       = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetdEX();
      tel4dEaY       = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetdEY();
      tel4Eb         = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetEnergyArray(1);
      tel4XID        = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetXID();
      tel4YID        = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetYID();
      tel4X          = static_cast<const art::TTelescopeData *>(tel4->At(0))->X();
      tel4Y          = static_cast<const art::TTelescopeData *>(tel4->At(0))->Y();
      tel4Z          = static_cast<const art::TTelescopeData *>(tel4->At(0))->Z();
      tel4Etotal     = tel4dEaX + tel4Eb;
      TVector3 tel4pos(tel4X, tel4Y, tel4Z);
      //raw data of energy channel

      //the direction of the emitted particle 
      //the emitted angle respect to the incident beam
      TVector3 emitdir = tel4pos - tarpos; 
      tel4Angle  = beamdir.Angle(emitdir);
      tel4Angle  = tel4Angle * rad2deg;

      ////calculate the solid angle
      //double dTX = detX-targetX;
      //double dTY = detY-targetY;
      //double dTZ = detZ-targetZ;
      ////double dX  = tel4X-targetX;
      ////double dY  = tel4Y-targetY;
      ////double dZ  = tel4Z-targetZ;
      //double dX  = X_cent-targetX;
      //double dY  = Y_cent-targetY;
      //double dZ  = Z_cent-targetZ;
      //double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      //costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      //if(costheta > 0) tel4SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
    }

    //tel5 raw data
    //tel5
    if(tel5->GetEntriesFast()==1){
      //tel5dEaXTiming = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetdEXTiming();
      //tel5EbTiming   = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetETimingvec(0);
      tel5dEaX       = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetdEX();
      tel5dEaY       = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetdEY();
      tel5Eb         = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetEnergyArray(1);
      tel5XID        = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetXID();
      tel5YID        = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetYID();
      tel5X          = static_cast<const art::TTelescopeData *>(tel5->At(0))->X();
      tel5Y          = static_cast<const art::TTelescopeData *>(tel5->At(0))->Y();
      tel5Z          = static_cast<const art::TTelescopeData *>(tel5->At(0))->Z();
      tel5Etotal     = tel5dEaX + tel5Eb;
      TVector3 tel5pos(tel5X, tel5Y, tel5Z);
      //raw data of energy channel
      //std::cout<<"tel5Eb = "<< tel5Eb<<", check = "<<(tel5->At(0))->GetEnergyArray(1)<<std::endl;

      //the direction of the emitted particle 
      //the emitted angle respect to the incident beam
      TVector3 emitdir = tel5pos - tarpos; 
      tel5Angle  = beamdir.Angle(emitdir);
      tel5Angle  = tel5Angle * rad2deg;

      ////calculate the solid angle
      //double dTX = detX-targetX;
      //double dTY = detY-targetY;
      //double dTZ = detZ-targetZ;
      ////double dX  = tel5X-targetX;
      ////double dY  = tel5Y-targetY;
      ////double dZ  = tel5Z-targetZ;
      //double dX  = X_cent-targetX;
      //double dY  = Y_cent-targetY;
      //double dZ  = Z_cent-targetZ;
      //double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      //costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      //if(costheta > 0) tel5SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
    }

    //tel6
    if(tel6->GetEntriesFast()==1){
      //tel6dEaXTiming = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetdEXTiming();
      //tel6EbTiming   = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetETimingvec(0);
      tel6dEaX       = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetdEX();
      tel6dEaY       = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetdEY();
      tel6Eb         = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetEnergyArray(1);
      tel6XID        = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetXID();
      tel6YID        = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetYID();
      tel6X          = static_cast<const art::TTelescopeData *>(tel6->At(0))->X();
      tel6Y          = static_cast<const art::TTelescopeData *>(tel6->At(0))->Y();
      tel6Z          = static_cast<const art::TTelescopeData *>(tel6->At(0))->Z();
      tel6Etotal     = tel6dEaX + tel6Eb;
      TVector3 tel6pos(tel6X, tel6Y, tel6Z);
      //raw data of energy channel

      //the direction of the emitted particle 
      //the emitted angle respect to the incident beam
      TVector3 emitdir = tel6pos - tarpos; 
      tel6Angle  = beamdir.Angle(emitdir);
      tel6Angle  = tel6Angle * rad2deg;

      ////calculate the solid angle
      //double dTX = detX-targetX;
      //double dTY = detY-targetY;
      //double dTZ = detZ-targetZ;
      ////double dX  = tel6X-targetX;
      ////double dY  = tel6Y-targetY;
      ////double dZ  = tel6Z-targetZ;
      //double dX  = X_cent-targetX;
      //double dY  = Y_cent-targetY;
      //double dZ  = Z_cent-targetZ;
      //double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      //costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      //if(costheta > 0)tel6SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
    }

    treeOut->Fill();
  }


 treeOut->Write();
 gROOT->ProcessLine(".q");
}
