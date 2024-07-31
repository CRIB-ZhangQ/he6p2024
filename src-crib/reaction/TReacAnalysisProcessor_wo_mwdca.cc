/**
 * @file    TTelescopeProcessor.cc
 * @brief   gather the information to the one object
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2024-01-17 16:51:53
 * @note    treat the largest value of each layor
 *          the data of X side is used for DSSSD
 *          assume beam position (0, 0) and direction (0, 0, 1)
 */

#include "TReacAnalysisProcessor_wo_mwdca.h"
#include "TReacAnalysisData.h"

#include "TDataObject.h"
#include "../telescope/TTelescopeData.h"
#include "TMWDCTrackingResult.h"
#include "constant.h"

#include <TClass.h>
#include <TClonesArray.h>
#include <TMath.h>
#include <TRandom.h>

//#include "../geo/TDetectorParameter.h"
//#include "../geo/TTargetParameter.h"
//#include <TRandom.h>

using art::TReacAnalysisProcessor_wo_mwdca;

ClassImp(art::TReacAnalysisProcessor_wo_mwdca);

// Default constructor
TReacAnalysisProcessor_wo_mwdca::TReacAnalysisProcessor_wo_mwdca()
    : fInData1(NULL), fInData2(NULL), fOutData(NULL) {
    RegisterInputCollection("InputCollection1",
                            "array of objects inheriting from art::TMWDCTrackingResult",
                            fInputColName1, TString("fmwdcb"));
    RegisterInputCollection("InputCollection2",
                            "array of objects inheriting from art::TTelescopeData",
                            fInputColName2, TString("ftel"));
    RegisterOutputCollection("OutputCollection", "output class will be art::TReacAnalysisData",
                             fOutputColName, TString("fPRA"));
    StringVec_t init_s_vec;
    IntVec_t init_i_vec;
    DoubleVec_t init_d_vec;

    RegisterProcessorParameter("Ebeam", "beam energy of 6He", fEbeam, 44.0);
    RegisterProcessorParameter("Mass", "mass number of the A(a,b)B reaction", fMass, init_d_vec);
    RegisterProcessorParameter("Charge", "charge number of the A(a,b)B reaction", fCharge, init_i_vec);
    RegisterProcessorParameter("Qvalue", "reaction energy of the A(a,b)B reaction", fQvalue, 0.0);
    RegisterProcessorParameter("Exci", "the excited state of compound nucleus:a+A", fExci, -1.8);
    RegisterProcessorParameter("Particle", "the emitted particle of A(a,b)B reaction: b or B", fParticle, 0);//0: light particle, 1: heavy particle
    RegisterProcessorParameter("TargetX_mu", "Fitting Parameters of targetX vs mwdcbX", fTargetX_mu, init_d_vec);
    RegisterProcessorParameter("TargetX_sigma", "Uncerntainty of the fitting parameters of sigma of targetX", fTargetX_sigma, init_d_vec);
    RegisterProcessorParameter("TargetY_mu", "Fitting Parameters of targetY vs mwdcbY", fTargetY_mu, init_d_vec);
    RegisterProcessorParameter("TargetY_sigma", "Uncerntainty of the fitting parameters of sigma of targetY", fTargetY_sigma, init_d_vec);
    //RegisterProcessorParameter("Intercept_x", "Linear function: the intercept of mwdcb_x and targe_x", fIntercept_x, 1.22);
    //RegisterProcessorParameter("Slope_x", "Linear function: the Slope of mwdcb_x and target_x", fSlope_x, -0.99);
    //RegisterProcessorParameter("Intercept_y", "Linear function: the intercept of mwdcb_x and targe_x", fIntercept_y, 1.22);
    //RegisterProcessorParameter("Slope_y", "Linear function: the Slope of mwdcb_x and target_x", fSlope_y, -0.99);
}

TReacAnalysisProcessor_wo_mwdca::~TReacAnalysisProcessor_wo_mwdca() {
    delete fOutData;
    fOutData = NULL;
}

