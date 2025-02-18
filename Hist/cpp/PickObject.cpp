#include "PickObject.h"
#include <TRandom.h>

// Constructor implementation
PickObject::PickObject(GlobalFlag& globalFlags) : 
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug())
{
}
// Destructor
PickObject::~PickObject() {
    // Cleanup if necessary
}

// Clear picked objects
void PickObject::clearObjects() {
    pickedElectrons_.clear();
    pickedMuons_.clear();
    pickedPhotons_.clear();
    pickedRefs_.clear();
    pickedJetsP4_.clear();
    pickedJetsIndex_.clear();

    pickedGenElectrons_.clear();
    pickedGenMuons_.clear();
    pickedGenPhotons_.clear();
    pickedGenRefs_.clear();
    pickedGenJetsP4_.clear();
    pickedGenJetsIndex_.clear();
}

// Helper function for debug printing
void PickObject::printDebug(const std::string& message) const {
    if (isDebug_) {
        std::cout << message << '\n';
    }
}

// Accessors for picked objects
auto PickObject::getPickedElectrons() const -> const std::vector<int>& {
    return pickedElectrons_;
}

auto PickObject::getPickedMuons() const -> const std::vector<int>& {
    return pickedMuons_;
}

auto PickObject::getPickedPhotonIndex() const -> const std::vector<int>& {
    return pickedPhotons_;
}

auto PickObject::getPickedRefs() const -> const std::vector<TLorentzVector>& {
    return pickedRefs_;
}

auto PickObject::getPickedJetsP4() const -> const std::vector<TLorentzVector>& {
    return pickedJetsP4_;
}

auto PickObject::getPickedJetsIndex() const -> const std::vector<int>& {
    return pickedJetsIndex_;
}

auto PickObject::getPickedGenElectrons() const -> const std::vector<int>& {
    return pickedGenElectrons_;
}

auto PickObject::getPickedGenMuons() const -> const std::vector<int>& {
    return pickedGenMuons_;
}

auto PickObject::getPickedGenPhotons() const -> const std::vector<int>& {
    return pickedGenPhotons_;
}

auto PickObject::getPickedGenRefs() const -> const std::vector<TLorentzVector>& {
    return pickedGenRefs_;
}

auto PickObject::getPickedGenJetsP4() const -> const std::vector<TLorentzVector>& {
    return pickedGenJetsP4_;
}

auto PickObject::getPickedGenJetsIndex() const -> const std::vector<int>& {
    return pickedGenJetsIndex_;
}

// Reco objects
void PickObject::pickMuons(const SkimTree& skimT) {
    printDebug("Starting Selection, nMuon = "+std::to_string(skimT.nMuon));
    pickedMuons_.clear();
    
    double ptThreshold = 20.f;
    double etaThreshold = 2.3;
    if(channel_ == GlobalFlag::Channel::Wqqm){
        etaThreshold = 2.4;
        if(year_ == GlobalFlag::Year::Year2017){
            ptThreshold = 29;
        }
        else{
            ptThreshold = 26;
        }
    }

    for (UInt_t m = 0; m < skimT.nMuon; ++m) {
        double eta = skimT.Muon_eta[m];
        double pt = skimT.Muon_pt[m];

        bool passPrompt = false;
        if (pt > ptThreshold) {
            passPrompt = (std::abs(eta) <= etaThreshold &&
                          skimT.Muon_tightId[m] &&
                          skimT.Muon_pfRelIso04_all[m] < 0.15 &&
                          skimT.Muon_dxy[m] < 0.2 &&
                          skimT.Muon_dz[m] < 0.5);
        }

        if (passPrompt) {
            pickedMuons_.push_back(m);
            printDebug("Muon " + std::to_string(m) + " selected: pt = " + std::to_string(pt) + ", eta = " + std::to_string(eta));
        } else {
            printDebug("Muon " + std::to_string(m) + " rejected: pt = " + std::to_string(pt) + ", eta = " + std::to_string(eta));
        }
    }

    printDebug("Total Muons Selected: " + std::to_string(pickedMuons_.size()));
}

