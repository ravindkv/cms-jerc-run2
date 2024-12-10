#include "ScaleElectron.h"
#include <iostream>
#include <iomanip>

// Constructor
ScaleElectron::ScaleElectron(ScaleObject *scaleObj, bool isData)
    : scaleObj_(scaleObj), isData_(isData) {
    Initialize();
}

void ScaleElectron::Initialize() {
    // Clear existing corrections
    p4MapElectron1_.clear();

    // Initialize jet corrections with labels
    std::vector<std::string> corrNames = {
        "Nano",  "Corr"
    };

    for (const auto& name : corrNames) {
        p4MapElectron1_[name] = TLorentzVector();
    }

}

// Apply corrections
void ScaleElectron::applyCorrections(std::shared_ptr<SkimTree>& skimT) {

    Initialize();

    TLorentzVector p4Electron;
    for (int j = 0; j < skimT->nElectron; ++j) {
        p4Electron.SetPtEtaPhiM(skimT->Electron_pt[j], skimT->Electron_eta[j], 
                            skimT->Electron_phi[j], skimT->Electron_mass[j]);
        if(j==0) p4MapElectron1_["Nano"] = p4Electron;
        
        double corr = 1.0;
        // ONLY for syst
        //double ptj = scaleObj_->getEleSsCorrection(j, "up/down") * skimT->Electron_pt[j];
        //double ptk = scaleObj_->getEleSsCorrection(k, "up/down") * skimT->Electron_pt[k];
        skimT->Electron_pt[j] *= corr;
        skimT->Electron_mass[j] *= corr;
        p4Electron.SetPtEtaPhiM(skimT->Electron_pt[j], skimT->Electron_eta[j], 
                            skimT->Electron_phi[j], skimT->Electron_mass[j]);
        if(j==0) p4MapElectron1_["Corr"] = p4Electron;
    }
}

// Print jet corrections
void ScaleElectron::print() const {
    auto printCorrections = [](const std::unordered_map<std::string, TLorentzVector>& corrections, const std::string& header) {
        std::cout << header << '\n';
        for (const auto& [label, p4] : corrections) {
            std::cout << "  " << label << " Pt: " << p4.Pt() << ", Mass: " << p4.M() << '\n';
        }
        std::cout << '\n';
    };

    std::cout << std::fixed << std::setprecision(3);
    printCorrections(p4MapElectron1_, "Electron1 Corrections:");
}

