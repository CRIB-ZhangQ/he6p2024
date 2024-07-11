// Arguments of AlphaCalibration
//    h2         : x: SSD fID, y: raw ADC channel
//    foutpath   : output parameter file path after prm/ssd/.
//               : NOTE: "telx/ch2MeV_dEX.dat", "telx/ch2MeV_dEY.dat", "telx/ch2MeV_E.dat" NEEDED
//    alphaID    : 2: alpha-2(4.780, 5.480, 5.795), 3: alpha-3(3.148, 5.462, 5.771)
//    width      : fitting width

void run_AlphaCalibration() {
    const TString ARTEMIS_WORKDIR = gSystem->pwd();

    gROOT->ProcessLine("fcd 0");
    gROOT->ProcessLine("zone");

    gStyle->SetOptStat(0);

    // tel1 dEX, alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel1dEX_raw.fCharge:tel1dEX_raw.fID>>h11(16,-0.5,15.5, 100,500.,1600.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h11, \"tel1/ch2MeV_dEX.dat\",3,15.)");
    // gROOT->ProcessLine("tree->Draw(\"tel1dEX_cal.fCharge:tel1dEX_cal.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel1/ch2MeV_dEX/all_cal.png\")");

    // tel1 dEY alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel1dEY_raw.fCharge:tel1dEY_raw.fID>>h12(16,-0.5,15.5, 100,500.,1600.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h12, \"tel1/ch2MeV_dEY.dat\",3,15.)");
    // gROOT->ProcessLine("tree->Draw(\"tel1_dEX_cal.fCharge:tel1_dEX_cal.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    // gROOT->ProcessLine("lgz");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel1/ch2MeV_dEX/all_cal.png\")");

    // tel2 dEX alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel2dEX_raw.fCharge:tel2dEX_raw.fID>>h21(16,-0.5,15.5, 100,300.,800.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h21, \"tel2/ch2MeV_dEX.dat\",3,15.)");
    // gROOT->ProcessLine("tree->Draw(\"tel1_dEX_cal.fCharge:tel1_dEX_cal.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    // gROOT->ProcessLine("lgz");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel1/ch2MeV_dEX/all_cal.png\")");

    // tel2 dEY alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel2dEY_raw.fCharge:tel2dEY_raw.fID>>h22(16,-0.5,15.5, 100,300.,800.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h22, \"tel2/ch2MeV_dEY.dat\",3,15.)");
    // gROOT->ProcessLine("tree->Draw(\"tel1_dEX_cal.fCharge:tel1_dEX_cal.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    // gROOT->ProcessLine("lgz");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel1/ch2MeV_dEX/all_cal.png\")");

    // tel3 dEX alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel3dEX_raw.fCharge:tel3dEX_raw.fID>>h31(16,-0.5,15.5, 150,600.,2000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h31, \"tel3/ch2MeV_dEX.dat\",3,15.)");
    // gROOT->ProcessLine("tree->Draw(\"tel1_dEX_cal.fCharge:tel1_dEX_cal.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    // gROOT->ProcessLine("lgz");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel1/ch2MeV_dEX/all_cal.png\")");

    // tel3 dEY alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel3dEY_raw.fCharge:tel3dEY_raw.fID>>h32(16,-0.5,15.5, 150,500.,2000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h32, \"tel3/ch2MeV_dEY.dat\",3,15.)");
    // gROOT->ProcessLine("tree->Draw(\"tel1_dEX_cal.fCharge:tel1_dEX_cal.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    // gROOT->ProcessLine("lgz");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel1/ch2MeV_dEX/all_cal.png\")");

}