void PickObject::pickElectrons(const SkimTree& skimT) {
    printDebug("Starting Selection, nElectron = "+std::to_string(skimT.nElectron));
    pickedElectrons_.clear();

    double ptThreshold = 25.f;
    double etaThreshold = 2.4;
    if(channel_ == GlobalFlag::Channel::Wqqe){
        etaThreshold = 2.4;
        if(year_ == GlobalFlag::Year::Year2016Pre ||  year_ == GlobalFlag::Year::Year2016Post){
            ptThreshold = 34;
        }
        else{
            ptThreshold = 35;
            etaThreshold = 2.5;
        }
    }

    for (int eleInd = 0; eleInd < skimT.nElectron; ++eleInd) {
        double eta = skimT.Electron_eta[eleInd];
        double absEta = std::abs(eta);
        double SCeta = eta + skimT.Electron_deltaEtaSC[eleInd];
        double absSCEta = std::abs(SCeta);
        double pt = skimT.Electron_pt[eleInd];

        // Ensure it doesn't fall within the gap
        bool passEtaEBEEGap = (absSCEta < 1.4442) || (absSCEta > 1.566);
        // Tight electron ID
        bool passTightID = skimT.Electron_cutBased[eleInd] == 4;

        bool eleSel = (passEtaEBEEGap && absEta <= etaThreshold && pt >= ptThreshold && passTightID);
        if (eleSel) {
            pickedElectrons_.push_back(eleInd);
            printDebug("Electron " + std::to_string(eleInd) + " selected: pt = " + std::to_string(pt) + ", eta = " + std::to_string(eta));
        } else {
            printDebug("Electron " + std::to_string(eleInd) + " rejected: pt = " + std::to_string(pt) + ", eta = " + std::to_string(eta));
        }
    }

    printDebug("Total Electrons Selected: " + std::to_string(pickedElectrons_.size()));
}

// Photon selection
void PickObject::pickPhotons(const SkimTree& skimT) {
    printDebug("Starting Selection, nPhoton = "+std::to_string(skimT.nPhoton));
    pickedPhotons_.clear();

    for (int phoInd = 0; phoInd < skimT.nPhoton; ++phoInd) {
        double pt  = skimT.Photon_pt[phoInd];
        double absEta = std::abs(skimT.Photon_eta[phoInd]);
        double r9  = skimT.Photon_r9[phoInd];
        double hoe = skimT.Photon_hoe[phoInd];
        Int_t id = skimT.Photon_cutBased[phoInd];  // Tight ID
        // R9>0.94 to avoid bias wrt R9Id90 triggers and from photon conversions
        if(pt > 25 && absEta < 1.3 && r9 > 0.94 && r9 < 1.0 && hoe < 0.02148 && id==3){
            pickedPhotons_.push_back(phoInd);
        }
        printDebug(
            "Photon " + std::to_string(phoInd) + 
            ", Id  = " + std::to_string(id) + 
            ", pt  = " + std::to_string(pt) + 
            ", absEta  = " + std::to_string(absEta) + 
            ", hoe  = " + std::to_string(hoe) + 
            ", r9  = " + std::to_string(r9)
       );
    }
    printDebug("Total Photons Selected: " + std::to_string(pickedPhotons_.size()));
}

