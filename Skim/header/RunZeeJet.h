#ifndef RUNZEEJET_H
#define RUNZEEJET_H

#include <iostream>
#include <TFile.h>

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

#endif // RUNZEEJET_H

