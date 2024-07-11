// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "TTGTIKProcessor2.h"

#include "ITiming.h"
#include "ICharge.h"
#include "constant.h"
#include "TDataObject.h"
#include "TTGTIKPrepData.h"
#include "TTGTIKData.h"

#include <TEnewzCalculator.h>

#include <TClonesArray.h>
#include <TClass.h>
#include <TMath.h>

using art::TTGTIKProcessor2;

ClassImp(art::TTGTIKProcessor2)

// Default constructor
TTGTIKProcessor2::TTGTIKProcessor2()
   : fInData(NULL), fOutData(NULL)
{
   RegisterInputCollection("InputCollection",
			   "TGTIK data array of objects inheriting from art::TTGTIKPrepData",
			   fInputColName,TString("prep"));
   RegisterOutputCollection("OutputCollection",
            "final result (yield) of TGTIK experiment",
			   fOutputColName,TString("result"));

   RegisterProcessorParameter("InitialBeamEnergy","beam energy just after the window",
			   fInitialBeamEnergy,0.0f);
   RegisterProcessorParameter("GasPressure","gas pressure",
			   fPressure,0.0f);
   RegisterProcessorParameter("Temperature","temperature",
			   fTemperature,0.0f);

   RegisterProcessorParameter("M1","mass of particle1",
			   fM1,0.0f);
   RegisterProcessorParameter("M2","mass of particle2",
			   fM2,0.0f);
   RegisterProcessorParameter("M3","mass of particle3",
			   fM3,0.0f);
   RegisterProcessorParameter("M4","mass of particle4",
			   fM4,0.0f);

   RegisterProcessorParameter("Z1","atomic number of particle1",
			   fZ1,0);
   RegisterProcessorParameter("Z2","atomic number of particle2",
			   fZ2,0);
   RegisterProcessorParameter("Z3","atomic number of particle3",
			   fZ3,0);
   RegisterProcessorParameter("Z4","atomic number of particle4",
			   fZ4,0);
}

TTGTIKProcessor2::~TTGTIKProcessor2()
{
   delete fOutData;
   fOutData = NULL;
}


