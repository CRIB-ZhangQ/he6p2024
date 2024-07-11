/**
 * @file    TTelescopeProcessor.cc
 * @brief   gather the information to the one object
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2024-01-17 16:51:53
 * @note    treat the largest value of each layor
 *          the data of X side is used for DSSSD
 *          assume beam position (0, 0) and direction (0, 0, 1)
 */

#include "TTelescopeProcessor.h"

#include "ICharge.h"
#include "ITiming.h"
#include "TDataObject.h"
#include "TTimingChargeData.h"
#include "TTelescopeData.h"
#include "TTelescopeParameter.h"
#include "constant.h"

#include <TClass.h>
#include <TClonesArray.h>
#include <TRandom.h>

using art::TTelescopeProcessor;

ClassImp(art::TTelescopeProcessor);

// Default constructor
TTelescopeProcessor::TTelescopeProcessor()
    //: fInData1(NULL), fInData2(NULL), fInData3(NULL), fOutData(NULL), fTelID(0) {
    : fInData1(NULL){
    RegisterInputCollection("InputCollection1",
                            "array of objects inheriting from art::TTimingChargeData",
                            fInputColName1, TString("dEX"));
    RegisterInputCollection("InputCollection2",
                            "array of objects inheriting from art::TTimingChargeData",
                            fInputColName2, TString("dEY"));
    RegisterInputCollection("InputCollection3",
                            "array of objects inheriting from art::TTimingChargeData",
                            fInputColName3, TString("E"));
    RegisterOutputCollection("OutputCollection", "output class will be art::TTelescopeData",
                             fOutputColName, TString("telescope"));

    RegisterOptionalInputInfo("TelescopeParameter", "name of telescope parameter", fParameterName,
                              TString("prm: telescope"), &fParameterSet, "TClonesArray", "art::TTelescopeParameter");
}

TTelescopeProcessor::~TTelescopeProcessor() {
    delete fOutData;
    fOutData = NULL;
}

void TTelescopeProcessor::Init(TEventCollection *col) {
    Info("Init", "%s %s %s => %s", fInputColName1.Data(), fInputColName2.Data(), fInputColName3.Data(), fOutputColName.Data());

    fInData1 = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName1.Data()));
    if (!fInData1) {
        SetStateError(TString::Format("input not found: %s", fInputColName1.Data()));
        return;
    }

    fInData2 = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName2.Data()));
    if (!fInData2) {
        SetStateError(TString::Format("input not found: %s", fInputColName2.Data()));
        return;
    }

    fInData3 = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName3.Data()));
    if (!fInData3) {
        SetStateError(TString::Format("input not found: %s", fInputColName3.Data()));
        return;
    }

    TTelescopeParameter *prm = dynamic_cast<TTelescopeParameter *>((*fParameterSet)->FindObject(fParameterName));

    if(!prm){
      SetStateError(TString::Format("No such parameter '%s' is found", fParameterName.Data()));
      return;
    }

    fTelescopeParameter = prm;
    // check if input collection is valid or not
    const TClass *const cl1 = (*fInData1)->GetClass();
    const TClass *const cl2 = (*fInData2)->GetClass();
    const TClass *const cl3 = (*fInData3)->GetClass();
    fInputHasData = (cl1->InheritsFrom(art::TTimingChargeData::Class())) && (cl2->InheritsFrom(art::TTimingChargeData::Class())) && (cl3->InheritsFrom(art::TTimingChargeData::Class()));

    if (!fInputHasData) {
        SetStateError("contents of input array must inherit from art::TTimingChargeData");
        return;
    }

    fOutData = new TClonesArray("art::TTelescopeData");
    fOutData->SetName(fOutputColName);
    col->Add(fOutputColName, fOutData, fOutputIsTransparent);

    debug_flag = kTRUE;

}

