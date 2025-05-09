#pragma once

#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>

#include <TH1F.h>
#include <TH1D.h>
#include <TLorentzVector.h>

#include "SkimTree.h"
#include "GlobalFlag.h"

class PickObjectZeeJet{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit PickObjectZeeJet(GlobalFlag& globalFlags);
    ~PickObjectZeeJet();

    // Reco objects
    void pickElectrons(const SkimTree& skimT);
    void pickRefs(const SkimTree& skimT);
    void pickJets(const SkimTree& skimT, const TLorentzVector& p4Ref);
    void pickGenJets(const SkimTree& skimT, const int& iJet1, const int& iJet2,
                     const TLorentzVector& p4Jet1, const TLorentzVector& p4Jet2);

    // Gen objects
    void pickGenElectrons(const SkimTree& skimT);
    void pickGenRefs(const SkimTree& skimT, const TLorentzVector& p4Ref);

    // Accessors for picked objects
    const std::vector<int>& getPickedElectrons() const { return pickedElectrons_; }
    const std::vector<TLorentzVector>& getPickedRefs() const { return pickedRefs_; }
    const std::vector<TLorentzVector>& getPickedJetsP4() const { return pickedJetsP4_; }
    const std::vector<int>& getPickedJetsIndex() const { return pickedJetsIndex_; }

    const std::vector<int>& getPickedGenElectrons() const { return pickedGenElectrons_; }
    const std::vector<TLorentzVector>& getPickedGenRefs() const { return pickedGenRefs_; }
    const std::vector<TLorentzVector>& getPickedGenJetsP4() const { return pickedGenJetsP4_; }
    const std::vector<int>& getPickedGenJetsIndex() const { return pickedGenJetsIndex_; }

private:
    // Reco objects
    std::vector<int> pickedElectrons_;
    std::vector<TLorentzVector> pickedRefs_;
    std::vector<TLorentzVector> pickedJetsP4_;
    std::vector<int> pickedJetsIndex_;

    // Gen objects
    std::vector<int> pickedGenElectrons_;
    std::vector<TLorentzVector> pickedGenRefs_;
    std::vector<TLorentzVector> pickedGenJetsP4_;
    std::vector<int> pickedGenJetsIndex_;

    // Configuration parameters (loaded from JSON)
    // Electron pick config
    double minPtEle_;
    double maxEtaEle_;
    int tightIdEle_;
    double minEbEeGap_;
    double maxEbEeGap_;

    // Reference pick config
    double massRef_;
    double massWindowRef_;
    double minPtRef_;

    // Jet pick config
    double minPtJet_;
    double maxEtaLeadingJet_;
    int Jet_electronIdx1_;
    int Jet_electronIdx2_;
    int minIdJet_;

    // Gen Electron pick config
    int pdgIdGenEle_;

    // Gen Reference pick config
    double maxDeltaRgenRef_;

    // Gen Jet pick config
    double maxDeltaRgenJet_;

    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;

    // Helper function for debug printing
    void printDebug(const std::string& message) const;

    // Load configuration from JSON file
    void loadConfig(const std::string& filename);
};

