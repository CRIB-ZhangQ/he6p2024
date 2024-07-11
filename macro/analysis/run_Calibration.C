void run_Calibration(TString RUNNAME="run", TString RUNNUM="0219")
{
    const TString ARTEMIS_WORKDIR = gSystem->pwd();

    //raw data
    gROOT->ProcessLine(Form("add steering/calibration.yaml NAME=%s NUM=%s",RUNNAME.Data(), RUNNUM.Data()));

    gROOT->ProcessLine("fcd 0");
    gROOT->ProcessLine("zone");
    gSystem->SetOptStat(0);

    std::cout<<"Start calibation!"<<std::endl;

    gROOT->ProcessLine("tree->Draw(\"tel1dEX_raw.fCharge:tel1dEX_raw>>tel1X(16,-0.5,15.5,250,500, 1500)\",\"\",\"colz\")");
    gROOT->ProcessLine(".x ARTEMIS_WORKDIR + "/macro/calibation.C(tel1X,\"tel1/ch2MeV_dEX\",3, 16, 15.0)");

    gROOT->ProcessLine("h11->Draw(\"colz\")");
    gROOT->artcanvas->Print(\"")

}