void TTelescopeProcessor::Process() {
    fOutData->Clear("C");

    Int_t nData1 = (*fInData1)->GetEntriesFast();
    Int_t nData2 = (*fInData2)->GetEntriesFast();
    Int_t nData3 = (*fInData3)->GetEntriesFast();

    //// if no hit, do nothing
    if (nData1 == 0 && nData2 == 0 && nData3 == 0) { return; }

    TTelescopeParameter *prm = (TTelescopeParameter*) fTelescopeParameter;
    Int_t numE = prm->GetEdetNum();
    if(nData3>numE){
        SetStateError(TString::Format("entry of E detector &d is over %d", nData3, numE));
        return;
    }


    Int_t itr = 0;
    TTelescopeData *outData = static_cast<TTelescopeData *>(fOutData->ConstructedAt(itr));
    itr++;
    outData->Init(numE);
    outData->SetID(prm->GetTelescopeID());


    Double_t dE = 0.0;
    Double_t dEX = 0.0;
    Double_t dEY = 0.0;
    Double_t E = 0.0;
    Double_t Etotal = 0.0;

    //// dEX process
    Int_t dEXID = -1;
    Double_t dEX_tmp = -500.0;
    for (Int_t iData1 = 0; iData1 < nData1; ++iData1) {
        const TDataObject *const inData1 = static_cast<TDataObject *>((*fInData1)->At(iData1));
        const TTimingChargeData *const Data1 = dynamic_cast<const TTimingChargeData *>(inData1);
        Double_t energy = Data1->GetCharge();
        if (dEX_tmp < energy) { 
            dEXID = iData1;
            dEX_tmp = Data1->GetCharge();
        }
    }

    //    fill the highest channel
    if (nData1 != 0 && dEXID != -1) {
        const TDataObject *const inData1 = static_cast<TDataObject *>((*fInData1)->At(dEXID));
        const TTimingChargeData *const Data1 = dynamic_cast<const TTimingChargeData *>(inData1);
        Double_t energy = Data1->GetCharge();

        if(energy > prm->GetdEXEnergyThreshold()){
          dEXID = Data1->GetDetID();
          dEX = Data1->GetCharge();

          outData->SetXID(dEXID);
          outData->SetdEXTiming(Data1->GetTiming());
          outData->SetdEX(energy);
        }
    }

    // dEY process
    Int_t dEYID = -1;
    Double_t dEY_tmp = -500.0;
    for (Int_t iData2 = 0; iData2 < nData2; ++iData2) {
        const TDataObject *const inData2 = static_cast<TDataObject *>((*fInData2)->At(iData2));
        const TTimingChargeData *const Data2 = dynamic_cast<const TTimingChargeData *>(inData2);
        Double_t energy = Data2->GetCharge();
        if (dEY_tmp < energy) { 
            dEYID = iData2;
        }
    }

    //    fill the highest channel
    if (nData1 != 0 && dEYID != -1) {
        const TDataObject *const inData2 = static_cast<TDataObject *>((*fInData2)->At(dEYID));
        const TTimingChargeData *const Data2 = dynamic_cast<const TTimingChargeData *>(inData2);
        Double_t energy = Data2->GetCharge();

        if(energy > prm->GetdEYEnergyThreshold()){
          dEYID = Data2->GetDetID();
          dEY = Data2->GetCharge();

          outData->SetYID(dEYID);
          outData->SetdEYTiming(Data2->GetTiming());
          outData->SetdEY(dEY);
        }
    }

    if(prm->GetdEIsDSSD()){
      dE = dEX;
      Etotal += dE;
    }



    // Thick SSDs process
    Double_t EnergyThreshold;
    Double_t energy;
    for(Int_t iData3 = 0; iData3 < nData3; ++iData3){
       const TDataObject *const inData3 = static_cast<TDataObject *>((*fInData3)->At(iData3));
       const TTimingChargeData *const Data3 = dynamic_cast<const TTimingChargeData *>(inData3);
       energy = Data3->GetCharge();

       for(Int_t inE = 0; inE < numE; ++inE){
         EnergyThreshold = 0.0;
         if(Data3->GetDetID() == inE){
           switch(inE){
             case 0:
               EnergyThreshold = prm->GetEnergyThreshold_b();
               break;
             case 1:
               EnergyThreshold = prm->GetEnergyThreshold_c();
               break;
             case 2:
               EnergyThreshold = prm->GetEnergyThreshold_d();
               break;
             default: std::cout<<"There is no more silicon detectors"<<std::endl;
           }
           if(energy >EnergyThreshold){
             outData->SetEvec(inE, energy);
             outData->SetETimingvec(inE, Data3->GetTiming());
             E += energy;
             //energy = 0.0;
           }

         }
       }
    }

    Etotal += E;

    outData->SetdE(dE);
    outData->SetE(E);
    outData->SetEtotal(Etotal);
}
