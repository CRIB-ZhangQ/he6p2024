#include <iostream>
#include <TFile.h>
#include <TROOT.h>
#include <TString.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TMath.h>
#include <TRandom3.h>
#include <time.h>

#include "/home/zq/Tools/artemis/artemis-share/src/cont/TTimingChargeData.h"
#include "/home/zq/Tools/artemis/artemis-share/src/cont/TTimingData.h"
#include "/home/zq/art_analysis/he6p/he6p/src-crib/telescope/TTelescopeData.h"
#include "/home/zq/art_analysis/he6p/he6p/src-crib/telescope/TTelescopeData.h"
#include "/home/zq/art_analysis/he6p/he6p/src-crib/telescope/TTelescopeData.h"

const double rad2deg = 180.0 / acos(-1);
const double strip_width = 50.0/16.0;
const double pixel_area = strip_width * strip_width;
const double tel_Angle[6]= {-16.0, 23.0, -39.0, 49.0, -65.0, 75.0};
const double sigma_x = 5.791,sigma_y = 4.225; 

int seed = time(NULL);
TRandom3* ran = new TRandom3(seed);


void ReadElastic(TString runnumber = "")
{
  TString run_name = "chkf3run";
  int run_number = atoi(runnumber);
  TString InputPath, OutputPath, infile, outfile;
  InputPath = "/home/zq/art_analysis/he6p/he6p/output/run/";
  OutputPath = "/home/zq/art_analysis/he6p/he6p/anadata/";
  infile.Form("%s%s%04d%s%s%04d.root", InputPath.Data(), "/", run_number, "/", run_name.Data(), run_number);
  outfile.Form("%s%s%04d.root", OutputPath.Data(), "run", run_number);
  TFile fin(infile);
  TFile fout(outfile, "recreate");
  //TFile fin("/home/zq/art_analysis/he6p/he6p/output/run/0145/chkf3run0145.root");
  //TFile fout("elastic_fixT145.root","recreate");

  TTree *tree = (TTree*)fin.Get("tree");
  TTree *treeOut = new TTree("tree","tree");

  //TH2D *Energy_Angle = new TH2D("energy_angle","kinematic",180,0,90,300,0,30);

  //rf
  TClonesArray *rf_cal_0 = new TClonesArray("art::TTimingData");
  TClonesArray *single = new TClonesArray("art::TTimingData");
  TClonesArray *coin = new TClonesArray("art::TTimingData");
  TClonesArray *coin_raw = new TClonesArray("art::TTimingData");

  //mwdc cathod
  //TClonesArray *trig_cal = new TClonesArray("art::TTimingChargeData");

  tree->SetBranchAddress("rf_cal_0", &rf_cal_0);
  tree->SetBranchAddress("single", &single);
  tree->SetBranchAddress("coin", &coin);
  tree->SetBranchAddress("coin_raw", &coin_raw);
  //tree->SetBranchAddress("trig_cal", &trig_cal);

  double rf0;
  //double trigTiming;
  int single_fID, coin_fID, pileup;
  double fA, fB;
  double targetX, targetY, targetZ;
  double targetX_L,targetX_R, targetY_U,targetY_D;
  double deltaX, deltaY, deltaZ, deltaRange, cosinInX, cosinInY, cosinInZ;
  treeOut->Branch("rf0", &rf0, "rf0/D");
  treeOut->Branch("single_fID", &single_fID, "single_fID/I");
  treeOut->Branch("coin_fID", &coin_fID, "coin_fID/I");
  treeOut->Branch("pileup", &pileup, "pileup/I");
  //treeOut->Branch("trigTiming", &trigTiming, "trigTiming/D");
  treeOut->Branch("targetX", &targetX, "targetX/D");
  treeOut->Branch("targetY", &targetY, "targetY/D");
  treeOut->Branch("targetZ", &targetZ, "targetZ/D");

  double disTarTel, cosinOutX, cosinOutY, cosinOutZ, emitangle;
  treeOut->Branch("emitangle", &emitangle,"emitangle/D");
  double disTarTel_L, disTarTel_R, disTarTel_U, disTarTel_D;

  //tel1
  TClonesArray *tel1 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel1", &tel1);
  double tel1dEaX,tel1dEaY,tel1Eb,tel1Ec,tel1Ed, tel1Etotal;
  double tel1dEaXTiming, tel1EbTiming, tel1EcTiming, tel1EdTiming; 
  double tel1X, tel1Y, tel1Z;
  double tel1SolidAngle;
  int    tel1XID, tel1YID;
  double tel1Angle;
  double tel1Angle_min, tel1Angle_max;
  treeOut->Branch("tel1dEaXTiming", &tel1dEaXTiming,"tel1dEaXTiming/D");
  treeOut->Branch("tel1EbTiming", &tel1EbTiming,"tel1EbTiming/D");
  treeOut->Branch("tel1EcTiming", &tel1EcTiming,"tel1EcTiming/D");
  treeOut->Branch("tel1EdTiming", &tel1EdTiming,"tel1EdTiming/D");
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
  treeOut->Branch("tel1SolidAngle", &tel1SolidAngle,"tel1SolidAngle/D");
  treeOut->Branch("tel1Angle", &tel1Angle,"tel1Angle/D");
  treeOut->Branch("tel1Angle_min", &tel1Angle_min,"tel1Angle_min/D");
  treeOut->Branch("tel1Angle_max", &tel1Angle_max,"tel1Angle_max/D");

  //tel2 
  TClonesArray *tel2 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel2", &tel2);
  double tel2dEaX,tel2dEaY,tel2Eb,tel2Ec,tel2Ed, tel2Etotal;
  double tel2dEaXTiming, tel2EbTiming, tel2EcTiming, tel2EdTiming;
  double tel2X, tel2Y, tel2Z;
  double tel2SolidAngle;
  int    tel2XID, tel2YID;
  double tel2Angle;
  double tel2Angle_min, tel2Angle_max;
  treeOut->Branch("tel2dEaXTiming", &tel2dEaXTiming,"tel2dEaXTiming/D");
  treeOut->Branch("tel2EbTiming", &tel2EbTiming,"tel2EbTiming/D");
  treeOut->Branch("tel2EcTiming", &tel2EcTiming,"tel2EcTiming/D");
  treeOut->Branch("tel2EdTiming", &tel2EdTiming,"tel2EdTiming/D");
  treeOut->Branch("tel2dEaX", &tel2dEaX,"tel2dEaX/D");
  treeOut->Branch("tel2dEaY", &tel2dEaY,"tel2dEaY/D");
  treeOut->Branch("tel2Eb", &tel2Eb,"tel2Eb/D");
  treeOut->Branch("tel2Ec", &tel2Ec,"tel2Ec/D");
  treeOut->Branch("tel2Ed", &tel2Ed,"tel2Ed/D");
  treeOut->Branch("tel2Etotal", &tel2Etotal,"tel2Etotal/D");
  treeOut->Branch("tel2XID", &tel2XID,"tel2XID/I");
  treeOut->Branch("tel2YID", &tel2YID,"tel2YID/I");
  treeOut->Branch("tel2X", &tel2X,"tel2X/D");
  treeOut->Branch("tel2Y", &tel2Y,"tel2Y/D");
  treeOut->Branch("tel2Z", &tel2Z,"tel2Z/D");
  treeOut->Branch("tel2SolidAngle", &tel2SolidAngle,"tel2SolidAngle/D");
  treeOut->Branch("tel2Angle", &tel2Angle,"tel2Angle/D");
  treeOut->Branch("tel2Angle_min", &tel2Angle_min,"tel2Angle_min/D");
  treeOut->Branch("tel2Angle_max", &tel2Angle_max,"tel2Angle_max/D");
  
  //tel3 
  TClonesArray *tel3 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel3", &tel3);
  double tel3dEaX,tel3dEaY,tel3Eb,tel3Ec, tel3Etotal;
  double tel3dEaXTiming, tel3EbTiming, tel3EcTiming; 
  double tel3X, tel3Y, tel3Z;
  int    tel3XID, tel3YID;
  double tel3SolidAngle;
  double tel3Angle;
  double tel3Angle_min, tel3Angle_max;
  treeOut->Branch("tel3dEaXTiming", &tel3dEaXTiming,"tel3dEaXTiming/D");
  treeOut->Branch("tel3EbTiming", &tel3EbTiming,"tel3EbTiming/D");
  treeOut->Branch("tel3EcTiming", &tel3EcTiming,"tel3EcTiming/D");
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
  treeOut->Branch("tel3SolidAngle", &tel3SolidAngle,"tel3SolidAngle/D");
  treeOut->Branch("tel3Angle", &tel3Angle,"tel3Angle/D");
  treeOut->Branch("tel3Angle_min", &tel3Angle_min,"tel3Angle_min/D");
  treeOut->Branch("tel3Angle_max", &tel3Angle_max,"tel3Angle_max/D");

  //tel4 
  TClonesArray *tel4 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel4", &tel4);
  double tel4dEaX,tel4dEaY,tel4Eb,tel4Etotal;
  double tel4dEaXTiming, tel4EbTiming; 
  double tel4X, tel4Y, tel4Z;
  int    tel4XID, tel4YID;
  double tel4SolidAngle;
  double tel4Angle;
  double tel4Angle_min, tel4Angle_max;
  treeOut->Branch("tel4dEaXTiming", &tel4dEaXTiming,"tel4dEaXTiming/D");
  treeOut->Branch("tel4EbTiming", &tel4EbTiming,"tel4EbTiming/D");
  treeOut->Branch("tel4dEaX", &tel4dEaX,"tel4dEaX/D");
  treeOut->Branch("tel4dEaY", &tel4dEaY,"tel4dEaY/D");
  treeOut->Branch("tel4Eb", &tel4Eb,"tel4Eb/D");
  treeOut->Branch("tel4Etotal", &tel4Etotal,"tel4Etotal/D");
  treeOut->Branch("tel4XID", &tel4XID,"tel4XID/I");
  treeOut->Branch("tel4YID", &tel4YID,"tel4YID/I");
  treeOut->Branch("tel4X", &tel4X,"tel4X/D");
  treeOut->Branch("tel4Y", &tel4Y,"tel4Y/D");
  treeOut->Branch("tel4Z", &tel4Z,"tel4Z/D");
  treeOut->Branch("tel4SolidAngle", &tel4SolidAngle,"tel4SolidAngle/D");
  treeOut->Branch("tel4Angle", &tel4Angle,"tel4Angle/D");
  treeOut->Branch("tel4Angle_min", &tel4Angle_min,"tel4Angle_min/D");
  treeOut->Branch("tel4Angle_max", &tel4Angle_max,"tel4Angle_max/D");

  //tel5 
  TClonesArray *tel5 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel5", &tel5);

  double tel5dEaX,tel5dEaY,tel5Eb,tel5Etotal;
  double tel5dEaXTiming, tel5EbTiming; 
  double tel5X, tel5Y, tel5Z;
  int    tel5XID, tel5YID;
  double tel5SolidAngle;
  double tel5Angle;
  double tel5Angle_min, tel5Angle_max;
  treeOut->Branch("tel5dEaXTiming", &tel5dEaXTiming,"tel5dEaXTiming/D");
  treeOut->Branch("tel5EbTiming", &tel5EbTiming,"tel5EbTiming/D");
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
  treeOut->Branch("tel5Angle_min", &tel5Angle_min,"tel5Angle_min/D");
  treeOut->Branch("tel5Angle_max", &tel5Angle_max,"tel5Angle_max/D");

  //tel6
  TClonesArray *tel6 = new TClonesArray("art::TTelescopeData");
  tree->SetBranchAddress("tel6", &tel6);
  double tel6dEaX,tel6dEaY,tel6Eb, tel6Etotal;
  double tel6dEaXTiming, tel6EbTiming; 
  double tel6X, tel6Y, tel6Z;
  int    tel6XID, tel6YID;
  double tel6SolidAngle;
  double tel6Angle;
  double tel6Angle_min, tel6Angle_max;
  treeOut->Branch("tel6dEaXTiming", &tel6dEaXTiming,"tel6dEaXTiming/D");
  treeOut->Branch("tel6EbTiming", &tel6EbTiming,"tel6EbTiming/D");
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
  treeOut->Branch("tel6Angle_min", &tel6Angle_min,"tel6Angle_min/D");
  treeOut->Branch("tel6Angle_max", &tel6Angle_max,"tel6Angle_max/D");


  int ID =TMath::QuietNaN();

  int scale = 10;
  int events = tree->GetEntriesFast();
  for(int event = 0; event < events; event++){
    tree->GetEntry(event);
    if(event%(int(events/scale))==0)
      std::cout<<"Processed: "<<(int)(event*100.0/events+1)<<"%";
      std::cout<<"\r";

    rf0                = TMath::QuietNaN();
    single_fID         = TMath::QuietNaN();
    coin_fID           = TMath::QuietNaN();
    pileup             = TMath::QuietNaN();
    //trigTiming         = TMath::QuietNaN();
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
    tel1SolidAngle     = TMath::QuietNaN();
    tel1Angle          = TMath::QuietNaN();
    tel1Angle_min      = TMath::QuietNaN();
    tel1Angle_max      = TMath::QuietNaN();
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
    tel2X              = TMath::QuietNaN();
    tel2Y              = TMath::QuietNaN();
    tel2Z              = TMath::QuietNaN();
    tel2SolidAngle     = TMath::QuietNaN();
    tel2Angle          = TMath::QuietNaN();
    tel2Angle_min      = TMath::QuietNaN();
    tel2Angle_max      = TMath::QuietNaN();
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
    tel3SolidAngle     = TMath::QuietNaN();
    tel3Angle          = TMath::QuietNaN();
    tel3Angle_min      = TMath::QuietNaN();
    tel3Angle_max      = TMath::QuietNaN();
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
    tel4SolidAngle     = TMath::QuietNaN();
    tel4Angle          = TMath::QuietNaN();
    tel4Angle_min      = TMath::QuietNaN();
    tel4Angle_max      = TMath::QuietNaN();
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
    tel5SolidAngle     = TMath::QuietNaN();
    tel5Angle          = TMath::QuietNaN();
    tel5Angle_min      = TMath::QuietNaN();
    tel5Angle_max      = TMath::QuietNaN();
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
    tel6SolidAngle     = TMath::QuietNaN();
    tel6Angle          = TMath::QuietNaN();
    tel6Angle_min      = TMath::QuietNaN();
    tel6Angle_max      = TMath::QuietNaN();

    deltaX             = TMath::QuietNaN();
    deltaY             = TMath::QuietNaN();
    deltaZ             = TMath::QuietNaN();
    deltaRange         = TMath::QuietNaN();
    cosinInX           = TMath::QuietNaN();
    cosinInY           = TMath::QuietNaN();
    cosinInZ           = TMath::QuietNaN();


    disTarTel          = TMath::QuietNaN();
    cosinOutX          = TMath::QuietNaN();
    cosinOutY          = TMath::QuietNaN();
    cosinOutZ          = TMath::QuietNaN();
    emitangle          = TMath::QuietNaN();

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


    //assume the beam at the center of the target
    if( tel1->GetEntriesFast()==1 || tel2->GetEntriesFast()==1 || tel3->GetEntriesFast()==1 || tel4->GetEntriesFast()==1 || tel5->GetEntriesFast()==1 || tel6->GetEntriesFast()==1){
      //assume the 6He beam is at target center
      targetX  = 0.0; 
      targetY  = 0.0; 
      targetZ  = 0.0;

      //assume the 6He beam profile is gaussian shape
      //targetX = ran->Gaus(targetX,sigma_x); 
      //targetY = ran->Gaus(targetY,sigma_y); 

      //assume the direction of the incident particle is (0,0,1) 
      cosinInX   = 0.0;
      cosinInY   = 0.0;
      cosinInZ   = 1.0;

      //std::cout<<"event = "<<event+1<<", single = "<<single<<", coin = "<<coin<<", single event = "<<event + 1<<", dirX = "<<cosinInX<<", dirY = "<<cosinInY<<", dirZ = "<<cosinInZ<<std::endl;
    }
    //if(single == 0)    
    //std::cout<<"single = "<<single<<", coin = "<<coin<<", single event = "<<event + 1<<", dirX = "<<cosinInX<<", dirY = "<<cosinInY<<", dirZ = "<<cosinInZ<<std::endl;
    //else 
    //std::cout<<"single = "<<single<<", coin = "<<coin<<", coin event = "<<event + 1<<", dirX = "<<cosinInX<<", dirY = "<<cosinInY<<", dirZ = "<<cosinInZ<<std::endl;

    //tel1-6
    //tel1
    if(tel1->GetEntriesFast()==1){
      tel1dEaXTiming = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetdEXTiming();
      tel1EbTiming   = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetETimingvec(0);
      tel1EcTiming   = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetETimingvec(1);
      tel1EdTiming   = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetETimingvec(2);
      tel1dEaX       = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetdEX();
      tel1dEaY       = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetdEY();
      tel1Eb         = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetEvec(0);
      tel1Ec         = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetEvec(1);
      tel1Ed         = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetEvec(2);
      tel1XID        = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetXID();
      tel1YID        = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetYID();
      tel1X          = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetX();
      tel1Y          = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetY();
      tel1Z          = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetZ();
      tel1Etotal     = tel1dEaX + tel1Eb + tel1Ec + tel1Ed;

      double detX   = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetTX();  
      double detY   = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetTY();  
      double detZ   = static_cast<const art::TTelescopeData *>(tel1->At(0))->GetTZ();  

      //assume the beam is at the pixel center
      double tmp_X_c = strip_width/2.0 + (Double_t)(tel1XID - 8)*50.0/16.0;
      double tmp_Y_c = strip_width/2.0 + (Double_t)(tel1YID - 8)*50.0/16.0;
      double X_cent = detX + tmp_X_c * TMath::Cos(tel_Angle[0] * TMath::Pi()/180.0);
      double Y_cent = detY - 1.0 * tmp_Y_c;
      double Z_cent = detZ - tmp_X_c * TMath::Sin(tel_Angle[0] * TMath::Pi()/180.0);

      //the direction of the emitted particle 
      //disTarTel      = (X_cent - targetX)*(X_cent - targetX) + (Y_cent - targetY)*(Y_cent - targetY) + (Z_cent - targetZ)*(Z_cent - targetZ);
      //disTarTel      = sqrt(disTarTel);
      //cosinOutX      = (X_cent - targetX) / disTarTel;
      //cosinOutY      = (Y_cent - targetY) / disTarTel;
      //cosinOutZ      = (Z_cent - targetZ) / disTarTel;
      disTarTel      = (tel1X - targetX)*(tel1X - targetX) + (tel1Y - targetY)*(tel1Y - targetY) + (tel1Z - targetZ)*(tel1Z - targetZ);
      disTarTel      = sqrt(disTarTel);
      cosinOutX      = (tel1X - targetX) / disTarTel;
      cosinOutY      = (tel1Y - targetY) / disTarTel;
      cosinOutZ      = (tel1Z - targetZ) / disTarTel;
      //the emitted angle respect to the incident beam
      tel1Angle  = cosinInX * cosinOutX + cosinInY * cosinOutY + cosinInZ * cosinOutZ;
      tel1Angle  = acos(tel1Angle) * rad2deg;

      //calculate the solid angle
      double dTX = detX-targetX;
      double dTY = detY-targetY;
      double dTZ = detZ-targetZ;
      //double dX  = tel1X-targetX;
      //double dY  = tel1Y-targetY;
      //double dZ  = tel1Z-targetZ;
      double dX  = X_cent-targetX;
      double dY  = Y_cent-targetY;
      double dZ  = Z_cent-targetZ;
      double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      if(costheta > 0) tel1SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
    }

    //tel2
    if(tel2->GetEntriesFast()==1){
      tel2dEaXTiming = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetdEXTiming();
      tel2EbTiming   = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetETimingvec(0);
      tel2EcTiming   = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetETimingvec(1);
      tel2EdTiming   = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetETimingvec(2);
      tel2dEaX       = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetdEX();
      tel2dEaY       = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetdEY();
      tel2Eb         = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetEvec(0);
      tel2Ec         = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetEvec(1);
      tel2Ed         = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetEvec(2);
      tel2XID        = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetXID();
      tel2YID        = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetYID();
      tel2X          = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetX();
      tel2Y          = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetY();
      tel2Z          = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetZ();
      tel2Etotal     = tel2dEaX + tel2Eb + tel2Ec + tel2Ed;
      
      double detX   = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetTX();  
      double detY   = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetTY();  
      double detZ   = static_cast<const art::TTelescopeData *>(tel2->At(0))->GetTZ();  
      //assume the beam is at the pixel center
      double tmp_X_c = strip_width/2.0 + (Double_t)(tel2XID - 8)*50.0/16.0;
      double tmp_Y_c = strip_width/2.0 + (Double_t)(tel2YID - 8)*50.0/16.0;
      double X_cent = detX + tmp_X_c * TMath::Cos(tel_Angle[1] * TMath::Pi()/180.0);
      double Y_cent = detY - 1.0 * tmp_Y_c;
      double Z_cent = detZ - tmp_X_c * TMath::Sin(tel_Angle[1] * TMath::Pi()/180.0);

      //the direction of the emitted particle 
      //disTarTel      = (X_cent - targetX)*(X_cent - targetX) + (Y_cent - targetY)*(Y_cent - targetY) + (Z_cent - targetZ)*(Z_cent - targetZ);
      //disTarTel      = sqrt(disTarTel);
      //cosinOutX      = (X_cent - targetX) / disTarTel;
      //cosinOutY      = (Y_cent - targetY) / disTarTel;
      //cosinOutZ      = (Z_cent - targetZ) / disTarTel;
      disTarTel      = (tel2X - targetX)*(tel2X - targetX) + (tel2Y - targetY)*(tel2Y - targetY) + (tel2Z - targetZ)*(tel2Z - targetZ);
      disTarTel      = sqrt(disTarTel);
      cosinOutX      = (tel2X - targetX) / disTarTel;
      cosinOutY      = (tel2Y - targetY) / disTarTel;
      cosinOutZ      = (tel2Z - targetZ) / disTarTel;

      //the emitted angle respect to the incident beam
      tel2Angle      = cosinInX * cosinOutX + cosinInY * cosinOutY + cosinInZ * cosinOutZ;
      tel2Angle      = acos(tel2Angle) * rad2deg;
      //calculate the solid angle
      double dTX = detX-targetX;
      double dTY = detY-targetY;
      double dTZ = detZ-targetZ;
      //double dX  = tel2X-targetX;
      //double dY  = tel2Y-targetY;
      //double dZ  = tel2Z-targetZ;
      double dX  = X_cent-targetX;
      double dY  = Y_cent-targetY;
      double dZ  = Z_cent-targetZ;
      double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      if(costheta > 0) tel2SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
    }
    
    //tel3
    if(tel3->GetEntriesFast()==1){
      tel3dEaXTiming = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetdEXTiming();
      tel3EbTiming   = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetETimingvec(0);
      tel3EcTiming   = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetETimingvec(1);
      tel3dEaX       = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetdEX();
      tel3dEaY       = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetdEY();
      tel3Eb         = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetEvec(0);
      tel3Ec         = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetEvec(1);
      tel3XID        = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetXID();
      tel3YID        = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetYID();
      tel3X          = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetX();
      tel3Y          = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetY();
      tel3Z          = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetZ();
      tel3Etotal     = tel3dEaX + tel3Eb + tel3Ec;

      double detX   = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetTX();  
      double detY   = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetTY();  
      double detZ   = static_cast<const art::TTelescopeData *>(tel3->At(0))->GetTZ();  
      //assume the beam is at the pixel center
      double tmp_X_c = strip_width/2.0 + (Double_t)(tel3XID - 8)*50.0/16.0;
      double tmp_Y_c = strip_width/2.0 + (Double_t)(tel3YID - 8)*50.0/16.0;
      double X_cent = detX + tmp_X_c * TMath::Cos(tel_Angle[2] * TMath::Pi()/180.0);
      double Y_cent = detY - 1.0 * tmp_Y_c;
      double Z_cent = detZ - tmp_X_c * TMath::Sin(tel_Angle[2] * TMath::Pi()/180.0);
      //the direction of the emitted particle 
      //disTarTel      = (X_cent - targetX)*(X_cent - targetX) + (Y_cent - targetY)*(Y_cent - targetY) + (Z_cent - targetZ)*(Z_cent - targetZ);
      //disTarTel      = sqrt(disTarTel);
      //cosinOutX      = (X_cent - targetX) / disTarTel;
      //cosinOutY      = (Y_cent - targetY) / disTarTel;
      //cosinOutZ      = (Z_cent - targetZ) / disTarTel;
      disTarTel      = (tel3X - targetX)*(tel3X - targetX) + (tel3Y - targetY)*(tel3Y - targetY) + (tel3Z - targetZ)*(tel3Z - targetZ);
      disTarTel      = sqrt(disTarTel);
      cosinOutX      = (tel3X - targetX) / disTarTel;
      cosinOutY      = (tel3Y - targetY) / disTarTel;
      cosinOutZ      = (tel3Z - targetZ) / disTarTel;

      //the emitted angle respect to the incident beam
      tel3Angle  = cosinInX * cosinOutX + cosinInY * cosinOutY + cosinInZ * cosinOutZ;
      tel3Angle  = acos(tel3Angle) * rad2deg;
      //calculate the solid angle
      double dTX = detX-targetX;
      double dTY = detY-targetY;
      double dTZ = detZ-targetZ;
      //double dX  = tel3X-targetX;
      //double dY  = tel3Y-targetY;
      //double dZ  = tel3Z-targetZ;
      double dX  = X_cent-targetX;
      double dY  = Y_cent-targetY;
      double dZ  = Z_cent-targetZ;
      double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      if(costheta > 0) tel3SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ);
    }

    //tel4
    if(tel4->GetEntriesFast()==1){
      tel4dEaXTiming = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetdEXTiming();
      tel4EbTiming   = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetETimingvec(0);
      tel4dEaX       = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetdEX();
      tel4dEaY       = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetdEY();
      tel4Eb         = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetEvec(0);
      tel4XID        = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetXID();
      tel4YID        = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetYID();
      tel4X          = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetX();
      tel4Y          = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetY();
      tel4Z          = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetZ();
      tel4Etotal     = tel4dEaX + tel4Eb;

      double detX   = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetTX();  
      double detY   = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetTY();  
      double detZ   = static_cast<const art::TTelescopeData *>(tel4->At(0))->GetTZ();  
      //assume the beam is at the pixel center
      double tmp_X_c = strip_width/2.0 + (Double_t)(tel4XID - 8)*50.0/16.0;
      double tmp_Y_c = strip_width/2.0 + (Double_t)(tel4YID - 8)*50.0/16.0;
      double X_cent = detX + tmp_X_c * TMath::Cos(tel_Angle[3] * TMath::Pi()/180.0);
      double Y_cent = detY - 1.0 * tmp_Y_c;
      double Z_cent = detZ - tmp_X_c * TMath::Sin(tel_Angle[3] * TMath::Pi()/180.0);
      //the direction of the emitted particle 
      //disTarTel      = (X_cent - targetX)*(X_cent - targetX) + (Y_cent - targetY)*(Y_cent - targetY) + (Z_cent - targetZ)*(Z_cent - targetZ);
      //disTarTel      = sqrt(disTarTel);
      //cosinOutX      = (X_cent - targetX) / disTarTel;
      //cosinOutY      = (Y_cent - targetY) / disTarTel;
      //cosinOutZ      = (Z_cent - targetZ) / disTarTel;
      disTarTel      = (tel4X - targetX)*(tel4X - targetX) + (tel4Y - targetY)*(tel4Y - targetY) + (tel4Z - targetZ)*(tel4Z - targetZ);
      disTarTel      = sqrt(disTarTel);
      cosinOutX      = (tel4X - targetX) / disTarTel;
      cosinOutY      = (tel4Y - targetY) / disTarTel;
      cosinOutZ      = (tel4Z - targetZ) / disTarTel;

      //the emitted angle respect to the incident beam
      tel4Angle      = cosinInX * cosinOutX + cosinInY * cosinOutY + cosinInZ * cosinOutZ;
      tel4Angle      = acos(tel4Angle) * rad2deg;
      //calculate the solid angle
      double dTX = detX-targetX;
      double dTY = detY-targetY;
      double dTZ = detZ-targetZ;
      //double dX  = tel4X-targetX;
      //double dY  = tel4Y-targetY;
      //double dZ  = tel4Z-targetZ;
      double dX  = X_cent-targetX;
      double dY  = Y_cent-targetY;
      double dZ  = Z_cent-targetZ;
      double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      if(costheta > 0) tel4SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
    }

    //tel5
    if(tel5->GetEntriesFast()==1){
      tel5dEaXTiming = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetdEXTiming();
      tel5EbTiming   = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetETimingvec(0);
      tel5dEaX       = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetdEX();
      tel5dEaY       = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetdEY();
      tel5Eb         = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetEvec(0);
      tel5XID        = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetXID();
      tel5YID        = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetYID();
      tel5X          = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetX();
      tel5Y          = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetY();
      tel5Z          = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetZ();
      tel5Etotal     = tel5dEaX + tel5Eb;

      double detX   = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetTX();  
      double detY   = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetTY();  
      double detZ   = static_cast<const art::TTelescopeData *>(tel5->At(0))->GetTZ();  
      //assume the beam is at the pixel center
      double tmp_X_c = strip_width/2.0 + (Double_t)(tel5XID - 8)*50.0/16.0;
      double tmp_Y_c = strip_width/2.0 + (Double_t)(tel5YID - 8)*50.0/16.0;
      double X_cent = detX + tmp_X_c * TMath::Cos(tel_Angle[4] * TMath::Pi()/180.0);
      double Y_cent = detY - 1.0 * tmp_Y_c;
      double Z_cent = detZ - tmp_X_c * TMath::Sin(tel_Angle[4] * TMath::Pi()/180.0);
      //the direction of the emitted particle 
      //disTarTel      = (X_cent - targetX)*(X_cent - targetX) + (Y_cent - targetY)*(Y_cent - targetY) + (Z_cent - targetZ)*(Z_cent - targetZ);
      //disTarTel      = sqrt(disTarTel);
      //cosinOutX      = (X_cent - targetX) / disTarTel;
      //cosinOutY      = (Y_cent - targetY) / disTarTel;
      //cosinOutZ      = (Z_cent - targetZ) / disTarTel;
      disTarTel      = (tel5X - targetX)*(tel5X - targetX) + (tel5Y - targetY)*(tel5Y - targetY) + (tel5Z - targetZ)*(tel5Z - targetZ);
      disTarTel      = sqrt(disTarTel);
      cosinOutX      = (tel5X - targetX) / disTarTel;
      cosinOutY      = (tel5Y - targetY) / disTarTel;
      cosinOutZ      = (tel5Z - targetZ) / disTarTel;

      //the emitted angle respect to the incident beam
      tel5Angle      = cosinInX * cosinOutX + cosinInY * cosinOutY + cosinInZ * cosinOutZ;
      tel5Angle      = acos(tel5Angle) * rad2deg;
      //calculate the solid angle
      double dTX = detX-targetX;
      double dTY = detY-targetY;
      double dTZ = detZ-targetZ;
      //double dX  = tel5X-targetX;
      //double dY  = tel5Y-targetY;
      //double dZ  = tel5Z-targetZ;
      double dX  = X_cent-targetX;
      double dY  = Y_cent-targetY;
      double dZ  = Z_cent-targetZ;
      double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      if(costheta > 0) tel5SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
    }

    //tel6
    if(tel6->GetEntriesFast()==1){
      tel6dEaXTiming = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetdEXTiming();
      tel6EbTiming   = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetETimingvec(0);
      tel6dEaX       = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetdEX();
      tel6dEaY       = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetdEY();
      tel6Eb         = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetEvec(0);
      tel6XID        = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetXID();
      tel6YID        = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetYID();
      tel6X          = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetX();
      tel6Y          = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetY();
      tel6Z          = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetZ();
      tel6Etotal     = tel6dEaX + tel6Eb;

      double detX   = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetTX();  
      double detY   = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetTY();  
      double detZ   = static_cast<const art::TTelescopeData *>(tel6->At(0))->GetTZ();  
      //assume the beam is at the pixel center
      double tmp_X_c = strip_width/2.0 + (Double_t)(tel6XID - 8)*50.0/16.0;
      double tmp_Y_c = strip_width/2.0 + (Double_t)(tel6YID - 8)*50.0/16.0;
      double X_cent = detX + tmp_X_c * TMath::Cos(tel_Angle[5] * TMath::Pi()/180.0);
      double Y_cent = detY - 1.0 * tmp_Y_c;
      double Z_cent = detZ - tmp_X_c * TMath::Sin(tel_Angle[5] * TMath::Pi()/180.0);
      //the direction of the emitted particle 
      //disTarTel      = (X_cent - targetX)*(X_cent - targetX) + (Y_cent - targetY)*(Y_cent - targetY) + (Z_cent - targetZ)*(Z_cent - targetZ);
      //disTarTel      = sqrt(disTarTel);
      //cosinOutX      = (X_cent - targetX) / disTarTel;
      //cosinOutY      = (Y_cent - targetY) / disTarTel;
      //cosinOutZ      = (Z_cent - targetZ) / disTarTel;
      disTarTel      = (tel6X - targetX)*(tel6X - targetX) + (tel6Y - targetY)*(tel6Y - targetY) + (tel6Z - targetZ)*(tel6Z - targetZ);
      disTarTel      = sqrt(disTarTel);
      cosinOutX      = (tel6X - targetX) / disTarTel;
      cosinOutY      = (tel6Y - targetY) / disTarTel;
      cosinOutZ      = (tel6Z - targetZ) / disTarTel;

      //the emitted angle respect to the incident beam
      tel6Angle      = cosinInX * cosinOutX + cosinInY * cosinOutY + cosinInZ * cosinOutZ;
      tel6Angle      = acos(tel6Angle) * rad2deg;
      //calculate the solid angle
      double dTX = detX-targetX;
      double dTY = detY-targetY;
      double dTZ = detZ-targetZ;
      //double dX  = tel6X-targetX;
      //double dY  = tel6Y-targetY;
      //double dZ  = tel6Z-targetZ;
      double dX  = X_cent-targetX;
      double dY  = Y_cent-targetY;
      double dZ  = Z_cent-targetZ;
      double costheta = dTX*dX + dTY*dY + dTZ*dZ;
      costheta = costheta/(sqrt(dTX*dTX + dTY*dTY + dTZ*dTZ) * sqrt(dX*dX + dY*dY + dZ*dZ));
      if(costheta > 0)tel6SolidAngle = pixel_area * costheta / (dTX*dTX + dTY*dTY + dTZ*dTZ); 
    }

    treeOut->Fill();
  }


 //emitangle_tel->Write();
 //Energy_Angle->Write();
 treeOut->Write();
 gROOT->ProcessLine(".q");
}
