/**
 * @file    TCRIBPIDProcessor.cc
 * @brief   Beam PID at F2 and F3 of CRIB
 * @author  Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
 * @date    2023-12-19 15:30:55
 * @note
 */

#include "TCRIBPIDProcessor.h"

#include <TAxis.h>
#include <TDataObject.h>
#include <TGraph.h>
#include <TH1.h>
#include <TMultiGraph.h>
#include <TText.h>
#include <constant.h>

#include <yaml-cpp/yaml.h>

#include <TCanvas.h>
#include <TClass.h>
#include <TStyle.h>
#include <TSystem.h>

using art::TCRIBPIDProcessor;

ClassImp(TCRIBPIDProcessor);

// definition of constant strings for the key of each node
namespace {
const char *kNodeKeyIons = "input_ions";
const char *kNodeKeyName = "name";
const char *kNodeKeyCharge = "charge";
const char *kNodeKeyMass = "mass";
const char *kNodeKeyColor = "color";
const char *kNodeKeyF1Param = "f1_parameters";
const char *kNodeKeyF2Param = "f2_parameters";
const char *kNodeKeyF3Param = "f3_parameters";
const char *kNodeKeyBrho = "brho";
const char *kNodeKeyRfPeriod = "rf_period";
const char *kNodeKeyPPACThickness = "PPAC_thickness";
const char *kNodeKeySSDThickness = "SSD_thickness";
const char *kNodeKeyAThickness = "a_thickness";
const char *kNodeKeyBThickness = "b_thickness";
const char *kNodeKeyDistance = "distance";
const char *kNodeKeyTrigger = "trigger";
const char *kNodeKeyF2Display = "f2_display";
const char *kNodeKeyF3Display = "f3_display";
const char *kNodeKeyRfOffset = "rf_offset";
const char *kNodeKeyTofOffset = "tof_offset";
const char *kNodeKeyRfRange = "rf_range";
const char *kNodeKeyTofRange = "tof_range";
const char *kNodeKeyERange = "energy_range";
} // namespace

TCRIBPIDProcessor::TCRIBPIDProcessor() {
    RegisterProcessorParameter("FileName", "parameter file of PID", fFileName, TString(""));
    RegisterProcessorParameter("Batch", "Batch mode", fIsBatch, kFALSE);
}

TCRIBPIDProcessor::~TCRIBPIDProcessor() {
    delete fElossTable;
    fElossTable = NULL;
}

