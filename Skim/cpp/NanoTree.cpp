#include "NanoTree.h"
#include "Helper.h"

#include <iostream>
#include <stdexcept>

NanoTree::NanoTree(GlobalFlag& globalFlags) : globalFlags_(globalFlags) {}

NanoTree::~NanoTree() {
    delete fChain;
    delete fChainRuns;
}

// Helper function: Validate and open a file
TFile* NanoTree::validateAndOpenFile(const std::string& fullPath) {
    TFile* file = TFile::Open(fullPath.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Failed to open or corrupted file " << fullPath << '\n';
        if (file) file->Close();
        return nullptr;
    }
    // Check file size (using 3000 bytes as a threshold)
    if (file->GetSize() < 3000) {
        std::cerr << "Warning: file " << fullPath << " has less than 3000 bytes, skipping.\n";
        file->Close();
        return nullptr;
    }
    // Check that the file contains the "Events" tree
    if (!file->GetListOfKeys()->Contains("Events")) {
        std::cerr << "Error: 'Events' not found in " << fullPath << '\n';
        file->Close();
        return nullptr;
    }
    // Check the entries in the "Events" tree
    TTree* tree = file->Get<TTree>("Events");
    if (tree->GetEntries() == 0) {
        std::cerr << "Warning: 'Events' TTree in file " << fullPath << " has 0 entries. Skipping file.\n";
        file->Close();
        return nullptr;
    }
    return file;
}

// Helper function: Add file to TChains
bool NanoTree::addFileToChains(const std::string& fullPath) {
    int added = fChain->Add(fullPath.c_str());
    fChainRuns->Add(fullPath.c_str());
    if (added == 0) {
        std::cerr << "Warning: TChain::Add failed for " << fullPath << '\n';
        return false;
    }
    return true;
}

void NanoTree::loadTree(std::vector<std::string> nanoFileList) {
    std::cout << "==> loadTree()\n";
    fChain->SetCacheSize(Helper::tTreeCatchSize);
    fChainRuns->SetCacheSize(Helper::tTreeCatchSize);
    bool isCopy = false;  // Set to true if you want to copy files locally
    std::string baseDir = "root://cms-xrd-global.cern.ch/";  // Default remote directory

    int totalFiles = 0;
    int addedFiles = 0;
    int failedFiles = 0;

    // Optimization parameters for xrdcp (when copying files locally)
    const int streams = 15;              // Number of parallel data streams
    const int tcpBufSize = 1048576;        // TCP buffer size (1MB)

    for (const auto& fileName : nanoFileList) {
        totalFiles++;
        std::string fullPath;

        if (isCopy) {
            // Example of copying the file locally (using system calls)
            std::string localFile = fileName.substr(fileName.find_last_of('/') + 1);
            std::string cmd = "xrdcp --streams " + std::to_string(streams) + " ";
            std::string remoteFile = baseDir + fileName;
            cmd += remoteFile + " " + localFile;
            std::cout << "Executing command: " << cmd << '\n';
            int ret = system(cmd.c_str());
            if (ret != 0) {
                std::cerr << "Error: Failed to copy " << remoteFile << " to local file " << localFile << '\n';
                failedFiles++;
                continue;  // Skip adding this file
            }
            if (!std::filesystem::exists(localFile)) {
                std::cerr << "Error: Local file " << localFile << " does not exist after copying.\n";
                failedFiles++;
                continue;
            }
            fullPath = localFile;
        } else {
            // Remote file handling: try local EOS path first
            std::filesystem::path filePath = "/eos/cms/" + fileName;
            if (std::filesystem::exists(filePath)) {
                baseDir = "/eos/cms/";
            } else {
                baseDir = "root://cms-xrd-global.cern.ch/";
            }
            fullPath = baseDir + fileName;
        }
        std::cout<<fullPath<<std::endl;
        // Validate and open the file
        TFile* file = validateAndOpenFile(fullPath);
        if (!file) {
            failedFiles++;
            continue;
        }
        // Add file to the TChains
        if (!addFileToChains(fullPath)) {
            failedFiles++;
            file->Close();
            continue;
        }
        std::cout << fullPath << ", EntriesRuns: " << fChainRuns->GetEntries()
                  << ", Entries: " << fChain->GetEntries() << '\n';
        addedFiles++;
        file->Close();
    }

    // Final summary
    std::cout << "==> Finished loading files.\n";
    std::cout << "Total files processed: " << totalFiles << '\n';
    std::cout << "Successfully added files: " << addedFiles << '\n';
    std::cout << "Failed to add files: " << failedFiles << '\n';

    if (fChain->GetNtrees() == 0) {
        std::cerr << "Error: No valid ROOT files were added to the TChain. Exiting.\n";
        return;
    }
    // Set branch statuses as required for later processing
    fChain->SetBranchStatus("*", false); // To be enabled in the processing class
    fChainRuns->SetBranchStatus("*", true);
}

auto NanoTree::getEntries() const -> Long64_t {
    return fChain->GetEntries();
}

auto NanoTree::getEntriesRuns() const -> Long64_t {
    return fChainRuns->GetEntries();
}

auto NanoTree::getEntry(Long64_t entry) -> Int_t {
    return fChain->GetEntry(entry);
}

auto NanoTree::loadEntry(Long64_t entry) -> Long64_t {
    if (!fChain) return EXIT_FAILURE;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent_) {
        fCurrent_ = fChain->GetTreeNumber();
    }
    return centry;
}

auto NanoTree::loadEntryRuns(Long64_t entry) -> Long64_t {
    if (!fChainRuns) return EXIT_FAILURE;
    Long64_t centry = fChainRuns->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChainRuns->GetTreeNumber() != fCurrentRuns_) {
        fCurrentRuns_ = fChainRuns->GetTreeNumber();
    }
    return centry;
}

// New member function: Enable a list of branches in fChain.
void NanoTree::enableBranches(const std::vector<std::string>& branchNames, const std::string & context) {
    for (const auto & branch : branchNames) {
        fChain->SetBranchStatus(branch.c_str(), true);
        if (globalFlags_.isDebug) {
            std::cout << "Debug: Enabled branch '" << branch << "' in " << context << ".\n";
        }
    }
}

// New member function: Set branch addresses in fChain and store pointers in maps.
void NanoTree::setBranchAddressForMap(const std::vector<std::string>& branchNames,
                                        std::map<std::string, Bool_t>& valMap,
                                        std::map<std::string, TBranch*>& branchMap,
                                        const std::string & context) {
    for (const auto & branch : branchNames) {
        if (!fChain->GetListOfBranches()->FindObject(branch.c_str())) {
            if (globalFlags_.isDebug) {
                std::cerr << "Debug: Branch '" << branch << "' not found in " << context << ".\n";
            }
        } else {
            fChain->SetBranchStatus(branch.c_str(), true);
            fChain->SetBranchAddress(branch.c_str(), &valMap[branch], &branchMap[branch]);
            if (globalFlags_.isDebug) {
                std::cout << "Debug: Set branch address for '" << branch << "' in " << context << ".\n";
            }
        }
    }
}

