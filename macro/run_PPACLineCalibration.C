{
  gStyle->SetOptStat(0);
  //gStyle->SetOptFit(0);

  const TString ARTEMIS_WORKDIR = "/home/crib/art_analysis/o14a/o14a";

  gROOT->ProcessLine("fcd 0");
  gROOT->ProcessLine("zone");

  // F3bPPAC(#3) prep0004
  gROOT->ProcessLine("tree->Draw(\"f3bppac.fY:f3bppac.fX>>hxy(500,-50.,50., 500,-50.,50.)\",\"\",\"colz\")");
  gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/ppac/ppacb#_xy.png\")");
  gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/PPACLineCalibration.C(hxy,3,2,0.0,1.7,175.0)");

  gROOT->ProcessLine("zone 2 2");
  gROOT->ProcessLine("ht 3");
  gROOT->ProcessLine("comment X projection");
  gROOT->ProcessLine("ht 2 colz");
  gROOT->ProcessLine("ht 4");
  gROOT->ProcessLine("comment Y projection");
  gROOT->ProcessLine("artcanvas->Print(\"" + ARTEMIS_WORKDIR + "/figure/calib/ppac/ppacb_line_calib.png\")");


}

