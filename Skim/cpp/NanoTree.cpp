#include "NanoTree.h"
#include "Helper.h"

#include <iostream>
#include <stdexcept>

NanoTree::NanoTree(GlobalFlag& globalFlags) : globalFlags_(globalFlags) {}

NanoTree::~NanoTree() {
    delete fChain;
    //delete fChainRuns;
}

void NanoTree::loadTree(std::vector<std::string> nanoFileList) {
    std::cout << "==> loadTree()" << '\n';
    fChain->SetCacheSize(Helper::tTreeCatchSize);
    fChainRuns->SetCacheSize(Helper::tTreeCatchSize);
    bool isCopy = false;  // Set to true if you want to copy files locally
    std::string dir = "root://cms-xrd-global.cern.ch/";  // Default remote directory

    int totalFiles = 0;
    int addedFiles = 0;
    int failedFiles = 0;

    // Optimization parameters for xrdcp
    const int streams = 15;              // Number of parallel data streams
    const int tcpBufSize = 1048576;        // TCP buffer size (1MB)

    for (const auto& fileName : nanoFileList) {
        totalFiles++;
        std::string fullPath;

        if (isCopy) {
            // Extract the local file name from the remote path
            std::string localFile = fileName.substr(fileName.find_last_of('/') + 1);
            std::string cmd = "xrdcp ";

            // Append optimization options to the xrdcp command
            cmd += "--streams " + std::to_string(streams) + " ";

            // Construct the full remote path
            std::string remoteFile = dir + fileName;

            // Build the final command
            cmd += remoteFile + " " + localFile;

            std::cout << "Executing command: " << cmd << '\n';
            int ret = system(cmd.c_str());

            if (ret != 0) {
                std::cerr << "Error: Failed to copy " << remoteFile << " to local file " << localFile << '\n';
                failedFiles++;
                continue;  // Skip adding this file
            }

            // Check if the file was successfully copied
            if (!std::filesystem::exists(localFile)) {
                std::cerr << "Error: Local file " << localFile << " does not exist after copying.\n";
                failedFiles++;
                continue;  // Skip adding this file
            }

            fullPath = localFile;  // Use the local file path
        } else {
            // Remote file handling
            std::filesystem::path filePath = "/eos/cms/" + fileName;
            if (std::filesystem::exists(filePath)) {
                dir = "/eos/cms/";  // Use local EOS path
                fullPath = dir + fileName;
            } else {
                dir = "root://cms-xrd-global.cern.ch/";  // Fallback to remote
                fullPath = dir + fileName;
            }
        }

        // Attempt to open the file to verify its validity
        TFile* f = TFile::Open(fullPath.c_str(), "READ");
        if (!f || f->IsZombie()) {
            std::cerr << "Error: Failed to open or corrupted file " << fullPath << '\n';
            if (f) f->Close();
            failedFiles++;
            continue;  // Skip adding this file
        }

        // Additional check: if file size is 0, skip the file.
        std::cout<<f->GetSize()<<'\n';
        if (f->GetSize() < 3000) {
            std::cerr << "Warning: file " << fullPath << " has less than 3000, skipping." << std::endl;
            f->Close();
            failedFiles++;
            continue;  // Skip adding this file
        }

        // Check if "Events" tree exists
        if (!f->GetListOfKeys()->Contains("Events")) {
            std::cerr << "Error: 'Events' not found in " << fullPath << '\n';
            f->Close();
            failedFiles++;
            continue;  // Skip adding this file
        }

        // Check the entries in the newly added TTree
        TTree* tree = f->Get<TTree>("Events");
        Long64_t fileEntries = tree->GetEntries();
        if (fileEntries == 0) {
            std::cerr << "\nWarning: 'Events' TTree in file " << fullPath << " has 0 entries. Skipping file.\n\n";
            f->Close();
            failedFiles++;
            continue;  // Skip adding this file to the final count
        }

        // File is valid, add it to the TChain
        int added = fChain->Add(fullPath.c_str());
        fChainRuns->Add(fullPath.c_str());
        if (added == 0) {
            std::cerr << "Warning: TChain::Add failed for " << fullPath << '\n';
            f->Close();
            failedFiles++;
            continue;  // Skip adding this file
        }

        std::cout << fullPath << ", EntriesRuns: " << fChainRuns->GetEntries() 
                              << ", Entries: " << fChain->GetEntries() << '\n';
        addedFiles++;
        f->Close();
    }

    // Final summary
    std::cout << "==> Finished loading files.\n";
    std::cout << "Total files processed: " << totalFiles << '\n';
    std::cout << "Successfully added files: " << addedFiles << '\n';
    std::cout << "Failed to add files: " << failedFiles << '\n';

    // Check if the chain has any trees
    if (fChain->GetNtrees() == 0) {
        std::cerr << "Error: No valid ROOT files were added to the TChain. Exiting.\n";
        return;
    }
    fChain->SetBranchStatus("*", false);// To be enabled in RunBase class
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
