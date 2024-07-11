/**
 * @file    TTelescopeData.h
 * @brief
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2024-01-17 17:11:50
 * @note
 */

#ifndef _TREACANALYSISDATA_H_
#define _TREACANALYSISDATA_H_

#include "TArtTypes.h"
#include <TDataObject.h>
#include <TVector3.h>
#include <constant.h>

namespace art {
class TReacAnalysisData;
}

class art::TReacAnalysisData : public TDataObject {
  public:

    enum{Light_b, Heavy_B};

    TReacAnalysisData();
    virtual ~TReacAnalysisData();
    TReacAnalysisData(const TReacAnalysisData &rhs);
    TReacAnalysisData &operator=(const TReacAnalysisData &rhs);

    //beam data
    //Double_t GetMWDCa_X() const { return fdEX; }
    //void SetMWDCa_X(Double_t arg) { fdEX = arg; }
    //Double_t GetMWDCa_Y() const { return fdEX; }
    //void SetMWDCa_Y(Double_t arg) { fdEX = arg; }
    Double_t GetMWDCb_X() const { return fmwdcb_x; }
    void SetMWDCb_X(Double_t arg) { fmwdcb_x = arg; }
    Double_t GetMWDCb_Y() const { return fmwdcb_y; }
    void SetMWDCb_Y(Double_t arg) { fmwdcb_y = arg; }
    Double_t Getf3MWDC_X() const { return f3mwdc_x; }
    void Setf3MWDC_X(Double_t arg) { f3mwdc_x = arg; }
    Double_t Getf3MWDC_Y() const { return f3mwdc_y; }
    void Setf3MWDC_Y(Double_t arg) { f3mwdc_y = arg; }
    Double_t GetBeamAngle_X() const { return fBeam_angle_x; }
    void SetBeamAngle_X(Double_t arg) { fBeam_angle_x = arg; }
    Double_t GetBeamAngle_Y() const { return fBeam_angle_y; }
    void SetBeamAngle_Y(Double_t arg) { fBeam_angle_y = arg; }


    //telescope data

    Int_t GetXID() const { return fXID;}
    void SetXID(Int_t arg) { fXID= arg;}
    Int_t GetYID() const { return fYID;}
    void SetYID(Int_t arg) { fYID= arg;}

    Double_t GetdEX() const { return fdEX; }
    void SetdEX(Double_t arg) { fdEX = arg; }

    Double_t GetE() const { return fE; }
    void SetE(Double_t arg) { fE = arg; }
    Double_t GetEtotal() const { return fEtotal; }
    void SetEtotal(Double_t arg) { fEtotal = arg; }
    Double_t GetdEXTiming() const { return fdEXTiming; }
    void SetdEXTiming(Double_t arg) { fdEXTiming = arg; }
    Double_t GetEbTiming() const { return fEbTiming; }
    void SetEbTiming(Double_t arg) { fEbTiming = arg; }

    Double_t GetTheta_lab() const { return fTheta_lab; }
    void SetTheta_lab(Double_t arg) { fTheta_lab = arg; }

    Double_t GetTheta_cm() const { return fTheta_cm; }
    void SetTheta_cm(Double_t arg) { fTheta_cm = arg; }


    virtual void Copy(TObject &dest) const;
    virtual void Clear(Option_t *opt = "");

  protected:
    /// Int_t fID is used for telescipe ID

    Double_t fmwdcb_x, fmwdcb_y;
    Double_t f3mwdc_x, f3mwdc_y;
    Double_t fBeam_angle_x, fBeam_angle_y;

    Int_t fXID;
    Int_t fYID;
    Double_t fdEX;      // energy at first layor
    Double_t fE;       // added energy at thick SSDs
    Double_t fEtotal;  // all energy deposit in the telescope
    Double_t fdEXTiming;  // timing information at the first layor (X side)
    Double_t fEbTiming;  // timing information at the second layor 
    Double_t fTheta_lab; // reaction angle in LAB system
    Double_t fTheta_cm; // reaction angle in c.m. system

    ClassDef(TReacAnalysisData, 1)
};

#endif // _TREACANALYSISDATA_H_
