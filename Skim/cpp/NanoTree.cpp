#include "NanoTree.h"
#include "Helper.h"

#include <iostream>
#include <stdexcept>

NanoTree::NanoTree(GlobalFlag& globalFlags) : globalFlags_(globalFlags) {}

NanoTree::~NanoTree() {
    delete fChain;
}

void NanoTree::setInput(const std::string& outputName) {
    outputName_ = outputName;
    std::cout << "+ setInput() = " << outputName_ << '\n';
}

void NanoTree::loadInput() {
    std::cout << "==> loadInput()" << '\n';
    try {
        std::vector<std::string> nameParts = Helper::splitString(outputName_, "_Skim_");
        loadedSampleKey_ = nameParts.at(0);
        std::cout << "Loaded sample key: " << loadedSampleKey_ << '\n';

        std::vector<std::string> jobParts = Helper::splitString(nameParts.at(1), ".root");
        std::string jobInfo = jobParts.at(0);
        std::vector<std::string> jobSplit = Helper::splitString(jobInfo, "of");

        loadedNthJob_ = std::stoul(jobSplit.at(0));
        loadedTotalJobs_ = std::stoul(jobSplit.at(1));
    } catch (const std::exception& e) {
        std::cerr << "\nEXCEPTION: Invalid output name format: " << outputName_ << '\n';
        std::cerr << "Expected format: DataOrMC_Year_Channel_Sample_Skim_nofN.root\n";
        std::cerr << "Run ./runMain -h for more details\n";
        std::cerr << e.what() << '\n';
        std::abort();
    }
}

void NanoTree::setInputJsonPath(const std::string& inputDir) {
    std::string year = globalFlags_.is2016Post ? "2016Post" : globalFlags_.is2017 ? "2017" : globalFlags_.is2018 ? "2018" : "2016Pre";
    std::string channel = Helper::splitString(loadedSampleKey_, "_").at(1);
    inputJsonPath_ = inputDir + "/FilesNano_" + channel + "_" + year + ".json";
    std::cout << "+ setInputJsonPath() = " << inputJsonPath_ << '\n';
}

void NanoTree::loadInputJson() {
    std::cout << "==> loadInputJson()" << '\n';
    std::ifstream inputFile(inputJsonPath_);
    nlohmann::json jsonContent;
    
    try {
        jsonContent = nlohmann::json::parse(inputFile);
    } catch (const std::exception& e) {
        std::cerr << "\nEXCEPTION: Error parsing JSON at " << inputJsonPath_ << '\n';
        std::cerr << e.what() << '\n';
        std::abort();
    }
    
    try {
        jsonContent.at(loadedSampleKey_).get_to(loadedAllFileNames_);
    } catch (const std::exception& e) {
        std::cerr << "\nEXCEPTION: Invalid sample key: " << loadedSampleKey_ << '\n';
        std::cerr << "Available keys:\n";
        for (const auto& element : jsonContent.items()) {
            std::cerr << element.key() << '\n';
        }
        std::abort();
    }
}

void NanoTree::loadJobFileNames() {
    std::cout << "==> loadJobFileNames()" << '\n';
    std::size_t totalFiles = loadedAllFileNames_.size();
    std::cout << "Total files: " << totalFiles << '\n';

    if (loadedTotalJobs_ > totalFiles) {
        std::cerr << "Warning: loadedTotalJobs exceeds total files; adjusting to " << totalFiles << '\n';
        loadedTotalJobs_ = totalFiles;
    }

    if (loadedNthJob_ > loadedTotalJobs_) {
        std::cerr << "Error: loadedNthJob exceeds loadedTotalJobs\n";
        std::abort();
    }

    if (loadedNthJob_ > 0 && loadedTotalJobs_ > 0) {
        std::cout << "Processing job " << loadedNthJob_ << " of " << loadedTotalJobs_ << '\n';
        std::cout << "Approximately " << totalFiles / loadedTotalJobs_ << " files per job" << '\n';
    } else {
        std::cerr << "Error: loadedNthJob and loadedTotalJobs must be greater than zero\n";
        std::abort();
    }

    loadedJobFileNames_ = Helper::splitVector(loadedAllFileNames_, loadedTotalJobs_).at(loadedNthJob_ - 1);
}

