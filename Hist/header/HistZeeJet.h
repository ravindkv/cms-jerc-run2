#ifndef HISTZEEJET_H
#define HISTZEEJET_H

#include <memory>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cmath>

// ROOT includes
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TRandom3.h>

// User-defined includes
#include "SkimTree.h"
#include "EventPick.h"
#include "ObjectPick.h"
#include "ObjectScale.h"
#include "GlobalFlag.h"

class HistZeeJet{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit HistZeeJet(GlobalFlag& globalFlags);
    ~HistZeeJet() = default;

    int Run(std::shared_ptr<SkimTree>& skimT, EventPick* eventP, ObjectPick* objP, ObjectScale* objS, TFile* fout);

private:
    bool smearJets_ = false;
    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;

    // Add any private member variables or methods here if needed
};

#endif // HISTZEEJET_H

