#ifndef RUNWQQM_H
#define RUNWQQM_H

#include <iostream>
#include <TFile.h>

#include "NanoTree.h"
#include "GlobalFlag.h"

class RunWqqm{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit RunWqqm(GlobalFlag& globalFlags);
    ~RunWqqm() = default;

    int Run(std::shared_ptr<NanoTree>& skimT, TFile* fout);

private:
    GlobalFlag& globalFlags_;

    std::vector<std::string> trigList_;
    std::map<std::string, Bool_t> trigVals_;
    std::map<std::string, TBranch*> trigTBranches_;
};

#endif // RUNWQQM_H

