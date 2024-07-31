void run_MUXParamMaker(TString RUNNAME="run", TString RUNNUM="0072"){
  TString ARTEMIS_WORK_DIR = gSystem->pwd();

  gROOT->ProcessLine(Form("add steering/phy/chkmux.yaml NAME=%s NUM=%s", RUNNAME.Data(), RUNNUM.Data()));
  art::TLoopManager::Instance()->GetLoop()->Resume();

  gROOT->ProcessLine("fcd 0");
  gROOT->ProcessLine("zone");
  //gROOT->ProcessLine("lgy");

  // run0042
  //tel4 dEX
  gROOT->ProcessLine("tree->Draw(\"tel4dEX_raw.fP1>>h1(150,3850.,4380.)\",\"tel4dEX_raw.fE1>500\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h1, \"tel4/pos_dEX/" + RUNNAME + RUNNUM + "\",16)");

  //tel4 dEY
  gROOT->ProcessLine("tree->Draw(\"tel4dEY_raw.fP1>>h2(150,3550.,4050.)\",\"tel4dEY_raw.fE1>500\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h2, \"tel4/pos_dEY/" + RUNNAME + RUNNUM + "\",16)");

  //tel5 dEX
  gROOT->ProcessLine("tree->Draw(\"tel5dEX_raw.fP1>>h3(150,2750.,3400.)\",\"tel5dEX_raw.fE1>500\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h3, \"tel5/pos_dEX/" + RUNNAME + RUNNUM + "\",16)");

  //tel5 dEY
  gROOT->ProcessLine("tree->Draw(\"tel5dEY_raw.fP1>>h4(150,2300.,2900.)\",\"tel5dEY_raw.fE1>500\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h4, \"tel5/pos_dEY/" + RUNNAME + RUNNUM + "\",16)");

  //tel6 dEX
  gROOT->ProcessLine("tree->Draw(\"tel6dEX_raw.fP1>>h5(150,1950.,2550.)\",\"tel6dEX_raw.fE1>500\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h5, \"tel6/pos_dEX/" + RUNNAME + RUNNUM + "\",16)");

  //tel6 dEY
  gROOT->ProcessLine("tree->Draw(\"tel6dEY_raw.fP1>>h6(150,1600.,2150.)\",\"tel6dEY_raw.fE1>500\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORK_DIR + "/macro/MUXParamMaker.C(h6, \"tel6/pos_dEY/" + RUNNAME + RUNNUM + "\",15)");



  gROOT->ProcessLine(".q");
}