void TCRIBPIDProcessor::Init(TEventCollection *col) {
    Info("Init", "PID parameters are loaded from %s", fFileName.Data());
    LoadPIDyaml();
    SetSRIMObject();

    gStyle->SetMarkerSize(1.5);
    gStyle->SetTextSize(0.03);
    if (fIsBatch) {
        gROOT->SetBatch(kTRUE);
    }

    std::vector<TGraph *> graphs_f2_pid;
    std::vector<TGraph *> graphs_f3_pid;

    for (Int_t i = 0; i < ion_names.size(); i++) {
        // calculate the kinetic energy from Brho
        Double_t Brhoqc = f1_brho * ion_charges[i] * kUnitCharge * kLightSpeed * kJ2MeV;
        Double_t Ekin = ion_masses[i] * (TMath::Sqrt(1.0 + TMath::Power(Brhoqc / ion_masses[i], 2.0)) - 1.0);

        // calculate energy loss
        Double_t E_after_f2ppac = fEloss_vvec[i][0]->GetNewE(Ekin, f2_ppac_thickness);
        Double_t E_in_f2ssd = E_after_f2ppac - fEloss_vvec[i][1]->GetNewE(E_after_f2ppac, f2_ssd_thickness);

        Double_t E_after_ppaca = fEloss_vvec[i][0]->GetNewE(Ekin, a_thickness);
        Double_t E_after_ppacb = fEloss_vvec[i][0]->GetNewE(E_after_ppaca, b_thickness);

        // calculate tof (ns)
        Double_t tof_f0f2 = 1.0e+9 * kLengthF0F2 / E2m_s(Ekin, ion_masses[i]);
        Double_t tof_f0f3 = 1.0e+9 * kLengthF0F3 / E2m_s(Ekin, ion_masses[i]);
        Double_t tof_ppacs = 1.0e+9 * distance * 1.0e-3 / E2m_s(E_after_ppaca, ion_masses[i]);

        // calculate data points
        // for F2
        TGraph *gr_f2 = new TGraph();
        Int_t itr_f2 = 0;
        Double_t rf_f2 = -tof_f0f2 + f2_rf_offset; // !! this is rf so the sign is minus!
        if (rf_f2 < 0.0) {
            while (rf_f2 < 0.0) {
                rf_f2 += rf_period;
            }
        } else {
            while (rf_f2 > 0.0) {
                rf_f2 -= rf_period;
            }
            rf_f2 += rf_period;
        }
        while (rf_f2 < f2_rf_range[1] - f2_rf_range[0]) {
            gr_f2->SetPoint(itr_f2, rf_f2 - f2_rf_range[0], E_in_f2ssd);
            itr_f2++;
            rf_f2 += rf_period;
        }
        // set TGraph style
        gr_f2->SetName(Form("%s+", ion_names[i].Data()));
        gr_f2->SetTitle(Form("%s+", ion_names[i].Data()));
        gr_f2->SetMarkerStyle(20 + i);
        if (ion_colors[i] == 0) {
            gr_f2->SetMarkerColor(kRed);
        } else if (ion_colors[i] == 1) {
            gr_f2->SetMarkerColor(kBlue);
        } else {
            gr_f2->SetMarkerColor(kBlack);
        }
        graphs_f2_pid.emplace_back(gr_f2);

        // for F3
        TGraph *gr_f3 = new TGraph();
        Int_t itr_f3 = 0;
        Double_t rf_f3 = -tof_f0f3 + f3_rf_offset; // !! this is rf so the sign is minus!
        if (trigger == 1) {
            rf_f3 -= tof_ppacs;
        }
        if (rf_f3 < 0.0) {
            while (rf_f3 < 0.0) {
                rf_f3 += rf_period;
            }
        } else {
            while (rf_f3 > 0.0) {
                rf_f3 -= rf_period;
            }
            rf_f3 += rf_period;
        }
        while (rf_f3 < f3_rf_range[1] - f3_rf_range[0]) {
            gr_f3->SetPoint(itr_f3, rf_f3 - f3_rf_range[0], tof_ppacs + tof_offset);
            itr_f3++;
            rf_f3 += rf_period;
        }
        // set TGraph style
        gr_f3->SetName(Form("%s+", ion_names[i].Data()));
        gr_f3->SetTitle(Form("%s+", ion_names[i].Data()));
        gr_f3->SetMarkerStyle(20 + i);
        if (ion_colors[i] == 0) {
            gr_f3->SetMarkerColor(kRed);
        } else if (ion_colors[i] == 1) {
            gr_f3->SetMarkerColor(kBlue);
        } else {
            gr_f3->SetMarkerColor(kBlack);
        }
        graphs_f3_pid.emplace_back(gr_f3);
    }

    // Save Object
    TCanvas *c_f2 = new TCanvas("F2_canvas", "F2_canvas", 800, 800);
    c_f2->cd();
    TMultiGraph *mg_f2 = new TMultiGraph();
    mg_f2->SetTitle("F2_PID; F2 RF [ns]; F2 SSD [MeV]");
    mg_f2->GetHistogram()->GetXaxis()->SetLimits(f2_rf_range[0], f2_rf_range[1]);
    mg_f2->GetHistogram()->GetYaxis()->SetRangeUser(energy_range[0], energy_range[1]);
    mg_f2->GetHistogram()->GetYaxis()->SetTitleOffset(1.3);
    mg_f2->GetHistogram()->GetXaxis()->SetTitleOffset(1.3);

    std::vector<TText *> txts_f2;
    for (Int_t i = 0; i < graphs_f2_pid.size(); i++) {
        mg_f2->Add(graphs_f2_pid[i]);
        for (Int_t j = 0; j < graphs_f2_pid[i]->GetN(); j++) {
            Double_t x, y;
            graphs_f2_pid[i]->GetPoint(j, x, y);
            TText *txt = new TText(x + 3.0, y, Form("%s+", ion_names[i].Data()));
            txt->SetTextAlign(12);
            if (ion_colors[i] == 0) {
                txt->SetTextColor(kRed);
            } else if (ion_colors[i] == 1) {
                txt->SetTextColor(kBlue);
            } else {
                txt->SetTextColor(kBlack);
            }
            txts_f2.emplace_back(txt);
        }
    }
    mg_f2->Draw("ap");
    for (Int_t i = 0; i < txts_f2.size(); i++) {
        txts_f2[i]->Draw("same");
    }
    gPad->SetGrid();
    c_f2->Update();

    TCanvas *c_f3 = new TCanvas("F3_canvas", "F3_canvas", 800, 800);
    c_f3->cd();
    TMultiGraph *mg_f3 = new TMultiGraph();
    mg_f3->SetTitle("F3_PID; F3 RF [ns]; PPACs/MWDCs TOF [ns]");
    mg_f3->GetHistogram()->GetXaxis()->SetLimits(f3_rf_range[0], f3_rf_range[1]);
    mg_f3->GetHistogram()->GetYaxis()->SetRangeUser(tof_range[0], tof_range[1]);
    mg_f3->GetHistogram()->GetYaxis()->SetTitleOffset(1.3);
    mg_f3->GetHistogram()->GetXaxis()->SetTitleOffset(1.3);

    std::vector<TText *> txts_f3;
    for (Int_t i = 0; i < graphs_f3_pid.size(); i++) {
        mg_f3->Add(graphs_f3_pid[i]);
        for (Int_t j = 0; j < graphs_f3_pid[i]->GetN(); j++) {
            Double_t x, y;
            graphs_f3_pid[i]->GetPoint(j, x, y);
            TText *txt = new TText(x + 3.0, y, Form("%s+", ion_names[i].Data()));
            txt->SetTextAlign(12);
            if (ion_colors[i] == 0) {
                txt->SetTextColor(kRed);
            } else if (ion_colors[i] == 1) {
                txt->SetTextColor(kBlue);
            } else {
                txt->SetTextColor(kBlack);
            }
            txts_f3.emplace_back(txt);
        }
    }
    mg_f3->Draw("ap");
    for (Int_t i = 0; i < txts_f3.size(); i++) {
        txts_f3[i]->Draw("same");
    }
    gPad->SetGrid();
    c_f3->Update();

    gDirectory->mkdir("pid");
    gDirectory->cd("pid");
    gDirectory->Add(mg_f2);
    gDirectory->Add(mg_f3);
    gDirectory->Add(c_f2);
    gDirectory->Add(c_f3);
    gDirectory->cd("..");

    if (fIsBatch) {
        gROOT->SetBatch(kFALSE);
    }
}

