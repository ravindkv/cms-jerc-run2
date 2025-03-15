#include "RunBase.h"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

RunBase::RunBase(GlobalFlag& globalFlags)
    : globalFlags_(globalFlags) {
}

nlohmann::json RunBase::loadConfig(const std::string& configPath) {
    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Could not open config file: " + configPath);
    }
    json config;
    configFile >> config;
    return config;
}

void RunBase::setFilters(NanoTree* nanoT, const nlohmann::json& commonConfig) {
    // Set filter list based on the run period
    if (globalFlags_.is2016Pre || globalFlags_.is2016Post) {
        filterList_ = commonConfig["filters"]["2016"].get<std::vector<std::string>>();
    } else if (globalFlags_.is2017) {
        filterList_ = commonConfig["filters"]["2017"].get<std::vector<std::string>>();
    } else if (globalFlags_.is2018) {
        filterList_ = commonConfig["filters"]["2018"].get<std::vector<std::string>>();
    }

    if (filterList_.empty()) {
        std::cerr << "No filters found " << '\n';
        exit(EXIT_FAILURE);
    }

    // Set branch statuses and addresses for the filters
    for (const auto& filter : filterList_) {
        nanoT->fChain->SetBranchStatus(filter.c_str(), true);
        nanoT->fChain->SetBranchAddress(filter.c_str(), &filterVals_[filter], &filterTBranches_[filter]);
    }
}


void RunBase::setBranches(NanoTree* nanoT, const nlohmann::json& channelConfig) {
    // Load and set common branches
    nlohmann::json commonConfig = loadConfig("config/RunBase.json");
    setFilters(nanoT, commonConfig);

    // Set common tree branches
    for (const auto& branchName : commonConfig["commonTreeBranches"]) {
        std::string branch = branchName.get<std::string>();
        nanoT->fChain->SetBranchStatus(branch.c_str(), true);
    }
    
    // Set channel-specific tree branches (if any)
    if (channelConfig.contains("treeBranches")) {
        for (const auto& branchName : channelConfig["treeBranches"]) {
            std::string branch = branchName.get<std::string>();
            nanoT->fChain->SetBranchStatus(branch.c_str(), true);
        }
    }
    
    // Set common MC branches if running on MC
    if (globalFlags_.isMC && commonConfig.contains("commonMCBranches")) {
        for (const auto& branchName : commonConfig["commonMCBranches"]) {
            std::string branch = branchName.get<std::string>();
            nanoT->fChain->SetBranchStatus(branch.c_str(), true);
        }
    }
    
    // Set channel-specific MC branches if running on MC
    if (globalFlags_.isMC && channelConfig.contains("mcBranches")) {
        for (const auto& branchName : channelConfig["mcBranches"]) {
            std::string branch = branchName.get<std::string>();
            nanoT->fChain->SetBranchStatus(branch.c_str(), true);
        }
    }
}

void RunBase::setTriggers(NanoTree* nanoT, const nlohmann::json& channelConfig) {
    // Set trigger list based on the run period
    if (globalFlags_.is2016Pre || globalFlags_.is2016Post) {
        trigList_ = channelConfig["triggers"]["2016"].get<std::vector<std::string>>();
    } else if (globalFlags_.is2017) {
        trigList_ = channelConfig["triggers"]["2017"].get<std::vector<std::string>>();
    } else if (globalFlags_.is2018) {
        trigList_ = channelConfig["triggers"]["2018"].get<std::vector<std::string>>();
    }

    if (trigList_.empty()) {
        std::cerr << "No triggers found " << '\n';
        exit(EXIT_FAILURE);
    }

    // Set branch statuses and addresses for the triggers
    for (const auto& trig : trigList_) {
        nanoT->fChain->SetBranchStatus(trig.c_str(), true);
        nanoT->fChain->SetBranchAddress(trig.c_str(), &trigVals_[trig], &trigTBranches_[trig]);
    }
}

void RunBase::runEventLoop(NanoTree* nanoT, TTree* newTree) {
    Long64_t nentries = nanoT->getEntries();
    std::cout << "\nSample has " << nentries << " entries" << '\n';

    // Set up cutflow histograms (common to all channels)
    std::vector<std::string> cuts = { "NanoAOD", "Filter", "Trigger" };
    auto h1EventInCutflow = std::make_unique<HistCutflow>("h1EventInCutflow", cuts,
                                                          newTree->GetCurrentFile()->mkdir("Cutflow"));

    double totalTime = 0.0;
    auto startClock = std::chrono::high_resolution_clock::now();
    Helper::initProgress();
    for (Long64_t i = 0; i < nentries; i++) {
        Helper::printProgress(i, nentries, startClock, totalTime);
        Long64_t entry = nanoT->loadEntry(i);
        h1EventInCutflow->fill("NanoAOD");

        bool passFilter = true;
		for (const auto& filter : filterList_) {
            if (!filterTBranches_[filter]) continue;
		    filterTBranches_[filter]->GetEntry(entry);//Read only MET filter branch data 
		    if (!filterVals_[filter]) {
                passFilter = false;
		        break; 
		    }
		}
        if(!passFilter) continue;
        h1EventInCutflow->fill("Filter");


        for (const auto& trig : trigList_) {
            if (!trigTBranches_[trig]) continue;
            trigTBranches_[trig]->GetEntry(entry); // Read only HLT branch data
            if (trigVals_[trig]) {
                nanoT->fChain->GetTree()->GetEntry(entry); // Then read all branches
                h1EventInCutflow->fill("Trigger");
                newTree->Fill();
                break; // Event passes if any trigger is true
            }
        }
    }
    Helper::printCutflow(h1EventInCutflow->getHistogram());
    std::cout << "nEvents_Skim = " << newTree->GetEntries() << '\n';
}

int RunBase::Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) {
    fout->cd();

    // Load the configuration using the path provided by the derived class
    json channelConfig = loadConfig(getConfigPath());

    // Set the branches and triggers from the channelConfiguration
    setBranches(nanoT.get(), channelConfig);
    setTriggers(nanoT.get(), channelConfig);

    // Clone the tree and set cache
    TTree* newTree = nanoT->fChain->GetTree()->CloneTree(0);
    newTree->SetCacheSize(Helper::tTreeCatchSize);

    // Process the events (common event loop)
    runEventLoop(nanoT.get(), newTree);

    TTree* newTreeRuns = nanoT->fChainRuns->GetTree()->CloneTree(0);
    newTreeRuns->SetDirectory(fout);
    Long64_t nentriesRuns = nanoT->getEntriesRuns();
    for (Long64_t i = 0; i < nentriesRuns; i++) {
       Long64_t entry = nanoT->loadEntryRuns(i);
       nanoT->fChainRuns->GetTree()->GetEntry(entry);
       newTreeRuns->Fill();      
    }

    std::cout<<"nRuns_Skim = "<<newTreeRuns->GetEntries()<<'\n';
    std::cout << "Output file path = " << fout->GetName() << '\n';
    fout->Write();
    return EXIT_SUCCESS;
}

