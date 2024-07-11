void proton_wo_mwdca(TString NAME="", TString NUM="", TString TEL="")
{
  gROOT->ProcessLine(Form("add steering/procproton_wo_mwdca.yaml NAME=%s NUM=%s TEL=%s", NAME.Data(), NUM.Data(), TEL.Data()));
  art::TLoopManager::Instance()->GetLoop()->Resume();

  gROOT->ProcessLine(".q");
}