// Reference object selection
void PickObject::pickRefs(const SkimTree& skimT) {
    pickedRefs_.clear();
    // Z->ee + jets channel
    if (channel_ == GlobalFlag::Channel::ZeeJet && pickedElectrons_.size() > 1) {
        int j = pickedElectrons_.at(0);
        int k = pickedElectrons_.at(1);

        TLorentzVector p4Lep1, p4Lep2;
        double ptj = skimT.Electron_pt[j];
        double ptk = skimT.Electron_pt[k];
        p4Lep1.SetPtEtaPhiM(ptj, skimT.Electron_eta[j], skimT.Electron_phi[j], skimT.Electron_mass[j]);
        p4Lep2.SetPtEtaPhiM(ptk, skimT.Electron_eta[k], skimT.Electron_phi[k], skimT.Electron_mass[k]);
        TLorentzVector p4Ref = p4Lep1 + p4Lep2;

        if ((skimT.Electron_charge[j] * skimT.Electron_charge[k]) == -1 &&
            std::abs(p4Ref.M() - 91.1876) < 20 &&
            p4Ref.Pt() > 15) {
            pickedRefs_.push_back(p4Ref);
            printDebug("Z->ee candidate selected with mass " + std::to_string(p4Ref.M()));
        }
    }

    // Z->mumu + jets channel
    else if (channel_ == GlobalFlag::Channel::ZmmJet && pickedMuons_.size() > 1) {
        int j = pickedMuons_.at(0);
        int k = pickedMuons_.at(1);

        TLorentzVector p4Lep1, p4Lep2;
        double ptj = skimT.Muon_pt[j];
        double ptk = skimT.Muon_pt[k];
        p4Lep1.SetPtEtaPhiM(ptj, skimT.Muon_eta[j], skimT.Muon_phi[j], skimT.Muon_mass[j]);
        p4Lep2.SetPtEtaPhiM(ptk, skimT.Muon_eta[k], skimT.Muon_phi[k], skimT.Muon_mass[k]);
        TLorentzVector p4Ref = p4Lep1 + p4Lep2;

        if ((skimT.Muon_charge[j] * skimT.Muon_charge[k]) == -1 &&
            std::abs(p4Ref.M() - 91.1876) < 20 &&
            p4Ref.Pt() > 15) {
            pickedRefs_.push_back(p4Ref);
            printDebug("Z->mumu candidate selected with mass " + std::to_string(p4Ref.M()));
        }
    }

    // Gamma + jets channel
    else if (channel_ == GlobalFlag::Channel::GamJet && !pickedPhotons_.empty()) {
        for (int idx : pickedPhotons_) {
            TLorentzVector p4Pho;
            p4Pho.SetPtEtaPhiM(skimT.Photon_pt[idx], skimT.Photon_eta[idx], skimT.Photon_phi[idx], skimT.Photon_mass[idx]);
            pickedRefs_.push_back(p4Pho);
            printDebug("Photon index added to references  = " + std::to_string(idx));
        }
    }
    printDebug("Total Reference Objects Selected: " + std::to_string(pickedRefs_.size()));
}


void PickObject::pickRefForFakeGamma(const SkimTree& skimT, const int& iJet) {
    printDebug("\n pickRefForFakeGamma: Starting Selection");
    pickedRefs_.clear();
    int iGenJet = skimT.Jet_genJetIdx[iJet];
    double offset = 1.0;
    if (iGenJet >=0 && iGenJet < skimT.nGenJet) {
        TLorentzVector p4Pho;
        p4Pho.SetPtEtaPhiM(skimT.GenJet_pt[iGenJet], skimT.GenJet_eta[iGenJet], skimT.GenJet_phi[iGenJet], skimT.GenJet_mass[iGenJet]);
        // QCD_CP5 has about 3.5 GeV/A of UE offset at generator level
        // https://github.com/blehtela/gamjet-analysis/blob/919921427e6a1ec96df677bd774f0f819a19d108/GamHistosFill.C#L2562
        if (p4Pho.Pt() > 0 ) offset = 1 - 3.5*skimT.Jet_area[iJet]/p4Pho.Pt();
        p4Pho *= offset;
        pickedRefs_.push_back(p4Pho);
    }
    printDebug("Jet index added to reference = " + std::to_string(iJet));
    printDebug("GenJet index added to reference = " + std::to_string(iGenJet));
    printDebug("UE offset = " + std::to_string(offset));
    printDebug("pickRefForFakeGamma: Done.\n");
}


