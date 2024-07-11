// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#ifndef _TTGTIKPROCESSOR2_H_
#define _TTGTIKPROCESSOR2_H_

#include <TProcessor.h>

namespace art {
   class TTGTIKProcessor2;
}

class TClonesArray;

class art::TTGTIKProcessor2 : public TProcessor {
public:
   // Default constructor
   TTGTIKProcessor2();
   virtual ~TTGTIKProcessor2();

   virtual void Init(TEventCollection *col);
   virtual void Process();

   //==============================
   // Main
   //==============================
   Float_t get_reac_zpos(Float_t a, Float_t b, Float_t ini_energy, Float_t winx, Float_t winy, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel);
   Float_t get_energy_cm(Float_t z, Float_t ini_energy, Float_t winx, Float_t winy);
   Float_t get_theta_lab(Float_t z, Float_t detx, Float_t dety, Float_t detz);
   Float_t get_theta_cm(Float_t z, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel, Float_t ecm, Float_t theta_lab);

   //==============================
   // for Newton method
   //==============================
   Float_t beam_to_Ecm(Float_t z, Float_t ini_energy, Float_t winx, Float_t winy);
   Float_t det_to_Ecm(Float_t z, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel);
   Float_t ObjFunc(Float_t z, Float_t ini_energy, Float_t winx, Float_t winy, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel);
   Float_t bisect(Float_t a, Float_t b, Float_t ini_energy, Float_t winx, Float_t winy, Float_t det_energy, Float_t detx, Float_t dety, Float_t detz, Int_t tel);

   //==============================
   // Sub function
   //==============================
   Float_t get_WinExpand(Float_t winx, Float_t winy, Float_t pressure);

   //==============================
   // kinematics function
   //==============================
   Float_t particle1_to_Ecm(Float_t e1);
   Float_t particle4_to_Ecm(Float_t e4, Float_t theta);
   Float_t particle4_to_vcm(Float_t e4, Float_t theta);



   static const Int_t kCHARNUMBER = 34;
   const Float_t EPS = 1.0e-10;
   const Float_t AMU2MEV = 931.478;
   const Float_t AMU2KG = 1.66054e-27;
   const Float_t KG2AMU = 6.0221374e+26;
   const Float_t J2MEV = 6241506363094.0;
   const Float_t MeV2J = 1.60217733e-13;
   const Float_t C = 299792458.0;

protected:
   TString        fInputColName;
   TString        fOutputColName;

   TClonesArray **fInData; //!
   TClonesArray  *fOutData; //!

   Float_t fInitialBeamEnergy;
   Float_t fPressure;
   Float_t fTemperature;

   Float_t fM1; // in amu unit
   Float_t fM2;
   Float_t fM3;
   Float_t fM4;
   Float_t M1;  // in kg unit
   Float_t M2;
   Float_t M3;
   Float_t M4;

   Float_t QValue;
   Float_t Alpha;
   Float_t Beta;

   Int_t fZ1;
   Int_t fZ2;
   Int_t fZ3;
   Int_t fZ4;

   //==========================
   // input parameter
   //==========================
   Char_t he[kCHARNUMBER] = "he";
   Char_t si[kCHARNUMBER] = "si";

   Float_t firstlayer = 0.020f;

private:
   // Copy constructor (prohibited)
   TTGTIKProcessor2(const TTGTIKProcessor2& rhs);
   // Assignment operator (prohibited)
   TTGTIKProcessor2& operator=(const TTGTIKProcessor2& rhs);

   ClassDef(TTGTIKProcessor2,1) // processor for calibration of timing and charge data
};

#endif // _TTGTIKPROCESSOR2_H_