void TCRIBPIDProcessor::Process() {}

void TCRIBPIDProcessor::LoadPIDyaml() {
    FileStat_t info;
    if (gSystem->GetPathInfo(fFileName.Data(), info) != 0) {
        SetStateError(Form("File %s does not exist.", fFileName.Data()));
        return;
    }

    YAML::Node yaml_all = YAML::LoadFile(fFileName.Data());

#if 0 // for debug
    YAML::Emitter emitter;
    emitter << yaml_all;
    std::cout << emitter.c_str() << std::endl;
    std::cout << yaml_all.size() << std::endl;
#endif

    // set ions
    YAML::Node yaml_ions = yaml_all[kNodeKeyIons].as<YAML::Node>();
    for (Int_t i = 0; i < yaml_ions.size(); i++) {
        TString name = yaml_ions[i][kNodeKeyName].as<std::string>();
        Int_t charge = yaml_ions[i][kNodeKeyCharge].as<int>();
        Double_t mass = yaml_ions[i][kNodeKeyMass].as<double>();
        Int_t color = yaml_ions[i][kNodeKeyColor].as<int>();

        ion_names.emplace_back(name);
        ion_charges.emplace_back(charge);
        ion_masses.emplace_back(mass * kAmu2MeV);
        ion_colors.emplace_back(color);
    }

    // set BLD parameters
    f1_brho = yaml_all[kNodeKeyF1Param][kNodeKeyBrho].as<double>();
    rf_period = yaml_all[kNodeKeyF1Param][kNodeKeyRfPeriod].as<double>();
    f2_ppac_thickness = 1.0e-3 * yaml_all[kNodeKeyF2Param][kNodeKeyPPACThickness].as<double>();
    f2_ssd_thickness = 1.0e-3 * yaml_all[kNodeKeyF2Param][kNodeKeySSDThickness].as<double>();
    a_thickness = 1.0e-3 * yaml_all[kNodeKeyF3Param][kNodeKeyAThickness].as<double>();
    b_thickness = 1.0e-3 * yaml_all[kNodeKeyF3Param][kNodeKeyBThickness].as<double>();
    distance = yaml_all[kNodeKeyF3Param][kNodeKeyDistance].as<double>();
    trigger = yaml_all[kNodeKeyF3Param][kNodeKeyTrigger].as<ushort>();

    // set display parameters
    f2_rf_offset = yaml_all[kNodeKeyF2Display][kNodeKeyRfOffset].as<double>();
    f3_rf_offset = yaml_all[kNodeKeyF3Display][kNodeKeyRfOffset].as<double>();
    tof_offset = yaml_all[kNodeKeyF3Display][kNodeKeyTofOffset].as<double>();
    f2_rf_range = yaml_all[kNodeKeyF2Display][kNodeKeyRfRange].as<std::vector<double>>();
    f3_rf_range = yaml_all[kNodeKeyF3Display][kNodeKeyRfRange].as<std::vector<double>>();
    tof_range = yaml_all[kNodeKeyF3Display][kNodeKeyTofRange].as<std::vector<double>>();
    energy_range = yaml_all[kNodeKeyF2Display][kNodeKeyERange].as<std::vector<double>>();
}

void TCRIBPIDProcessor::SetSRIMObject() {
    fElossTable = new SRIMData();
    for (Int_t iPart = 0; iPart < ion_names.size(); iPart++) {
        std::vector<SRIMtable *> srim_v;
        srim_v.emplace_back(fElossTable->GetTable(ion_names[iPart], "Mylar"));
        srim_v.emplace_back(fElossTable->GetTable(ion_names[iPart], "Si"));

        fEloss_vvec.emplace_back(srim_v);
        srim_v.clear();
    }
}