void TTGTIKProcessor2::Init(TEventCollection *col)
{
   Info("Init","%s => %s", fInputColName.Data(), fOutputColName.Data());

   fInData = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fInputColName.Data()));
   if (!fInData) {
      SetStateError(TString::Format("input not found: %s",fInputColName.Data()));
      return;
   }

   Info("Init","[[Info]] calculation for (Z=%d + Z=%d) -> (Z=%d + Z=%d) reaction", fZ1, fZ2, fZ3, fZ4);
   Info("Init","\tBeam Energy  = %5.8f", fInitialBeamEnergy);
   Info("Init","\tGas Pressure = %5.8f", fPressure);
   Info("Init","\tTemperature  = %5.8f", fTemperature);
   Info("Init","\tM1 = %5.8f", fM1);
   Info("Init","\tM2 = %5.8f", fM2);
   Info("Init","\tM3 = %5.8f", fM3);
   Info("Init","\tM4 = %5.8f", fM4);
   Info("Init","Warning: Float_t have 6-7 significant digits (enewz need Float_t)");

   M1 = fM1 * AMU2KG;
   M2 = fM2 * AMU2KG;
   M3 = fM3 * AMU2KG;
   M4 = fM4 * AMU2KG;

   QValue = (M1 + M2 - M3 - M4) * C * C;
   Alpha = AMU2KG * (fM2*(fM1+fM2))/(2.0*fM1);
   Beta = AMU2KG * (fM4*(fM3+fM4))/(2.0*fM3);
   Info("Init","Q-Value: %5.8f", QValue*J2MEV);
   Info("Init","Alpha  : %5.8f", Alpha*KG2AMU);
   Info("Init","Beta   : %5.8f", Beta*KG2AMU);
   if(TMath::Abs(Alpha - Beta) * KG2AMU < EPS){
      std::cout << "\ttreat alpha == beta" << std::endl;
   }

   fOutData = new TClonesArray("art::TTGTIKData");
   fOutData->SetName(fOutputColName);
   col->Add(fOutputColName,fOutData,fOutputIsTransparent);

   //====================================================================
   //std::cout << std::endl;
   //std::cout << "\tkinematic calculation test" << std::endl;
   //std::cout << "\t------------------------------" << std::endl;
   //Float_t winx = 10.0;
   //Float_t winy = 10.0;
   //Float_t det_energy = 10.0;
   //Float_t detx = 10.0;
   //Float_t dety = 10.0;
   //Float_t detz = 10.0;
   //Int_t tel = 1;
   //std::cout << "\tinitial energy   : " << fInitialBeamEnergy << std::endl;
   //std::cout << "\twindow x pos     : " << winx << std::endl;
   //std::cout << "\twindow y pos     : " << winy << std::endl;
   //std::cout << "\tdetected energy  : " << det_energy << std::endl;
   //std::cout << "\tdetected x pos   : " << detx << std::endl;
   //std::cout << "\tdetected y pos   : " << dety << std::endl;
   //std::cout << "\tdetected z pos   : " << detz << std::endl;
   //std::cout << "\ttelescope        : " << tel << std::endl;
   //Float_t z = bisect(0.0f, 1000.0f, fInitialBeamEnergy, winx, winy, det_energy, detx, dety, detz, tel);
   //std::cout << "\t------------------------------" << std::endl;
   //std::cout << "\treaction position: " << z << std::endl;
   //std::cout << "\tEcm              : " << get_energy_cm(z, fInitialBeamEnergy, winx, winy)   << std::endl;
   //std::cout << "\tEcm (from det)   : " << det_to_Ecm(z, det_energy, detx, dety, detz, tel)   << std::endl;
   //std::cout << "\ttheta lab        : " << get_theta_lab(z, detx, dety, detz)                 << std::endl;
   //std::cout << "\ttheta cm         : " << get_theta_cm(z, det_energy, detx, dety, detz, tel, det_to_Ecm(z, det_energy, detx, dety, detz, tel), get_theta_lab(z, detx, dety, detz)) << std::endl;
   //std::cout << "\t------------------------------" << std::endl;
   //std::cout << "\thit position     : (0.0, 0.0, " << z << ")" << std::endl;
   //std::cout << "\tdetected position: (" << detx << ", " << dety << ", " << detz << ")" << std::endl;
   //std::cout << "\t------------------------------" << std::endl;
   //std::cout << std::endl;
   //====================================================================

}


void TTGTIKProcessor2::Process()
{
   fOutData->Clear("C");

   Int_t nData = (*fInData)->GetEntriesFast();

   if(nData != 1){ 
      std::cout << "Warning!" << std::endl;
      return;
   }

   const TDataObject *const inData = static_cast<TDataObject*>((*fInData)->At(0));
   const TTGTIKPrepData *const Data = dynamic_cast<const TTGTIKPrepData*>(inData);

   Int_t flag = Data->GetReac_flag();
   Int_t id = Data->GetID();

   Float_t det_energy = (Float_t)Data->GetEtotal();
   Float_t winx       = (Float_t)Data->GetWinx();
   Float_t winy       = (Float_t)Data->GetWiny();
   Float_t detx       = (Float_t)Data->GetDetx();
   Float_t dety       = (Float_t)Data->GetDety();
   Float_t detz       = (Float_t)Data->GetDetz();

   TTGTIKData *outData = static_cast<TTGTIKData*>(fOutData->ConstructedAt(0));

   Float_t z        = get_reac_zpos(-300.0f, 900.0f, fInitialBeamEnergy, winx, winy, det_energy, detx, dety, detz, id);
   Float_t ecm      = get_energy_cm(z, fInitialBeamEnergy, winx, winy);
   Float_t theta_l  = get_theta_lab(z, detx, dety, detz);
   Float_t theta_cm = get_theta_cm(z, det_energy, detx, dety, detz, id, ecm, theta_l);

   outData->SetID(id);
   outData->SetFlag(flag);
   outData->SetReacZ(z);
   outData->SetEcm(ecm);
   outData->SetThetaL(theta_l);
   outData->SetThetaCM(theta_cm);

}