void NanoTree::loadTree() {
    std::cout << "==> loadTree()" << '\n';
    fChain->SetCacheSize(Helper::tTreeCatchSize);
    bool isCopy = false;  // Set to true if you want to copy files locally
    std::string dir = "root://cms-xrd-global.cern.ch/";  // Default remote directory
    // std::string dir = "./";  // Uncomment if using local directory

    int totalFiles = 0;
    int addedFiles = 0;
    int failedFiles = 0;

    for (const auto& fileName : loadedJobFileNames_) {
        totalFiles++;
        std::string fullPath;

        if (isCopy) {
            // Extract the local file name from the remote path
            std::string localFile = fileName.substr(fileName.find_last_of('/') + 1);
            std::string cmd = "xrdcp ";
            cmd.append(dir).append(fileName).append(" ").append(localFile);
            std::cout << "Executing command: " << cmd << '\n';
            int ret = system(cmd.c_str());

            if (ret != 0) {
                std::cerr << "Error: Failed to copy " << fileName << " to local file " << localFile << '\n';
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
                dir = "root://cms-xrd-global.cern.ch/";  // Fallback to remote xrootd path
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

        // Optionally, check for the presence of essential branches or histograms
        // For example, check if "Events" exists
        if (!f->GetListOfKeys()->Contains("Events")) {
            std::cerr << "Error: 'Events' not found in " << fullPath << '\n';
            f->Close();
            failedFiles++;
            continue;  // Skip adding this file
        }

        // File is valid, add it to the TChain
        int added = fChain->Add(fullPath.c_str());
        if (added == 0) {
            std::cerr << "Warning: TChain::Add failed for " << fullPath << '\n';
            f->Close();
            failedFiles++;
            continue;  // Skip adding this file
        }

        std::cout << fullPath << "  Entries: " << fChain->GetEntries() << '\n';
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


    fChain->SetBranchStatus("*", false);
    fChain->SetBranchStatus("run", true);
    fChain->SetBranchStatus("event", true);
    fChain->SetBranchStatus("luminosityBlock", true);

	//--------------------------------------- 
    // Jet branches for all channels 
    //--------------------------------------- 
    fChain->SetBranchStatus("Jet_btagDeepFlav*", true);
    fChain->SetBranchStatus("Jet_qgl", true);
    fChain->SetBranchStatus("Jet_chEmEF", true);
    fChain->SetBranchStatus("Jet_chHEF", true);
    fChain->SetBranchStatus("Jet_eta", true);
    fChain->SetBranchStatus("Jet_mass", true);
    fChain->SetBranchStatus("Jet_muEF", true);
    fChain->SetBranchStatus("Jet_neEmEF", true);
    fChain->SetBranchStatus("Jet_neHEF", true);
    fChain->SetBranchStatus("Jet_phi", true);
    fChain->SetBranchStatus("Jet_pt", true);
    fChain->SetBranchStatus("Jet_rawFactor", true);
    fChain->SetBranchStatus("Jet_jetId", true);
    fChain->SetBranchStatus("Jet_puId", true);
    fChain->SetBranchStatus("Jet_area", true);
    fChain->SetBranchStatus("nJet", true); 


    fChain->SetBranchStatus("PV_z", true);
    fChain->SetBranchStatus("PV_npvs", true);
    fChain->SetBranchStatus("PV_npvsGood", true);
    fChain->SetBranchStatus("MET_pt", true);
    fChain->SetBranchStatus("MET_phi", true);
    fChain->SetBranchStatus("ChsMET_pt", true);
    fChain->SetBranchStatus("ChsMET_phi", true);
    fChain->SetBranchStatus("RawPuppiMET_pt", true);
    fChain->SetBranchStatus("RawPuppiMET_phi", true);
    fChain->SetBranchStatus("fixedGridRhoFastjetAll", true);

    fChain->SetBranchStatus("Flag_goodVertices", true);
    fChain->SetBranchStatus("Flag_globalSuperTightHalo2016Filter", true);
    fChain->SetBranchStatus("Flag_HBHENoiseFilter", true);
    fChain->SetBranchStatus("Flag_HBHENoiseIsoFilter", true);
    fChain->SetBranchStatus("Flag_EcalDeadCellTriggerPrimitiveFilter", true);
    fChain->SetBranchStatus("Flag_BadPFMuonFilter", true);
    fChain->SetBranchStatus("Flag_eeBadScFilter", true);
    if(globalFlags_.is2017 || globalFlags_.is2018){
        fChain->SetBranchStatus("Flag_ecalBadCalibFilter", true);
    }

    if (globalFlags_.isMC) {
        fChain->SetBranchStatus("Jet_hadronFlavour", true);
        fChain->SetBranchStatus("Jet_genJetIdx", true);
        fChain->SetBranchStatus("genWeight", true);
        fChain->SetBranchStatus("nPSWeight", true);
        fChain->SetBranchStatus("PSWeight", true);
        fChain->SetBranchStatus("LHE_HT", true);
        fChain->SetBranchStatus("Pileup_nTrueInt", true);
        fChain->SetBranchStatus("GenJet_*", true);
        fChain->SetBranchStatus("nGenJet", true);
    }
}

auto NanoTree::getEntries() const -> Long64_t {
    return fChain->GetEntries();
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

