#include "ScalePhoton.h"
#include <iostream>
#include <iomanip>

// Constructor
ScalePhoton::ScalePhoton(ScaleObject *scaleObj, bool isData)
    : scaleObj_(scaleObj), isData_(isData) {
    Initialize();
}

void ScalePhoton::Initialize() {
    // Clear existing corrections
    p4MapPhoton1_.clear();

    // Initialize jet corrections with labels
    std::vector<std::string> corrNames = {
        "Nano",  "Corr"
    };

    for (const auto& name : corrNames) {
        p4MapPhoton1_[name] = TLorentzVector();
    }

}

// Apply corrections
void ScalePhoton::applyCorrections(std::shared_ptr<SkimTree>& skimT) {

    Initialize();

    TLorentzVector p4Photon;
    for (int j = 0; j < skimT->nPhoton; ++j) {
        p4Photon.SetPtEtaPhiM(skimT->Photon_pt[j], skimT->Photon_eta[j], 
                            skimT->Photon_phi[j], skimT->Photon_mass[j]);
        if(j==0) p4MapPhoton1_["Nano"] = p4Photon;
        
        double corr = 1.0;
        // ONLY for syst
        //double ptj = scaleObj_->getPhoSsCorrection(j, "up/down") * skimT->Photon_pt[j];
        //double ptk = scaleObj_->getPhoSsCorrection(k, "up/down") * skimT->Photon_pt[k];
        skimT->Photon_pt[j] *= corr;
        skimT->Photon_mass[j] *= corr;
        p4Photon.SetPtEtaPhiM(skimT->Photon_pt[j], skimT->Photon_eta[j], 
                            skimT->Photon_phi[j], skimT->Photon_mass[j]);
        if(j==0) p4MapPhoton1_["Corr"] = p4Photon;
    }
}

// Print jet corrections
void ScalePhoton::print() const {
    auto printCorrections = [](const std::unordered_map<std::string, TLorentzVector>& corrections, const std::string& header) {
        std::cout << header << '\n';
        for (const auto& [label, p4] : corrections) {
            std::cout << "  " << label << " Pt: " << p4.Pt() << ", Mass: " << p4.M() << '\n';
        }
        std::cout << '\n';
    };

    std::cout << std::fixed << std::setprecision(3);
    printCorrections(p4MapPhoton1_, "Photon1 Corrections:");
}

