
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

class RunBase {
public:
    explicit RunBase(GlobalFlag& globalFlags);
    virtual ~RunBase() = default;

    // Implements the full processing; derived classes only need to provide configuration details.
    virtual int Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout);

protected:
    GlobalFlag& globalFlags_;
    std::vector<std::string> filterList_;
    std::map<std::string, Bool_t> filterVals_;
    std::map<std::string, TBranch*> filterTBranches_;

    std::vector<std::string> trigList_;
    std::map<std::string, Bool_t> trigVals_;
    std::map<std::string, TBranch*> trigTBranches_;

    // Load configuration file (JSON)
    nlohmann::json loadConfig(const std::string& configPath);

    // Derived classes must provide the config file path
    virtual std::string getConfigPath() const = 0;

    // Set branch statuses from config
    void setBranches(NanoTree* nanoT, const nlohmann::json& config);
    // Set filter, trigger list and branch addresses from config
    void setFilters(NanoTree* nanoT, const nlohmann::json& config);
    void setTriggers(NanoTree* nanoT, const nlohmann::json& config);
    // Run the common event loop that fills the output tree
    void runEventLoop(NanoTree* nanoT, TTree* newTree);
};

