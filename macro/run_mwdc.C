void run_mwdc(TString RUNNAME="run", TString RUNNUM="0099") {

    TString ARTEMIS_WORKDIR = gSystem->pwd();

    gROOT->ProcessLine(Form("add steering/chkf3.yaml NAME=%s NUM=%s", RUNNAME.Data(), RUNNUM.Data()));
    art::TLoopManager::Instance()->GetLoop()->Resume();

    gROOT->ProcessLine("fcd 0");
    gROOT->ProcessLine("zone");

    gStyle->SetOptStat(0);

    ////mwdcb position vs beam angle 
    for(Int_t ch = 16; ch>12 ; ch-=2){
      gROOT->ProcessLine(Form("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h%d(100,-5,5,100,-25,25)\",\"mwdcb.fX>%d && mwdcb.fX<=%d\",\"colz\")",ch,ch-2,ch));
      gROOT->ProcessLine(Form("\".x\"  + \"ARTEMIS_WORKDIR\" + \"/macro/mwdc.C(h%d,\"mwdc/angle_x/ch%d\", 2.)\"",ch,ch));
      //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
      //gROOT->ProcessLine(Form(".x  + ARTEMIS_WORKDIR + \"/macro/mwdc.C(h11, \"mwdc/angle_x/ch%d\", 2.)\"",ch));
    }
    //for(Int_t ch = 18; ch>14 ; ch-=2){ 
    //  gROOT->ProcessLine("tree->Draw(\"mwdcb.fY:(f3mwdc.fB*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",Form(\"mwdcb.fY>%d && mwdcb.fY<=%d\",ch-2,ch),\"colz\")");
    //  gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, Form(\"mwdc/angle_y/ch%d\",ch), 2.)");
    //}
}
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>16 && mwdcb.fX<=18\",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>14 && mwdcb.fX<=16\",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>12 && mwdcb.fX<=14 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>10 && mwdcb.fX<=12\",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>8 && mwdcb.fX<=10 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>6 && mwdcb.fX<=8 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>4 && mwdcb.fX<=6 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>2 && mwdcb.fX<=4 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>0 && mwdcb.fX<=2  \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>-2 && mwdcb.fX<=0 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>-4 && mwdcb.fX<=-2 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>-6 && mwdcb.fX<=-4 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>-8 && mwdcb.fX<=-6 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>-10 && mwdcb.fX<=-8 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>-12 && mwdcb.fX<=-10 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>-14 && mwdcb.fX<=-12 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>-16 && mwdcb.fX<=-14 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
    //gROOT->ProcessLine("tree->Draw(\"mwdcb.fX:(f3mwdc.fA*TMath::RadToDeg())>>h11(100,-5,5,100,-25,25)\",\"mwdcb.fX>-18 && mwdcb.fX<=-16 \",\"colz\")");
    //gROOT->ProcessLine(".x " + ARTEMIS_WORKDIR + "/macro/mwdc.C(h11, \"mwdc/angle_x\", 2.)");
