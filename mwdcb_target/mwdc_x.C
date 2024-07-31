void mwdc_x(){
  TString dir = "/home/zq/art_analysis/he6p2024/he6p2024/output/coin/gate/mwdc/mwdcb_f3target.root";

  TCanvas *c1 = new TCanvas();
  gStyle->SetOptStat(0);
  TFile *fopen = new TFile(dir);
   if(!fopen){
    std::cerr<<"Error: could not open the root file"<<std::endl;
    exit(0);
  }
  TDirectoryFile *dirfile = (TDirectoryFile *)fopen->Get("f3");
  if(!dirfile) {
    std::cerr<<"Error: Could not find directory in file"<<std::endl;
    exit(0);
  }

  TH2F *h2 = (TH2F*)dirfile->Get("f3mwdcX_dcbX");
  if(!h2) {
    std::cerr<<"Error: Could not find h2 in file!"<<std::endl;
    exit(0);
  }



  TGraphErrors *gr_mu = new TGraphErrors();
  TGraphErrors *gr_sigma = new TGraphErrors();

  double x_min = -18.5;//-20.0; // mm
  double x_max = 17.;//18.0; // mm 
  double delta = 1.; // mm 
  int x_bin_min = 0;
  int x_bin_max = 0;

  int i = 0;
  for(double x_center = x_min + delta/2.0; x_center<x_max; x_center=x_center+delta){
    x_bin_min = h2->GetXaxis()->FindBin(x_center - delta/2.0);
    x_bin_max = h2->GetXaxis()->FindBin(x_center + delta/2.0);
    //std::cout<<"x_center = "<<x_center<<", x_bin_min = "<<x_bin_min<<", x_bin_max = "<<x_bin_max<<std::endl;
    TH1D *h1 = h2->ProjectionY("target_x", x_bin_min, x_bin_max); 

    TF1 *f_gaus = new TF1("gaus_fit", "gaus(0)");
    f_gaus->SetParameters(2, 1.0,0.4);
    h1->Fit(f_gaus);//,"R");
    //std::cout<<"mean = "<<f1->GetParameter(1)<<", sigam = "<<f1->GetParameter(2)<<std::endl;
    h1->Draw();
    f_gaus->Draw("same");
    c1->SaveAs(Form("mwdcb_target/target_X/mwdc_%d.png",i));

    c1->Draw();
    c1->Update();
    gr_mu->SetPoint(i, x_center, f_gaus->GetParameter(1));
    gr_mu->SetPointError(i, 0.0,  f_gaus->GetParError(1));
    gr_sigma->SetPoint(i, x_center, f_gaus->GetParameter(2));
    gr_sigma->SetPointError(i, 0.0, f_gaus->GetParError(2));
    i++;
  }
  TF1 *f_mu = new TF1("pol_fit", "pol3(0)");
  f_mu->SetParameters(3, 1.0,0.4,0.5);
  gr_mu->SetMarkerStyle(20);
  gr_mu->SetMarkerSize(1.5);
  gr_mu->SetMarkerColor(2);
  gr_mu->Draw("ap");
  gr_mu->Fit(f_mu);
  f_mu->Draw("same");

  gPad->Update(); // 确保统计框已经被创建
  TPaveStats *stats_mu = (TPaveStats*)gr_mu->FindObject("stats");
  if (stats_mu) {
    stats_mu->SetX1NDC(0.65); // 设置统计框的左下角 X 坐标
    stats_mu->SetY1NDC(0.2); // 设置统计框的左下角 Y 坐标
    stats_mu->SetX2NDC(0.9); // 设置统计框的右上角 X 坐标
    stats_mu->SetY2NDC(0.6); // 设置统计框的右上角 Y 坐标
  }

  TLegend *legend_mu = new TLegend(0.1, 0.8, 0.4, 0.9);  // 调整这里以改变位置
  legend_mu->AddEntry(gr_mu, "Target_x", "ap");
  legend_mu->AddEntry(f_mu, "Poly. fitting", "l");
  legend_mu->Draw();
  c1->SaveAs("mwdcb_target/target_X/mwdc_mu.png");

  TF1 *f_sigma = new TF1("pol_fit", "pol5(0)");
  f_sigma->SetParameters(5, 1.0,0.4,0.5,0.5,0.1);
  gr_sigma->SetMarkerStyle(20);
  gr_sigma->SetMarkerSize(1.5);
  gr_sigma->SetMarkerColor(1);
  gr_sigma->Draw("ap");
  gr_sigma->Fit(f_sigma);
  f_sigma->Draw("same");

  gPad->Update(); // 确保统计框已经被创建
  TPaveStats *stats_sigma = (TPaveStats*)gr_sigma->FindObject("stats");
  if (stats_sigma) {
    stats_sigma->SetX1NDC(0.3); // 设置统计框的左下角 X 坐标
    stats_sigma->SetY1NDC(0.3); // 设置统计框的左下角 Y 坐标
    stats_sigma->SetX2NDC(0.7); // 设置统计框的右上角 X 坐标
    stats_sigma->SetY2NDC(0.6); // 设置统计框的右上角 Y 坐标
  }

  TLegend *legend_sigma = new TLegend(0.1, 0.8, 0.4, 0.9);  // 调整这里以改变位置
  legend_sigma->AddEntry(gr_sigma, "Target_x_sigma", "ap");
  legend_sigma->AddEntry(f_sigma, "Poly. fitting", "l");
  legend_sigma->Draw();
  c1->SaveAs("mwdcb_target/target_X/mwdc_sigma.png");
}
