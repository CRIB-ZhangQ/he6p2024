void ssd(){
  //  gROOT->ProcessLine("add steering/chkssd.yaml");
  //gROOT->ProcessLine("add steering/chkssd.yaml NAME=run NUM=0094");
  gROOT->ProcessLine("add steering/chkssd_allch.yaml NAME=run NUM=0042");
  gROOT->ProcessLine("cd 0");
  gROOT->ProcessLine("zone");
}