void PickObject::pickJets(const SkimTree& skimT, const TLorentzVector& p4Ref) {
    printDebug("pickJets: Starting Selection, nJet = " + std::to_string(skimT.nJet));

    pickedJetsIndex_.clear();
    pickedJetsP4_.clear();

    //-----------------------------------------
    // 1) Decide on pT threshold based on channel
    //-----------------------------------------
    float ptThreshold = (channel_ == GlobalFlag::Channel::GamJet) ? 15.f : 12.f;
    float etaThreshold = 1.3;

    //-----------------------------------------
    // 2) Identify the photon->jet index if needed
    //-----------------------------------------
    int phoJetIdx = -1;
    if (channel_ == GlobalFlag::Channel::GamJet && !pickedPhotons_.empty()) {
        int phoInd   = pickedPhotons_.at(0);
        phoJetIdx    = skimT.Photon_jetIdx[phoInd];
        printDebug("GamJet channel: photon->jet index = " + std::to_string(phoJetIdx));
    }

    //-----------------------------------------
    // 3) Gather candidate jet indices 
    //    (pass minimal pT, skip photon jet if GamJet)
    //-----------------------------------------
    std::vector<int> candIndices;
    candIndices.reserve(skimT.nJet);

    for (int i = 0; i < skimT.nJet; ++i) {
        float pt = skimT.Jet_pt[i];
        if (pt < ptThreshold) {
            continue; 
        }
        if (std::abs(skimT.Jet_eta[i]) >= etaThreshold) {
            continue; 
        }

        // Skip photon jet index in GamJet channel
        if (channel_ == GlobalFlag::Channel::GamJet && (i == phoJetIdx)) {
            printDebug("Skipping jet " + std::to_string(i) + " because it matches photon->jet index");
            continue;
        }

        candIndices.push_back(i);
    }

    //-----------------------------------------
    // 4) Sort the candidate jet indices by pT
    //    in descending order
    //-----------------------------------------
    std::sort(candIndices.begin(), candIndices.end(),
              [&](int idx1, int idx2) {
                  return skimT.Jet_pt[idx1] > skimT.Jet_pt[idx2];
              });

    //-----------------------------------------
    // 5) Pick the leading and subleading jets
    //    (if they exist)
    //-----------------------------------------
    int iJet1 = -1;
    int iJet2 = -1;

    if (!candIndices.empty()) {
        iJet1 = candIndices[0];
    }
    if (candIndices.size() > 1) {
        iJet2 = candIndices[1];
    }

    printDebug("After picking top-2 pT jets: iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2));

    //-----------------------------------------
    // 6) Apply Jet ID (TightLepVeto >= 6)
    //-----------------------------------------
    if (iJet1 != -1 && skimT.Jet_jetId[iJet1] < 6) {
        printDebug("iJet1 = " + std::to_string(iJet1) + " fails JetID check -> reset to -1");
        iJet1 = -1;
    }
    if (iJet2 != -1 && skimT.Jet_jetId[iJet2] < 6) {
        printDebug("iJet2 = " + std::to_string(iJet2) + " fails JetID check -> reset to -1");
        iJet2 = -1;
    }

    printDebug("After JetID check: iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2));

    //-----------------------------------------
    // 7) Check \deltaR with reference object
    //    (e.g., photon or something else)
    //-----------------------------------------
    auto passDeltaR = [&](int jetIdx) {
        if (jetIdx < 0) return false; // invalid index
        TLorentzVector p4Jet;
        p4Jet.SetPtEtaPhiM(skimT.Jet_pt[jetIdx],
                           skimT.Jet_eta[jetIdx],
                           skimT.Jet_phi[jetIdx],
                           skimT.Jet_mass[jetIdx]);

        // Return true if dR >= 0.2 (i.e., pass)
        return (p4Ref.DeltaR(p4Jet) >= 0.2);
    };

    if (iJet1 != -1 && !passDeltaR(iJet1)) {
        printDebug("iJet1 = " + std::to_string(iJet1) + " fails dR check -> reset to -1");
        iJet1 = -1;
    }
    if (iJet2 != -1 && !passDeltaR(iJet2)) {
        printDebug("iJet2 = " + std::to_string(iJet2) + " fails dR check -> reset to -1");
        iJet2 = -1;
    }

    printDebug("After dR check: iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2));

    //-----------------------------------------
    // 8) Store the final picked jet indices
    //-----------------------------------------
    pickedJetsIndex_.push_back(iJet1);
    pickedJetsIndex_.push_back(iJet2);

    //-----------------------------------------
    // 9) Build the four-vectors: leading jet,
    //    subleading jet, and sum of the rest
    //-----------------------------------------
    TLorentzVector p4Jet1(0, 0, 0, 0);
    TLorentzVector p4Jet2(0, 0, 0, 0);
    TLorentzVector p4Jetn(0, 0, 0, 0);

    for (int i = 0; i < skimT.nJet; ++i) {
        TLorentzVector p4Jeti;
        p4Jeti.SetPtEtaPhiM(skimT.Jet_pt[i],
                            skimT.Jet_eta[i],
                            skimT.Jet_phi[i],
                            skimT.Jet_mass[i]);

        // If this is the chosen leading or subleading jet, store individually
        if (i == iJet1) {
            p4Jet1 = p4Jeti;
        } 
        else if (i == iJet2) {
            p4Jet2 = p4Jeti;
        } 
        // Otherwise, accumulate in p4Jetn
        else {
            p4Jetn += p4Jeti;
        }
    }

    pickedJetsP4_.push_back(p4Jet1);
    pickedJetsP4_.push_back(p4Jet2);
    pickedJetsP4_.push_back(p4Jetn);

    //-----------------------------------------
    // 10) Final debug info
    //-----------------------------------------
    printDebug("Final Jets: iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2));
    printDebug("pickJets: Done.");
}


void PickObject::pickJetsForFakeGamma(const SkimTree& skimT) {
    printDebug("pickJetsForFakeGamma: Starting Selection, nJet = " + std::to_string(skimT.nJet));

    pickedJetsIndex_.clear();
    pickedJetsP4_.clear();

    //-----------------------------------------
    // 1) Decide on pT threshold based on channel
    //-----------------------------------------
    float ptThreshold = 15.f; 
    float etaThreshold = 1.3;

    //-----------------------------------------
    // 2) Gather candidate jet indices 
    //    (pass minimal pT, skip photon jet if GamJet)
    //-----------------------------------------
    std::vector<int> candIndices;
    candIndices.reserve(skimT.nJet);

    for (int i = 0; i < skimT.nJet; ++i) {
        float pt = skimT.Jet_pt[i];
        if (pt < ptThreshold) {
            continue; 
        }
        if (std::abs(skimT.Jet_eta[i]) >= etaThreshold) {
            continue; 
        }
        candIndices.push_back(i);
    }

    //-----------------------------------------
    // 3) Sort the candidate jet indices by pT
    //    in descending order
    //-----------------------------------------
    std::sort(candIndices.begin(), candIndices.end(),
              [&](int idx1, int idx2) {
                  return skimT.Jet_pt[idx1] > skimT.Jet_pt[idx2];
              });

    //-----------------------------------------
    // 4) Pick the leading three jets
    //    (if they exist)
    //-----------------------------------------
    int iJet1 = -1;
    int iJet2 = -1;
    int iJet3 = -1;

    if (!candIndices.empty()) {
        iJet1 = candIndices[0];
    }
    if (candIndices.size() > 1) {
        iJet2 = candIndices[1];
    }
    if (candIndices.size() > 2) {
        iJet3 = candIndices[2];
    }

    printDebug("After picking top-3 pT jets: iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2) +  ", iJet3 = " + std::to_string(iJet3));

    //-----------------------------------------
    // 5) Apply Jet ID (TightLepVeto >= 6)
    //-----------------------------------------
    if (iJet1 != -1 && skimT.Jet_jetId[iJet1] < 6) {
        printDebug("iJet1 = " + std::to_string(iJet1) + " fails JetID check -> reset to -1");
        iJet1 = -1;
    }
    if (iJet2 != -1 && skimT.Jet_jetId[iJet2] < 6) {
        printDebug("iJet2 = " + std::to_string(iJet2) + " fails JetID check -> reset to -1");
        iJet2 = -1;
    }
    if (iJet3 != -1 && skimT.Jet_jetId[iJet3] < 6) {
        printDebug("iJet3 = " + std::to_string(iJet3) + " fails JetID check -> reset to -1");
        iJet3 = -1;
    }

    printDebug("After JetID check: iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2) +  ", iJet3 = " + std::to_string(iJet3));

    //-----------------------------------------
    // 6) Store the final picked jet indices
    //-----------------------------------------
    pickedJetsIndex_.push_back(iJet1);
    pickedJetsIndex_.push_back(iJet2);
    pickedJetsIndex_.push_back(iJet3);

    //-----------------------------------------
    // 7) Build the four-vectors: leading jet,
    //    subleading jet, and sum of the rest
    //-----------------------------------------
    TLorentzVector p4Jet1(0, 0, 0, 0);
    TLorentzVector p4Jet2(0, 0, 0, 0);
    TLorentzVector p4Jet3(0, 0, 0, 0);
    TLorentzVector p4Jetn(0, 0, 0, 0);

    for (int i = 0; i < skimT.nJet; ++i) {
        TLorentzVector p4Jeti;
        p4Jeti.SetPtEtaPhiM(skimT.Jet_pt[i],
                            skimT.Jet_eta[i],
                            skimT.Jet_phi[i],
                            skimT.Jet_mass[i]);

        // If this is the chosen leading or subleading jet, store individually
        if (i == iJet1) {
            p4Jet1 = p4Jeti;
        } 
        else if (i == iJet2) {
            p4Jet2 = p4Jeti;
        } 
        else if (i == iJet3) {
            p4Jet3 = p4Jeti;
        } 
        // Otherwise, accumulate in p4Jetn
        else {
            p4Jetn += p4Jeti;
        }
    }

    pickedJetsP4_.push_back(p4Jet1);
    pickedJetsP4_.push_back(p4Jet2);
    pickedJetsP4_.push_back(p4Jet3);
    pickedJetsP4_.push_back(p4Jetn);

    //-----------------------------------------
    // 10) Final debug info
    //-----------------------------------------
    printDebug("Final Jets: iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2) +  ", iJet3 = " + std::to_string(iJet3));
    printDebug("pickJetsForFakeGamma: Done.");
}

// Gen objects
void PickObject::pickGenMuons(const SkimTree& skimT) {
    printDebug("Starting Selection, nGenDressedLepton = "+std::to_string(skimT.nGenDressedLepton));

    for (int i = 0; i < skimT.nGenDressedLepton; ++i) {
        if (std::abs(skimT.GenDressedLepton_pdgId[i]) == 13) {
            pickedGenMuons_.push_back(i);
            printDebug("Gen Muon " + std::to_string(i) + " selected");
        }
    }

    printDebug("Total Gen Muons Selected: " + std::to_string(pickedGenMuons_.size()));
}

void PickObject::pickGenElectrons(const SkimTree& skimT) {
    printDebug("Starting Selection, nGenDressedLepton = "+std::to_string(skimT.nGenDressedLepton));

    for (int i = 0; i < skimT.nGenDressedLepton; ++i) {
        if (std::abs(skimT.GenDressedLepton_pdgId[i]) == 11) {
            pickedGenElectrons_.push_back(i);
            printDebug("Gen Electron " + std::to_string(i) + " selected");
        }
    }

    printDebug("Total Gen Electrons Selected: " + std::to_string(pickedGenElectrons_.size()));
}

void PickObject::pickGenPhotons(const SkimTree& skimT) {
    printDebug("Starting Selection, nGenIsolatedPhoton = "+std::to_string(skimT.nGenIsolatedPhoton));

    for (int i = 0; i < skimT.nGenIsolatedPhoton; ++i) {
        pickedGenPhotons_.push_back(i);
        printDebug("Gen Photon " + std::to_string(i) + " selected");
    }

    printDebug("Total Gen Photons Selected: " + std::to_string(pickedGenPhotons_.size()));
}

void PickObject::pickGenRefs(const SkimTree& skimT, const TLorentzVector& p4Ref) {
    // Z->ee + jets channel
    if (channel_ == GlobalFlag::Channel::ZeeJet && pickedGenElectrons_.size() > 1) {
        for (size_t j = 0; j < pickedGenElectrons_.size(); ++j) {
            for (size_t k = j + 1; k < pickedGenElectrons_.size(); ++k) {
                TLorentzVector p4Lep1, p4Lep2;
                int idx1 = pickedGenElectrons_[j];
                int idx2 = pickedGenElectrons_[k];

                p4Lep1.SetPtEtaPhiM(skimT.GenDressedLepton_pt[idx1],
                                    skimT.GenDressedLepton_eta[idx1],
                                    skimT.GenDressedLepton_phi[idx1],
                                    skimT.GenDressedLepton_mass[idx1]);

                p4Lep2.SetPtEtaPhiM(skimT.GenDressedLepton_pt[idx2],
                                    skimT.GenDressedLepton_eta[idx2],
                                    skimT.GenDressedLepton_phi[idx2],
                                    skimT.GenDressedLepton_mass[idx2]);

                TLorentzVector p4GenRef = p4Lep1 + p4Lep2;
                if(p4GenRef.DeltaR(p4Ref) > 0.2) continue;
                pickedGenRefs_.push_back(p4GenRef);
                printDebug("Gen Z->ee candidate selected with mass " + std::to_string(p4GenRef.M()));
            }
        }
    }

    // Z->mumu + jets channel
    else if (channel_ == GlobalFlag::Channel::ZmmJet && pickedGenMuons_.size() > 1) {
        for (size_t j = 0; j < pickedGenMuons_.size(); ++j) {
            for (size_t k = j + 1; k < pickedGenMuons_.size(); ++k) {
                TLorentzVector p4Lep1, p4Lep2;
                int idx1 = pickedGenMuons_[j];
                int idx2 = pickedGenMuons_[k];

                p4Lep1.SetPtEtaPhiM(skimT.GenDressedLepton_pt[idx1],
                                    skimT.GenDressedLepton_eta[idx1],
                                    skimT.GenDressedLepton_phi[idx1],
                                    skimT.GenDressedLepton_mass[idx1]);

                p4Lep2.SetPtEtaPhiM(skimT.GenDressedLepton_pt[idx2],
                                    skimT.GenDressedLepton_eta[idx2],
                                    skimT.GenDressedLepton_phi[idx2],
                                    skimT.GenDressedLepton_mass[idx2]);

                TLorentzVector p4GenRef = p4Lep1 + p4Lep2;
                if(p4GenRef.DeltaR(p4Ref) > 0.2) continue;
                pickedGenRefs_.push_back(p4GenRef);
                printDebug("Gen Z->mumu candidate selected with mass " + std::to_string(p4GenRef.M()));
            }
        }
    }

    // Gamma + jets channel
    else  if (channel_ == GlobalFlag::Channel::GamJet && !pickedGenPhotons_.empty()) {
        for (int idx : pickedGenPhotons_) {
            TLorentzVector p4GenRef;
            p4GenRef.SetPtEtaPhiM(skimT.GenIsolatedPhoton_pt[idx],
                                  skimT.GenIsolatedPhoton_eta[idx],
                                  skimT.GenIsolatedPhoton_phi[idx],
                                  skimT.GenIsolatedPhoton_mass[idx]);
            if(p4GenRef.DeltaR(p4Ref) > 0.2) continue;
            pickedGenRefs_.push_back(p4GenRef);
            printDebug("Gen Photon added to references: pt = " + std::to_string(skimT.GenIsolatedPhoton_pt[idx]));
        }
    }

    printDebug("Total Gen Reference Objects Selected: " + std::to_string(pickedGenRefs_.size()));
}

void PickObject::pickGenJets(const SkimTree& skimT, const int& iJet1, const int& iJet2, const TLorentzVector& p4Jet1, const TLorentzVector& p4Jet2) {
    printDebug("pickGenJets: Starting Selection, nJet = " + std::to_string(skimT.nJet));

    pickedGenJetsIndex_.clear();
    pickedGenJetsP4_.clear();

    // Select p4GenJet1 matching leading and subleading reco jet
    int iGenJet1(-1), iGenJet2(-1);
    TLorentzVector p4GenJeti, p4GenJet1, p4GenJet2;
    p4GenJet1.SetPtEtaPhiM(0, 0, 0, 0);
    p4GenJet2.SetPtEtaPhiM(0, 0, 0, 0);
    for (Int_t i = 0; i != skimT.nGenJet; ++i) {
        p4GenJeti.SetPtEtaPhiM(skimT.GenJet_pt[i], skimT.GenJet_eta[i],
                               skimT.GenJet_phi[i], skimT.GenJet_mass[i]);
        if (iJet1 != -1 && p4GenJeti.DeltaR(p4Jet1) < 0.4 && iGenJet1 == -1) {
            iGenJet1 = i;
            p4GenJet1 = p4GenJeti;
        } else if (iJet2 != -1 && p4GenJeti.DeltaR(p4Jet2) < 0.4 && iGenJet2 == -1) {
            iGenJet2 = i;
            p4GenJet2 = p4GenJeti;
        }
    }

    pickedGenJetsIndex_.push_back(iGenJet1);
    pickedGenJetsIndex_.push_back(iGenJet2);

    pickedGenJetsP4_.push_back(p4GenJet1);
    pickedGenJetsP4_.push_back(p4GenJet2);

    // debug info
    printDebug("Final Jets: iJet1 = " + std::to_string(iGenJet1) +
               ", iJet2 = " + std::to_string(iGenJet2));
}
