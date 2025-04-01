#include "RunChannel.h"
#include "ReadConfig.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

using json = nlohmann::json;

RunChannel::RunChannel(GlobalFlag& globalFlags)
    : globalFlags_(globalFlags) {
}

void RunChannel::runEventLoop(NanoTree* nanoT, TFile* fout) {
    Long64_t nentries = nanoT->getEntries();
    std::cout << "\nSample has " << nentries << " entries\n";

    // Clone the tree and set cache.
    TTree* newTree = nanoT->fChain->GetTree()->CloneTree(0);
    newTree->SetCacheSize(Helper::tTreeCatchSize);

    // Setup cutflow histogram using newTree's file directory.
    std::vector<std::string> cuts = { "NanoAOD", "Filter", "Trigger" };
    auto h1EventInCutflow = std::make_unique<HistCutflow>("h1EventInCutflow", cuts, gDirectory);

    double totalTime = 0.0;
    auto startClock = std::chrono::high_resolution_clock::now();

    if (globalFlags_.isDebug) {
        std::cout << "Debug: Starting event loop...\n";
    }
    Helper::initProgress();
    for (Long64_t i = 0; i < nentries; i++) {
        Helper::printProgress(i, nentries, startClock, totalTime);
        Long64_t entry = nanoT->loadEntry(i);
        h1EventInCutflow->fill("NanoAOD");

        bool passFilter = true;
        for (const auto& filter : filterList_) {
            if (!filterTBranches_[filter]) continue;
            filterTBranches_[filter]->GetEntry(entry);
            if (!filterVals_[filter]) {
                passFilter = false;
                break;
            }
        }
        if (!passFilter) continue;
        h1EventInCutflow->fill("Filter");

        for (const auto& trig : trigList_) {
            if (!trigTBranches_[trig]) continue;
            trigTBranches_[trig]->GetEntry(entry);
            if (trigVals_[trig]) {
                nanoT->fChain->GetTree()->GetEntry(entry);
                h1EventInCutflow->fill("Trigger");
                newTree->Fill();
                break;
            }
        }
    }
    // Process the Runs tree.
    TTree* newTreeRuns = nanoT->fChainRuns->GetTree()->CloneTree(0);
    newTreeRuns->SetDirectory(fout);
    Long64_t nentriesRuns = nanoT->getEntriesRuns();
    for (Long64_t i = 0; i < nentriesRuns; i++) {
       Long64_t entry = nanoT->loadEntryRuns(i);
       nanoT->fChainRuns->GetTree()->GetEntry(entry);
       newTreeRuns->Fill();
    }

    Helper::printCutflow(h1EventInCutflow->getHistogram());
    std::cout << "nEvents_Skim = " << newTree->GetEntries() << "\n";
    std::cout << "nEvents_Runs = " << newTreeRuns->GetEntries() << "\n";
    std::cout << "Output file path = " << fout->GetName() << "\n";
    fout->Write();
}


int RunChannel::Run(std::shared_ptr<NanoTree>& nanoT, ReadConfig &readConfig, TFile* fout) {
    fout->cd();

    // Determine run period based on global flags.
    std::string runPeriod;
    if (globalFlags_.is2016Pre || globalFlags_.is2016Post)
        runPeriod = "2016";
    else if (globalFlags_.is2017)
        runPeriod = "2017";
    else if (globalFlags_.is2018)
        runPeriod = "2018";
    else
        throw std::runtime_error("Unknown run period in RunChannel::Run");

    // Get filter list and configure filter branches using NanoTree methods.
    filterList_ = readConfig.getFilterList(runPeriod);
    nanoT->setBranchAddressForMap(filterList_, filterVals_, filterTBranches_, "Filters");

    // Enable common tree branches.
    auto commonTreeBranches = readConfig.getCommonTreeBranches();
    nanoT->enableBranches(commonTreeBranches, "Common Tree Branches");

    // Enable channel-specific tree branches if available.
    auto channelTreeBranches = readConfig.getChannelTreeBranches();
    if (!channelTreeBranches.empty()) {
        nanoT->enableBranches(channelTreeBranches, "Channel Tree Branches");
    }

    // Enable MC branches if running on MC.
    if (globalFlags_.isMC) {
        auto commonMCBranches = readConfig.getCommonMCBranches();
        if (!commonMCBranches.empty()) {
            nanoT->enableBranches(commonMCBranches, "Common MC Branches");
        }
        auto channelMCBranches = readConfig.getChannelMCBranches();
        if (!channelMCBranches.empty()) {
            nanoT->enableBranches(channelMCBranches, "Channel MC Branches");
        }
    }

    // Configure triggers using NanoTree's branch-handling.
    trigList_ = readConfig.getTriggerList(runPeriod);
    nanoT->setBranchAddressForMap(trigList_, trigVals_, trigTBranches_, "Triggers");

    // Run the common event loop.
    runEventLoop(nanoT.get(), fout);

    return EXIT_SUCCESS;
}

