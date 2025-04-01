#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "SkimFile.h"
#include "Helper.h"

SkimFile::SkimFile(GlobalFlag& globalFlags, const std::string& outName, const std::string& inJsonDir)
    : outName_(outName),
      globalFlags_(globalFlags),
      year_(globalFlags_.getYear()),
      era_(globalFlags_.getEra()),
      channel_(globalFlags_.getChannel()),
      isDebug_(globalFlags_.isDebug()),
      isData_(globalFlags_.isData()),
      isMC_(globalFlags_.isMC()) 
{
    std::cout << "+ SkimFile initialized with outName = " << outName_ << '\n';
    loadInput();
    setInputJsonPath(inJsonDir);
    loadInputJson();
    loadJobFileNames();
}

SkimFile::~SkimFile() noexcept {
    // No manual resource cleanup required.
}

void SkimFile::loadInput() {
    std::cout << "==> loadInput()" << '\n';
    try {
        const std::vector<std::string> tokens = Helper::splitString(outName_, "_Hist_");
        if (tokens.size() < 2) {
            throw std::runtime_error("Invalid outName format: Expected at least two parts separated by '_Hist_'");
        }
        loadedSampKey_ = tokens.at(0);
        std::cout << "loadedSampKey_: " << loadedSampKey_ << '\n';

        const std::string nofN_root = tokens.at(1);
        const std::vector<std::string> rootTokens = Helper::splitString(nofN_root, ".root");
        if (rootTokens.empty()) {
            throw std::runtime_error("Invalid outName format: Missing '.root' extension");
        }
        const std::string nofN = rootTokens.at(0);
        std::cout << "nofN: " << nofN << '\n';

        const std::vector<std::string> jobTokens = Helper::splitString(nofN, "of");
        if (jobTokens.size() != 2) {
            throw std::runtime_error("Invalid job numbering format in outName: Expected format 'NofM'");
        }
        loadedNthJob_ = std::stoi(jobTokens.at(0));
        loadedTotJob_ = std::stoi(jobTokens.at(1));
    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "Error in loadInput(): " << e.what() << "\n"
            << "Check the outName_: " << outName_ << "\n"
            << "Expected format: DataOrMC_Year_Channel_Sample_Hist_NofM.root\n"
            << "Run ./runMain -h for more details";
        throw std::runtime_error(oss.str());
    }
}

void SkimFile::setInputJsonPath(const std::string& inDir) {
    // Convert GlobalFlag::Year to a string using if-else logic.
    std::string yearStr;
    if (year_ == GlobalFlag::Year::Year2016Pre)
        yearStr = "2016Pre";
    else if (year_ == GlobalFlag::Year::Year2016Post)
        yearStr = "2016Post";
    else if (year_ == GlobalFlag::Year::Year2017)
        yearStr = "2017";
    else if (year_ == GlobalFlag::Year::Year2018)
        yearStr = "2018";
    else {
        throw std::runtime_error("Error: Provide correct year in SkimFile::setInputJsonPath()");
    }

    const std::vector<std::string> tokens = Helper::splitString(loadedSampKey_, "_");
    if (tokens.size() < 3) {
        throw std::runtime_error("Invalid loadedSampKey_ format: Expected at least three parts separated by '_'");
    }
    const std::string channel = tokens.at(1);
    inputJsonPath_ = inDir + "/FilesSkim_" + channel + "_" + yearStr + ".json";
    std::cout << "+ setInputJsonPath() = " << inputJsonPath_ << '\n';
}

void SkimFile::loadInputJson() {
    std::cout << "==> loadInputJson()" << '\n';
    std::ifstream inputFile(inputJsonPath_);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Unable to open input JSON file: " + inputJsonPath_);
    }

    nlohmann::json js;
    try {
        inputFile >> js;
    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "Error parsing input JSON file: " << inputJsonPath_ << "\n"
            << e.what();
        throw std::runtime_error(oss.str());
    }

    try {
        // Parse JSON content and update member variables
        js.at(loadedSampKey_).at(1).get_to(loadedAllFileNames_);
        js.at(loadedSampKey_).at(0).at("xsecOrLumi").get_to(nanoXssOrLumi_);
        js.at(loadedSampKey_).at(0).at("nEvents").get_to(nanoEvents_);
        std::cout << "nanoXssOrLumi = " << nanoXssOrLumi_ 
                  << ", nanoEvents = " << nanoEvents_ << '\n';
    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "Key not found in JSON: " << loadedSampKey_ << "\n"
            << e.what() << "\n"
            << "Available keys in the JSON file:";
        for (const auto& element : js.items()) {
            oss << "\n- " << element.key();
        }
        throw std::runtime_error(oss.str());
    }
}

void SkimFile::loadJobFileNames() {
    std::cout << "==> loadJobFileNames()" << '\n';
    const int nFiles = static_cast<int>(loadedAllFileNames_.size());
    std::cout << "Total files = " << nFiles << '\n';

    if (loadedTotJob_ > nFiles) {
        std::cout << "Since loadedTotJob_ > nFiles, setting loadedTotJob_ to nFiles: " << nFiles << '\n';
        loadedTotJob_ = nFiles;
    }

    if (loadedNthJob_ > loadedTotJob_) {
        throw std::runtime_error("Error: loadedNthJob_ > loadedTotJob_ in loadJobFileNames()");
    }

    if (loadedNthJob_ > 0 && loadedTotJob_ > 0) {
        std::cout << "Jobs: " << loadedNthJob_ << " of " << loadedTotJob_ << '\n';
        std::cout << static_cast<double>(nFiles) / loadedTotJob_ 
                  << " files per job on average" << '\n';
    } else {
        throw std::runtime_error("Error: Ensure loadedNthJob_ and loadedTotJob_ are greater than 0 in loadJobFileNames()");
    }

    const std::vector<std::vector<std::string>> smallVectors = Helper::splitVector(loadedAllFileNames_, loadedTotJob_);
    if (loadedNthJob_ - 1 >= static_cast<int>(smallVectors.size())) {
        throw std::runtime_error("Error: loadedNthJob_ is out of range after splitting file names in loadJobFileNames()");
    }
    loadedJobFileNames_ = smallVectors[loadedNthJob_ - 1];
}

