/*
   @File name     : TEffectiveThickness.cc
   @description   :
   @Author        : Kodai Okawa<okawa@cns.s.u-tokyo.ac.jp>
   @Created date  : 2023-06-13 16:06:15
   @Last modified : 2023-08-01 11:08:52
*/

#include "TEffectiveThickness.h"

#include <TArtAtomicMassTable.h>
#include <TArtemisUtil.h>

#include <TClass.h>
#include <TGraph.h>
#include <TFile.h>

#include "constant.h"

using art::TEffectiveThickness;

ClassImp(TEffectiveThickness);

TEffectiveThickness::TEffectiveThickness()
{
   DoubleVec_t init_d_vec;
   StringVec_t init_s_vec;
   RegisterProcessorParameter("FileName", "name of the output ROOT file", fOutputName, TString(""));
   RegisterProcessorParameter("ParticleNames", "name of reaction particles", fParticleNames, init_s_vec);
   RegisterProcessorParameter("ParticleMass", "name of reaction particles", fParticleMass, init_d_vec);
   RegisterProcessorParameter("TargetName", "name of Target", fTarget, TString(""));
   RegisterProcessorParameter("TargetA", "g/mol of Target", fTargetA, 0.0);

   RegisterProcessorParameter("EnergyRange", "Energy Range of the Ecm (MeV)", fRange, init_d_vec);
   RegisterProcessorParameter("DeltaE", "Delta E of the Effective Thickness", fDeltaE, 0.0);
   RegisterProcessorParameter("Density", "Target density (g/cm3)", fDensity, 0.0);
}

TEffectiveThickness::~TEffectiveThickness()
{
   delete fElossTable;
   fElossTable = NULL;
}

void TEffectiveThickness::Init(TEventCollection *col)
{
   if (fParticleNames.size() != 2 || fParticleMass.size() != 2) {
      SetStateError("input steering yaml: not 2 particles (beam and target)");
      return;
   }

   Info("Init", "reaction: %s + %s", fParticleNames[0].Data(), fParticleNames[1].Data());
   for (Int_t i = 0; i < fParticleMass.size(); i++) {
      Info("Init", "\t%s: %lf amu", fParticleNames[i].Data(), fParticleMass[i]);
      fParticleMass[i] *= TArtAtomicMassTable::kAtomicMassUnit; // amu to MeV
   }

   Info("Init", "target: %s (%lf g/cm3)", fTarget.Data(), fDensity);

   fElossTable = new SRIMData();
   fElossBeam = fElossTable->GetTable(fParticleNames[0], fTarget);

   TGraph *g_mm = new TGraph();
   TGraph *g_g = new TGraph();
   TGraph *g_atm = new TGraph();
   Int_t itr = 0;
   for (Double_t energy = fRange[0]; energy < fRange[1]; energy += 0.01) {
      Double_t e_min = energy - fDeltaE / 2.0;
      Double_t e_max = energy + fDeltaE / 2.0;
      if (e_min < 0.0) {
         e_min = 0.0;
      }
      // thickness (mm)
      Double_t thickness = fElossBeam->GetRange(Ecm2Elab(e_max)) - fElossBeam->GetRange(Ecm2Elab(e_min));
      g_mm->SetPoint(itr, energy, thickness);

      // thickness ((mm->cm) * g/cm3) -> g/cm2
      g_g->SetPoint(itr, energy, thickness * 1.0e-1 * fDensity);

      // thickness ((mm->cm) * g/cm3 * /mol / g/mol) -> /cm2
      g_atm->SetPoint(itr, energy, thickness * 1.0e-1 * fDensity * kAvogadro / fTargetA);

      itr++;
   }
   g_mm->SetTitle(Form("thickness(mm, DeltaE=%.2lf MeV);Ecm (MeV);Effective thickness (mm)", fDeltaE));
   g_g->SetTitle(Form("thickness(g_cm2, DeltaE=%.2lf MeV);Ecm (MeV);Effective thickness (g/cm2)", fDeltaE));
   g_atm->SetTitle(Form("thickness(atom_cm2, DeltaE=%.2lf MeV);Ecm (MeV);Effective thickness (atom/cm2)", fDeltaE));

   gDirectory->Add(g_mm);
   gDirectory->Add(g_g);
   gDirectory->Add(g_atm);

   if (fOutputName != "") {
      Util::PrepareDirectoryFor(fOutputName);
      TFile *file = new TFile(fOutputName, "recreate");
      if (!file) {
         std::cerr << "ERROR : cannot open " << fOutputName << std::endl;
         return;
      }
      g_mm->Write("thickness_mm");
      g_g->Write("thickness_g_cm2");
      g_atm->Write("thickness_atom_cm2");

      file->Close();

      Info("Init", "make histogram rootfile: %s", fOutputName.Data());
   }
}

void TEffectiveThickness::Process() {}

Double_t TEffectiveThickness::Ecm2Elab(Double_t energy)
{
   // kinematics
   Double_t tmp =
      energy * energy + 2.0 * (energy * (fParticleMass[0] + fParticleMass[1]) + fParticleMass[0] * fParticleMass[1]);
   return tmp / (2.0 * fParticleMass[1]) - fParticleMass[0];
}

// ============================================================
// kinematics
// using Lorentz invariant (c = 1 unit)
// Ecm^2 = (E1lab + E2lab)^2 - (p1lab + p2lab)^2
// (Ekincm + m1 + m2)^2 = (E1lab + m2)^2 - p1lab^2
// (Ekincm + m1 + m2)^2 = (E1lab + m2)^2 - (E1lab^2 - m1^2)
// ...
// E1kinlab = E1lab - m1 = ...
// from this relationship, the Ecm2Elab is calculated
