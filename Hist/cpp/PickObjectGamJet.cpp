#include "PickObjectGamJet.h"
#include "ReadConfig.h"

// Constructor implementation
PickObjectGamJet::PickObjectGamJet(GlobalFlag& globalFlags) :
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug())
{
    loadConfig("config/PickObjectGamJet.json");
}

// Destructor
PickObjectGamJet::~PickObjectGamJet() {
    // Cleanup if necessary
}

// Load configuration from JSON file and store values in private members
void PickObjectGamJet::loadConfig(const std::string& filename) {
    ReadConfig config(filename);

    // Photon pick configuration
    minPtPho_       = config.getValue<double>({"photonPick", "minPt"});
    maxEtaPho_      = config.getValue<double>({"photonPick", "maxEta"});
    tightIdPho_     = config.getValue<int>({"photonPick", "tightId"});
    minR9Pho_       = config.getValue<double>({"photonPick", "minR9"});
    maxR9Pho_       = config.getValue<double>({"photonPick", "maxR9"});
    maxHoePho_      = config.getValue<double>({"photonPick", "maxHoe"});

    // Jet pick configuration
    minPtJet_           = config.getValue<double>({"jetPick", "minPt"});
    maxEtaLeadingJet_   = config.getValue<double>({"jetPick", "maxEtaLeading"});
    minIdJet_           = config.getValue<int>({"jetPick", "minId"});
    minDeltaRrefJet_    = config.getValue<double>({"jetPick", "minDeltaR"});

    // Gen Photon pick configuration
    pdgIdGenPho_ = config.getValue<int>({"genPhotonPick", "pdgId"});

    // Gen Reference pick configuration
    maxDeltaRgenRef_ = config.getValue<double>({"genReferencePick", "maxDeltaR"});

    // Gen Jet pick configuration
    maxDeltaRgenJet_ = config.getValue<double>({"genJetPick", "maxDeltaR"});
}

// Helper function for debug printing
void PickObjectGamJet::printDebug(const std::string& message) const {
    if (isDebug_) {
        std::cout << message << '\n';
    }
}

