{
  // // coin
  //tree->SetAlias("single", "single.fID == 0");
  //tree->SetAlias("pileup", "pileup.fID == 0");
  tree->SetAlias("beam_single", "coin_raw.fID == 0");
  tree->SetAlias("beam_pileup", "coin_raw.fID == 2");

  //rf_with_coin
  tree->SetAlias("rf_single","abs(rf_cal_0.fTiming + 1.51323e3)<4.0*2.84641 || abs(rf_cal_0.fTiming + 1.44277e3)<4.*2.87926");

  //cath_b, cath_a
  tree->SetAlias("cath_timing_single","abs(catha_cal_gate.fTiming-cathb_cal_gate.fTiming-6.90389)<4.0*2.76773");
  tree->SetAlias("catha_timing_single","catha_cal_gate.fTiming<-1480 && catha_cal_gate.fTiming>-1522");
  tree->SetAlias("cathb_timing_single","cathb_cal_gate.fTiming<-1480 && cathb_cal_gate.fTiming>-1522");
  tree->SetAlias("catha_charge_single","abs(catha_cal_gate.fCharge-4.37037e2)<4.0*2.90589");
  tree->SetAlias("cathb_charge_single","abs(cathb_cal_gate.fCharge-4.63301e2)<4.0*1.74050");

  //mwdca, mwdcb
  tree->SetAlias("mwdc_timing_single","abs(mwdca_cal_gate.fTiming-mwdcb_cal_gate.fTiming-9.09876)<4.0*5.03888");
  tree->SetAlias("mwdca_timing_single","mwdca_cal_gate.fTiming>-1620 && mwdcb_cal_gate.fTiming<-1480");
  tree->SetAlias("mwdcb_timing_single","mwdcb_cal_gate.fTiming>-1610 && mwdcb_cal_gate.fTiming<-1530");
  tree->SetAlias("mwdca_charge_single","abs(mwdca_cal_gate.fCharge-6.44105e2)<4.0*1.00629e2");
  tree->SetAlias("mwdcb_charge_single","abs(mwdcb_cal_gate.fCharge-7.78090e2)<4.0*8.35092e1");

  // target
  tree->SetAlias("target","(f3mwdc.GetY()*f3mwdc.GetY()+f3mwdc.GetX()*f3mwdc.GetX())<15.0*15.0");
}
