void run_MUXParamMaker(TString RUNNAME="run", TString RUNNUM="0219"){
  TString ARTEMIS_WORK_DIR = gSystem->pwd();

  gROOT->ProcessLine(Form("add steering/calibration.yaml NAME=%s NUM=%s", RUNNAME.Data(), RUNNUM.Data()));
  art::TLoopManager::Instance()->GetLoop()->Resume();

  gROOT->ProcessLine("fcd 0");
  gROOT->ProcessLine("zone");
  //gROOT->ProcessLine("lgy");

  // run0042
  //tel4 dEX
  //gROOT->ProcessLine("tree->Draw(\"tel4dEX_raw.fP1>>h1(400,3950.,4350.)\")");
  //gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h1, \"tel4/pos_dEX/" + RUNNAME + RUNNUM + "\")");

  ////tel4 dEY
  //gROOT->ProcessLine("tree->Draw(\"tel4dEY_raw.fP1>>h2(350,3500.,4000.)\")");
  //gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h2, \"tel4/pos_dEY/" + RUNNAME + RUNNUM + "\")");

  ////tel5 dEX
  //gROOT->ProcessLine("tree->Draw(\"tel5dEX_raw.fP1>>h3(600,2900.,3500.)\")");
  //gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h3, \"tel5/pos_dEX/" + RUNNAME + RUNNUM + "\")");

  ////tel5 dEY
  //gROOT->ProcessLine("tree->Draw(\"tel5dEY_raw.fP1>>h4(600,2500.,3150.)\")");
  //gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h4, \"tel5/pos_dEY/" + RUNNAME + RUNNUM + "\")");

  ////tel6 dEX
  //gROOT->ProcessLine("tree->Draw(\"tel6dEX_raw.fP1>>h5(400,1850.,2350.)\")");
  //gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h5, \"tel6/pos_dEX/" + RUNNAME + RUNNUM + "\")");

  //tel6 dEY
  gROOT->ProcessLine("tree->Draw(\"tel6dEY_raw.fP1>>h6(350,1500.,2000.)\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h6, \"tel6/pos_dEY/" + RUNNAME + RUNNUM + "\")");



  gROOT->ProcessLine(".q");
}
