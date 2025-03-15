#include "NanoFile.h"
#include "Helper.h"

#include <iostream>
#include <stdexcept>

NanoFile::NanoFile(GlobalFlag& globalFlags, const std::string& outName, const std::string& inJsonDir) : globalFlags_(globalFlags) {
    setInput(outName);
    loadInput();
    setInputJsonPath(inJsonDir);
    loadInputJson();
    loadJobFileNames();
}

NanoFile::~NanoFile() {
}

void NanoFile::setInput(const std::string& outputName) {
    outputName_ = outputName;
    std::cout << "+ setInput() = " << outputName_ << '\n';
}

void NanoFile::loadInput() {
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

void NanoFile::setInputJsonPath(const std::string& inputDir) {
    std::string year = globalFlags_.is2016Post ? "2016Post" : globalFlags_.is2017 ? "2017" : globalFlags_.is2018 ? "2018" : "2016Pre";
    std::string channel = Helper::splitString(loadedSampleKey_, "_").at(1);
    inputJsonPath_ = inputDir + "/FilesNano_" + channel + "_" + year + ".json";
    std::cout << "+ setInputJsonPath() = " << inputJsonPath_ << '\n';
}

void NanoFile::loadInputJson() {
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

void NanoFile::loadJobFileNames() {
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

