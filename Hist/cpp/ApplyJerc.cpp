#include "ApplyJerc.h"
#include <iostream>
#include <iomanip>

// Constructor
ApplyJerc::ApplyJerc(ObjectScale *objScale, bool isData, bool applyJer)
    : objScale_(objScale), isData_(isData), applyJer_(applyJer) {
    Initialize();
}

void ApplyJerc::Initialize() {
    // Clear existing corrections
    p4MapJet1_.clear();
    p4MapJetSum_.clear();
    p4MapMet_.clear();

    // Initialize jet corrections with labels
    std::vector<std::string> jecNames = {
        "Nano", "Raw", "L1RcCorr", "L2RelCorr", "L2L3ResCorr", "JerCorr", "Corr"
    };

    for (const auto& name : jecNames) {
        p4MapJet1_[name] = TLorentzVector();
        p4MapJetSum_[name] = TLorentzVector();
    }

    // Initialize MET labels
    std::vector<std::string> metNames = {
        "Nano", "Type1Corr"
    };
    for (const auto& name : metNames) {
        p4MapMet_[name] = TLorentzVector();
    }
}

// Apply corrections
void ApplyJerc::applyCorrections(std::shared_ptr<SkimTree>& skimT, CorrectionLevel level) {

    Initialize();

    TLorentzVector p4Met;
    p4Met.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);
    p4MapMet_["Nano"] = p4Met;

    for (int i = 0; i < skimT->nJet; ++i) {
        // Original NanoAOD jet
        TLorentzVector p4Jet;
        p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                           skimT->Jet_phi[i], skimT->Jet_mass[i]);
        if (i == 0) p4MapJet1_["Nano"] += p4Jet;
        p4MapJetSum_["Nano"] += p4Jet;

        p4Met += p4Jet;//Add default p4Jet

        // Undo NanoAOD correction
        undoNanoAODCorrection(*skimT, i);
        p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                           skimT->Jet_phi[i], skimT->Jet_mass[i]);
        if (i == 0) p4MapJet1_["Raw"] += p4Jet;
        p4MapJetSum_["Raw"] += p4Jet;

        // Apply corrections based on level
        if (level >= CorrectionLevel::L1Rc) {
            applyL1FastJetCorrection(*skimT, i);
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (i == 0) p4MapJet1_["L1RcCorr"] += p4Jet;
            p4MapJetSum_["L1RcCorr"] += p4Jet;
        }

        if (level >= CorrectionLevel::L2Rel) {
            applyL2RelativeCorrection(*skimT, i);
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (i == 0) p4MapJet1_["L2RelCorr"] += p4Jet;
            p4MapJetSum_["L2RelCorr"] += p4Jet;
        }

        if (level >= CorrectionLevel::L2L3Res && isData_) {
            applyL2L3ResidualCorrection(*skimT, i);
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (i == 0) p4MapJet1_["L2L3ResCorr"] += p4Jet;
            p4MapJetSum_["L2L3ResCorr"] += p4Jet;
        }

        if (applyJer_ && !isData_) {
            applyJerCorrection(*skimT, i);
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (i == 0) p4MapJet1_["JerCorr"] += p4Jet;
            p4MapJetSum_["JerCorr"] += p4Jet;
        }

        //Final correction
        if (i == 0) p4MapJet1_["Corr"] += p4Jet;
        p4MapJetSum_["Corr"] += p4Jet;

        p4Met -= p4Jet;//Substract corrected p4Jet

    }//for nJet

    //Update the MET
    p4MapMet_["Type1Corr"] = p4Met;
    skimT->ChsMET_pt  = p4Met.Pt(); 
    skimT->ChsMET_phi = p4Met.Phi();
}

// Undo NanoAOD correction
void ApplyJerc::undoNanoAODCorrection(SkimTree& skimT, int i) {
    float scale = 1.0f - skimT.Jet_rawFactor[i];
    skimT.Jet_pt[i] *= scale;
    skimT.Jet_mass[i] *= scale;
}

// L1 FastJet correction
void ApplyJerc::applyL1FastJetCorrection(SkimTree& skimT, int i) {
    double corr = objScale_->getL1FastJetCorrection(skimT.Jet_area[i], skimT.Jet_eta[i],
                                                    skimT.Jet_pt[i], skimT.Rho);
    skimT.Jet_pt[i] *= corr;
    skimT.Jet_mass[i] *= corr;
}

// L2 Relative correction
void ApplyJerc::applyL2RelativeCorrection(SkimTree& skimT, int i) {
    double corr = objScale_->getL2RelativeCorrection(skimT.Jet_eta[i], skimT.Jet_pt[i]);
    skimT.Jet_pt[i] *= corr;
    skimT.Jet_mass[i] *= corr;
}

// L2L3 Residual correction
void ApplyJerc::applyL2L3ResidualCorrection(SkimTree& skimT, int i) {
    double corr = objScale_->getL2L3ResidualCorrection(skimT.Jet_eta[i], skimT.Jet_pt[i]);
    skimT.Jet_pt[i] *= corr;
    skimT.Jet_mass[i] *= corr;
}

// Jer correction
void ApplyJerc::applyJerCorrection(SkimTree& skimT, int i) {
    double corr = objScale_->getJerCorrection(skimT, i, "nom");
    skimT.Jet_pt[i] *= corr;
    skimT.Jet_mass[i] *= corr;
}

// Print jet corrections
void ApplyJerc::print() const {
    auto printCorrections = [](const std::unordered_map<std::string, TLorentzVector>& corrections, const std::string& header) {
        std::cout << header << '\n';
        for (const auto& [label, p4] : corrections) {
            std::cout << "  " << label << " Pt: " << p4.Pt() << ", Mass: " << p4.M() << '\n';
        }
        std::cout << '\n';
    };

    std::cout << std::fixed << std::setprecision(3);
    printCorrections(p4MapJet1_, "Jet1 Corrections:");
    printCorrections(p4MapJetSum_, "JetSum Corrections:");
    printCorrections(p4MapMet_, "Met Corrections:");
}

