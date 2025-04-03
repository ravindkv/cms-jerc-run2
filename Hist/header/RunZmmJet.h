
#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

// ROOT includes
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TLorentzVector.h>

// User-defined includes
#include "SkimTree.h"
#include "PickEvent.h"
#include "ScaleEvent.h"
#include "ScaleObject.h"
#include "GlobalFlag.h"
#include "VarCut.h"

class RunZmmJet{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit RunZmmJet(GlobalFlag& globalFlags);
    ~RunZmmJet() = default;

    int Run(std::shared_ptr<SkimTree>& skimT, PickEvent* pickEvent, ScaleEvent* scaleEvent, ScaleObject* scaleObject, TFile* fout);

private:
    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;

    // Configuration parameters loaded from JSON
    std::vector<std::string> cutflows_;
    std::vector<int> minRefPts_;
    int nMuonMin_; 
    int nMuonMax_; 
    double maxDeltaPhiRefJet1_;
    double maxAlpha_;
    double maxRefEta_;
    std::vector<double> alphaCuts_;
    double minResp_;
    double maxResp_;

    // Method to load the configuration.
    void loadConfig(const std::string& filename);
};