void TReacAnalysisProcessor_wo_mwdca::Init(TEventCollection *col) {
    Info("Init", "%s %s => %s", fInputColName1.Data(), fInputColName2.Data(), fOutputColName.Data());


    // check if mwdcb object exist or not
    fInData1 = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName1.Data()));
    if (!fInData1) {
        SetStateError(TString::Format("input not found: %s", fInputColName1.Data()));
        return;
    }

    // check if telescope object exist or not
    fInData2 = reinterpret_cast<TClonesArray **>(col->GetObjectRef(fInputColName2.Data()));
    if (!fInData2) {
        SetStateError(TString::Format("input not found: %s", fInputColName2.Data()));
        return;
    }

    // check if input collection is valid or not
    //const TClass *const cl1 = (*fInData1)->GetClass();
    const TClass *const cl1 = (*fInData1)->GetClass();
    const TClass *const cl2 = (*fInData2)->GetClass();
    fInputHasData = (cl1->InheritsFrom(art::TMWDCTrackingResult::Class())) && (cl2->InheritsFrom(art::TTelescopeData::Class()));

    if (!fInputHasData) {
        SetStateError("contents of input array must inherit from art::TTimingChargeData");
        return;
    }

    fOutData = new TClonesArray("art::TReacAnalysisData");
    fOutData->SetName(fOutputColName);
    col->Add(fOutputColName, fOutData, fOutputIsTransparent);
    gRandom->SetSeed(time(nullptr));
}

