/**
 * @file    AlphaCalibration.C
 * @brief   alpha calibration macro for CRIB expereriment
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2023-12-21 14:20:03
 * @note    updated by Zhang Qian for mwdc
 */
//#include<"TApplication.h">
#include "TApplication.h"
void mwdc(TH2 *h2 = NULL, TString output = "", const Double_t width = 2.0) {
    TString ARTEMIS_WORKDIR = gSystem->pwd();
    TString output_prm = ARTEMIS_WORKDIR + "/prm/" + output + ".dat";
    TString output_fig_base = ARTEMIS_WORKDIR + "/figure/" + output;
    TString output_raw_path = output_fig_base;
    TString output_E_ch = output_fig_base + ".dat";

    if (!h2) {
        std::cout << "mwdc angle estiamtion" << std::endl;
        std::cout << "\twidth    : fitting width" << std::endl;
        return;
    }

    if (gSystem->ChangeDirectory(output_raw_path)) {
        gSystem->ChangeDirectory(ARTEMIS_WORKDIR);
    } else {
        gSystem->mkdir(output_raw_path, kTRUE);
    }

    gROOT->ProcessLine("zone");
    h2->Draw("colz");
    gROOT->ProcessLine(Form("artcanvas->Print(\"%s/mwdc_angle.png\")", output_fig_base.Data()));



    std::ofstream fout(output_prm.Data());
    std::ofstream fout_E_ch(output_E_ch.Data());
    if (!fout) {
        std::cerr << "--ERROR: cannot open " << output_prm << std::endl;
        return;
    }
    std::cout << "--Info: " << output_prm << " is created" << std::endl;
    fout << "# mean sigma" << endl;

    if (!fout_E_ch) {
        std::cerr << "--ERROR: cannot open " << output_E_ch << std::endl;
        return;
    }

    // start searching the peaks
    TSpectrum *spec = new TSpectrum(1, 1.5);
    TF1 *f = new TF1("f", "gaus(0)");

    TH1 *angle_x = h2->ProjectionX("angle_x");
    if (!angle_x) {
        std::cout << "--Warning: angle_x is NULL" << std::endl;
        fout << "0.0 0.0" << endl;
        return;
    }
    gROOT->ProcessLine("zone");
    angle_x->Draw();
    Int_t nfound = spec->Search(angle_x, 1, "", 0.1);
    
    if (nfound != 1) {
        fout << "0.0 0.0" << endl;
        return;
    }
    Double_t *xpeak = spec->GetPositionX();

    f->SetRange(*xpeak - width, *xpeak + width);
    f->SetParameters(200, *xpeak, 20.0); // constant, mean, sigma
    //f->SetParLimits(2, 0, 65);
    angle_x->Fit(f, "rq");
    Double_t mean = f -> GetParameter(1);
    Double_t sigma = f -> GetParameter(2);
    f->Draw("same");

    //gStyle->SetOptFit(0);
    gROOT->ProcessLine(Form("artcanvas->Print(\"%s/angle.png\")", output_raw_path.Data()));
    fout_E_ch << f->GetParameter(1) << " " << f->GetParameter(2)<<endl;
    fout_E_ch.close();
    fout << f->GetParameter(1) << " " << f->GetParameter(2)<<endl;
    fout.close();
}
