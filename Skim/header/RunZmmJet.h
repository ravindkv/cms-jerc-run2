#ifndef RUNZMMJET_H
#define RUNZMMJET_H

#include <iostream>
#include <TFile.h>

#include "NanoTree.h"
#include "GlobalFlag.h"

class RunZmmJet{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit RunZmmJet(GlobalFlag& globalFlags);
    ~RunZmmJet() = default;

    int Run(std::shared_ptr<NanoTree>& skimT, TFile* fout);

private:
    bool smearJets_ = false;
    GlobalFlag& globalFlags_;

    std::vector<std::string> trigList_;
    std::map<std::string, Bool_t> trigVals_;
    std::map<std::string, TBranch*> trigTBranches_;
};

#endif // RUNZMMJET_H