void TReacAnalysisProcessor_wo_mwdca::Process() {
    fOutData->Clear("C");

    Int_t nData1 = (*fInData1)->GetEntriesFast();
    Int_t nData2 = (*fInData2)->GetEntriesFast();

    // if no hit, do nothing
    if (nData1 == 0 || nData2 == 0) {
        return;
    }


    TReacAnalysisData *outData = static_cast<TReacAnalysisData *>(fOutData->ConstructedAt(0));
    outData->Clear();

    //outData->SetID(fTelID);
    //if (fHasDetPrm && fHasTargetPrm) {
    //    outData->SetN(fDetParameter->GetN());
    //} else {
    //    outData->SetN(DEFAULT_SSD_MAX_NUMBER);
    //}

    const TDataObject *const inData1 = static_cast<TDataObject *>((*fInData1)->At(0));
    const TMWDCTrackingResult *const fmwdcb = dynamic_cast<const TMWDCTrackingResult *>(inData1);
    const TDataObject *const inData2 = static_cast<TDataObject *>((*fInData2)->At(0));
    const TTelescopeData *const ftel = dynamic_cast<const TTelescopeData *>(inData2);


    // mwdcb process
    Double_t dcb_x = fmwdcb->GetX();
    Double_t dcb_y = fmwdcb->GetY();
    Double_t dcb_z = -171.5;


    // f3mwdc: CH2 target
    //Double_t tar_x = dcb_x * fSlope_x + fIntercept_x;
    //Double_t tar_y = dcb_y * fSlope_y + fIntercept_y;

    Double_t tar_x = fTargetX_mu[0] + fTargetX_mu[1] * dcb_x + fTargetX_mu[2] * pow(dcb_x,2)
                      + fTargetX_mu[3] * pow(dcb_x,3);
    Double_t sigma_x = fTargetX_sigma[0] + fTargetX_sigma[1] * dcb_x
                      + fTargetX_sigma[2] * pow(dcb_x,2) + fTargetX_sigma[3] * pow(dcb_x,3)
                      + fTargetX_sigma[4] * pow(dcb_x,4) + fTargetX_sigma[5] * pow(dcb_x,5);
    tar_x = gRandom->Gaus(tar_x, sigma_x);  
    //Double_t check_x = gRandom->Gaus(tar_x, sigma_x);  

    Double_t tar_y = fTargetY_mu[0] + fTargetY_mu[1] * dcb_y + fTargetY_mu[2] * pow(dcb_y,2)
                      + fTargetY_mu[3] * pow(dcb_y,3);
    Double_t sigma_y = fTargetY_sigma[0] + fTargetY_sigma[1] * dcb_y
                      + fTargetY_sigma[2] * pow(dcb_y,2) + fTargetY_sigma[3] * pow(dcb_y,3)
                      + fTargetY_sigma[4] * pow(dcb_y,4) + fTargetY_sigma[5] * pow(dcb_y,5)
                      + fTargetY_sigma[6] * pow(dcb_y,6) + fTargetY_sigma[7] * pow(dcb_y,7);
    tar_y = gRandom->Gaus(tar_y, sigma_y);  

    //Double_t check_y = gRandom->Gaus(tar_y, sigma_y);  
    //std::cout<<"sigma_x = "<<sigma_x<<", tar_x"<<tar_x<<", check_x = "<<check_x<<", sigma_y = "<<sigma_y<<", check_y = "<<check_y<<", tar_y = "<<tar_y<<std::endl;

    Double_t tar_z = 0.0;
    Double_t beam_fA = TMath::ATan((tar_x-dcb_x)/(tar_z-dcb_z));
    Double_t beam_fB = TMath::ATan((tar_y-dcb_y)/(tar_z-dcb_z));
    TVector3 tar_p(tar_x, tar_y, tar_z);

    Double_t beam_fX = TMath::Tan(beam_fA);
    Double_t beam_fY = TMath::Tan(beam_fB);
    Double_t beam_fZ = 1.0;
    Double_t norm = TMath::Sqrt(beam_fX*beam_fX + beam_fY*beam_fY + 1.0);
    Double_t beam_dir_x = beam_fX/norm;
    Double_t beam_dir_y = beam_fY/norm;
    Double_t beam_dir_z = beam_fZ/norm;



    //telescope
    Int_t telXID = ftel->GetXID();
    Int_t telYID = ftel->GetYID();
    Double_t teldEX = ftel->GetdE();
    Double_t telE_re = ftel->GetE();
    Double_t telEtotal = ftel->GetEtotal();
    Double_t teldEXTiming = ftel->GetTelTiming();
    Double_t telEbTiming = ftel->GetTimingArray(1);
    Double_t px = ftel->X(); 
    Double_t py = ftel->Y(); 
    Double_t pz = ftel->Z(); 
    TVector3 det_p(px, py, pz);

    //check the timing valid
    if(!IsValid(teldEXTiming))return;

    //the direction of 6He beam
    //emit_dir
    TVector3 emit_dir;
    TVector3 beam_dir(beam_dir_x, beam_dir_y, beam_dir_z);
    Double_t angle_lab, angle_cm;

    //from mwdca and mwdcb
    //beam_dir = dcb_p - dca_p;
    emit_dir = det_p - tar_p; 
    angle_lab = beam_dir.Angle(emit_dir) * TMath::RadToDeg();
    angle_cm = 180.0 - (angle_lab + asin(fgamma()*sin(angle_lab*TMath::DegToRad()))*TMath::RadToDeg());
    //std::cout<<"theta_lab1 = "<<angle_lab<<", theta_test = "<<angle_lab_test<<std::endl;


    outData->SetMWDCb_X(dcb_x);
    outData->SetMWDCb_Y(dcb_y);
    outData->Setf3MWDC_X(tar_x);
    outData->Setf3MWDC_Y(tar_y);
    outData->SetBeamAngle_X(beam_fA*TMath::RadToDeg());
    outData->SetBeamAngle_Y(beam_fB*TMath::RadToDeg());
    outData->SetXID(telXID);
    outData->SetYID(telYID);
    outData->SetdEX(teldEX);
    outData->SetdEXTiming(teldEXTiming);
    outData->SetE(telE_re);
    outData->SetEbTiming(telEbTiming);
    outData->SetEtotal(telEtotal);
    outData->SetTheta_lab(angle_lab);
    outData->SetTheta_cm(angle_cm);
}


Double_t TReacAnalysisProcessor_wo_mwdca::fgamma() {
  Double_t mass_a = fMass[1], mass_A = fMass[0] , mass_b = fMass[2] , mass_B = fMass[3];
  Double_t Ecm = fEbeam * mass_A / (mass_a + mass_A);
  Double_t gamma;

  if(fParticle == Light_b)
    gamma = mass_a*mass_b*(mass_b+mass_B)*Ecm / (mass_A*mass_B*(mass_a+mass_A)*(Ecm+fQvalue));
  else
    gamma = mass_a*mass_B*(mass_B+mass_b)*Ecm / (mass_A*mass_b*(mass_a+mass_A)*(Ecm+fQvalue));
  return TMath::Sqrt(gamma);
}
