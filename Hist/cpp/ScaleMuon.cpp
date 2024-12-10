#include "ScaleMuon.h"
#include <iostream>
#include <iomanip>

// Constructor
ScaleMuon::ScaleMuon(ScaleObject *scaleObj, bool isData)
    : scaleObj_(scaleObj), isData_(isData) {
    Initialize();
}

void ScaleMuon::Initialize() {
    // Clear existing corrections
    p4MapMuon1_.clear();

    // Initialize jet corrections with labels
    std::vector<std::string> corrNames = {
        "Nano",  "Corr"
    };

    for (const auto& name : corrNames) {
        p4MapMuon1_[name] = TLorentzVector();
    }

}

// Apply corrections
void ScaleMuon::applyCorrections(std::shared_ptr<SkimTree>& skimT) {

    Initialize();

    TLorentzVector p4Muon;
    for (int j = 0; j < skimT->nMuon; ++j) {
        p4Muon.SetPtEtaPhiM(skimT->Muon_pt[j], skimT->Muon_eta[j], 
                            skimT->Muon_phi[j], skimT->Muon_mass[j]);
        if(j==0) p4MapMuon1_["Nano"] = p4Muon;
        
        double corr = 1.0;
        corr = scaleObj_->getMuRochCorrection(*skimT, j, "nom");
        skimT->Muon_pt[j] *= corr;
        p4Muon.SetPtEtaPhiM(skimT->Muon_pt[j], skimT->Muon_eta[j], 
                            skimT->Muon_phi[j], skimT->Muon_mass[j]);
        if(j==0) p4MapMuon1_["Corr"] = p4Muon;
    }
}

// Print jet corrections
void ScaleMuon::print() const {
    auto printCorrections = [](const std::unordered_map<std::string, TLorentzVector>& corrections, const std::string& header) {
        std::cout << header << '\n';
        for (const auto& [label, p4] : corrections) {
            std::cout << "  " << label << " Pt: " << p4.Pt() << ", Mass: " << p4.M() << '\n';
        }
        std::cout << '\n';
    };

    std::cout << std::fixed << std::setprecision(3);
    printCorrections(p4MapMuon1_, "Muon1 Corrections:");
}

