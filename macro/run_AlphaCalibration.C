// Arguments of AlphaCalibration
//    h2         : x: SSD fID, y: raw ADC channel
//    foutpath   : output parameter file path after prm/ssd/.
//               : NOTE: "telx/ch2MeV_dEX.dat", "telx/ch2MeV_dEY.dat", "telx/ch2MeV_E.dat" NEEDED
//    alphaID    : 2: alpha-2(4.780, 5.480, 5.795), 3: alpha-3(3.148, 5.462, 5.771)
//    width      : fitting width
//DSSD:run0187 and run 0188
//SSD: run0192 and run0193
//void run_AlphaCalibration(TString RUNNAME="run", TString RUNNUM1="0187", TString RUNNUM2="0188") {
//    const TString ARTEMIS_WORKDIR = gSystem->pwd();
//E detectors
//void run_AlphaCalibration(TString RUNNAME="run", TString RUNNUM1="0192", TString RUNNUM2="0193") {
    const TString ARTEMIS_WORKDIR = gSystem->pwd();
void run_AlphaCalibration(TString RUNNAME="run", TString RUNNUM1="0187") {

//    gROOT->ProcessLine(Form("add steering/calibration.yaml NAME=%s NUM1=%s NUM2=%s", RUNNAME.Data(), RUNNUM1.Data(), RUNNUM2.Data()));
    gROOT->ProcessLine(Form("add steering/calibration.yaml NAME=%s NUM1=%s", RUNNAME.Data(), RUNNUM1.Data()));
    art::TLoopManager::Instance()->GetLoop()->Resume();

    gROOT->ProcessLine("fcd 0");
    gROOT->ProcessLine("zone");

    gStyle->SetOptStat(0);

    //// tel1 dEX, alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel1dEX_raw.fCharge:tel1dEX_raw.fID>>h11(16,-0.5,15.5, 200,500.,1600.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h11, \"tel1/ch2MeV_dEX.dat\",\"tel1/dEX\", 3,20.)");
    gROOT->ProcessLine("tree->Draw(\"tel1dEX_raw.fCharge:tel1dEX_raw.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    //gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel1/dEX/all_cal.png\")");

    ////// tel1 dEY alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel1dEY_raw.fCharge:tel1dEY_raw.fID>>h12(16,-0.5,15.5, 200,500.,1600.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h12, \"tel1/ch2MeV_dEY.dat\",\"tel1/dEY\", 3,20.)");
    gROOT->ProcessLine("tree->Draw(\"tel1dEY_raw.fCharge:tel1dEY_raw.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    //gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel1/dEY/all_cal.png\")");


    // //tel2 dEX alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel2dEX_raw.fCharge:tel2dEX_raw.fID>>h21(16,-0.5,15.5, 150,300.,1000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h21, \"tel2/ch2MeV_dEX.dat\",\"tel2/dEX\", 3,20.)");
    gROOT->ProcessLine("tree->Draw(\"tel2dEX_raw.fCharge:tel2dEX_raw.fID>>h26(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel2/dEX/all_cal.png\")");

    //// tel2 dEY alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel2dEY_raw.fCharge:tel2dEY_raw.fID>>h22(16,-0.5,15.5, 250,300.,800.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h22, \"tel2/ch2MeV_dEY.dat\",\"tel2/dEY\", 3,20.)");
    gROOT->ProcessLine("tree->Draw(\"tel2dEY_raw.fCharge:tel2dEY_raw.fID>>h26(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    //gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel2/dEY/all_cal.png\")");

    //// tel3 dEX alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel3dEX_raw.fCharge:tel3dEX_raw.fID>>h31(16,-0.5,15.5, 200,400.,2000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h31, \"tel3/ch2MeV_dEX.dat\",\"tel3/dEX\", 3,25.)");
    gROOT->ProcessLine("tree->Draw(\"tel3dEX_raw.fCharge:tel3dEX_raw.fID>>h36(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    // gROOT->ProcessLine("lgz");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel3/dEX/all_cal.png\")");

    //// tel3 dEY alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel3dEY_raw.fCharge:tel3dEY_raw.fID>>h32(16,-0.5,15.5, 140,400.,2000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h32, \"tel3/ch2MeV_dEY.dat\",\"tel3/dEY\", 3,35.)");
     gROOT->ProcessLine("tree->Draw(\"tel3dEY_raw.fCharge:tel3dEY_raw.fID>>h36(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel3/dEY/all_cal.png\")");

    // tel4 dEX alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel4dEX_cal.fCharge:tel4dEX_cal.fID>>h41(16,-0.5,15.5, 200,1000.,4000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h41, \"tel4/ch2MeV_dEX.dat\",\"tel4/dEX\", 3, 30.0)");
    gROOT->ProcessLine("tree->Draw(\"tel4dEX_cal.fCharge:tel4dEX_cal.fID>>h46(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    // gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel4/dEX/all_cal.png\")");

    //// tel4 dEY alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel4dEY_cal.fCharge:tel4dEY_cal.fID>>h42(16,-0.5,15.5, 200,1000.,4000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h42, \"tel4/ch2MeV_dEY.dat\",\"tel4/dEY\", 3, 30.0)");
    gROOT->ProcessLine("tree->Draw(\"tel4dEY_cal.fCharge:tel4dEY_cal.fID>>46(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    // gROOT->ProcessLine("lgz");
    //gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel4/dEY/all_cal.png\")");

    //// tel5 dEX alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel5dEX_cal.fCharge:tel5dEX_cal.fID>>h51(16,-0.5,15.5, 200,1000.,4000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h51, \"tel5/ch2MeV_dEX.dat\",\"tel5/dEX\", 3, 30.)");
    gROOT->ProcessLine("tree->Draw(\"tel5dEX_cal.fCharge:tel5dEX_cal.fID>>h56(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    // gROOT->ProcessLine("lgz");
    //gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel5/dEX/all_cal.png\")");

    //// tel5 dEY alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel5dEY_cal.fCharge:tel5dEY_cal.fID>>h52(16,-0.5,15.5, 200,1000.,4000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h52, \"tel5/ch2MeV_dEY.dat\",\"tel5/dEY\", 3, 30.)");
    gROOT->ProcessLine("tree->Draw(\"tel5dEY_cal.fCharge:tel5dEY_cal.fID>>h56(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    //gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel5/dEY/all_cal.png\")");


    ////// tel6 dEX alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel6dEX_cal.fCharge:tel6dEX_cal.fID>>h61(16,-0.5,15.5, 200,1000.,4000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h61, \"tel6/ch2MeV_dEX.dat\",\"tel6/dEX\", 3, 50.)");
    gROOT->ProcessLine("tree->Draw(\"tel6dEX_cal.fCharge:tel6dEX_cal.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    //gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel6/dEX/all_cal.png\")");

    //// tel6 dEY alpha-3
    gROOT->ProcessLine("tree->Draw(\"tel6dEY_cal.fCharge:tel6dEY_cal.fID>>h62(16,-0.5,15.5, 200,1000.,4000.)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(h62, \"tel6/ch2MeV_dEY.dat\",\"tel6/dEY\", 3, 50.)");
    gROOT->ProcessLine("tree->Draw(\"tel6dEY_cal.fCharge:tel6dEY_cal.fID>>h16(16,-0.5,15.5, 200,2.,6.)\",\"\",\"colz\")");
    //gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel6/dEY/all_cal.png\")");

   //E detectors calibration: run192 and run193
   //tel1
    //gROOT->ProcessLine("tree->Draw(\"tel1E_raw.fCharge:tel1E_raw.fID>>tel1E(3,-0.5,2.5, 150,500.,1300.)\",\"\",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(tel1E, \"tel1/ch2MeV_E.dat\",\"tel1/E\", 3, 30.)");
    //gROOT->ProcessLine("tree->Draw(\"tel1E_raw.fCharge:tel1E_raw.fID>>tel1E(3,-0.5,2.5, 200,2.,6.)\",\"\",\"colz\")");
    ////gROOT->ProcessLine("tree->Draw(\"tel1E_cal.fCharge:tel1E_cal.fID>>tel1E(3,-0.5,2.5, 150,2.,7.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    ////gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel1/E/all_cal.png\")");

   ////tel2
    //gROOT->ProcessLine("tree->Draw(\"tel2E_raw.fCharge:tel2E_raw.fID>>tel2E(3,-0.5,2.5, 150,400.,1400.)\",\"\",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(tel2E, \"tel2/ch2MeV_E.dat\",\"tel2/E\", 3, 25.)");
    //gROOT->ProcessLine("tree->Draw(\"tel2E_raw.fCharge:tel2E_raw.fID>>tel2E(3,-0.5,2.5, 150,2.,6.)\",\"\",\"colz\")");
    ////gROOT->ProcessLine("tree->Draw(\"tel2E_cal.fCharge:tel2E_cal.fID>>tel2E(3,-0.5,2.5, 150,2.,7.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    ////gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel2/E/all_cal.png\")");

   ////tel3
    //gROOT->ProcessLine("tree->Draw(\"tel3E_raw.fCharge:tel3E_raw.fID>>tel3E(2,-0.5,1.5, 200,500.,1400.)\",\"\",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(tel3E, \"tel3/ch2MeV_E.dat\",\"tel3/E\", 3, 20.)");
    //gROOT->ProcessLine("tree->Draw(\"tel3E_raw.fCharge:tel3E_raw.fID>>tel3E(3,-0.5,2.5, 150,2.,6.)\",\"\",\"colz\")");
    ////gROOT->ProcessLine("tree->Draw(\"tel3E_cal.fCharge:tel3E_cal.fID>>tel3E(2,-0.5,1.5, 150,2.,7.)\",\"\",\"colz\")");
    ////// gROOT->ProcessLine("lgz");
    ////gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel3/E/all_cal.png\")");

   ////tel4
    //gROOT->ProcessLine("tree->Draw(\"tel4E_raw.fCharge:tel4E_raw.fID>>tel4E(1,-0.5,0.5, 150,600.,1500.)\",\"\",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(tel4E, \"tel4/ch2MeV_E.dat\",\"tel4/E\", 3, 25.)");
    //gROOT->ProcessLine("tree->Draw(\"tel4E_raw.fCharge:tel4E_raw.fID>>tel4E(3,-0.5,2.5, 150,2.,6.)\",\"\",\"colz\")");
    ////gROOT->ProcessLine("tree->Draw(\"tel4E_cal.fCharge:tel4E_cal.fID>>tel4E(1,-0.5,0.5, 150,2.,7.)\",\"\",\"colz\")");
    ////// gROOT->ProcessLine("lgz");
    ////gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel4/E/all_cal.png\")");

   ////tel5
    //gROOT->ProcessLine("tree->Draw(\"tel5E_raw.fCharge:tel5E_raw.fID>>tel5E(1,-0.5,0.5, 150,800.,2000.)\",\"\",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(tel5E, \"tel5/ch2MeV_E.dat\",\"tel5/E\", 3, 25.)");
    //gROOT->ProcessLine("tree->Draw(\"tel5E_raw.fCharge:tel5E_raw.fID>>tel5E(3,-0.5,2.5, 150,2.,6.)\",\"\",\"colz\")");
    ////gROOT->ProcessLine("tree->Draw(\"tel5E_cal.fCharge:tel5E_cal.fID>>tel5E(1,-0.5,0.5, 150,2.,7.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    ////gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel5/E/all_cal.png\")");

   ////tel6
    //gROOT->ProcessLine("tree->Draw(\"tel6E_raw.fCharge:tel6E_raw.fID>>tel6E(1,-0.5,0.5, 100,1000.,2500.)\",\"\",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/AlphaCalibration.C(tel6E, \"tel6/ch2MeV_E.dat\",\"tel6/E\", 3, 35.)");
    //gROOT->ProcessLine("tree->Draw(\"tel6E_raw.fCharge:tel6E_raw.fID>>tel6E(3,-0.5,2.5, 150,2.,6.)\",\"\",\"colz\")");
    //gROOT->ProcessLine("tree->Draw(\"tel6E_cal.fCharge:tel6E_cal.fID>>tel6E(1,-0.5,0.5, 150,2.,7.)\",\"\",\"colz\")");
    //// gROOT->ProcessLine("lgz");
    //gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/tel6/E/all_cal.png\")");


}
