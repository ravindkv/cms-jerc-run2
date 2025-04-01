#pragma once

#include <iostream>
#include <TFile.h>
#include <TBranch.h>
#include <TTree.h>
#include <vector>
#include <map>
#include <chrono>
#include <memory>
#include <nlohmann/json.hpp>

#include "NanoTree.h"
#include "GlobalFlag.h"
#include "HistCutflow.h"
#include "Helper.h"
#include "ReadConfig.h" // Include the new ReadConfig

class RunChannel {
public:
    explicit RunChannel(GlobalFlag& globalFlags);
    virtual ~RunChannel() = default;

    // Implements the full processing; derived classes only need to provide configuration details.
    virtual int Run(std::shared_ptr<NanoTree>& nanoT, ReadConfig &readConfig, TFile* fout);

protected:
    GlobalFlag& globalFlags_;
    std::vector<std::string> filterList_;
    std::map<std::string, Bool_t> filterVals_;
    std::map<std::string, TBranch*> filterTBranches_;

    std::vector<std::string> trigList_;
    std::map<std::string, Bool_t> trigVals_;
    std::map<std::string, TBranch*> trigTBranches_;

    // Event loop
    void runEventLoop(NanoTree* nanoT, TFile* fout);

};