// Photon selection
void PickObjectGamJet::pickPhotons(const SkimTree& skimT) {
    printDebug("Starting Selection, nPhoton = "+std::to_string(skimT.nPhoton));
    pickedPhotons_.clear();

    for (int phoInd = 0; phoInd < skimT.nPhoton; ++phoInd) {
        double pt  = skimT.Photon_pt[phoInd];
        double absEta = std::abs(skimT.Photon_eta[phoInd]);
        double r9  = skimT.Photon_r9[phoInd];
        double hoe = skimT.Photon_hoe[phoInd];
        Int_t id = skimT.Photon_cutBased[phoInd];  // Tight ID
        // minR9 to avoid bias wrt R9Id90 triggers and from photon conversions
        if(pt > minPtPho_ && absEta < maxEtaPho_ && r9 > minR9Pho_ && r9 < maxR9Pho_ && hoe < maxHoePho_ && id==tightIdPho_){
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

// Reference object picking (e.g., for Z->ee)
void PickObjectGamJet::pickRefs(const SkimTree& skimT) {
    pickedRefs_.clear();
    for (int idx : pickedPhotons_) {
        TLorentzVector p4Pho;
        p4Pho.SetPtEtaPhiM(skimT.Photon_pt[idx], skimT.Photon_eta[idx], skimT.Photon_phi[idx], skimT.Photon_mass[idx]);
        pickedRefs_.push_back(p4Pho);
        printDebug("Photon index added to references  = " + std::to_string(idx));
    }
    printDebug("Total Reference Objects Selected: " + std::to_string(pickedRefs_.size()));
}


void PickObjectGamJet::pickJets(const SkimTree& skimT, const TLorentzVector& p4Ref) {
    printDebug("pickJets: Starting, nJet = " + std::to_string(skimT.nJet));

    pickedJetsIndex_.clear();
    pickedJetsP4_.clear();

    // Gather candidate jet indices based on minimum Pt and maximum Eta from config.
    std::vector<int> candIndices;
    candIndices.reserve(skimT.nJet);

    //-----------------------------------------
    // Identify the photon->jet index if needed
    //-----------------------------------------
    int phoJetIdx = -1;
    if (!pickedPhotons_.empty()) {
        int phoInd   = pickedPhotons_.at(0);
        phoJetIdx    = skimT.Photon_jetIdx[phoInd];
        printDebug("GamJet channel: photon->jet index = " + std::to_string(phoJetIdx));
    }
    for (int i = 0; i < skimT.nJet; ++i) {
        float pt = skimT.Jet_pt[i];
        if (pt < minPtJet_) continue;
        if(i == phoJetIdx) continue;
        candIndices.push_back(i);
    }

    // Sort candidate jets by Pt (descending)
    std::sort(candIndices.begin(), candIndices.end(),
              [&](int idx1, int idx2) {
                  return skimT.Jet_pt[idx1] > skimT.Jet_pt[idx2];
              });

    int iJet1 = -1;
    int iJet2 = -1;
    if (!candIndices.empty()) {
        iJet1 = candIndices[0];
    }
    if (candIndices.size() > 1) {
        iJet2 = candIndices[1];
    }

    printDebug("After picking top-2 Pt jets: iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2));

    // Apply Jet ID criteria from config on leading jet
    if (iJet1 != -1 && skimT.Jet_jetId[iJet1] < minIdJet_) {
        printDebug("iJet1 = " + std::to_string(iJet1) + " fails Jet ID check -> reset to -1");
        iJet1 = -1;
    }
    printDebug("After Jet ID check on ONLY iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2));

    // Apply eta criteria from config on leading jet
    if (iJet1 != -1 && std::abs(skimT.Jet_eta[iJet1]) > maxEtaLeadingJet_) {
        printDebug("iJet1 = " + std::to_string(iJet1) + " fails eta check -> reset to -1");
        iJet1 = -1;
    }
    printDebug("After eta check on ONLY iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2));
    
    //-----------------------------------------
    //    Check \deltaR with reference object
    //    (e.g., photon or something else)
    //-----------------------------------------
    auto passDeltaR = [&](int jetIdx) {
        if (jetIdx < 0) return false; // invalid index
        TLorentzVector p4Jet;
        p4Jet.SetPtEtaPhiM(skimT.Jet_pt[jetIdx],
                           skimT.Jet_eta[jetIdx],
                           skimT.Jet_phi[jetIdx],
                           skimT.Jet_mass[jetIdx]);

        // Return true if dR >= minDeltaR (i.e., pass)
        return (p4Ref.DeltaR(p4Jet) >= minDeltaRrefJet_);
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

    pickedJetsIndex_.push_back(iJet1);
    pickedJetsIndex_.push_back(iJet2);

    // Build four-vectors for the leading, subleading, and the remainder jets.
    TLorentzVector p4Jet1Vec(0, 0, 0, 0);
    TLorentzVector p4Jet2Vec(0, 0, 0, 0);
    TLorentzVector p4JetN(0, 0, 0, 0);

    for (int i = 0; i < skimT.nJet; ++i) {
        TLorentzVector p4Jeti;
        p4Jeti.SetPtEtaPhiM(skimT.Jet_pt[i],
                            skimT.Jet_eta[i],
                            skimT.Jet_phi[i],
                            skimT.Jet_mass[i]);
        if (i == iJet1) {
            p4Jet1Vec = p4Jeti;
        } else if (i == iJet2) {
            p4Jet2Vec = p4Jeti;
        } else {
            p4JetN += p4Jeti;
        }
    }

    pickedJetsP4_.push_back(p4Jet1Vec);
    pickedJetsP4_.push_back(p4Jet2Vec);
    pickedJetsP4_.push_back(p4JetN);

    printDebug("Final Jets: iJet1 = " + std::to_string(iJet1) +
               ", iJet2 = " + std::to_string(iJet2));
    printDebug("pickJets: Done.");
}


void PickObjectGamJet::pickGenPhotons(const SkimTree& skimT) {
    printDebug("Starting Selection, nGenIsolatedPhoton = "+std::to_string(skimT.nGenIsolatedPhoton));
    pickedGenPhotons_.clear();

    for (int i = 0; i < skimT.nGenIsolatedPhoton; ++i) {
        pickedGenPhotons_.push_back(i);
        printDebug("Gen Photon " + std::to_string(i) + " selected");
    }

    printDebug("Total Gen Photons Selected: " + std::to_string(pickedGenPhotons_.size()));
}

void PickObjectGamJet::pickGenRefs(const SkimTree& skimT, const TLorentzVector& p4Ref) {
    pickedGenRefs_.clear();
    if (channel_ == GlobalFlag::Channel::GamJet && !pickedGenPhotons_.empty()) {
        for (int idx : pickedGenPhotons_) {
            TLorentzVector p4GenRef;
            p4GenRef.SetPtEtaPhiM(skimT.GenIsolatedPhoton_pt[idx],
                                  skimT.GenIsolatedPhoton_eta[idx],
                                  skimT.GenIsolatedPhoton_phi[idx],
                                  skimT.GenIsolatedPhoton_mass[idx]);
            if(p4GenRef.DeltaR(p4Ref) > maxDeltaRgenRef_) continue;
            pickedGenRefs_.push_back(p4GenRef);
            printDebug("Gen Photon added to references: pt = " + std::to_string(skimT.GenIsolatedPhoton_pt[idx]));
        }
    }

    printDebug("Total Gen Reference Objects Picked: " + std::to_string(pickedGenRefs_.size()));
}

void PickObjectGamJet::pickGenJets(const SkimTree& skimT, const int& iJet1, const int& iJet2,
                             const TLorentzVector& p4Jet1, const TLorentzVector& p4Jet2) {
    printDebug("pickGenJets: Starting, nJet = " + std::to_string(skimT.nGenJet));

    pickedGenJetsIndex_.clear();
    pickedGenJetsP4_.clear();

    int iGenJet1 = -1, iGenJet2 = -1;
    TLorentzVector p4GenJet1(0, 0, 0, 0), p4GenJet2(0, 0, 0, 0);
    TLorentzVector p4GenJeti;
    for (Int_t i = 0; i < skimT.nGenJet; ++i) {
        p4GenJeti.SetPtEtaPhiM(skimT.GenJet_pt[i], skimT.GenJet_eta[i],
                               skimT.GenJet_phi[i], skimT.GenJet_mass[i]);
        if (iJet1 != -1 && p4GenJeti.DeltaR(p4Jet1) < maxDeltaRgenJet_ && iGenJet1 == -1) {
            iGenJet1 = i;
            p4GenJet1 = p4GenJeti;
        } else if (iJet2 != -1 && p4GenJeti.DeltaR(p4Jet2) < maxDeltaRgenJet_ && iGenJet2 == -1) {
            iGenJet2 = i;
            p4GenJet2 = p4GenJeti;
        }
    }

    pickedGenJetsIndex_.push_back(iGenJet1);
    pickedGenJetsIndex_.push_back(iGenJet2);
    pickedGenJetsP4_.push_back(p4GenJet1);
    pickedGenJetsP4_.push_back(p4GenJet2);

    printDebug("Final Gen Jets: iGenJet1 = " + std::to_string(iGenJet1) +
               ", iGenJet2 = " + std::to_string(iGenJet2));
}

