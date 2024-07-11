/**
 * @file    TModuleData.cc
 * @brief   inherit from TModuleInfo
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2024-01-08 17:49:53
 * @note
 */

#include "TModuleData.h"
#include <TROOT.h>

using art::TModuleData;

ClassImp(TModuleData);

TModuleData::TModuleData(const TModuleInfo &info)
    : art::TModuleInfo(info), fNCh(0), fMod(-1) {
}

TModuleData::~TModuleData() {
}

TModuleData::TModuleData(const TModuleData &rhs) {
}

TModuleData &TModuleData::operator=(const TModuleData &rhs) {
    if (this != &rhs) {
    }
    return *this;
}