//==============================
// Main
//==============================
// not modify
Float_t TTGTIKProcessor2::get_reac_zpos(Float_t a, Float_t b, Float_t ini_energy, Float_t winx, Float_t winy, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel)
{
   Float_t z = bisect(a, b, ini_energy, winx, winy, det_energy, detx, dety, detz, tel);
   return z;
}

Float_t TTGTIKProcessor2::get_energy_cm(Float_t z, Float_t ini_energy, Float_t winx, Float_t winy)
{
   if(!IsValid(z)) return kInvalidD;
   return beam_to_Ecm(z, ini_energy, winx, winy);
}

Float_t TTGTIKProcessor2::get_theta_lab(Float_t z, Float_t detx, Float_t dety, Float_t detz)
{
   if(!IsValid(z)) return kInvalidD;
   Float_t theta = (180.0/TMath::Pi()) * TMath::ACos( (detz-z)/(TMath::Sqrt(detx*detx + dety*dety + (detz-z)*(detz-z) )) );
   return theta;
}

Float_t TTGTIKProcessor2::get_theta_cm(Float_t z, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel, Float_t ecm, Float_t theta_lab)
{
   if(!IsValid(z)) return kInvalidD;
   Float_t ini_energy = det_energy;
   if(tel <= 3){
      Float_t tmp_energy = enewz::eoldz(fZ4, fM4, ini_energy, si, fPressure, fTemperature, firstlayer);
      ini_energy = tmp_energy;
   }

   Float_t distance = TMath::Sqrt(detx*detx + dety*dety + (detz-z)*(detz-z) );
   Float_t eRemain = MeV2J * enewz::eoldz(fZ4, fM4, ini_energy, he, fPressure, fTemperature, distance);
   Float_t v3 = TMath::Sqrt(2.0*eRemain/M4);

   Float_t ecm_j = ecm * MeV2J;
   Float_t vcm = TMath::Sqrt(ecm_j/Alpha);
   Float_t v3cm = TMath::Sqrt((ecm_j + QValue)/Beta);

   Float_t theta_cm = (180.0/TMath::Pi()) * TMath::ACos( (v3 * TMath::Cos(theta_lab * TMath::Pi()/180.0) - vcm)/v3cm );
   return 180.0 - theta_cm;
}



//==============================
// for Newton method
//==============================
Float_t TTGTIKProcessor2::beam_to_Ecm(Float_t z, Float_t ini_energy, Float_t winx, Float_t winy)
{
   Float_t win_expand = get_WinExpand(winx, winy, fPressure);
   Float_t distance, eRemain; 
   if(z>0){
      //distance = z + win_expand; 
      distance = z; 
      eRemain = enewz::enewz(fZ1, fM1, ini_energy, he, fPressure, fTemperature, distance);
   }else{
      //distance = -z + win_expand; 
      distance = -z; 
      eRemain = enewz::eoldz(fZ1, fM1, ini_energy, he, fPressure, fTemperature, distance);
   }
   return J2MEV * particle1_to_Ecm(eRemain * MeV2J);
}


Float_t TTGTIKProcessor2::det_to_Ecm(Float_t z, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel)
{
   Float_t ini_energy = det_energy;
   if(tel <= 3){
      Float_t tmp_energy = enewz::eoldz(fZ4, fM4, ini_energy, si, fPressure, fTemperature, firstlayer);
      ini_energy = tmp_energy;
   }

   Float_t distance = TMath::Sqrt(detx*detx + dety*dety + (detz-z)*(detz-z) );
   Float_t eRemain = enewz::eoldz(fZ4, fM4, ini_energy, he, fPressure, fTemperature, distance);

   Float_t theta = get_theta_lab(z, detx, dety, detz);
   return J2MEV * particle4_to_Ecm(eRemain * MeV2J, theta);
}


