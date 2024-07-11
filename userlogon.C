{
    // load user function
    gROOT->ProcessLine(".L macro/load_tcutg.C");
    gROOT->ProcessLine(".L macro/kinematics.C");
    gROOT->ProcessLine(".L macro/dE_E_plot.C");
    gROOT->ProcessLine(".L macro/scaler.C");

    // User commands register
    // cf definition : TCatCmdFactory *cf = TCatCmdFactory::Instance();
    cf->Register(TCatCmdLoopStart::Instance());
    cf->Register(TCatCmdLoopStop::Instance());
    cf->Register(new art::TCmdXfitg);
    cf->Register(new art::TCmdXstatus);
    cf->Register(new art::TCmdXYblow);
    cf->Register(new art::TCmdXblow);
    cf->Register(TCatCmdTCutG::Instance());
    cf->Register(new art::TCmdErase);
    //cf->Register(new art::TCmdDraw);

    // User decoder register
    // df definition: art::TModuleDecoderFactory *df = art::TModuleDecoderFactory::Instance();
    gInterpreter->ProcessLine("art::TModuleDecoderFactory::Instance()->Register(new art::TModuleDecoderTimestamp);");

    // TCutG object (path to rootfile, tcutg object name)
    getAllTCutObject();

    // Http server
    // auto serv = new THttpServer("http:8080");
    // serv->SetItemField("/", "_monitoring", "100");   // monitoring interval in ms

    // TTree merge setting
    TTree::SetMaxTreeSize(1000000000000LL); // 1TB
}
