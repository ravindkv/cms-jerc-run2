#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <filesystem>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TMath.h>

#include "GlobalFlag.h"

const Int_t MAX_PARTICLES = 600;

class NanoTree { 
public:
    explicit NanoTree(GlobalFlag& globalFlags);
    ~NanoTree();

    Long64_t getEntries() const;
    Long64_t getEntriesRuns() const;
    Int_t getEntry(Long64_t entry);
    Long64_t loadEntry(Long64_t entry);
    Long64_t loadEntryRuns(Long64_t entry);

    void loadTree(std::vector<std::string> nanoFileList);

    // Data access layer: enable branches and set branch addresses.
    void enableBranches(const std::vector<std::string>& branchNames, const std::string & context);
    void setBranchAddressForMap(const std::vector<std::string>& branchNames,
                                std::map<std::string, Bool_t>& valMap,
                                std::map<std::string, TBranch*>& branchMap,
                                const std::string & context);

    TChain* fChain = new TChain("Events");   
    TChain* fChainRuns = new TChain("Runs");

private:
    // Member variables
    GlobalFlag& globalFlags_;
    Int_t fCurrent_{-1}; 
    Int_t fCurrentRuns_{-1}; 

    UInt_t run_{};
    ULong64_t event_{};
    UInt_t lumis_{};

    TFile* validateAndOpenFile(const std::string& fullPath);
    bool addFileToChains(const std::string& fullPath);
};

