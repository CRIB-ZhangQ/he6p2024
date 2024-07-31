void proton(TString NAME="", TString NUM="", TString TEL="")
{
  gROOT->ProcessLine(Form("add steering/phy/procproton.yaml RUNNAME=%s RUNNUM=%s TEL=%s", NAME.Data(), NUM.Data(), TEL.Data()));
  art::TLoopManager::Instance()->GetLoop()->Resume();

  gROOT->ProcessLine(".q");
}
