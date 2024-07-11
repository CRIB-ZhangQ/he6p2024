#include "/home/crib/art_analysis/he6p/zhang/src/enewz/TEnewzCalculator.h"

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
}Mass;

Char_t det_mat[34] = "SI";
const double rad2deg = 180.0 / acos(-1.0); 

void shift() 
{
    
    TTree *tree_out1 = new TTree("kine:p0+6He","");
    //TTree *tree_out2 = new TTree("kine:p1+6He","");
    //TTree *tree_out3 = new TTree("kine:p+3H","");
    TTree *tree_out4 = new TTree("kine:a+t","");
    TTree *tree_out5 = new TTree("kine:d+5He","");
   
   Double_t energy_6He{0.0};
    Double_t theta_6He_p0{0.0}, energy_6He_p0{0.0};
    Double_t theta_p0_6He{0.0}, energy_p0_6He{0.0};
    Double_t theta_t{0.0}, energy_t{0.0};
    Double_t theta_a{0.0}, energy_a{0.0};
    Double_t theta_d{0.0}, energy_d{0.0};
    Double_t dEXa_c_p0{0.0},  Eb_c_p0{0.0},  Ec_c_p0{0.0},  Ed_c_p0{0.0}, Etotal_c_p0{0.0};
    Double_t dEXa_c_6He{0.0}, Eb_c_6He{0.0}, Ec_c_6He{0.0}, Ed_c_6He{0.0},Etotal_c_6He{0.0};
    Double_t dEXa_c_t{0.0},   Eb_c_t{0.0},   Ec_c_t{0.0},   Ed_c_t{0.0},  Etotal_c_t{0.0};
    Double_t dEXa_c_a{0.0},   Eb_c_a{0.0},   Ec_c_a{0.0},   Ed_c_a{0.0},  Etotal_c_a{0.0};
    Double_t dEXa_c_d{0.0},   Eb_c_d{0.0},   Ec_c_d{0.0},   Ed_c_d{0.0},  Etotal_c_d{0.0};


    TH2D *kinematic_p0_6He = new TH2D("kinematic_p0_6He","p+6He elastic scattering",180,0,15,180,0,60);
    TH2D *kinematic_6He_p0 = new TH2D("kinematic_6He_p0","p+6He elastic scattering",180,0,90,180,0,60);
    TH2D *kinematic_a_t = new TH2D("kinematic_at","a+t",180,0,90,180,-5,50);
    TH2D *kinematic_t_a = new TH2D("kinematic_ta","t+a",180,0,90,180,-5,50);
    TH2D *kinematic_5He_d= new TH2D("kinematic_d_5He","d+5He",180,0,90,180,-5,50);
    
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
        double tem_energy3 = art::enewz::enewz(1, mass_B, tem_energy2, det_mat, 0., 0., 1.494);
        Ec_c_p0 = tem_energy2 - tem_energy3;

        double tem_energy4 = art::enewz::enewz(2, mass_b0, energy_p0_6He, det_mat, 0., 0., 0.301);
        dEXa_c_6He = energy_p0_6He - tem_energy4;
	    double tem_energy5 = art::enewz::enewz(2, mass_b0, tem_energy4, det_mat, 0., 0., 1.494);
        Eb_c_6He = tem_energy4 - tem_energy5;
      tree_out1->Fill();

    }

    tree_out1->Draw("energy_6He_p0:dEXa_c_p0>>(181,0,30,181,0,60)","","");
    tree_out1->Draw("energy_p0_6He:dEXa_c_6He>>(181,0,30,181,0,60","","lsame Kred");

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
    tree_out4->Draw("energy_a:dEXa_c_a>>(181,0,30,181,0,60)","","lsame");
    tree_out4->Draw("energy_t:dEXa_c_t>>(181,0,30,181,0,60)","","lsame");
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

        tree_out5->Fill();
    }
    
    tree_out5->Draw("energy_d:dEXa_c_d>>(181,0,30,181,0,60)","","lsame");
    //tree_out4->Draw("energy_t:energy_a");
    //gROOT->ProcessLine(".q");
    std::cout<<"Convention completed!"<<std::endl;
}
