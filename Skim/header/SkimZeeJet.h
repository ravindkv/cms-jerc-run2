#ifndef SKIMZEEJET_H
#define SKIMZEEJET_H

#include <iostream>
#include <ctime>
#include <iomanip>

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TH1D.h>
#include <TProfile.h>
#include <TStopwatch.h>

// User-defined includes
#include "NanoTree.h"
#include "GlobalFlag.h"

class SkimZeeJet{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit SkimZeeJet(GlobalFlag& globalFlags);
    ~SkimZeeJet() = default;

    int Run(std::shared_ptr<NanoTree>& skimT, TFile* fout);

private:
    bool smearJets_ = false;
    GlobalFlag& globalFlags_;
};

#endif // SKIMZEEJET_H