// not modify
Float_t TTGTIKProcessor2::ObjFunc(Float_t z, Float_t ini_energy, Float_t winx, Float_t winy, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel)
{
   //std::cout << "z : " << z << ", beam_to_Ecm: " << beam_to_Ecm(z, ini_energy, winx, winy) << " det_to_Ecm: " << det_to_Ecm(z, det_energy, detx, dety, detz, tel) << std::endl;
   return beam_to_Ecm(z, ini_energy, winx, winy) - det_to_Ecm(z, det_energy, detx, dety, detz, tel);
}


// not modify
Float_t TTGTIKProcessor2::bisect(Float_t a, Float_t b, Float_t ini_energy, Float_t winx, Float_t winy, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel)
{
   Double_t eps = 0.01;
   Int_t N = 1000;
   Int_t i = 0;

   Float_t fa = ObjFunc(a, ini_energy, winx, winy, det_energy, detx, dety, detz, tel);
   Float_t fb = ObjFunc(b, ini_energy, winx, winy, det_energy, detx, dety, detz, tel);
   if(fa * fb > 0){
      std::cout << "[Warning in bisect] f(a) * f(b) > 0, f(a): " << fa << " f(b): " << fb << std::endl;
      return kInvalidD;
   }

   Float_t left  = a;
   Float_t right = b;
   
   Float_t s = 0.0;
   while (!(TMath::Abs(right - left) < eps)){
      i++;
      s = (left+right)/2.0;
      Float_t fleft   = ObjFunc(left, ini_energy, winx, winy, det_energy, detx, dety, detz, tel);
      Float_t fmiddle = ObjFunc(s, ini_energy, winx, winy, det_energy, detx, dety, detz, tel);

      if(fleft * fmiddle < 0){ right = s; }
      else{ left = s; }

      if(i==1000){
         std::cout << "not converge!" << std::endl;
         return kInvalidD;
      }
      if(!isfinite(fmiddle)){
         std::cout << "not real number! det_energy is low? val=" << det_energy << std::endl;
         return kInvalidD;
      }
   }
   return s;
}



//==============================
// Sub function
//==============================
Float_t TTGTIKProcessor2::get_WinExpand(Float_t winx, Float_t winy, Float_t pressure)
{
   Float_t max_expand = 5.0*pressure/250.0;
   Float_t factor = 0.0025*max_expand;
   return max_expand - factor*(winx*winx + winy*winy);
}


//==============================
// kinematics function
//==============================
Float_t TTGTIKProcessor2::particle1_to_Ecm(Float_t e1)
{
   return e1 * M2/(M1+M2);
}

Float_t TTGTIKProcessor2::particle4_to_Ecm(Float_t e4, Float_t theta)
{
   Float_t vcm = particle4_to_vcm(e4, theta);
   if(!IsValid(vcm)) return kInvalidD;

   return Alpha * vcm * vcm;
}

Float_t TTGTIKProcessor2::particle4_to_vcm(Float_t e4, Float_t theta)
{
   Float_t theta_rad = theta * TMath::Pi()/180.0;

   Float_t v3 = TMath::Sqrt(2.0*e4/M4);

   if(TMath::Abs(Alpha - Beta)*KG2AMU<EPS ){
      return (Beta*v3*v3 - QValue)/(2.0*Beta*v3*TMath::Cos(theta_rad));
   }

   Float_t b = (Beta * v3 * TMath::Cos(theta_rad))/(Alpha - Beta);
   Float_t c = (QValue - Beta*v3*v3)/(Alpha - Beta);

   if((b*b - c) < 0){
      //std::cout << "b^2 - c = " << b*b - c << " < 0, det_energy : " << e4*J2MEV << ", theta : " << theta << std::endl;
      return kInvalidD;
   }

   Float_t vcm = -b + TMath::Sqrt(b*b - c);
   if(vcm < 0){
      //std::cout << "vcm < 0! : vcm = " << vcm << ", det_energy :" << e4*J2MEV << ", theta : " << theta << std::endl;
      return kInvalidD;
   }

   return vcm;
}
