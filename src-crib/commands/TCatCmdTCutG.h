/**
 * @file    TCatCmdTCutG.h
 * @brief
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2023-06-13 18:33:54
 * @note
 */

#ifndef _TCATCMDTCUTG_H_
#define _TCATCMDTCUTG_H_

#include <TCatCmd.h>

class TPad;

class TCatCmdTCutG : public TCatCmd {
  protected:
    TCatCmdTCutG();
    Double_t fX;
    Double_t fY;
    Int_t fEventtype;
    Bool_t fisFirst;

  public:
    ~TCatCmdTCutG();

    static const char *kFuncNameBase;
    static const char *kLabelNameBase;
    static TCatCmdTCutG *Instance();
    Long_t Cmd(vector<TString>);
    Long_t Run(TPad *pad, Double_t *x = NULL, Double_t *y = NULL);
    Long_t Run(Double_t *x = NULL, Double_t *y = NULL);
    void GetEvent();
    void Help();

    ClassDef(TCatCmdTCutG, 1);
};

#endif // end of #ifndef _TCATCMDTCUTG_H_