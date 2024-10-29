#include "ObjectPick.h"

// Constructor implementation
ObjectPick::ObjectPick(GlobalFlag& globalFlags) : 
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug())
{
}
// Destructor
ObjectPick::~ObjectPick() {
    // Cleanup if necessary
}

// Set the SkimTree pointer
void ObjectPick::setInput(const std::shared_ptr<SkimTree>& skimTree, const std::shared_ptr<ObjectScale>& objectScale) {
    skimTree_ = skimTree;
    objectScale_ = objectScale;
}

// Clear picked objects
void ObjectPick::clearObjects() {
    pickedElectrons_.clear();
    pickedMuons_.clear();
    pickedPhotons_.clear();
    pickedRefs_.clear();

    pickedGenElectrons_.clear();
    pickedGenMuons_.clear();
    pickedGenPhotons_.clear();
    pickedGenRefs_.clear();
}

// Helper function for debug printing
void ObjectPick::printDebug(const std::string& message) const {
    if (isDebug_) {
        std::cout << message << '\n';
    }
}

// Accessors for picked objects
auto ObjectPick::getPickedElectrons() const -> const std::vector<int>& {
    return pickedElectrons_;
}

auto ObjectPick::getPickedMuons() const -> const std::vector<int>& {
    return pickedMuons_;
}

auto ObjectPick::getPickedPhotons() const -> const std::vector<int>& {
    return pickedPhotons_;
}

auto ObjectPick::getPickedRefs() const -> const std::vector<TLorentzVector>& {
    return pickedRefs_;
}

auto ObjectPick::getPickedGenElectrons() const -> const std::vector<int>& {
    return pickedGenElectrons_;
}

auto ObjectPick::getPickedGenMuons() const -> const std::vector<int>& {
    return pickedGenMuons_;
}

auto ObjectPick::getPickedGenPhotons() const -> const std::vector<int>& {
    return pickedGenPhotons_;
}

auto ObjectPick::getPickedGenRefs() const -> const std::vector<TLorentzVector>& {
    return pickedGenRefs_;
}

// Reco objects
void ObjectPick::pickMuons() {
    printDebug("Starting Selection, nMuon = "+std::to_string(skimTree_->nMuon));

    for (UInt_t m = 0; m < skimTree_->nMuon; ++m) {
        double eta = skimTree_->Muon_eta[m];
        double pt = skimTree_->Muon_pt[m];

        bool passPrompt = false;
        if (pt > 20) {
            passPrompt = (std::abs(eta) <= 2.3 &&
                          skimTree_->Muon_tightId[m] &&
                          skimTree_->Muon_pfRelIso04_all[m] < 0.15 &&
                          skimTree_->Muon_dxy[m] < 0.2 &&
                          skimTree_->Muon_dz[m] < 0.5);
        }

        if (passPrompt) {
            pickedMuons_.push_back(m);
            printDebug("Muon " + std::to_string(m) + " selected: pt=" + std::to_string(pt) + ", eta=" + std::to_string(eta));
        } else {
            printDebug("Muon " + std::to_string(m) + " rejected: pt=" + std::to_string(pt) + ", eta=" + std::to_string(eta));
        }
    }

    printDebug("Total Muons Selected: " + std::to_string(pickedMuons_.size()));
}

void ObjectPick::pickElectrons() {
    printDebug("Starting Selection, nElectron = "+std::to_string(skimTree_->nElectron));

    for (int eleInd = 0; eleInd < skimTree_->nElectron; ++eleInd) {
        double eta = skimTree_->Electron_eta[eleInd];
        double absEta = std::abs(eta);
        double SCeta = eta + skimTree_->Electron_deltaEtaSC[eleInd];
        double absSCEta = std::abs(SCeta);
        double pt = skimTree_->Electron_pt[eleInd];

        // Ensure it doesn't fall within the gap
        bool passEtaEBEEGap = (absSCEta < 1.4442) || (absSCEta > 1.566);
        // Tight electron ID
        bool passTightID = skimTree_->Electron_cutBased[eleInd] == 4;

        bool eleSel = (passEtaEBEEGap && absEta <= 2.4 && pt >= 25 && passTightID);
        if (eleSel) {
            pickedElectrons_.push_back(eleInd);
            printDebug("Electron " + std::to_string(eleInd) + " selected: pt=" + std::to_string(pt) + ", eta=" + std::to_string(eta));
        } else {
            printDebug("Electron " + std::to_string(eleInd) + " rejected: pt=" + std::to_string(pt) + ", eta=" + std::to_string(eta));
        }
    }

    printDebug("Total Electrons Selected: " + std::to_string(pickedElectrons_.size()));
}

