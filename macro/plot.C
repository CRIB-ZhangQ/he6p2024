void pcd(Int_t i);

void plot(){
  char *a;

  //zone 3 3
  //  pcd 1;
  gROOT->ProcessLine("cd /"); 
  gROOT->ProcessLine("zone 3 2"); 
  pcd(1);
  tel1dE_raw->FindObject("ID_EX")->Draw("COLZ");
  pcd(2);
  tel1dE_raw->FindObject("ID_EY")->Draw("COLZ");
  pcd(3);
  tel2dE_raw->FindObject("ID_EX")->Draw("COLZ");
  pcd(4);
  tel2dE_raw->FindObject("ID_EY")->Draw("COLZ");
  pcd(5);
  tel3dE_raw->FindObject("ID_EX")->Draw("COLZ");
  pcd(6);
  tel3dE_raw->FindObject("ID_EY")->Draw("COLZ");
  
  gPad->Update();
  cout << "Input anything to proceed:";
  cin >> a;


  gROOT->ProcessLine("zone 3 2"); 
  pcd(1);
  tel4dEX_raw->FindObject("E1_P1")->Draw("COLZ");
  pcd(2);
  tel4dEY_raw->FindObject("E1_P1")->Draw("COLZ");
  pcd(3);
  tel5dEX_raw->FindObject("E1_P1")->Draw("COLZ");
  pcd(4);
  tel5dEY_raw->FindObject("E1_P1")->Draw("COLZ");
  pcd(5);
  tel6dEX_raw->FindObject("E1_P1")->Draw("COLZ");
  pcd(6);
  tel6dEY_raw->FindObject("E1_P1")->Draw("COLZ");


}

void pcd(Int_t i) {
   char cmd[50];
   sprintf(cmd,"pcd %d",i);
   gROOT->ProcessLine(cmd); 

}
