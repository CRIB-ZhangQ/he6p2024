void mwdcb_f3target()
{
  //ROOT::EnableImplicitMT(20);
  //#pragma omp parallel num_threads(20)

  gROOT->ProcessLine(Form("add steering/phy/proc_mwdcb_f3target.yaml"));
  art::TLoopManager::Instance()->GetLoop()->Resume();

  gROOT->ProcessLine(".q");
}
