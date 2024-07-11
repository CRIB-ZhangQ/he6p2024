/**
 * @file    TTelescopeData.h
 * @brief
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2024-01-17 17:11:50
 * @note
 */

#ifndef _TTELESCOPEDATA_H_
#define _TTELESCOPEDATA_H_

#include "TArtTypes.h"
#include <TDataObject.h>
#include <TVector3.h>
#include <constant.h>

namespace art {
class TTelescopeData;
}

class art::TTelescopeData : public TDataObject {
  public:
    typedef enum { kID,
                   kTiming } ESortType;
    typedef enum { kASC,
                   kDESC } ESortOrder;

    TTelescopeData();
    virtual ~TTelescopeData();
    TTelescopeData(const TTelescopeData &rhs);
    TTelescopeData &operator=(const TTelescopeData &rhs);

    TVector3 GetPosition() const { return fPos; }
    Double_t X() const { return fPos.X(); }
    Double_t Y() const { return fPos.Y(); }
    Double_t Z() const { return fPos.Z(); }
    void SetPosition(TVector3 vec) { fPos = vec; }
    void SetPosition(Double_t x, Double_t y, Double_t z) { fPos.SetXYZ(x, y, z); }

    Int_t GetN() const { return fNE; }
    void SetN(Int_t arg) { fNE = arg; }

    Int_t GetXID() const { return fXID; }
    void SetXID(Int_t arg) { fXID = arg; }
    Int_t GetYID() const { return fYID; }
    void SetYID(Int_t arg) { fYID = arg; }

    Double_t GetdE() const { return fdE; }
    void SetdE(Double_t arg) { fdE = arg; }
    Double_t GetdEX() const { return fdEX; }
    void SetdEX(Double_t arg) { fdEX = arg; }
    Double_t GetdEY() const { return fdEY; }
    void SetdEY(Double_t arg) { fdEY = arg; }

    Double_t GetE() const { return fE; }
    void SetE(Double_t arg) { fE = arg; }
    Double_t GetEtotal() const { return fEtotal; }
    void SetEtotal(Double_t arg) { fEtotal = arg; }
    Double_t GetTelTiming() const { return fTiming; }
    void SetTelTiming(Double_t arg) { fTiming = arg; }
    Double_t GetTelYTiming() const { return fYTiming; }
    void SetTelYTiming(Double_t arg) { fYTiming = arg; }

    Double_t GetTheta_L() const { return fTheta_L; }
    void SetTheta_L(Double_t arg) { fTheta_L = arg; }
    Double_t A() const { return fTheta_L; }

    DoubleVec_t GetEnergyArray() const { return fEnergyArray; }
    Double_t GetEnergyArray(Int_t id) const { return fEnergyArray[id]; }
    void PushEnergyArray(Double_t arg) { fEnergyArray.emplace_back(arg); }
    DoubleVec_t GetTimingArray() const { return fTimingArray; }
    Double_t GetTimingArray(Int_t id) const { return fTimingArray[id]; }
    void PushTimingArray(Double_t arg) { fTimingArray.emplace_back(arg); }

    Double_t E(Int_t id = -1) const {
        if (id < 0)
            return fEtotal;
        return fEnergyArray[id];
    }
    Double_t T(Int_t id = -1) const {
        if (id < 0)
            return fTimingArray[0];
        return fTimingArray[id];
    }

    virtual void Copy(TObject &dest) const;
    virtual void Clear(Option_t *opt = "");

  protected:
    /// Int_t fID is used for telescipe ID
    TVector3 fPos; // detected position (X, Y, Z)

    Int_t fXID; // X strip number
    Int_t fYID; // Y strip number
    Int_t fNE;  // number of all SSDs

    Double_t fdE;      // energy at first layor
    Double_t fdEX;     // X side energy (=~ fdEY)
    Double_t fdEY;     // Y side energy (=~ fdEX)
    Double_t fE;       // added energy at thick SSDs
    Double_t fEtotal;  // all energy deposit in the telescope
    Double_t fTiming;  // timing information at the first layor (X side)
    Double_t fYTiming; // for case that X side have trouble (Y side)

    Double_t fTheta_L; // reaction angle in LAB system

    DoubleVec_t fEnergyArray; // energy array for each SSD
    DoubleVec_t fTimingArray; // timing array for each SSD

    ClassDef(TTelescopeData, 3)
};

#endif // _TTELESCOPEDATA_H_
