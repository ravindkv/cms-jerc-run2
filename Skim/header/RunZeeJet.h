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

class RunZeeJet{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit RunZeeJet(GlobalFlag& globalFlags);
    ~RunZeeJet() = default;

    int Run(std::shared_ptr<NanoTree>& skimT, TFile* fout);

private:
    bool smearJets_ = false;
    GlobalFlag& globalFlags_;

    std::vector<std::string> trigList_;
    std::map<std::string, Bool_t> trigVals_;
    std::map<std::string, TBranch*> trigTBranches_;


};

#endif // SKIMZEEJET_H

