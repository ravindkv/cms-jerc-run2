#ifndef RUNZEEJET_H
#define RUNZEEJET_H

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

class RunZeeJet{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit RunZeeJet(GlobalFlag& globalFlags);
    ~RunZeeJet() = default;

    int Run(std::shared_ptr<SkimTree>& skimT, PickEvent* pickEvent, ScaleEvent* scaleEvent, ScaleObject* scaleObject, TFile* fout);

private:
    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;

    // Configuration parameters loaded from JSON
    std::vector<std::string> cuts_;
    std::vector<int> minRefPts_;
    int minElectron_; 
    int maxElectron_; 
    int minJet_; 
    double maxDphi_;
    double maxAlpha_;
    double maxRefEta_;
    double minDbResp_;
    double minMpfResp_;

    // Method to load the configuration.
    void loadConfig(const std::string& filename);
};

#endif // RUNZEEJET_H

