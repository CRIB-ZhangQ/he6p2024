{
  // // coin
  tree->SetAlias("Single", "single.fID == 0");
  tree->SetAlias("Coin", "coin.fID == 0");
  tree->SetAlias("Pileup", "pileup.fID == 0");

  //rf_with_Single
  tree->SetAlias("rf_single","abs(rf_cal_0.fTiming + 1.51323e3)<4.0*2.84641 || abs(rf_cal_0.fTiming + 1.44277e3)<4.*2.87926");
  tree->SetAlias("rf_coin","abs(rf_cal_0.fTiming + 1.54401e3)<4.0*3.17554 || abs(rf_cal_0.fTiming + 1.47565e3)<4.*3.20920 && abs(rf_cal_0.fTiming + 1.40435e3)<4.*3.25950");

  //catha and cathb
  //tree->SetAlias("TOF","abs(catha_cal.fTiming-cathb_cal.fTiming-6)<16");
  ////tree->SetAlias("rf_Coin","abs(rf0.fTiming + 1501)<12 || abs(rf0.fTiming + 1431)<12 || abs(rf0.fTiming+1361)<11");
  ////tree->SetAlias("TOF","abs(cathaTiming-cathbTiming-6)<16");

  ////cath_b, cath_a
  //tree->SetAlias("cathb","abs(cathb_cal.fTiming+1215.53)<3.0*3.56038");
  ////tree->SetAlias("cathb","abs(cathb_cal.fTiming+1215.53)<3.0*3.56038");

  // // trigger
  //tree->SetAlias("mwdcb_trigger","abs(mwdcb.GetTrack().GetY()-0.9)<21 && abs(mwdcb.GetTrack().GetX()-0.5)<20");
  ////tree->SetAlias("mwdcb","abs(mwdcbY-0.9)<21 && abs(mwdcbX-0.5)<20");

  //// target
  //tree->SetAlias("mwdc_target","(f3mwdc.GetY()*f3mwdc.GetY()+f3mwdc.GetX()*f3mwdc.GetX())<15.0*15.0");
  ////tree->SetAlias("target","(targetY*targetY+targetX*targetX)<15.0*15.0");
  TCut mwdcb_charge = "mwdcb_raw.fCharge>360 && mwdcb_raw.fCharge<860";
  TCut tof = "(tel1E_raw[0].fTiming-mwdcb_raw.fTiming)>17600 && (tel1E_raw[0].fTiming-mwdcb_raw.fTiming)<18600"
}
