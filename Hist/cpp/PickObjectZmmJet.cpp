#include "PickObjectZmmJet.h"
#include "ReadConfig.h"

// Constructor implementation
PickObjectZmmJet::PickObjectZmmJet(GlobalFlag& globalFlags) :
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug())
{
    loadConfig("config/PickObjectZmmJet.json");
}

// Destructor
PickObjectZmmJet::~PickObjectZmmJet() {
    // Cleanup if necessary
}

// Load configuration from JSON file and store values in private members
void PickObjectZmmJet::loadConfig(const std::string& filename) {
    ReadConfig config(filename);

    // Muon pick configuration
    minPtMu_      = config.getValue<double>({"muonPick", "minPt"});
    maxEtaMu_     = config.getValue<double>({"muonPick", "maxEta"});
    tightIdMu_    = config.getValue<int>({"muonPick", "tightId"});
    maxRelIsoMu_  = config.getValue<double>({"muonPick", "maxRelIso"});
    maxDxyMu_     = config.getValue<double>({"muonPick", "maxDxy"});
    maxDzMu_      = config.getValue<double>({"muonPick", "maxDz"});

    // Reference pick configuration
    massRef_       = config.getValue<double>({"referencePick", "mass"});
    massWindowRef_ = config.getValue<double>({"referencePick", "massWindow"});
    minPtRef_      = config.getValue<double>({"referencePick", "minPt"});

    // Jet pick configuration
    minPtJet_           = config.getValue<double>({"jetPick", "minPt"});
    maxEtaLeadingJet_   = config.getValue<double>({"jetPick", "maxEtaLeading"});
    Jet_muonIdx1_       = config.getValue<int>({"jetPick", "Jet_muonIdx1"});
    Jet_muonIdx2_       = config.getValue<int>({"jetPick", "Jet_muonIdx2"});
    minIdJet_           = config.getValue<int>({"jetPick", "minId"});

    // Gen Muon pick configuration
    pdgIdGenMu_ = config.getValue<int>({"genMuonPick", "pdgId"});

    // Gen Reference pick configuration
    maxDeltaRgenRef_ = config.getValue<double>({"genReferencePick", "maxDeltaR"});

    // Gen Jet pick configuration
    maxDeltaRgenJet_ = config.getValue<double>({"genJetPick", "maxDeltaR"});
}

// Helper function for debug printing
void PickObjectZmmJet::printDebug(const std::string& message) const {
    if (isDebug_) {
        std::cout << message << '\n';
    }
}

void PickObjectZmmJet::pickMuons(const SkimTree& skimT) {
    printDebug("Starting pickMuons, nMuon = " + std::to_string(skimT.nMuon));
    pickedMuons_.clear();

    for (int muInd = 0; muInd < skimT.nMuon; ++muInd) {
        double eta = skimT.Muon_eta[muInd];
        double pt = skimT.Muon_pt[muInd];

        bool muSel = (std::abs(eta) <= maxEtaMu_ &&
             pt >= minPtMu_ &&
             skimT.Muon_tightId[muInd] == tightIdMu_ &&
             skimT.Muon_pfRelIso04_all[muInd] < maxRelIsoMu_ &&
             skimT.Muon_dxy[muInd] < maxDxyMu_ &&
             skimT.Muon_dz[muInd] < maxDzMu_);

        if (muSel) {
            pickedMuons_.push_back(muInd);
            printDebug("Muon " + std::to_string(muInd) + " smucted: pt = " +
                       std::to_string(pt) + ", eta = " + std::to_string(eta));
        } else {
            printDebug("Muon " + std::to_string(muInd) + " rejected: pt = " +
                       std::to_string(pt) + ", eta = " + std::to_string(eta));
        }
    }

    printDebug("Total Muons Picked: " + std::to_string(pickedMuons_.size()));
}

// Reference object picking (e.g., for Z->ee)
void PickObjectZmmJet::pickRefs(const SkimTree& skimT) {
    pickedRefs_.clear();
    if (channel_ == GlobalFlag::Channel::ZmmJet && pickedMuons_.size() > 1) {
        int j = pickedMuons_.at(0);
        int k = pickedMuons_.at(1);

        TLorentzVector p4Lep1, p4Lep2;
        double ptj = skimT.Muon_pt[j];
        double ptk = skimT.Muon_pt[k];
        p4Lep1.SetPtEtaPhiM(ptj, skimT.Muon_eta[j], skimT.Muon_phi[j], skimT.Muon_mass[j]);
        p4Lep2.SetPtEtaPhiM(ptk, skimT.Muon_eta[k], skimT.Muon_phi[k], skimT.Muon_mass[k]);
        TLorentzVector p4Ref = p4Lep1 + p4Lep2;

        if ((skimT.Muon_charge[j] * skimT.Muon_charge[k]) == -1 &&
            std::abs(p4Ref.M() - massRef_) < massWindowRef_ &&
            p4Ref.Pt() > minPtRef_) {
            pickedRefs_.push_back(p4Ref);
            printDebug("Z->ee candidate picked with mass " + std::to_string(p4Ref.M()));
        }
    }

    printDebug("Total Reference Objects Picked: " + std::to_string(pickedRefs_.size()));
}


void PickObjectZmmJet::pickJets(const SkimTree& skimT, const TLorentzVector& p4Ref) {
    printDebug("pickJets: Starting, nJet = " + std::to_string(skimT.nJet));

    pickedJetsIndex_.clear();
    pickedJetsP4_.clear();

    // Gather candidate jet indices based on minimum Pt and maximum Eta from config.
    std::vector<int> candIndices;
    candIndices.reserve(skimT.nJet);

    for (int i = 0; i < skimT.nJet; ++i) {
        float pt = skimT.Jet_pt[i];
        if (pt < minPtJet_) continue;
        if(skimT.Jet_muonIdx1[i] != Jet_muonIdx1_) continue;
        if(skimT.Jet_muonIdx2[i] != Jet_muonIdx2_) continue;
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


void PickObjectZmmJet::pickGenMuons(const SkimTree& skimT) {
    pickedGenMuons_.clear();
    printDebug("Starting pickGenMuons, nGenDressedLepton = " + std::to_string(skimT.nGenDressedLepton));

    for (int i = 0; i < skimT.nGenDressedLepton; ++i) {
        if (std::abs(skimT.GenDressedLepton_pdgId[i]) == pdgIdGenMu_) {
            pickedGenMuons_.push_back(i);
            printDebug("Gen Muon " + std::to_string(i) + " picked");
        }
    }

    printDebug("Total Gen Muons Picked: " + std::to_string(pickedGenMuons_.size()));
}

void PickObjectZmmJet::pickGenRefs(const SkimTree& skimT, const TLorentzVector& p4Ref) {
    pickedGenRefs_.clear();
    if (channel_ == GlobalFlag::Channel::ZmmJet && pickedGenMuons_.size() > 1) {
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
                if (p4GenRef.DeltaR(p4Ref) > maxDeltaRgenRef_) continue;
                pickedGenRefs_.push_back(p4GenRef);
                printDebug("Gen Z->ee candidate picked with mass " + std::to_string(p4GenRef.M()));
            }
        }
    }

    printDebug("Total Gen Reference Objects Picked: " + std::to_string(pickedGenRefs_.size()));
}

void PickObjectZmmJet::pickGenJets(const SkimTree& skimT, const int& iJet1, const int& iJet2,
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

