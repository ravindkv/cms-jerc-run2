#include "ScaleJetMet.h"
#include <iostream>
#include <iomanip>

// Constructor
ScaleJetMet::ScaleJetMet(ScaleObject *scaleObj, bool isData, bool applyJer)
    : scaleObj_(scaleObj), isData_(isData), applyJer_(applyJer) {
    Initialize();
}

void ScaleJetMet::Initialize() {
    // Clear existing corrections
    p4MapJet1_.clear();
    p4MapSelJetSum_.clear();
    p4MapMet_.clear();

    // Initialize jet corrections with labels
    std::vector<std::string> jecNames = {
        "Nano", "Raw", "L1RcCorr", "L2RelCorr", "L2L3ResCorr", "JerCorr", "Corr"
    };

    for (const auto& name : jecNames) {
        p4MapJet1_[name] = TLorentzVector();
        p4MapSelJetSum_[name] = TLorentzVector();
    }

    // Initialize MET labels
    std::vector<std::string> metNames = {
        "Nano", "Corr"
    };
    for (const auto& name : metNames) {
        p4MapMet_[name] = TLorentzVector();
    }
    p4SumAllNano_ = TLorentzVector();
    p4SumCorrAndUnCorr_ = TLorentzVector();
}

// Apply corrections
void ScaleJetMet::applyCorrections(std::shared_ptr<SkimTree>& skimT, CorrectionLevel level) {
    // Check if the SkimTree pointer is valid
    if (!skimT) {
        std::cerr << "Error: Invalid SkimTree pointer passed to applyCorrections." << std::endl;
        return;
    }
    // Check if the ScaleObject pointer is valid
    if (!scaleObj_) {
        std::cerr << "Error: scaleObj_ is null in ScaleJetMet." << std::endl;
        return;
    }
    Initialize();

    TLorentzVector p4Met;
    p4Met.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);
    p4MapMet_["Nano"] = p4Met;

    for (int i = 0; i < skimT->nJet; ++i) {
        TLorentzVector p4Jet;
        // JECs are not reliable for low pTs and high etas. It is better
        // to skip such jets than applying unreliable JEC
        if(skimT->Jet_pt[i] > 15 && std::abs(skimT->Jet_eta[i]) < 5.2){ 
            // Original NanoAOD jet
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (i == 0) p4MapJet1_["Nano"] += p4Jet;
            p4MapSelJetSum_["Nano"] += p4Jet;
            p4SumAllNano_ += p4Jet;
            
            p4Met += p4Jet;//Add default p4Jet
            
            // Undo NanoAOD correction
            undoNanoAODCorrection(*skimT, i);
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (i == 0) p4MapJet1_["Raw"] += p4Jet;
            p4MapSelJetSum_["Raw"] += p4Jet;
            
            // Apply corrections based on level
            if (level >= CorrectionLevel::L1Rc) {
                applyL1FastJetCorrection(*skimT, i);
                p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                                   skimT->Jet_phi[i], skimT->Jet_mass[i]);
                if (i == 0) p4MapJet1_["L1RcCorr"] += p4Jet;
                p4MapSelJetSum_["L1RcCorr"] += p4Jet;
            }
            
            if (level >= CorrectionLevel::L2Rel) {
                applyL2RelativeCorrection(*skimT, i);
                p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                                   skimT->Jet_phi[i], skimT->Jet_mass[i]);
                if (i == 0) p4MapJet1_["L2RelCorr"] += p4Jet;
                p4MapSelJetSum_["L2RelCorr"] += p4Jet;
            }
            
            if (level >= CorrectionLevel::L2L3Res && isData_) {
                applyL2L3ResidualCorrection(*skimT, i);
                p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                                   skimT->Jet_phi[i], skimT->Jet_mass[i]);
                if (i == 0) p4MapJet1_["L2L3ResCorr"] += p4Jet;
                p4MapSelJetSum_["L2L3ResCorr"] += p4Jet;
            }
            
            if (applyJer_ && !isData_) {
                applyJerCorrection(*skimT, i);
                p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                                   skimT->Jet_phi[i], skimT->Jet_mass[i]);
                if (i == 0) p4MapJet1_["JerCorr"] += p4Jet;
                p4MapSelJetSum_["JerCorr"] += p4Jet;
            }
            
            //Final correction
            if (i == 0) p4MapJet1_["Corr"] += p4Jet;
            p4MapSelJetSum_["Corr"] += p4Jet;
            p4SumCorrAndUnCorr_ += p4Jet;
            
            p4Met -= p4Jet;//Subtract corrected p4Jet
        }//if pT, eta
        else{
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            p4SumAllNano_ += p4Jet;
            p4SumCorrAndUnCorr_ += p4Jet;
        }
    }//for nJet
    //Update the MET
    p4MapMet_["Corr"] = p4Met;
    skimT->ChsMET_pt  = p4Met.Pt(); 
    skimT->ChsMET_phi = p4Met.Phi();
}

