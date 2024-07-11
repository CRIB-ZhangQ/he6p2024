// K.Okawa created <okawa@cns.s.u-tokyo.ac.jp>

#include "tgtik/TTGTIKPrepProcessor.h"

#include "ITiming.h"
#include "ICharge.h"
#include "constant.h"
#include "TDataObject.h"
#include "telescope/TTelescopeData.h"
#include "TPPACData.h"
#include "tgtik/TTGTIKPrepData.h"

#include <TClonesArray.h>
#include <TRandom.h>
#include <TClass.h>
#include <TVector3.h>

using art::TTGTIKPrepProcessor;

ClassImp(art::TTGTIKPrepProcessor)

// Default constructor
TTGTIKPrepProcessor::TTGTIKPrepProcessor()
   : fInData_a(NULL), fInData_b(NULL), fInData_tel(NULL), fOutData(NULL)
{
   RegisterInputCollection("InputCollection_a",
			   "PPACa data array of objects inheriting from art::TPPACData",
			   fInputColName_a,TString("ppaca"));
   RegisterInputCollection("InputCollection_b",
			   "PPACb array of objects inheriting from art::TPPACData",
			   fInputColName_b,TString("ppacb"));
   RegisterInputCollection("InputCollection_tel",
			   "Telescope array of objects inheriting from art::TTelescopeData",
			   fInputColName_tel,TString("telescope"));
   RegisterOutputCollection("OutputCollection","prep data of thick gas target inverse kinematics art::TTGTIKPrepProcessor",
			    fOutputColName,TString("prep_data"));

   RegisterProcessorParameter("WindowPosition","z coodinate of the window",
			      fWindowpos,0.0);

}

TTGTIKPrepProcessor::~TTGTIKPrepProcessor()
{
   delete fOutData;
   fOutData = NULL;
}


void TTGTIKPrepProcessor::Init(TEventCollection *col)
{
   Info("Init","%s %s %s => %s", fInputColName_a.Data(), fInputColName_b.Data(), fInputColName_tel.Data(), fOutputColName.Data());

   fInData_a = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fInputColName_a.Data()));
   if (!fInData_a) {
      SetStateError(TString::Format("input not found: %s",fInputColName_a.Data()));
      return;
   }

   fInData_b = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fInputColName_b.Data()));
   if (!fInData_b) {
      SetStateError(TString::Format("input not found: %s",fInputColName_b.Data()));
      return;
   }

   fInData_tel = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fInputColName_tel.Data()));
   if (!fInData_tel) {
      SetStateError(TString::Format("input not found: %s",fInputColName_tel.Data()));
      return;
   }

   fOutData = new TClonesArray("art::TTGTIKPrepData");
   fOutData->SetName(fOutputColName);
   col->Add(fOutputColName,fOutData,fOutputIsTransparent);

   debug_flag = kTRUE;

   gRandom->SetSeed(time(NULL));
}


TVector3 TTGTIKPrepProcessor::get_DetPosition(Int_t telID, Int_t XID, Int_t YID, TVector3 win_offset, TVector3 win_dir)
{
   TVector3 vec_raw;
   if(telID == 1) vec_raw = get_rawDetPosition(XID, YID, 999.0, 244.0, -4.0, 0.0, false);
   else if(telID == 2) vec_raw = get_rawDetPosition(XID, YID, 999.0, 154.5, 27.0, 0.0, false);
   else if(telID == 3) vec_raw = get_rawDetPosition(XID, YID, 999.0, 115.0, -48.0, -0.3, false);
   else if(telID == 4) vec_raw = get_rawDetPosition(XID, YID, 949.0, 113.5, 62.0, 0.4, false);
   else if(telID == 5) vec_raw = get_rawDetPosition(XID, YID, 899.0, 112.5, -73.0, 0.0, false);

   vec_raw -= win_offset;
   TVector3 vec = vec_raw;

   TVector3 z_axis(0.0, 0.0, 1.0);
   TVector3 rotate_axis = z_axis.Cross(win_dir);
   vec.Rotate(-win_dir.Theta(), rotate_axis);

   return vec;
}

TVector3 TTGTIKPrepProcessor::get_rawDetPosition(Int_t XID, Int_t YID, Double_t orig_rotate_z, Double_t distance, Double_t angle, Double_t offset, Bool_t x_reflection)
{
   Double_t x = (-25.0 + 25.0/16.0 + (Double_t)XID * 25.0/8.0) + gRandom->Uniform(-25.0/16.0, 25.0/16.0);
   Double_t y = (-25.0 + 25.0/16.0 + (Double_t)YID * 25.0/8.0) + gRandom->Uniform(-25.0/16.0, 25.0/16.0);
   Double_t z = distance;
   if(x_reflection) x = -1.0*x;
   x += offset;

   TVector3 vec(x, y, z);
   Double_t angle_rad = angle * TMath::Pi()/180.0;
   vec.RotateY(angle_rad);

   Double_t tmp_z = vec.Z();
   vec.SetZ(tmp_z + orig_rotate_z);

   return vec;
}






void TTGTIKPrepProcessor::Process()
{
   fOutData->Clear("C");

   Int_t nData_a = (*fInData_a)->GetEntriesFast();
   Int_t nData_b = (*fInData_b)->GetEntriesFast();
   Int_t nData_tel = (*fInData_tel)->GetEntriesFast();

   if(nData_a != 1 || nData_b != 1 || nData_tel != 1){ 
      std::cout << "Warning!" << std::endl;
      return;
   }

   const TDataObject *const inData_a = static_cast<TDataObject*>((*fInData_a)->At(0));
   const TPPACData *const Data_a = dynamic_cast<const TPPACData*>(inData_a);

   const TDataObject *const inData_b = static_cast<TDataObject*>((*fInData_b)->At(0));
   const TPPACData *const Data_b = dynamic_cast<const TPPACData*>(inData_b);

   Double_t dX        = Data_b->GetX() - Data_a->GetX();
   Double_t dY        = Data_b->GetY() - Data_a->GetY();
   Double_t dZ        = Data_b->GetZ() - Data_a->GetZ();
   Double_t to_window = fWindowpos - Data_a->GetZ();

   Double_t winX = Data_a->GetX() + dX*to_window/dZ;
   Double_t winY = Data_a->GetY() + dY*to_window/dZ;
   Double_t winZ = fWindowpos;
   TVector3 win_offset(winX, winY, winZ);
   TVector3 win_dir(dX/dZ, dY/dZ, 1.0);

   TTGTIKPrepData *outData = static_cast<TTGTIKPrepData*>(fOutData->ConstructedAt(0));
   outData->SetWinx(winX);
   outData->SetWiny(winY);
   outData->SetWinz(winZ);

   const TDataObject *const inData_tel = static_cast<TDataObject*>((*fInData_tel)->At(0));
   const TTelescopeData *const Data_tel = dynamic_cast<const TTelescopeData*>(inData_tel);
   outData->SetEtotal(Data_tel->GetEtotal());

   const Int_t telID = Data_tel->GetID();
   if(telID < 1 || 5 < telID){
      std::cout << "Warning!" << std::endl;
      return;
   }
   outData->SetID(telID);

   Int_t XID = Data_tel->GetXID();
   Int_t YID = Data_tel->GetYID();
   if(XID < 0) XID = gRandom->Integer(16);
   if(YID < 0) YID = gRandom->Integer(16);

   TVector3 det_vec = get_DetPosition(telID, XID, YID, win_offset, win_dir);
   outData->SetDetx(det_vec.X());
   outData->SetDety(det_vec.Y());
   outData->SetDetz(det_vec.Z());

}
