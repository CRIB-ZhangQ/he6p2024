void MakeSimpleRoot(TString RUNNAME, TString RUNNUM, Int_t ID){
    if(ID < 0 || ID > 2){
      std::cerr << "ERROR: invalid ID" << std::endl;
      std::cerr << "usage: ID = 0 for f1 data" << std::endl;
      std::cerr << "     : ID = 1 for f2 data" << std::endl;
      std::cerr << "     : ID = 2 for f3 data" << std::endl;
      return;
    }

    //setting
    const TString ARTEMIS_WORKDIR = "/home/zq/art_analysis/he6p/he6p";
    Bool_t debug = true;

    TString focal;
    if(ID == 0){
      focal.Form("f1");
    }else if(ID == 1){
      focal.Form("f2");
    }else{
      focal.Form("f3");
    }

    TString INROOT = ARTEMIS_WORKDIR + "output/" + RUNNAME + "/" + RUNNUM + "/chk" + focal + RUNNAME + RUNNUM + ".root";
    TString OUTROOT = ARTEMIS_WORKDIR + "rootfile/" + RUNNAME + RUNNUM + ".root";
    std::cout << "[info] output file is " << OUTROOT.Data() << std::endl;

    FileStat_t info;
    if(gSystem->GetPathInfo(INROOT.Data(), info) != 0){
      std::cerr << "ERROR: file: '" << INROOT.Data() << "' does not exist." << std::endl;
      return;
    }

    TFile *fin  = TFile::Open(INROOT.Data());
    TTree *intree = (TTree*)fin->Get("tree");

    // output tree setting
    TString pwd = gSystem->pwd();
    TString target = gSystem->DirName(OUTROOT);
    if (gSystem->ChangeDirectory(target)) {
      gSystem->ChangeDirectory(pwd);
    } else {
      gSystem->mkdir(target,kTRUE);
    }
    TFile *fout = new TFile(OUTROOT.Data(),"recreate");
    TTree *outtree = new TTree("tree","tree");

    //=================================================
    // convert F1 ROOT
    //=================================================
    if(ID == 0){
      // input tree
      TClonesArray *f1ppac = NULL;

      intree->SetBranchAddress("f1ppac", &f1ppac);

      Double_t f1ppac_tx1;
      Double_t f1ppac_tx2;
      Double_t f1ppac_ty1;
      Double_t f1ppac_ty2;
      Double_t f1ppac_x;
      Double_t f1ppac_y;

      outtree->Branch("f1ppac_tx1",&f1ppac_tx1,"f1ppac_tx1/D");
      outtree->Branch("f1ppac_tx2",&f1ppac_tx2,"f1ppac_tx2/D");
      outtree->Branch("f1ppac_ty1",&f1ppac_ty1,"f1ppac_ty1/D");
      outtree->Branch("f1ppac_ty2",&f1ppac_ty2,"f1ppac_ty2/D");
      outtree->Branch("f1ppac_x",&f1ppac_x,"f1ppac_x/D");
      outtree->Branch("f1ppac_y",&f1ppac_y,"f1ppac_y/D");

      Int_t progress_tmp = 1;
      for(Int_t iEntry=0; iEntry<intree->GetEntriesFast(); ++iEntry){
        if((iEntry+1) % (Int_t)(intree->GetEntriesFast()/100) == 0){
          std::cout << iEntry << " / " << intree->GetEntriesFast() << " : " << progress_tmp << " %" << std::endl;
          progress_tmp++;
        }
        intree->GetEntry(iEntry);

        f1ppac_tx1 = TMath::QuietNaN();
        f1ppac_tx2 = TMath::QuietNaN();
        f1ppac_ty1 = TMath::QuietNaN();
        f1ppac_ty2 = TMath::QuietNaN();
        f1ppac_x   = TMath::QuietNaN();
        f1ppac_y   = TMath::QuietNaN();

        if(f1ppac->GetEntriesFast() == 1){
          f1ppac_tx1 = ((art::TPPACData*)f1ppac->At(0))->GetTX1();
          f1ppac_tx2 = ((art::TPPACData*)f1ppac->At(0))->GetTX2();
          f1ppac_ty1 = ((art::TPPACData*)f1ppac->At(0))->GetTY1();
          f1ppac_ty2 = ((art::TPPACData*)f1ppac->At(0))->GetTY2();
          f1ppac_x   = ((art::TPPACData*)f1ppac->At(0))->GetX();
          f1ppac_y   = ((art::TPPACData*)f1ppac->At(0))->GetY();
        //}else{
        //  std::cout << "warning: f1ppac event is not 1" << std::endl;
        }
        outtree->Fill();
      }
      outtree->Write();
      fout->Close();

    //=================================================
    // convert F2 ROOT
    //=================================================
    }else if(ID == 1){

      // input tree
      TClonesArray *f2ppac    = NULL;
      TClonesArray *f2ssd_raw = NULL;
      TClonesArray *f2ssd_cal = NULL;
      TClonesArray *rf_cal_0  = NULL;
      TClonesArray *rf_cal_1  = NULL;

      intree->SetBranchAddress("f2ppac", &f2ppac);
      intree->SetBranchAddress("f2ssd_raw", &f2ssd_raw);
      intree->SetBranchAddress("f2ssd_cal", &f2ssd_cal);
      intree->SetBranchAddress("rf_cal_0", &rf_cal_0);
      intree->SetBranchAddress("rf_cal_1", &rf_cal_1);

      Double_t f2ppac_tx1;
      Double_t f2ppac_tx2;
      Double_t f2ppac_ty1;
      Double_t f2ppac_ty2;
      Double_t f2ppac_t;
      Double_t f2ppac_x;
      Double_t f2ppac_y;
      Double_t f2ssd_e_raw;
      Double_t f2ssd_t_raw;
      Double_t f2ssd_e;
      Double_t f2ssd_t;
      Double_t rf0;
      Double_t rf1;

      outtree->Branch("f2ppac_tx1",&f2ppac_tx1,"f2ppac_tx1/D");
      outtree->Branch("f2ppac_tx2",&f2ppac_tx2,"f2ppac_tx2/D");
      outtree->Branch("f2ppac_ty1",&f2ppac_ty1,"f2ppac_ty1/D");
      outtree->Branch("f2ppac_ty2",&f2ppac_ty2,"f2ppac_ty2/D");
      outtree->Branch("f2ppac_t",&f2ppac_t,"f2ppac_t/D");
      outtree->Branch("f2ppac_x",&f2ppac_x,"f2ppac_x/D");
      outtree->Branch("f2ppac_y",&f2ppac_y,"f2ppac_y/D");
      outtree->Branch("f2ssd_e_raw",&f2ssd_e_raw,"f2ssd_e_raw/D");
      outtree->Branch("f2ssd_t_raw",&f2ssd_t_raw,"f2ssd_t_raw/D");
      outtree->Branch("f2ssd_e",&f2ssd_e,"f2ssd_e/D");
      outtree->Branch("f2ssd_t",&f2ssd_t,"f2ssd_t/D");
      outtree->Branch("rf0",&rf0,"rf0/D");
      outtree->Branch("rf1",&rf1,"rf1/D");

      Int_t progress_tmp = 1;
      for(Int_t iEntry=0; iEntry<intree->GetEntriesFast(); ++iEntry){
        if((iEntry+1) % (Int_t)(intree->GetEntriesFast()/100) == 0){
          std::cout << iEntry << " / " << intree->GetEntriesFast() << " : " << progress_tmp << " %" << std::endl;
          progress_tmp++;
        }
        intree->GetEntry(iEntry);


        f2ppac_tx1  = TMath::QuietNaN();
        f2ppac_tx2  = TMath::QuietNaN();
        f2ppac_ty1  = TMath::QuietNaN();
        f2ppac_ty2  = TMath::QuietNaN();
        f2ppac_t    = TMath::QuietNaN();
        f2ppac_x    = TMath::QuietNaN();
        f2ppac_y    = TMath::QuietNaN();
        f2ssd_e_raw = TMath::QuietNaN();
        f2ssd_t_raw = TMath::QuietNaN();
        f2ssd_e     = TMath::QuietNaN();
        f2ssd_t     = TMath::QuietNaN();
        rf0         = TMath::QuietNaN();
        rf1         = TMath::QuietNaN();

        if(f2ppac->GetEntriesFast() == 1){
          f2ppac_tx1  = ((art::TPPACData*)f2ppac->At(0))->GetTX1();
          f2ppac_tx2  = ((art::TPPACData*)f2ppac->At(0))->GetTX2();
          f2ppac_ty1  = ((art::TPPACData*)f2ppac->At(0))->GetTY1();
          f2ppac_ty2  = ((art::TPPACData*)f2ppac->At(0))->GetTY2();
          f2ppac_t    = ((art::TPPACData*)f2ppac->At(0))->GetTAnode();
          f2ppac_x    = ((art::TPPACData*)f2ppac->At(0))->GetX();
          f2ppac_y    = ((art::TPPACData*)f2ppac->At(0))->GetY();
        }

        if(f2ssd_raw->GetEntriesFast() == 1){
          f2ssd_e_raw = ((art::TTimingChargeData*)f2ssd_raw->At(0))->GetCharge();
          f2ssd_t_raw = ((art::TTimingChargeData*)f2ssd_raw->At(0))->GetTiming();
        }

        if(f2ssd_cal->GetEntriesFast() == 1){
          f2ssd_e     = ((art::TTimingChargeData*)f2ssd_cal->At(0))->GetCharge();
          f2ssd_t     = ((art::TTimingChargeData*)f2ssd_cal->At(0))->GetTiming();
        }

        if(rf_cal_0->GetEntriesFast() == 1){
          rf0         = ((art::TTimingData*)rf_cal_0->At(0))->GetTiming();
        }

        if(rf_cal_1->GetEntriesFast() == 1){
          rf1         = ((art::TTimingData*)rf_cal_1->At(0))->GetTiming();
        }

        outtree->Fill();
      }
      outtree->Write();
      fout->Close();

    //=================================================
    // convert F3 ROOT
    //=================================================
    }else{

      // input tree
      TClonesArray *f3appac  = NULL;
      TClonesArray *f3bppac  = NULL;
      TClonesArray *f3ppac   = NULL;
      TClonesArray *rf_cal_0 = NULL;
      TClonesArray *rf_cal_1 = NULL;

      intree->SetBranchAddress("f3appac", &f3appac);
      intree->SetBranchAddress("f3bppac", &f3bppac);
      intree->SetBranchAddress("f3ppac", &f3ppac);
      intree->SetBranchAddress("rf_cal_0", &rf_cal_0);
      intree->SetBranchAddress("rf_cal_1", &rf_cal_1);

      Double_t f3ppaca_tx1;
      Double_t f3ppaca_tx2;
      Double_t f3ppaca_ty1;
      Double_t f3ppaca_ty2;
      Double_t f3ppaca_t;
      Double_t f3ppaca_x;
      Double_t f3ppaca_y;
      Double_t f3ppacb_tx1;
      Double_t f3ppacb_tx2;
      Double_t f3ppacb_ty1;
      Double_t f3ppacb_ty2;
      Double_t f3ppacb_t;
      Double_t f3ppacb_x;
      Double_t f3ppacb_y;
      Double_t target_x;
      Double_t target_y;
      Double_t rf0;
      Double_t rf1;

      outtree->Branch("f3ppaca_tx1",&f3ppaca_tx1,"f3ppaca_tx1/D");
      outtree->Branch("f3ppaca_tx2",&f3ppaca_tx2,"f3ppaca_tx2/D");
      outtree->Branch("f3ppaca_ty1",&f3ppaca_ty1,"f3ppaca_ty1/D");
      outtree->Branch("f3ppaca_ty2",&f3ppaca_ty2,"f3ppaca_ty2/D");
      outtree->Branch("f3ppaca_t",&f3ppaca_t,"f3ppaca_t/D");
      outtree->Branch("f3ppaca_x",&f3ppaca_x,"f3ppaca_x/D");
      outtree->Branch("f3ppaca_y",&f3ppaca_y,"f3ppaca_y/D");
      outtree->Branch("f3ppacb_tx1",&f3ppacb_tx1,"f3ppacb_tx1/D");
      outtree->Branch("f3ppacb_tx2",&f3ppacb_tx2,"f3ppacb_tx2/D");
      outtree->Branch("f3ppacb_ty1",&f3ppacb_ty1,"f3ppacb_ty1/D");
      outtree->Branch("f3ppacb_ty2",&f3ppacb_ty2,"f3ppacb_ty2/D");
      outtree->Branch("f3ppacb_t",&f3ppacb_t,"f3ppacb_t/D");
      outtree->Branch("f3ppacb_x",&f3ppacb_x,"f3ppacb_x/D");
      outtree->Branch("f3ppacb_y",&f3ppacb_y,"f3ppacb_y/D");
      outtree->Branch("target_x",&target_x,"target_x/D");
      outtree->Branch("target_y",&target_y,"target_y/D");
      outtree->Branch("rf0",&rf0,"rf0/D");
      outtree->Branch("rf1",&rf1,"rf1/D");

      Int_t progress_tmp = 1;
      for(Int_t iEntry=0; iEntry<intree->GetEntriesFast(); ++iEntry){
        if((iEntry+1) % (Int_t)(intree->GetEntriesFast()/100) == 0){
          std::cout << iEntry << " / " << intree->GetEntriesFast() << " : " << progress_tmp << " %" << std::endl;
          progress_tmp++;
        }
        intree->GetEntry(iEntry);

        f3ppaca_tx1 = TMath::QuietNaN();
        f3ppaca_tx2 = TMath::QuietNaN();
        f3ppaca_ty1 = TMath::QuietNaN();
        f3ppaca_ty2 = TMath::QuietNaN();
        f3ppaca_t   = TMath::QuietNaN();
        f3ppaca_x   = TMath::QuietNaN();
        f3ppaca_y   = TMath::QuietNaN();
        f3ppacb_tx1 = TMath::QuietNaN();
        f3ppacb_tx2 = TMath::QuietNaN();
        f3ppacb_ty1 = TMath::QuietNaN();
        f3ppacb_ty2 = TMath::QuietNaN();
        f3ppacb_t   = TMath::QuietNaN();
        f3ppacb_x   = TMath::QuietNaN();
        f3ppacb_y   = TMath::QuietNaN();
        target_x    = TMath::QuietNaN();
        target_y    = TMath::QuietNaN();
        rf0         = TMath::QuietNaN();
        rf1         = TMath::QuietNaN();

        if(f3appac->GetEntriesFast() == 1){
          f3ppaca_tx1 = ((art::TPPACData*)f3appac->At(0))->GetTX1();
          f3ppaca_tx2 = ((art::TPPACData*)f3appac->At(0))->GetTX2();
          f3ppaca_ty1 = ((art::TPPACData*)f3appac->At(0))->GetTY1();
          f3ppaca_ty2 = ((art::TPPACData*)f3appac->At(0))->GetTY2();
          f3ppaca_t   = ((art::TPPACData*)f3appac->At(0))->GetTAnode();
          f3ppaca_x   = ((art::TPPACData*)f3appac->At(0))->GetX();
          f3ppaca_y   = ((art::TPPACData*)f3appac->At(0))->GetY();
        }

        if(f3bppac->GetEntriesFast() == 1){
          f3ppacb_tx1 = ((art::TPPACData*)f3bppac->At(0))->GetTX1();
          f3ppacb_tx2 = ((art::TPPACData*)f3bppac->At(0))->GetTX2();
          f3ppacb_ty1 = ((art::TPPACData*)f3bppac->At(0))->GetTY1();
          f3ppacb_ty2 = ((art::TPPACData*)f3bppac->At(0))->GetTY2();
          f3ppacb_t   = ((art::TPPACData*)f3bppac->At(0))->GetTAnode();
          f3ppacb_x   = ((art::TPPACData*)f3bppac->At(0))->GetX();
          f3ppacb_y   = ((art::TPPACData*)f3bppac->At(0))->GetY();
        }

        if(f3ppac->GetEntriesFast() == 1){
          target_x    = ((art::TTrack*)f3ppac->At(0))->GetX();
          target_y    = ((art::TTrack*)f3ppac->At(0))->GetY();
        }

        if(rf_cal_0->GetEntriesFast() == 1){
          rf0         = ((art::TTimingData*)rf_cal_0->At(0))->GetTiming();
        }

        if(rf_cal_1->GetEntriesFast() == 1){
          rf1         = ((art::TTimingData*)rf_cal_1->At(0))->GetTiming();
        }

        outtree->Fill();
      }
      outtree->Write();
      fout->Close();

    }

    gROOT->ProcessLine(".q");
}