// Undo NanoAOD correction
void ScaleJetMet::undoNanoAODCorrection(SkimTree& skimT, int i) {
    float scale = 1.0f - skimT.Jet_rawFactor[i];
    skimT.Jet_pt[i] *= scale;
    skimT.Jet_mass[i] *= scale;
}

// L1 FastJet correction
void ScaleJetMet::applyL1FastJetCorrection(SkimTree& skimT, int i) {
    double corr = scaleObj_->getL1FastJetCorrection(skimT.Jet_area[i], skimT.Jet_eta[i],
                                                    skimT.Jet_pt[i], skimT.Rho);
    skimT.Jet_pt[i] *= corr;
    skimT.Jet_mass[i] *= corr;
}

// L2 Relative correction
void ScaleJetMet::applyL2RelativeCorrection(SkimTree& skimT, int i) {
    double corr = scaleObj_->getL2RelativeCorrection(skimT.Jet_eta[i], skimT.Jet_pt[i]);
    skimT.Jet_pt[i] *= corr;
    skimT.Jet_mass[i] *= corr;
}

// L2L3 Residual correction
void ScaleJetMet::applyL2L3ResidualCorrection(SkimTree& skimT, int i) {
    double corr = scaleObj_->getL2L3ResidualCorrection(skimT.Jet_eta[i], skimT.Jet_pt[i]);
    skimT.Jet_pt[i] *= corr;
    skimT.Jet_mass[i] *= corr;
}

// Jer correction
void ScaleJetMet::applyJerCorrection(SkimTree& skimT, int i) {
    double corr = scaleObj_->getJerCorrection(skimT, i, "nom");
    skimT.Jet_pt[i] *= corr;
    skimT.Jet_mass[i] *= corr;
}

// Print jet corrections
void ScaleJetMet::print() const {
    auto printCorrections = [](const std::unordered_map<std::string, TLorentzVector>& corrections, const std::string& header) {
        std::cout << header << '\n';
        for (const auto& [label, p4] : corrections) {
            std::cout << "  " << label << " Pt: " << p4.Pt() << ", Mass: " << p4.M() << '\n';
        }
        std::cout << '\n';
    };

    std::cout << std::fixed << std::setprecision(3);
    printCorrections(p4MapJet1_, "Jet1 Corrections:");
    printCorrections(p4MapSelJetSum_, "JetSum Corrections:");
    printCorrections(p4MapMet_, "Met Corrections:");
    std::cout<<"Check momentum conservation, (p4Met - p4AllJet).Pt(): "<<'\n';
    std::cout<<"    At Nano   : "<<(p4MapMet_.at("Nano") - p4SumAllNano_).Pt()<<'\n';
    std::cout<<"    After JEC : "<<(p4MapMet_.at("Corr") - p4SumCorrAndUnCorr_).Pt()<<'\n';
}

