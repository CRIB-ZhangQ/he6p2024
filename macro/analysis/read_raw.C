#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TROOT.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include "/home/crib/art_analysis/he6p/zhang/share/src/mwdc/TMWDCTrackingResult.h"
#include "/home/crib/art_analysis/he6p/zhang/share/src/cont/TTimingChargeData.h"
#include "/home/crib/art_analysis/he6p/zhang/src/enewz/TEnewzCalculator.h"
#include "/home/crib/art_analysis/he6p/zhang/crib-share/src/telescope/TTelescopeData.h"
namespace art{
    class TMWDCTrackingResult;
    class TTelescopeData;
}

const double deg2rad = acos(-1.0) / 180.0;
const double atomic_u = 931.494013; 
double Q_value = 0.0;//1.797
struct mass{
    const double c2 = 511.0 / 0.00054858; 
    const double He_6_0  = 6.01885889;
    const double p = 1.007825031898;
    const double He_6_1  = (He_6_0 + Q_value / c2);
    const double H3 = 3.01604927767; 
    const double He = 4.002602; 
    const double He_5 = 5.012223624;
    const double deu = 2.01410177785;
    const double Carbon = 12.011;
}Mass;
Char_t det_mat[34] = "SI";


const double rad2deg = 180.0 / acos(-1.0); 
void read() 
{
    TFile fin("/home/crib/art_analysis/he6p/zhang/output/run/0180/chkf3run0180.root");
    TFile fout("/home/crib/art_analysis/he6p/zhang/macro/analysis/data/data0180.root", "recreate");
    
    TTree *tree = (TTree*) fin.Get("tree");
    TTree *tree_out = new TTree("read","just try");
    TTree *tree_out1 = new TTree("kine:p0+6He","");
    //TTree *tree_out2 = new TTree("kine:p1+6He","");
    TTree *tree_out3 = new TTree("kine:p+3H","");
    TTree *tree_out4 = new TTree("kine:a+t","");
    TTree *tree_out5 = new TTree("kine:d+5He","");
    TTree *tree_out6 = new TTree("kine:C+6He","");
    

    TClonesArray *mwdca = new TClonesArray("art::TMWDCTrackingResult");
    TClonesArray *mwdcb = new TClonesArray("art::TMWDCTrackingResult");
    TClonesArray *f3mwdc = new TClonesArray("art::TMWDCTrackingResult");
    TClonesArray *tel1 = new TClonesArray("art::TTelescopeData");
    TClonesArray *tel2 = new TClonesArray("art::TTelescopeData");
    TClonesArray *tel3 = new TClonesArray("art::TTelescopeData");
    TClonesArray *tel4 = new TClonesArray("art::TTelescopeData");
    TClonesArray *tel5 = new TClonesArray("art::TTelescopeData");
    TClonesArray *tel6 = new TClonesArray("art::TTelescopeData");

    tree->SetBranchAddress("mwdca", &mwdca);
    tree->SetBranchAddress("mwdcb", &mwdcb);
    tree->SetBranchAddress("tel1", &tel1);
    tree->SetBranchAddress("tel2", &tel2);
    tree->SetBranchAddress("tel3", &tel3);
    tree->SetBranchAddress("tel4", &tel4);
    tree->SetBranchAddress("tel5", &tel5);
    tree->SetBranchAddress("tel6", &tel6);

    Double_t mwdca_x{0.0}, mwdca_y{0.0}, mwdca_z{0.0};
    Double_t mwdcb_x{0.0}, mwdcb_y{0.0}, mwdcb_z{0.0};
    Double_t Beam_angle{0.0};
    Double_t tar_X{0.0}, tar_Y{0.0}, tar_Z{0.0};
    Double_t emitangle_tel1{0.0}, emitangle_tel2{0.0}, emitangle_tel3{0.0}, emitangle_tel4{0.0}, emitangle_tel5{0.0}, emitangle_tel6{0.0}; 
    //tel1
    Double_t tel1dEXa{0.0}, tel1dEYa{0.0}, tel1Eb{0.0}, tel1Ec{0.0}, tel1Ed{0.0}, tel1Etotal{0.0};
    Double_t tel1X{0.0}, tel1Y{0.0}, tel1Z{0.0};
    Double_t tel2dEXa{0.0}, tel2dEYa{0.0}, tel2Eb{0.0}, tel2Ec{0.0}, tel2Ed{0.0}, tel2Etotal{0.0};
    Double_t tel2X{0.0}, tel2Y{0.0}, tel2Z{0.0};
    Double_t tel3dEXa{0.0}, tel3dEYa{0.0}, tel3Eb{0.0}, tel3Ec{0.0}, tel3Ed{0.0}, tel3Etotal{0.0};
    Double_t tel3X{0.0}, tel3Y{0.0}, tel3Z{0.0};
    Double_t tel4dEXa{0.0}, tel4dEYa{0.0}, tel4Eb{0.0}, tel4Ec{0.0}, tel4Ed{0.0}, tel4Etotal{0.0};
    Double_t tel4X{0.0}, tel4Y{0.0}, tel4Z{0.0};
    Double_t tel5dEXa{0.0}, tel5dEYa{0.0}, tel5Eb{0.0}, tel5Etotal{0.0};
    Double_t tel5X{0.0}, tel5Y{0.0}, tel5Z{0.0};
    Double_t tel6dEXa{0.0}, tel6dEYa{0.0}, tel6Eb{0.0}, tel6Etotal{0.0};
    Double_t tel6X{0.0}, tel6Y{0.0}, tel6Z{0.0};
    
    Int_t    tel1XID{0}, tel1YID{0};
    Int_t    tel2XID{0}, tel2YID{0};
    Int_t    tel3XID{0}, tel3YID{0};
    Int_t    tel4XID{0}, tel4YID{0};
    Int_t    tel5XID{0}, tel5YID{0};
    Int_t    tel6XID{0}, tel6YID{0};
    Double_t tel1E_raw[3]={0};
    Double_t energy_6He{0.0};
    Double_t theta_6He_p0{0.0}, energy_6He_p0{0.0};
    Double_t theta_p0_6He{0.0}, energy_p0_6He{0.0};
    Double_t theta_p1_6He{0.0}, energy_p1_6He{0.0};
    Double_t theta_3H_p{0.0}, energy_3H_p{0.0};
    Double_t theta_p_3H{0.0}, energy_p_3H{0.0};
    Double_t theta_t{0.0}, energy_t{0.0};
    Double_t theta_a{0.0}, energy_a{0.0};
    Double_t theta_d{0.0}, energy_d{0.0};
    Double_t theta_Carbon_6He{0.0}, energy_Carbon_6He{0.0};
    Double_t dEXa_c_p0{0.0},  Eb_c_p0{0.0},  Ec_c_p0{0.0},  Ed_c_p0{0.0}, Etotal_c_p0{0.0};
    Double_t dEXa_c_6He{0.0}, Eb_c_6He{0.0}, Ec_c_6He{0.0}, Ed_c_6He{0.0},Etotal_c_6He{0.0};
    Double_t dEXa_c_t{0.0},   Eb_c_t{0.0},   Ec_c_t{0.0},   Ed_c_t{0.0},  Etotal_c_t{0.0};
    Double_t dEXa_c_a{0.0},   Eb_c_a{0.0},   Ec_c_a{0.0},   Ed_c_a{0.0},  Etotal_c_a{0.0};
    Double_t dEXa_c_d{0.0},   Eb_c_d{0.0},   Ec_c_d{0.0},   Ed_c_d{0.0},  Etotal_c_d{0.0};
    Double_t dEXa_c_p2{0.0},  Eb_c_p2{0.0},  Ec_c_p2{0.0},  Ed_c_p2{0.0}, Etotal_c_p2{0.0};
    Double_t dEXa_c_Carbon_6He{0.0},  Eb_c_Carbon_6He{0.0},  Ec_c_Carbon_6He{0.0},  Ed_c_Carbon_6He{0.0}, Etotal_c_Carbon_6He{0.0};


    tree_out->Branch("mwdca_x", &mwdca_x, "mwdca_x/D");
    tree_out->Branch("mwdca_y", &mwdca_y, "mwdca_y/D");
    tree_out->Branch("mwdcb_x", &mwdcb_x, "mwdcb_x/D");
    tree_out->Branch("mwdcb_y", &mwdcb_y, "mwdcb_y/D");
    tree_out->Branch("energy_6He", &energy_6He, "energy_6He/D");
    
    //telescope 1
    tree_out->Branch("tel1Eb", &tel1Eb, "tel1Eb/D");
    tree_out->Branch("tel1Ec", &tel1Ec, "tel1Ec/D");
    tree_out->Branch("tel1Ed", &tel1Ed, "tel1Ed/D");
    tree_out->Branch("tel1Etotal", &tel1Etotal, "tel1Etotal/D");
    tree_out->Branch("tel1dEXa", &tel1dEXa, "tel1dEXa/D");
    tree_out->Branch("tel1dEYa", &tel1dEYa, "tel1dEYa/D");
    tree_out->Branch("emitangle_tel1", &emitangle_tel1, "emitangle_tel1/D");
    tree_out->Branch("tel1XID", &tel1XID, "tel1XID/I");
    tree_out->Branch("tel1YID", &tel1YID, "tel1YID/I");
    tree_out->Branch("tel1X", &tel1X, "tel1X/D");
    tree_out->Branch("tel1Y", &tel1Y, "tel1Y/D");
    tree_out->Branch("tel1Z", &tel1Z, "tel1Z/D");
    
    //telescope2 
    
    //telescope2 
    tree_out->Branch("tel2Eb", &tel2Eb, "tel2Eb/D");
    tree_out->Branch("tel2Ec", &tel2Ec, "tel2Ec/D");
    tree_out->Branch("tel2Ed", &tel2Ed, "tel2Ed/D");
    tree_out->Branch("tel2Etotal", &tel2Etotal, "tel2Etotal/D");
    tree_out->Branch("tel2dEXa", &tel2dEXa, "tel2dEXa/D");
    tree_out->Branch("tel2dEYa", &tel2dEYa, "tel2dEYa/D");
    tree_out->Branch("emitangle_tel2", &emitangle_tel2, "emitangle_tel2/D");
    tree_out->Branch("tel2XID", &tel2XID, "tel2XID/I");
    tree_out->Branch("tel2YID", &tel2YID, "tel2YID/I");
    tree_out->Branch("tel2X", &tel2X, "tel2X/D");
    tree_out->Branch("tel2Y", &tel2Y, "tel2Y/D");
    tree_out->Branch("tel2Z", &tel2Z, "tel2Z/D");
    
    //telescope3 
    tree_out->Branch("tel3Eb", &tel3Eb, "tel3Eb/D");
    tree_out->Branch("tel3Ec", &tel3Ec, "tel3Ec/D");
    tree_out->Branch("tel3Ed", &tel3Ed, "tel3Ed/D");
    tree_out->Branch("tel3Etotal", &tel3Etotal, "tel3Etotal/D");
    tree_out->Branch("tel3dEXa", &tel3dEXa, "tel3dEXa/D");
    tree_out->Branch("tel3dEYa", &tel3dEYa, "tel3dEYa/D");
    tree_out->Branch("emitangle_tel3", &emitangle_tel3, "emitangle_tel3/D");
    tree_out->Branch("tel3XID", &tel3XID, "tel3XID/I");
    tree_out->Branch("tel3YID", &tel3YID, "tel3YID/I");
    tree_out->Branch("tel3X", &tel3X, "tel3X/D");
    tree_out->Branch("tel3Y", &tel3Y, "tel3Y/D");
    tree_out->Branch("tel3Z", &tel3Z, "tel3Z/D");
    
    
    //telescope4 
    tree_out->Branch("tel4Eb", &tel4Eb, "tel4Eb/D");
    tree_out->Branch("tel4Etotal", &tel4Etotal, "tel4Etotal/D");
    tree_out->Branch("tel4dEXa", &tel4dEXa, "tel4dEXa/D");
    tree_out->Branch("tel4dEYa", &tel4dEYa, "tel4dEYa/D");
    tree_out->Branch("emitangle_tel4", &emitangle_tel4, "emitangle_tel4/D");
    tree_out->Branch("tel4XID", &tel4XID, "tel4XID/I");
    tree_out->Branch("tel4YID", &tel4YID, "tel4YID/I");
    tree_out->Branch("tel4X", &tel4X, "tel4X/D");
    tree_out->Branch("tel4Y", &tel4Y, "tel4Y/D");
    tree_out->Branch("tel4Z", &tel4Z, "tel4Z/D");
    
    //telescope5 
    tree_out->Branch("tel5Eb", &tel5Eb, "tel5Eb/D");
    tree_out->Branch("tel5Etotal", &tel5Etotal, "tel5Etotal/D");
    tree_out->Branch("tel5dEXa", &tel5dEXa, "tel5dEXa/D");
    tree_out->Branch("tel5dEYa", &tel5dEYa, "tel5dEYa/D");
    tree_out->Branch("emitangle_tel5", &emitangle_tel5, "emitangle_tel5/D");
    tree_out->Branch("tel5XID", &tel5XID, "tel5XID/I");
    tree_out->Branch("tel5YID", &tel5YID, "tel5YID/I");
    tree_out->Branch("tel5X", &tel5X, "tel5X/D");
    tree_out->Branch("tel5Y", &tel5Y, "tel5Y/D");
    tree_out->Branch("tel5Z", &tel5Z, "tel5Z/D");

    //telescope6 
    tree_out->Branch("tel6Eb", &tel6Eb, "tel6Eb/D");
    tree_out->Branch("tel6Etotal", &tel6Etotal, "tel6Etotal/D");
    tree_out->Branch("tel6dEXa", &tel6dEXa, "tel6dEXa/D");
    tree_out->Branch("tel6dEYa", &tel6dEYa, "tel6dEYa/D");
    tree_out->Branch("emitangle_tel6", &emitangle_tel6, "emitangle_tel6/D");
    tree_out->Branch("tel6XID", &tel6XID, "tel6XID/I");
    tree_out->Branch("tel6YID", &tel6YID, "tel6YID/I");
    tree_out->Branch("tel6X", &tel6X, "tel6X/D");
    tree_out->Branch("tel6Y", &tel6Y, "tel6Y/D");
    tree_out->Branch("tel6Z", &tel6Z, "tel6Z/D");

    // kinematics for 6He(p,p0)6he
    tree_out1->Branch("theta_6He_p0",  &theta_6He_p0, "theta_6He_p0/D");
    tree_out1->Branch("energy_6He_p0", &energy_6He_p0,"energy_6He_p0/D");
    tree_out1->Branch("theta_p0_6He",  &theta_p0_6He, "theta_p0_6He/D");
    tree_out1->Branch("energy_p0_6He", &energy_p0_6He,"energy_p0_6He/D");
    tree_out1->Branch("dEXa_c_p0", &dEXa_c_p0, "dEXa_c_p0/D");
    tree_out1->Branch("Eb_c_p0", &Eb_c_p0, "Eb_c_p0/D");
    tree_out1->Branch("Ec_c_p0", &Ec_c_p0, "Ec_c_p0/D");
    tree_out1->Branch("Ed_c_p0", &Ed_c_p0, "Ed_c_p0/D");
    tree_out1->Branch("dEXa_c_6He", &dEXa_c_6He, "dEXa_c_6He/D");
    tree_out1->Branch("Eb_c_6He", &Eb_c_6He, "Eb_c_6He/D");
    tree_out1->Branch("Ec_c_6He", &Ec_c_6He, "Ec_c_6He/D");
    tree_out1->Branch("Ed_c_6He", &Ed_c_6He, "Ed_c_6He/D");
    
    // kinematics for 3H(p,p)
    tree_out3->Branch("theta_3H_p", &theta_3H_p, "theta_3H_p/D");
    tree_out3->Branch("energy_3H_p", &energy_3H_p, "energy_3H_p/D");
    tree_out3->Branch("theta_p_3H", &theta_p_3H, "theta_p_3H/D");
    tree_out3->Branch("energy_p_3H", &energy_p_3H, "energy_p_3H/D");
    tree_out3->Branch("dEXa_c_p2", &dEXa_c_p2, "dEXa_c_p2/D");
    tree_out3->Branch("Eb_c_p2", &Eb_c_p2, "Eb_c_p2/D");
    tree_out3->Branch("Ec_c_p2", &Ec_c_p2, "Ec_c_p2/D");
    tree_out3->Branch("Ed_c_p2", &Ed_c_p2, "Ed_c_p2/D");

    // kinematics for 6He(p,t)a
    tree_out4->Branch("energy_t", &energy_t, "energy_t/D");
    tree_out4->Branch("theta_t", &theta_t, "theta_t/D");
    tree_out4->Branch("energy_a", &energy_a, "energy_a/D");
    tree_out4->Branch("theta_a", &theta_a, "theta_a/D");
    tree_out4->Branch("dEXa_c_t", &dEXa_c_t, "dEXa_c_t/D");
    tree_out4->Branch("Eb_c_t", &Eb_c_t, "Eb_c_t/D");
    tree_out4->Branch("Ec_c_t", &Ec_c_t, "Ec_c_t/D");
    tree_out4->Branch("Ed_c_t", &Ed_c_t, "Ed_c_t/D");
    tree_out4->Branch("dEXa_c_a", &dEXa_c_a, "dEXa_c_a/D");
    tree_out4->Branch("Eb_c_a", &Eb_c_a, "Eb_c_a/D");
    tree_out4->Branch("Ec_c_a", &Ec_c_a, "Ec_c_a/D");
    tree_out4->Branch("Ed_c_a", &Ed_c_a, "Ed_c_a/D");

    // kinematics for the 6He(p,d)5He reaction
    tree_out5->Branch("energy_d", &energy_d, "energy_d/D");
    tree_out5->Branch("theta_d", &theta_d, "theta_d/D");
    tree_out5->Branch("dEXa_c_d", &dEXa_c_d, "dEXa_c_d/D");
    tree_out5->Branch("Eb_c_d", &Eb_c_d, "Eb_c_d/D");
    tree_out5->Branch("Ec_c_d", &Ec_c_d, "Ec_c_d/D");
    tree_out5->Branch("Ed_c_d", &Ed_c_d, "Ed_c_d/D");
    
    // kinematics for the 6He(C,C)6He reaction
    tree_out5->Branch("energy_Carbon_6He", &energy_Carbon_6He, "energy_Carbon_6He/D");
    tree_out5->Branch("theta_Carbon_6He", &theta_Carbon_6He, "theta_Carbon_6He/D");
    tree_out5->Branch("dEXa_c_Carbon_6He", &dEXa_c_Carbon_6He, "dEXa_c_Carbon_6He/D");
    tree_out5->Branch("Eb_c_Carbon_6He", &Eb_c_Carbon_6He, "Eb_c_Carbon_6He/D");
    tree_out5->Branch("Ec_c_Carbon_6He", &Ec_c_Carbon_6He, "Ec_c_Carbon_6He/D");
    tree_out5->Branch("Ed_c_Carbon_6He", &Ed_c_Carbon_6He, "Ed_c_Carbon_6He/D");



    TH2D *mwdcaXY = new TH2D("mwdca", "mwdca Y vs X",100, -60, 60, 100, -60, 60);
    TH2D *mwdcbXY = new TH2D("mwdcb", "mwdcb Y vs X",100, -60, 60, 100, -60, 60);
    TH2D *hpxpy = new TH2D("hpxpy", "beam in target",100, -60, 60, 100, -60, 60);
    TH2D *hpxpy2 = new TH2D("hpxpy2_X_Y", "beam in DSSD",100, -60, 60, 100, -60, 60);
    TH2D *hpxpy3 = new TH2D("hpxpy3_X_Y", "beam in DSSD",100, -60, 60, 100, -60, 60);
    TH2D *Beam_energy = new TH2D("Beam_proton_angle", "Beam energy calculated by proton",100, 0, 35, 400, 0, 60);
    TH2D *kinematic_p0_6He = new TH2D("kinematic_p0_6He","p+6He elastic scattering",180,0,15,180,0,60);
    TH2D *kinematic_6He_p0 = new TH2D("kinematic_6He_p0","p+6He elastic scattering",180,0,90,180,0,60);
    TH2D *kinematic_p3H = new TH2D("kinematic_p3H","p+3H",180,0,90,180,0,30);
    TH2D *kinematic_a_t = new TH2D("kinematic_at","a+t",180,0,90,180,-5,50);
    TH2D *kinematic_t_a = new TH2D("kinematic_ta","t+a",180,0,90,180,-5,50);
    TH2D *kinematic_5He_d= new TH2D("kinematic_d_5He","d+5He",180,0,90,180,-5,50);
    
    

    Int_t entries = tree->GetEntriesFast();
    for(Int_t i = 0; i < entries; i++){
        emitangle_tel1 = 0.0, emitangle_tel2 = 0.0, emitangle_tel3 = 0.0, emitangle_tel4 = 0.0, emitangle_tel5=0.0, emitangle_tel6 = 0.0;
        tel1dEXa = 0.0, tel1dEYa = 0.0, tel1Eb = 0.0, tel1Ec = 0.0, tel1Ed = 0.0, tel1Etotal = 0.0;
        tel1X    = 0.0, tel1Y    = 0.0, tel1Z  = 0.0;
        tel2dEXa = 0.0, tel2dEYa = 0.0, tel2Eb = 0.0, tel2Ec = 0.0, tel2Ed = 0.0, tel2Etotal = 0.0;
        tel2X    = 0.0, tel2Y    = 0.0, tel2Z  = 0.0;
        tel3dEXa = 0.0, tel3dEYa = 0.0, tel3Eb = 0.0, tel3Ec = 0.0, tel3Ed = 0.0, tel3Etotal = 0.0;
        tel3X    = 0.0, tel3Y    = 0.0, tel3Z  = 0.0;
        tel4dEXa = 0.0, tel4dEYa = 0.0, tel4Eb = 0.0, tel4Ec = 0.0, tel4Ed = 0.0, tel4Etotal = 0.0;
        tel4X    = 0.0, tel4Y    = 0.0, tel4Z  = 0.0;
        tel5dEXa = 0.0, tel5dEYa = 0.0, tel5Eb = 0.0, tel5Etotal = 0.0;
        tel5X    = 0.0, tel5Y    = 0.0, tel5Z  = 0.0;
        tel6dEXa = 0.0, tel6dEYa = 0.0, tel6Eb = 0.0, tel6Etotal = 0.0;
        tel6X    = 0.0, tel6Y    = 0.0, tel6Z  = 0.0;
        tree->GetEntry(i);
        //Int_t wire_a = mwdca->GetEntriesFast();
        //for(int j = 0; j < wire_a; j++){
        //    const art::TMWDCTrackingResult *read = static_cast<const art::TMWDCTrackingResult *>(mwdca->At(j));
        //    mwdca_x = read->GetTrack()->GetX();
        //    mwdca_y = read->GetTrack()->GetY();
        //    mwdca_z = -462.0; 
        //}

        Int_t wire_b = mwdcb->GetEntriesFast();
        for(int j = 0; j < wire_b; j++){
            const art::TMWDCTrackingResult *read = static_cast<const art::TMWDCTrackingResult *>(mwdcb->At(j));
            mwdcb_x = read->GetTrack()->GetX();
            mwdcb_y = read->GetTrack()->GetY();
            mwdcb_z = -171.5;  
        }

        Int_t target_t = f3mwdc->GetEntriesFast();
        for(int j = 0; j < target_t; j++){
            const art::TMWDCTrackingResult *read = static_cast<const art::TMWDCTrackingResult *>(f3mwdc->At(j));
            tar_X = read->GetTrack()->GetX();
            tar_Y = read->GetTrack()->GetY();
            tar_Z = 0.0;  
        }
        
        Double_t dir_beam[3] = {0.0};
        Double_t dis_mwdcb_tar = sqrt((tar_X-mwdcb_x)*(tar_X-mwdcb_x)+(tar_Y-mwdcb_y)*(tar_Y-mwdcb_y)+(tar_Z-mwdcb_z)*(tar_Z-mwdcb_z));
        dir_beam[0] = (tar_X-mwdcb_x)/dis_mwdcb_tar;
        dir_beam[1] = (tar_Y-mwdcb_y)/dis_mwdcb_tar;
        dir_beam[2] = (tar_Z-mwdcb_z)/dis_mwdcb_tar;



        Int_t tel1_entries = tel1 -> GetEntriesFast();
        for(Int_t j = 0; j < tel1_entries; j++){
            const art::TTelescopeData *read = static_cast<const art::TTelescopeData *>(tel1->At(j));
            //for(Int_t k = 0; k < read->GetEvecSize(); k++)
            //    E_raw[k] = read->GetEvec(k);
            tel1dEXa = read->GetdEX();
            tel1dEYa = read->GetdEY();
            tel1XID = read->GetXID();
            tel1YID = read->GetYID();
            tel1Etotal = read->GetEtotal();
            tel1X = read ->GetX();
            tel1Y = read ->GetY();
            tel1Z = read ->GetZ();
        }

        Int_t tel2_entries = tel2 -> GetEntriesFast();
        for(Int_t j = 0; j < tel2_entries; j++){
            const art::TTelescopeData *read = static_cast<const art::TTelescopeData *>(tel2->At(j));
            //for(Int_t k = 0; k < read->GetEvecSize(); k++)
            //    E_raw[k] = read->GetEvec(k);
            tel2dEXa = read->GetdEX();
            tel2dEYa = read->GetdEY();
            tel2XID = read->GetXID();
            tel2YID = read->GetYID();
            tel2Etotal = read->GetEtotal();
            tel2X = read ->GetX();
            tel2Y = read ->GetY();
            tel2Z = read ->GetZ();
        }

        Int_t tel3_entries = tel3 -> GetEntriesFast();
        for(Int_t j = 0; j < tel3_entries; j++){
            const art::TTelescopeData *read = static_cast<const art::TTelescopeData *>(tel3->At(j));
            //for(Int_t k = 0; k < read->GetEvecSize(); k++)
            //    E_raw[k] = read->GetEvec(k);
            tel3dEXa = read->GetdEX();
            tel3dEYa = read->GetdEY();
            tel3XID = read->GetXID();
            tel3YID = read->GetYID();
            tel3Etotal = read->GetEtotal();
            tel3X = read ->GetX();
            tel3Y = read ->GetY();
            tel3Z = read ->GetZ();
        }

        Int_t tel4_entries = tel4 -> GetEntriesFast();
        for(Int_t j = 0; j < tel4_entries; j++){
            const art::TTelescopeData *read = static_cast<const art::TTelescopeData *>(tel4->At(j));
            //for(Int_t k = 0; k < read->GetEvecSize(); k++)
            //    E_raw[k] = read->GetEvec(k);
            tel4dEXa = read->GetdEX();
            tel4dEYa = read->GetdEY();
            tel4XID = read->GetXID();
            tel4YID = read->GetYID();
            tel4Etotal = read->GetEtotal();
            tel4X = read ->GetX();
            tel4Y = read ->GetY();
            tel4Z = read ->GetZ();
        }

        Int_t tel5_entries = tel5 -> GetEntriesFast();
        for(Int_t j = 0; j < tel5_entries; j++){
            const art::TTelescopeData *read = static_cast<const art::TTelescopeData *>(tel5->At(j));
            //for(Int_t k = 0; k < read->GetEvecSize(); k++)
            //    E_raw[k] = read->GetEvec(k);
            tel5dEXa = read->GetdEX();
            tel5dEYa = read->GetdEY();
            tel5XID = read->GetXID();
            tel5YID = read->GetYID();
            tel5Etotal = read->GetEtotal();
            tel5X = read ->GetX();
            tel5Y = read ->GetY();
            tel5Z = read ->GetZ();
        }

        Int_t tel6_entries = tel6 -> GetEntriesFast();
        for(Int_t j = 0; j < tel6_entries; j++){
            const art::TTelescopeData *read = static_cast<const art::TTelescopeData *>(tel6->At(j));
            //for(Int_t k = 0; k < read->GetEvecSize(); k++)
            //    E_raw[k] = read->GetEvec(k);
            tel6dEXa = read->GetdEX();
            tel6dEYa = read->GetdEY();
            tel6XID = read->GetXID();
            tel6YID = read->GetYID();
            tel6Etotal = read->GetEtotal();
            tel6X = read ->GetX();
            tel6Y = read ->GetY();
            tel6Z = read ->GetZ();
        }

        //emitted angle of the particles
        //tel1
        Double_t emitdir_tel1[3] = {0.0}, Tar_tels[6] = {0.0};
        Tar_tels[0] = sqrt((tel1X-tar_X)*(tel1X-tar_X) + (tel1Y-tar_Y)*(tel1Y-tar_Y) + (tel1Z-tar_Z)*(tel1Z-tar_Z));
        emitdir_tel1[0]=(tel1X-tar_X)/Tar_tels[0];
        emitdir_tel1[1]=(tel1Y-tar_Y)/Tar_tels[0];
        emitdir_tel1[2]=(tel1Z-tar_Z)/Tar_tels[0];
        for(Int_t i = 0; i < 3; i++){
          emitangle_tel1 += dir_beam[i]*emitdir_tel1[i];
         }
         emitangle_tel1 = acos(emitangle_tel1) * rad2deg;
         //std::cout<<dir_beam[0]<<" "<<dir_beam[1]<<"  "<<dir_beam[2]<<std::endl;
         //std::cout<<"distance = "<<Tar_tels[0]<<", emitangle_tel1 = "<<emitangle_tel1<<", emitangle_tel1 = "<<emitangle_tel1<<std::endl;

        //std::cout<<tel1X<<"  "<<tel1Y<<"  "<<tel1Z<<" "<<emitangle_tel1<<std::endl;
         //tel2
        Double_t emitdir_tel2[3] = {0.0};
        Tar_tels[1] = sqrt((tel2X-tar_X)*(tel2X-tar_X) + (tel2Y-tar_Y)*(tel2Y-tar_Y) + (tel2Z-tar_Z)*(tel2Z-tar_Z));
        emitdir_tel2[0]=(tel2X-tar_X)/Tar_tels[1];
        emitdir_tel2[1]=(tel2Y-tar_Y)/Tar_tels[1];
        emitdir_tel2[2]=(tel2Z-tar_Z)/Tar_tels[1];
        for(Int_t i = 0; i < 3; i++){
         emitangle_tel2 += dir_beam[i]*emitdir_tel2[i];
         }
         emitangle_tel2 = acos(emitangle_tel2) * rad2deg;


        //tel3
        Double_t emitdir_tel3[3] = {0.0};
        Tar_tels[2] = sqrt((tel3X-tar_X)*(tel3X-tar_X) + (tel3Y-tar_Y)*(tel3Y-tar_Y) + (tel3Z-tar_Z)*(tel3Z-tar_Z));
        emitdir_tel3[0]=(tel3X-tar_X)/Tar_tels[2];
        emitdir_tel3[1]=(tel3Y-tar_Y)/Tar_tels[2];
        emitdir_tel3[2]=(tel3Z-tar_Z)/Tar_tels[2];
        for(Int_t i = 0; i < 3; i++){
          emitangle_tel3 += dir_beam[i]*emitdir_tel3[i];
         }
         emitangle_tel3 = acos(emitangle_tel3) * rad2deg;

        //tel4
        Double_t emitdir_tel4[3] = {0.0};
        Tar_tels[3] = sqrt((tel4X-tar_X)*(tel4X-tar_X) + (tel4Y-tar_Y)*(tel4Y-tar_Y) + (tel4Z-tar_Z)*(tel4Z-tar_Z));
        emitdir_tel4[0]=(tel4X-tar_X)/Tar_tels[3];
        emitdir_tel4[1]=(tel4Y-tar_Y)/Tar_tels[3];
        emitdir_tel4[2]=(tel4Z-tar_Z)/Tar_tels[3];
        for(Int_t i = 0; i < 3; i++){
          emitangle_tel4 += dir_beam[i]*emitdir_tel4[i];
         }
         emitangle_tel4 = acos(emitangle_tel4) * rad2deg;

        ////tel5
        Double_t emitdir_tel5[3] = {0.0};
        Tar_tels[4] = sqrt((tel5X-tar_X)*(tel5X-tar_X) + (tel5Y-tar_Y)*(tel5Y-tar_Y) + (tel5Z-tar_Z)*(tel5Z-tar_Z));
        emitdir_tel5[0]=(tel5X-tar_X)/Tar_tels[4];
        emitdir_tel5[1]=(tel5Y-tar_Y)/Tar_tels[4];
        emitdir_tel5[2]=(tel5Z-tar_Z)/Tar_tels[4];
        for(Int_t i = 0; i < 3; i++){
          emitangle_tel5 += dir_beam[i]*emitdir_tel5[i];
         }
         emitangle_tel5 = acos(emitangle_tel5) * rad2deg;

        //tel6
        Double_t emitdir_tel6[3] = {0.0};
        Tar_tels[5] = sqrt((tel6X-tar_X)*(tel6X-tar_X) + (tel6Y-tar_Y)*(tel6Y-tar_Y) + (tel6Z-tar_Z)*(tel6Z-tar_Z));
        emitdir_tel6[0]=(tel6X-tar_X)/Tar_tels[5];
        emitdir_tel6[1]=(tel6Y-tar_Y)/Tar_tels[5];
        emitdir_tel6[2]=(tel6Z-tar_Z)/Tar_tels[5];
        for(Int_t i = 0; i < 3; i++){
          emitangle_tel6 += dir_beam[i]*emitdir_tel6[i];
         }
         emitangle_tel6 = acos(emitangle_tel6) * rad2deg;

        tree_out->Fill();
    }

    double mass_a = Mass.He_6_0, mass_A = Mass.p, mass_b0 = Mass.He_6_0, mass_b1 = Mass.He_6_1, mass_B = Mass.p; 
    double energy = 44.1166;//calculated by enewz
    double pa2 =  2.0 * energy * mass_a;
    double velocity_a = sqrt(pa2 / (mass_a * mass_a));
    double velocity_cm =  velocity_a * mass_a  / (mass_A + mass_a);
    Q_value = 0.0;
    double pb_cm2_0 = (pa2 * mass_A / (2.0 * mass_a * (mass_A + mass_a)) + Q_value) * 2.0 * mass_b0 * mass_B / (mass_b0 + mass_B);
    double pb_cm_0 = sqrt(pb_cm2_0);
    double velocity_b0 = pb_cm_0 / mass_b0;

    for(int i = 0; i < 181; i++){
        double alpha_0 = atan(sin(i * deg2rad) / (mass_b0 * velocity_cm / pb_cm_0 + cos(i * deg2rad)));
        alpha_0 = alpha_0 / deg2rad;

        double beta_0 = 0.0;
        if(Q_value == 0.0 && i == 0)
            beta_0 = M_PI / 2.0;
        else beta_0 = atan(sin(i * deg2rad) / (mass_B * velocity_cm / pb_cm_0 - cos(i * deg2rad)));
        beta_0 = beta_0 / deg2rad;
        theta_6He_p0 = beta_0;
        theta_p0_6He  = alpha_0;
        
        double E_6He_0 = 0.5 * mass_b0 * (velocity_cm * velocity_cm + velocity_b0 * velocity_b0 + 2.0 * velocity_b0 * velocity_cm * cos(i * deg2rad));
        
        energy_6He_p0 = energy- E_6He_0;
        energy_p0_6He = E_6He_0;

        kinematic_p0_6He -> Fill(theta_p0_6He, energy_p0_6He);
        kinematic_6He_p0 -> Fill(theta_6He_p0, energy_6He_p0);

        double tem_energy1 = art::enewz::enewz(1, mass_B, energy_6He_p0, det_mat, 0., 0., 0.301);
        dEXa_c_p0 = energy_6He_p0 - tem_energy1;
	      double tem_energy2 = art::enewz::enewz(1, mass_B, tem_energy1, det_mat, 0., 0., 1.494);
        Eb_c_p0 = tem_energy1 - tem_energy2;
        double tem_energy3 = art::enewz::enewz(1, mass_B, tem_energy2, det_mat, 0., 0., 1.486);
        Ec_c_p0 = tem_energy2 - tem_energy3;

        double tem_energy4 = art::enewz::enewz(2, mass_b0, energy_p0_6He, det_mat, 0., 0., 0.301);
        dEXa_c_6He = energy_p0_6He - tem_energy4;
	      double tem_energy5 = art::enewz::enewz(2, mass_b0, tem_energy4, det_mat, 0., 0., 1.494);
        Eb_c_6He = tem_energy4 - tem_energy5;

        tree_out1->Fill();
    }

    mass_a = Mass.H3, mass_A = Mass.p, mass_b0 = Mass.H3, mass_B = Mass.p; 
    energy = energy * Mass.H3 / Mass.He_6_0;
    pa2 =  2.0 * energy * mass_a;
    velocity_a = sqrt(pa2 / (mass_a * mass_a));
    velocity_cm =  velocity_a * mass_a  / (mass_A + mass_a);
    Q_value = 0.0;
    pb_cm2_0 = (pa2 * mass_A / (2.0 * mass_a * (mass_A + mass_a)) + Q_value) * 2 * mass_b0 * mass_B / (mass_b0 + mass_B);
    pb_cm_0 = sqrt(pb_cm2_0);
    velocity_b0 = pb_cm_0 / mass_b0;
 
    for(int i = 0; i < 181; i++){
        double alpha_0 = atan(sin(i * deg2rad) / (mass_b0 * velocity_cm / pb_cm_0 + cos(i * deg2rad)));
        alpha_0 = alpha_0 / deg2rad;
 
        double beta_0 = 0.0;
        if(Q_value == 0.0 && i == 0)
            beta_0 = M_PI / 2.0;
        else beta_0 = atan(sin(i * deg2rad) / (mass_B * velocity_cm / pb_cm_0 - cos(i * deg2rad)));
        beta_0 = beta_0 / deg2rad;
        theta_3H_p = beta_0;
        theta_p_3H = alpha_0;
        
        double E_3H_0 = 0.5 * mass_b0 * (velocity_cm * velocity_cm + velocity_b0 * velocity_b0 + 2.0 * velocity_b0 * velocity_cm * cos(i * deg2rad));
        energy_p_3H =  E_3H_0;
        energy_3H_p = energy - E_3H_0;
        
        kinematic_p3H -> Fill(theta_3H_p, energy_3H_p);
 
        double tem_energy1 = art::enewz::enewz(1, mass_B, energy_3H_p, det_mat, 0., 0., 0.301);
        dEXa_c_p2 = energy_p_3H - tem_energy1;
        double tem_energy2 = art::enewz::enewz(1, mass_B, tem_energy1, det_mat, 0., 0., 1.494);
        Eb_c_p2 = tem_energy1 - tem_energy2;
        double tem_energy3 = art::enewz::enewz(1, mass_B, tem_energy2, det_mat, 0., 0., 1.507);
        Ec_c_p2 = tem_energy2 - tem_energy3;
 
        tree_out3->Fill();
    }




    mass_a = Mass.He_6_0, mass_A = Mass.p, mass_b0 = Mass.He, mass_B = Mass.H3; 
    energy = 44.1166;//calculated by enewz
    pa2 =  2.0 * energy * mass_a;
    velocity_a = sqrt(pa2 / (mass_a * mass_a));
    velocity_cm =  velocity_a * mass_a  / (mass_A + mass_a);
    Q_value = 7.50634;
    pb_cm2_0 = (pa2 * mass_A / (2.0 * mass_a * (mass_A + mass_a)) + Q_value) * 2.0 * mass_b0 * mass_B / (mass_b0 + mass_B);
    pb_cm_0 = sqrt(pb_cm2_0);
    velocity_b0 = pb_cm_0 / mass_b0;

    for(int i = 0; i < 181; i++){
        double alpha_0 = atan(sin(i * deg2rad) / (mass_b0 * velocity_cm / pb_cm_0 + cos(i * deg2rad)));
        alpha_0 = alpha_0 / deg2rad;

        double beta_0 = 0.0;
        if(Q_value == 0.0 && i == 0)
            beta_0 = M_PI / 2.0;
        else beta_0 = atan(sin(i * deg2rad) / (mass_B * velocity_cm / pb_cm_0 - cos(i * deg2rad)));
        beta_0 = beta_0 / deg2rad;
        theta_t = beta_0;
        theta_a = alpha_0;
        
        double E_He = 0.5 * mass_b0 * (velocity_cm * velocity_cm + velocity_b0 * velocity_b0 + 2.0 * velocity_b0 * velocity_cm * cos(i * deg2rad));
        energy_t = energy + Q_value - E_He;
        energy_a = E_He;
        //std::cout<<energy<<"  "<<E_He<<"  "<<energy_t<<std::endl;
        
        kinematic_a_t -> Fill(theta_t, energy_t);
        kinematic_t_a -> Fill(theta_a, energy_a);
        
        double tem_energy1 = art::enewz::enewz(1, mass_B, energy_t, det_mat, 0., 0., 0.301);
        dEXa_c_t = energy_t - tem_energy1;
	      double tem_energy2 = art::enewz::enewz(1, mass_B, tem_energy1, det_mat, 0., 0., 1.494);
        Eb_c_t = tem_energy1 - tem_energy2;
        double tem_energy3 = art::enewz::enewz(1, mass_B, tem_energy2, det_mat, 0., 0., 1.494);
        Ec_c_t = tem_energy2 - tem_energy3;
        double tem_energy4 = art::enewz::enewz(1, mass_B, tem_energy3, det_mat, 0., 0., 1.494);
        Ed_c_t = tem_energy3 - tem_energy4;

        double tem_energy11{0.0}, tem_energy22{0.0}, tem_energy33{0.0},tem_energy44{0.0};
        tem_energy11 = art::enewz::enewz(2, mass_b0, energy_a, det_mat, 0., 0., 0.301);
        dEXa_c_a = energy_a - tem_energy11;
	      tem_energy22 = art::enewz::enewz(2, mass_b0, tem_energy11, det_mat, 0., 0., 1.494);
        Eb_c_a = tem_energy11 - tem_energy22;
        tem_energy33 = art::enewz::enewz(2, mass_b0, tem_energy22, det_mat, 0., 0., 1.494);
        Ec_c_a = tem_energy22 - tem_energy33;
        tem_energy44 = art::enewz::enewz(2, mass_b0, tem_energy33, det_mat, 0., 0., 1.494);
        Ed_c_a = tem_energy33 - tem_energy44;

        tree_out4->Fill();
    }
    
    mass_a = Mass.He_6_0, mass_A = Mass.p, mass_b0 = Mass.He_5, mass_B = Mass.deu; 
    energy = 44.1166;//calculated by enewz
    pa2 =  2.0 * energy * mass_a;
    velocity_a = sqrt(pa2 / (mass_a * mass_a));
    velocity_cm =  velocity_a * mass_a  / (mass_A + mass_a);
    Q_value = 0.514;
    pb_cm2_0 = (pa2 * mass_A / (2.0 * mass_a * (mass_A + mass_a)) + Q_value) * 2.0 * mass_b0 * mass_B / (mass_b0 + mass_B);
    pb_cm_0 = sqrt(pb_cm2_0);
    velocity_b0 = pb_cm_0 / mass_b0;

    for(int i = 0; i < 181; i++){
        double alpha_0 = atan(sin(i * deg2rad) / (mass_b0 * velocity_cm / pb_cm_0 + cos(i * deg2rad)));
        alpha_0 = alpha_0 / deg2rad;

        double beta_0 = 0.0;
        if(Q_value == 0.0 && i == 0)
            beta_0 = M_PI / 2.0;
        else beta_0 = atan(sin(i * deg2rad) / (mass_B * velocity_cm / pb_cm_0 - cos(i * deg2rad)));
        beta_0 = beta_0 / deg2rad;
        theta_d = beta_0;
        
        double E_5He = 0.5 * mass_b0 * (velocity_cm * velocity_cm + velocity_b0 * velocity_b0 + 2.0 * velocity_b0 * velocity_cm * cos(i * deg2rad));
        energy_d = energy + Q_value - E_5He;
        //std::cout<<energy<<"  "<<E_He<<"  "<<energy_t<<std::endl;
        
        kinematic_5He_d -> Fill(theta_d, energy_d);
        
        double tem_energy1 = art::enewz::enewz(1, mass_B, energy_d, det_mat, 0., 0., 0.301);
        dEXa_c_d = energy_d - tem_energy1;
	    double tem_energy2 = art::enewz::enewz(1, mass_B, tem_energy1, det_mat, 0., 0., 1.494);
        Eb_c_d = tem_energy1 - tem_energy2;
        double tem_energy3 = art::enewz::enewz(1, mass_B, tem_energy2, det_mat, 0., 0., 1.494);
        Ec_c_d = tem_energy2 - tem_energy3;
        double tem_energy4 = art::enewz::enewz(1, mass_B, tem_energy3, det_mat, 0., 0., 1.494);
        Ed_c_d = tem_energy3 - tem_energy4;

        //std::cout<<dEX_a<<"  "<<Eb
        tree_out5->Fill();
    }

    mass_a = Mass.He_6_0, mass_A = Mass.Carbon, mass_b0 = Mass.He_6_0, mass_B = Mass.Carbon; 
    energy = 44.1166;//calculated by enewz
    pa2 =  2.0 * energy * mass_a;
    velocity_a = sqrt(pa2 / (mass_a * mass_a));
    velocity_cm =  velocity_a * mass_a  / (mass_A + mass_a);
    Q_value = 0.514;
    pb_cm2_0 = (pa2 * mass_A / (2.0 * mass_a * (mass_A + mass_a)) + Q_value) * 2.0 * mass_b0 * mass_B / (mass_b0 + mass_B);
    pb_cm_0 = sqrt(pb_cm2_0);
    velocity_b0 = pb_cm_0 / mass_b0;

    for(int i = 0; i < 181; i++){
        double alpha_0 = atan(sin(i * deg2rad) / (mass_b0 * velocity_cm / pb_cm_0 + cos(i * deg2rad)));
        alpha_0 = alpha_0 / deg2rad;

        double beta_0 = 0.0;
        if(Q_value == 0.0 && i == 0)
            beta_0 = M_PI / 2.0;
        else beta_0 = atan(sin(i * deg2rad) / (mass_B * velocity_cm / pb_cm_0 - cos(i * deg2rad)));
        beta_0 = beta_0 / deg2rad;
        theta_Carbon_6He = alpha_0;
        
        double E_6He = 0.5 * mass_b0 * (velocity_cm * velocity_cm + velocity_b0 * velocity_b0 + 2.0 * velocity_b0 * velocity_cm * cos(i * deg2rad));
        energy_Carbon_6He = E_6He;
        
        tree_out6->Fill();
    }

    mwdcaXY->Write();
    mwdcbXY->Write();
    hpxpy->Write();
    hpxpy2->Write();
    hpxpy3->Write();
    Beam_energy->Write();
    kinematic_p0_6He -> Write();
    kinematic_6He_p0 -> Write();
    kinematic_a_t -> Write();
    kinematic_t_a -> Write();
    kinematic_5He_d -> Write();
    tree_out->Write();
    tree_out1->Write();
    tree_out4->Write();
    tree_out3->Write();
    tree_out5->Write();
    tree_out6->Write();
    gROOT->ProcessLine(".q");
    std::cout<<"Convention completed!"<<std::endl;
}
