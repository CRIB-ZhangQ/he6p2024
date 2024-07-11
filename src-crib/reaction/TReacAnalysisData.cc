/**
 * @file    TReacAnalysisData.cc
 * @brief
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2024-01-17 17:14:39
 * @note
 */

#include "TReacAnalysisData.h"
#include <TConverterBase.h>

using art::TReacAnalysisData;

ClassImp(art::TReacAnalysisData);

TReacAnalysisData::TReacAnalysisData(): fmwdcb_x(-500.0), fmwdcb_y(-500.0), 
                                        f3mwdc_x(-500.0), f3mwdc_y(-500.0),
                                        fBeam_angle_x(-500.0), fBeam_angle_y(-500.0),
                                        fXID(-500), fYID(-500),
                                        fdEX(0.0), fE(0.0), fEtotal(0.0),
                                        fdEXTiming(kInvalidD), fEbTiming(kInvalidD),
                                        fTheta_lab(kInvalidD), fTheta_cm(kInvalidD)
{
    TDataObject::SetID(kInvalidI);
}

TReacAnalysisData::~TReacAnalysisData() {
}

TReacAnalysisData::TReacAnalysisData(const TReacAnalysisData &rhs)
    : TDataObject(rhs),
      fmwdcb_x(rhs.fmwdcb_x),fmwdcb_y(rhs.fmwdcb_y),
      f3mwdc_x(rhs.f3mwdc_x),f3mwdc_y(rhs.f3mwdc_y),
      fBeam_angle_x(rhs.fBeam_angle_x),fBeam_angle_y(rhs.fBeam_angle_y),
      fXID(rhs.fXID), fYID(rhs.fYID),
      fdEX(rhs.fdEX),
      fE(rhs.fE),
      fEtotal(rhs.fEtotal),
      fdEXTiming(rhs.fdEXTiming),
      fEbTiming(rhs.fEbTiming),
      fTheta_lab(rhs.fTheta_lab),
      fTheta_cm(rhs.fTheta_cm)
{

}

TReacAnalysisData &TReacAnalysisData::operator=(const TReacAnalysisData &rhs) {
    if (this != &rhs) {
        ((TReacAnalysisData &)rhs).Copy(*this);
    }
    return *this;
}

void TReacAnalysisData::Copy(TObject &dest) const {
    TDataObject::Copy(dest);
    TReacAnalysisData &cobj = *(TReacAnalysisData *)&dest;

    cobj.fmwdcb_x = this->GetMWDCb_X();
    cobj.fmwdcb_y = this->GetMWDCb_Y();
    cobj.f3mwdc_x = this->Getf3MWDC_X();
    cobj.f3mwdc_y = this->Getf3MWDC_Y();
    cobj.fBeam_angle_x = this->GetBeamAngle_X();
    cobj.fBeam_angle_y = this->GetBeamAngle_Y();


    cobj.fXID = this->GetXID();
    cobj.fYID = this->GetYID();

    cobj.fdEX = this->GetdEX();
    cobj.fdEXTiming = this->GetdEXTiming();

    cobj.fE = this->GetE();
    cobj.fEbTiming = this->GetEbTiming();

    cobj.fEtotal = this->GetEtotal();

    cobj.fTheta_lab = this->GetTheta_lab();
    cobj.fTheta_cm = this->GetTheta_cm();

}

void TReacAnalysisData::Clear(Option_t *opt) {
    TDataObject::Clear(opt);
    TDataObject::SetID(kInvalidI);

    fmwdcb_x = -500.0;
    fmwdcb_y = -500.0;
    f3mwdc_x = -500.0;
    f3mwdc_y = -500.0;
    fBeam_angle_x = -500.0;
    fBeam_angle_y = -500.0;


    fXID = -500;
    fYID = -500;

    fdEX = 0.0;
    fdEXTiming = kInvalidD;

    fE= 0.0;
    fEbTiming = kInvalidD;

    fEtotal = 0.0;

    fTheta_lab = kInvalidD;
    fTheta_cm = kInvalidD;
}