// Photon selection
void ObjectPick::pickPhotons() {
    printDebug("Starting Selection, nPhoton = "+std::to_string(skimTree_->nPhoton));

    for (int phoInd = 0; phoInd < skimTree_->nPhoton; ++phoInd) {
        double et = skimTree_->Photon_pt[phoInd];
        double eta = skimTree_->Photon_eta[phoInd];
        double absEta = std::abs(eta);
        bool passPhoId = skimTree_->Photon_cutBased[phoInd];  // Tight ID
        bool phoPresel = (et >= 20.0 && absEta <= 1.4442);

        if (phoPresel && passPhoId) {
            pickedPhotons_.push_back(phoInd);
            printDebug("Photon " + std::to_string(phoInd) + " selected: et=" + std::to_string(et) + ", eta=" + std::to_string(eta));
        } else {
            printDebug("Photon " + std::to_string(phoInd) + " rejected: et=" + std::to_string(et) + ", eta=" + std::to_string(eta));
        }
    }

    printDebug("Total Photons Selected: " + std::to_string(pickedPhotons_.size()));
}

// Reference object selection
void ObjectPick::pickRefs() {
    // Z->ee + jets channel
    if (channel_ == GlobalFlag::Channel::ZeeJet && pickedElectrons_.size() > 1) {
        int j = pickedElectrons_.at(0);
        int k = pickedElectrons_.at(1);

        TLorentzVector p4Lep1, p4Lep2;
        double ptj = objectScale_->getEleSsCorrection(j, "nom") * skimTree_->Electron_pt[j];
        double ptk = objectScale_->getEleSsCorrection(k, "nom") * skimTree_->Electron_pt[k];
        p4Lep1.SetPtEtaPhiM(ptj, skimTree_->Electron_eta[j], skimTree_->Electron_phi[j], skimTree_->Electron_mass[j]);
        p4Lep2.SetPtEtaPhiM(ptk, skimTree_->Electron_eta[k], skimTree_->Electron_phi[k], skimTree_->Electron_mass[k]);
        TLorentzVector p4Ref = p4Lep1 + p4Lep2;

        if ((skimTree_->Electron_charge[j] * skimTree_->Electron_charge[k]) == -1 &&
            std::abs(p4Ref.M() - 91.1876) < 20 &&
            p4Ref.Pt() > 15) {
            pickedRefs_.push_back(p4Ref);
            printDebug("Z->ee candidate selected with mass " + std::to_string(p4Ref.M()));
        }
    }

    // Z->mumu + jets channel
    if (channel_ == GlobalFlag::Channel::ZmmJet && pickedMuons_.size() > 1) {
        int j = pickedMuons_.at(0);
        int k = pickedMuons_.at(1);

        TLorentzVector p4Lep1, p4Lep2;
        double ptj = objectScale_->getMuRochCorrection(j, "nom") * skimTree_->Muon_pt[j];
        double ptk = objectScale_->getMuRochCorrection(k, "nom") * skimTree_->Muon_pt[k];
        p4Lep1.SetPtEtaPhiM(ptj, skimTree_->Muon_eta[j], skimTree_->Muon_phi[j], skimTree_->Muon_mass[j]);
        p4Lep2.SetPtEtaPhiM(ptk, skimTree_->Muon_eta[k], skimTree_->Muon_phi[k], skimTree_->Muon_mass[k]);
        TLorentzVector p4Ref = p4Lep1 + p4Lep2;

        if ((skimTree_->Muon_charge[j] * skimTree_->Muon_charge[k]) == -1 &&
            std::abs(p4Ref.M() - 91.1876) < 20 &&
            p4Ref.Pt() > 15) {
            pickedRefs_.push_back(p4Ref);
            printDebug("Z->mumu candidate selected with mass " + std::to_string(p4Ref.M()));
        }
    }

    // Gamma + jets channel
    if (channel_ == GlobalFlag::Channel::GamJet && !pickedPhotons_.empty()) {
        for (int idx : pickedPhotons_) {
            TLorentzVector p4Pho;
            p4Pho.SetPtEtaPhiM(skimTree_->Photon_pt[idx], skimTree_->Photon_eta[idx], skimTree_->Photon_phi[idx], skimTree_->Photon_mass[idx]);
            pickedRefs_.push_back(p4Pho);
            printDebug("Photon added to references: et=" + std::to_string(skimTree_->Photon_pt[idx]));
        }
    }

    printDebug("Total Reference Objects Selected: " + std::to_string(pickedRefs_.size()));
}

// Gen objects
void ObjectPick::pickGenMuons() {
    printDebug("Starting Selection, nGenDressedLepton = "+std::to_string(skimTree_->nGenDressedLepton));

    for (int i = 0; i < skimTree_->nGenDressedLepton; ++i) {
        if (std::abs(skimTree_->GenDressedLepton_pdgId[i]) == 13) {
            pickedGenMuons_.push_back(i);
            printDebug("Gen Muon " + std::to_string(i) + " selected");
        }
    }

    printDebug("Total Gen Muons Selected: " + std::to_string(pickedGenMuons_.size()));
}

