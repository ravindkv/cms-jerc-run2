#include "PickObjectZeeJet.h"
#include "ReadConfig.h"

// Constructor implementation
PickObjectZeeJet::PickObjectZeeJet(GlobalFlag& globalFlags) :
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug())
{
    loadConfig("config/PickObjectZeeJet.json");
}

// Destructor
PickObjectZeeJet::~PickObjectZeeJet() {
    // Cleanup if necessary
}

// Load configuration from JSON file and store values in private members
void PickObjectZeeJet::loadConfig(const std::string& filename) {
    ReadConfig config(filename);

    // Electron pick configuration
    minPtEle_       = config.getValue<double>({"electronPick", "minPt"});
    maxEtaEle_      = config.getValue<double>({"electronPick", "maxEta"});
    tightIdEle_     = config.getValue<int>({"electronPick", "tightId"});
    minEbEeGap_     = config.getValue<double>({"electronPick", "ebEeGap", "min"});
    maxEbEeGap_     = config.getValue<double>({"electronPick", "ebEeGap", "max"});

    // Reference pick configuration
    massRef_        = config.getValue<double>({"referencePick", "mass"});
    massWindowRef_  = config.getValue<double>({"referencePick", "massWindow"});
    minPtRef_       = config.getValue<double>({"referencePick", "minPt"});

    // Jet pick configuration
    minPtJet_           = config.getValue<double>({"jetPick", "minPt"});
    maxEtaLeadingJet_   = config.getValue<double>({"jetPick", "maxEtaLeading"});
    Jet_electronIdx1_   = config.getValue<int>({"jetPick", "Jet_electronIdx1"});
    Jet_electronIdx2_   = config.getValue<int>({"jetPick", "Jet_electronIdx2"});
    minIdJet_           = config.getValue<int>({"jetPick", "minId"});

    // Gen Electron pick configuration
    pdgIdGenEle_ = config.getValue<int>({"genElectronPick", "pdgId"});

    // Gen Reference pick configuration
    maxDeltaRgenRef_ = config.getValue<double>({"genReferencePick", "maxDeltaR"});

    // Gen Jet pick configuration
    maxDeltaRgenJet_ = config.getValue<double>({"genJetPick", "maxDeltaR"});
}

// Helper function for debug printing
void PickObjectZeeJet::printDebug(const std::string& message) const {
    if (isDebug_) {
        std::cout << message << '\n';
    }
}

void PickObjectZeeJet::pickElectrons(const SkimTree& skimT) {
    printDebug("Starting pickElectrons, nElectron = " + std::to_string(skimT.nElectron));
    pickedElectrons_.clear();

    for (int eleInd = 0; eleInd < skimT.nElectron; ++eleInd) {
        double eta = skimT.Electron_eta[eleInd];
        double absEta = std::abs(eta);
        double SCeta = eta + skimT.Electron_deltaEtaSC[eleInd];
        double absSCEta = std::abs(SCeta);
        double pt = skimT.Electron_pt[eleInd];

        // Check that the supercluster does not fall within the barrel-endcap gap.
        bool passEtaEBEEGap = (absSCEta < minEbEeGap_) || (absSCEta > maxEbEeGap_);
        // Tight electron ID check
        bool passTightID = (skimT.Electron_cutBased[eleInd] == tightIdEle_);

        bool eleSel = (passEtaEBEEGap && absEta <= maxEtaEle_ && pt >= minPtEle_ && passTightID);
        if (eleSel) {
            pickedElectrons_.push_back(eleInd);
            printDebug("Electron " + std::to_string(eleInd) + " selected: pt = " +
                       std::to_string(pt) + ", eta = " + std::to_string(eta));
        } else {
            printDebug("Electron " + std::to_string(eleInd) + " rejected: pt = " +
                       std::to_string(pt) + ", eta = " + std::to_string(eta));
        }
    }

    printDebug("Total Electrons Picked: " + std::to_string(pickedElectrons_.size()));
}

// Reference object picking (e.g., for Z->ee)
void PickObjectZeeJet::pickRefs(const SkimTree& skimT) {
    pickedRefs_.clear();
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
            std::abs(p4Ref.M() - massRef_) < massWindowRef_ &&
            p4Ref.Pt() > minPtRef_) {
            pickedRefs_.push_back(p4Ref);
            printDebug("Z->ee candidate picked with mass " + std::to_string(p4Ref.M()));
        }
    }

    printDebug("Total Reference Objects Picked: " + std::to_string(pickedRefs_.size()));
}


void PickObjectZeeJet::pickJets(const SkimTree& skimT, const TLorentzVector& p4Ref) {
    printDebug("pickJets: Starting, nJet = " + std::to_string(skimT.nJet));

    pickedJetsIndex_.clear();
    pickedJetsP4_.clear();

    // Gather candidate jet indices based on minimum Pt and maximum Eta from config.
    std::vector<int> candIndices;
    candIndices.reserve(skimT.nJet);

    for (int i = 0; i < skimT.nJet; ++i) {
        float pt = skimT.Jet_pt[i];
        if (pt < minPtJet_) continue;
        if(skimT.Jet_electronIdx1[i] != Jet_electronIdx1_) continue;
        if(skimT.Jet_electronIdx2[i] != Jet_electronIdx2_) continue;
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


void PickObjectZeeJet::pickGenElectrons(const SkimTree& skimT) {
    pickedGenElectrons_.clear();
    printDebug("Starting pickGenElectrons, nGenDressedLepton = " + std::to_string(skimT.nGenDressedLepton));

    for (int i = 0; i < skimT.nGenDressedLepton; ++i) {
        if (std::abs(skimT.GenDressedLepton_pdgId[i]) == pdgIdGenEle_) {
            pickedGenElectrons_.push_back(i);
            printDebug("Gen Electron " + std::to_string(i) + " picked");
        }
    }

    printDebug("Total Gen Electrons Picked: " + std::to_string(pickedGenElectrons_.size()));
}

void PickObjectZeeJet::pickGenRefs(const SkimTree& skimT, const TLorentzVector& p4Ref) {
    pickedGenRefs_.clear();
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
                if (p4GenRef.DeltaR(p4Ref) > maxDeltaRgenRef_) continue;
                pickedGenRefs_.push_back(p4GenRef);
                printDebug("Gen Z->ee candidate picked with mass " + std::to_string(p4GenRef.M()));
            }
        }
    }

    printDebug("Total Gen Reference Objects Picked: " + std::to_string(pickedGenRefs_.size()));
}

void PickObjectZeeJet::pickGenJets(const SkimTree& skimT, const int& iJet1, const int& iJet2,
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

