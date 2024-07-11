// MUXParamMaker.C
// this creates the MUX position parameters
// only for pos1 (16 peaks)

#include <algorithm>
#include <iterator>
#include <vector>

void Usage()
{
  std::cout << "MUX position calibration" << std::endl;
  std::cout << "Usage: MUXParamMaker(TH1* h1, TString output=\"test/test\", const Int_t peaknum=16);" << std::endl;
  std::cout << "\th1     : create position histogram before using this" << std::endl;
  std::cout << "\toutput : the name of parameter file (not include .dat)" << std::endl;
  std::cout << "\tpeaknum: the number of peaks" << std::endl;
}

void MUXParamMaker(TH1* h1=NULL, TString output="test/test", const Int_t peaknum=15)
{
  if(!h1){
    Usage();
    return;
  }

  TString ARTEMIS_WORKDIR = gSystem->pwd();
  TString output_prm = ARTEMIS_WORKDIR + "/prm/ssd/" + output + ".dat";
  TString output_fig = ARTEMIS_WORKDIR + "/figure/calib/" + output + ".png";

  if(output == "test/test"){
    std::cout << "Warning: output file name is DEFAULT" << std::endl;
  }

  TSpectrum *spec = new TSpectrum(peaknum,1);
  std::ofstream fout(output_prm.Data());
  if(!fout){
    std::cout << "[Error] cannot create" << output_prm.Data() << std::endl;
    return;
  }

  std::cout << "--Info: " << output_prm << " is created" << std::endl;
  fout << "# " << output << " is created from MUXParamMaker.C" << endl;

  Int_t nfound = spec->Search(h1, 2, "", 0.001);
  if(nfound != peaknum){
    std::cout << "Error: 33 in MUXParamMaker: cannot find " << peaknum << " peaks (only found " << nfound << " peaks)" << std::endl;
      for(Int_t i=0; i<peaknum; ++i){
        fout << "0.0 0.0" << endl;
      }
    fout.close();
    return;
  }
  Double_t *xpeaks = spec->GetPositionX();
  std::sort(xpeaks, xpeaks + peaknum);

  std::vector<Double_t> x(peaknum, 0.0);
  TF1 *f[peaknum];
  for(Int_t i=0; i<peaknum; ++i){
    TString tmp;
    tmp.Form("f[%d]",i);
    f[i] = new TF1(tmp.Data(), "gaus(0)");
    f[i]->SetRange(xpeaks[i]-10.0, xpeaks[i]+10.0);
    f[i]->SetParameters(100, xpeaks[i], 1);
    f[i]->SetParLimits(2, 0, 100);
    h1->Fit(f[i], "rq");
    x[i] = f[i]->GetParameter(1);
    f[i]->Draw("same");
  }

  TString target = gSystem->DirName(output_fig);
  if(gSystem->ChangeDirectory(target)){
    gSystem->ChangeDirectory(ARTEMIS_WORKDIR);
  }else{
    gSystem->mkdir(target,kTRUE);
  }
  gROOT->ProcessLine(Form("artcanvas->Print(\"%s\")", output_fig.Data()));

  fout << x[0]-10.0 << " " << (x[0] + x[1])/2.0 << endl;
  for(Int_t i=1; i<peaknum-1; ++i){
    fout << (x[i-1] + x[i])/2.0 << " " << (x[i] + x[i+1])/2.0 << endl;
  }
  fout << (x[peaknum-2] + x[peaknum-1])/2.0 << " " << x[peaknum-1]+10 << endl;


  fout.close();


}