void ObjectPick::pickGenElectrons() {
    printDebug("Starting Selection, nGenDressedLepton = "+std::to_string(skimTree_->nGenDressedLepton));

    for (int i = 0; i < skimTree_->nGenDressedLepton; ++i) {
        if (std::abs(skimTree_->GenDressedLepton_pdgId[i]) == 11) {
            pickedGenElectrons_.push_back(i);
            printDebug("Gen Electron " + std::to_string(i) + " selected");
        }
    }

    printDebug("Total Gen Electrons Selected: " + std::to_string(pickedGenElectrons_.size()));
}

void ObjectPick::pickGenPhotons() {
    printDebug("Starting Selection, nGenIsolatedPhoton = "+std::to_string(skimTree_->nGenIsolatedPhoton));

    for (int i = 0; i < skimTree_->nGenIsolatedPhoton; ++i) {
        pickedGenPhotons_.push_back(i);
        printDebug("Gen Photon " + std::to_string(i) + " selected");
    }

    printDebug("Total Gen Photons Selected: " + std::to_string(pickedGenPhotons_.size()));
}

void ObjectPick::pickGenRefs() {
    // Z->ee + jets channel
    if (channel_ == GlobalFlag::Channel::ZeeJet && pickedGenElectrons_.size() > 1) {
        for (size_t j = 0; j < pickedGenElectrons_.size(); ++j) {
            for (size_t k = j + 1; k < pickedGenElectrons_.size(); ++k) {
                TLorentzVector p4Lep1, p4Lep2;
                int idx1 = pickedGenElectrons_[j];
                int idx2 = pickedGenElectrons_[k];

                p4Lep1.SetPtEtaPhiM(skimTree_->GenDressedLepton_pt[idx1],
                                    skimTree_->GenDressedLepton_eta[idx1],
                                    skimTree_->GenDressedLepton_phi[idx1],
                                    skimTree_->GenDressedLepton_mass[idx1]);

                p4Lep2.SetPtEtaPhiM(skimTree_->GenDressedLepton_pt[idx2],
                                    skimTree_->GenDressedLepton_eta[idx2],
                                    skimTree_->GenDressedLepton_phi[idx2],
                                    skimTree_->GenDressedLepton_mass[idx2]);

                TLorentzVector p4GenRef = p4Lep1 + p4Lep2;
                pickedGenRefs_.push_back(p4GenRef);
                printDebug("Gen Z->ee candidate selected with mass " + std::to_string(p4GenRef.M()));
            }
        }
    }

    // Z->mumu + jets channel
    if (channel_ == GlobalFlag::Channel::ZmmJet && pickedGenMuons_.size() > 1) {
        for (size_t j = 0; j < pickedGenMuons_.size(); ++j) {
            for (size_t k = j + 1; k < pickedGenMuons_.size(); ++k) {
                TLorentzVector p4Lep1, p4Lep2;
                int idx1 = pickedGenMuons_[j];
                int idx2 = pickedGenMuons_[k];

                p4Lep1.SetPtEtaPhiM(skimTree_->GenDressedLepton_pt[idx1],
                                    skimTree_->GenDressedLepton_eta[idx1],
                                    skimTree_->GenDressedLepton_phi[idx1],
                                    skimTree_->GenDressedLepton_mass[idx1]);

                p4Lep2.SetPtEtaPhiM(skimTree_->GenDressedLepton_pt[idx2],
                                    skimTree_->GenDressedLepton_eta[idx2],
                                    skimTree_->GenDressedLepton_phi[idx2],
                                    skimTree_->GenDressedLepton_mass[idx2]);

                TLorentzVector p4GenRef = p4Lep1 + p4Lep2;
                pickedGenRefs_.push_back(p4GenRef);
                printDebug("Gen Z->mumu candidate selected with mass " + std::to_string(p4GenRef.M()));
            }
        }
    }

    // Gamma + jets channel
    if (channel_ == GlobalFlag::Channel::GamJet && !pickedGenPhotons_.empty()) {
        for (int idx : pickedGenPhotons_) {
            TLorentzVector p4GenPho;
            p4GenPho.SetPtEtaPhiM(skimTree_->GenIsolatedPhoton_pt[idx],
                                  skimTree_->GenIsolatedPhoton_eta[idx],
                                  skimTree_->GenIsolatedPhoton_phi[idx],
                                  skimTree_->GenIsolatedPhoton_mass[idx]);
            pickedGenRefs_.push_back(p4GenPho);
            printDebug("Gen Photon added to references: pt=" + std::to_string(skimTree_->GenIsolatedPhoton_pt[idx]));
        }
    }

    printDebug("Total Gen Reference Objects Selected: " + std::to_string(pickedGenRefs_.size()));
}

