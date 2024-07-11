{
  // // coin
  tree->SetAlias("beam_coin", "coin_raw.fID == 1");

  //rf_with_coin
  tree->SetAlias("rf_coin","abs(rf_cal_0.fTiming + 1.54196e3)<4.0*4.33145 || abs(rf_cal_0.fTiming + 1.47519e3)<4.*5.01248 || abs(rf_cal_0.fTiming + 1.40363e3)<4.*4.27355");

  //cath_b, cath_a
  tree->SetAlias("cath_timing_coin","(catha_cal_gate.fTiming-cathb_cal_gate.fTiming)< 150 && (catha_cal_gate.fTiming-cathb_cal_gate.fTiming)> -150 ");
  tree->SetAlias("catha_timing_coin","catha_cal_gate.fTiming<-1500 && catha_cal_gate.fTiming>-1650");
  tree->SetAlias("cathb_timing_coin","cathb_cal_gate.fTiming<-1550 && cathb_cal_gate.fTiming>-1650");
  tree->SetAlias("catha_charge_coin","abs(catha_cal_gate.fCharge-4.38146e2)<4.0*2.15717");
  tree->SetAlias("cathb_charge_coin","abs(cathb_cal_gate.fCharge-4.63613e2)<4.0*1.94223");

  //mwdca, mwdcb
  tree->SetAlias("mwdc_timing_coin","(mwdca_cal_gate.fTiming-mwdcb_cal_gate.fTiming)< 200 && (mwdca_cal_gate.fTiming-mwdcb_cal_gate.fTiming)> -200");
  tree->SetAlias("mwdca_timing_coin","mwdca_cal_gate.fTiming<-1600 && mwdca_cal_gate.fTiming>-1700");
  tree->SetAlias("mwdcb_timing_coin","mwdcb_cal_gate.fTiming<-1600 && mwdcb_cal_gate.fTiming>-1700");
  tree->SetAlias("mwdca_charge_coin","abs(mwdca_cal_gate.fCharge-6.44685e2)<4.0*1.01555e2");
  tree->SetAlias("mwdcb_charge_coin","abs(mwdcb_cal_gate.fCharge-7.83976e2)<4.0*8.77652e1");

  // target
  tree->SetAlias("target","(f3mwdc.GetY()*f3mwdc.GetY()+f3mwdc.GetX()*f3mwdc.GetX())<15.0*15.0");
  //TCut tof = "(tel1E_raw[0].fTiming-mwdcb_raw.fTiming)>17600 && (tel1E_raw[0].fTiming-mwdcb_raw.fTiming)<18600"
}
