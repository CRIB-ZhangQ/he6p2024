void proton_bg_wo_mwdca(TString NAME="", TString NUM="", TString TEL="")
{
  gROOT->ProcessLine(Form("add steering/bg/procproton_wo_mwdca.yaml RUNNAME=%s RUNNUM=%s TEL=%s", NAME.Data(), NUM.Data(), TEL.Data()));
  art::TLoopManager::Instance()->GetLoop()->Resume();

  gROOT->ProcessLine(".q");
}

