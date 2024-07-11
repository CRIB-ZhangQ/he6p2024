void run_MUXParamMaker(TString RUNNAME="run", TString RUNNUM="0099"){
  TString ARTEMIS_WORK_DIR = gSystem->pwd();

  gROOT->ProcessLine(Form("add steering/calibration.yaml NAME=%s NUM1=%s", RUNNAME.Data(), RUNNUM.Data()));
  art::TLoopManager::Instance()->GetLoop()->Resume();

  gROOT->ProcessLine("fcd 0");
  gROOT->ProcessLine("zone");
  //gROOT->ProcessLine("lgy");

  // run0042
  //tel4 dEX
  gROOT->ProcessLine("tree->Draw(\"tel4dEX_raw.fP1>>h1(200,3850.,4380.)\",\"tel4dEX_raw.fE1>300\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h1, \"tel4/pos_dEX/" + RUNNAME + RUNNUM + "\",16)");

  //tel4 dEY
  gROOT->ProcessLine("tree->Draw(\"tel4dEY_raw.fP1>>h2(250,3550.,4050.)\",\"tel4dEX_raw.fE1>300\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h2, \"tel4/pos_dEY/" + RUNNAME + RUNNUM + "\",16)");

  //tel5 dEX
  gROOT->ProcessLine("tree->Draw(\"tel5dEX_raw.fP1>>h3(300,2750.,3250.)\",\"tel5dEX_raw.fE1>300\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h3, \"tel5/pos_dEX/" + RUNNAME + RUNNUM + "\",16)");

  //tel5 dEY
  gROOT->ProcessLine("tree->Draw(\"tel5dEY_raw.fP1>>h4(300,2300.,2900.)\",\"tel5dEX_raw.fE1>300\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h4, \"tel5/pos_dEY/" + RUNNAME + RUNNUM + "\",16)");

  //tel6 dEX
  gROOT->ProcessLine("tree->Draw(\"tel6dEX_raw.fP1>>h5(200,1950.,2550.)\",\"tel6dEX_raw.fE1>300\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h5, \"tel6/pos_dEX/" + RUNNAME + RUNNUM + "\",16)");

  //tel6 dEY
  gROOT->ProcessLine("tree->Draw(\"tel6dEY_raw.fP1>>h6(350,1600.,2150.)\",\"tel6dEX_raw.fE1>300\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h6, \"tel6/pos_dEY/" + RUNNAME + RUNNUM + "\",15)");



  gROOT->ProcessLine(".q");
}
