#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "GlobalFlag.h"

class SkimFile {
public:
    // Constructor accepting a reference to GlobalFlag
    explicit SkimFile(GlobalFlag& globalFlags, const std::string& outName, const std::string& inJsonDir);
    ~SkimFile() noexcept;

    // Delete copy constructor and assignment operator since the class holds references.
    SkimFile(const SkimFile&) = delete;
    SkimFile& operator=(const SkimFile&) = delete;

    void loadInput();

    void setInputJsonPath(const std::string& inDir);
    void loadInputJson();

    [[nodiscard]] const std::string& getSampleKey() const { 
        return loadedSampKey_; 
    }

    [[nodiscard]] const std::vector<std::string>& getAllFileNames() const { 
        return loadedAllFileNames_; 
    }

    void loadJobFileNames();

    [[nodiscard]] const std::vector<std::string>& getJobFileNames() const { 
        return loadedJobFileNames_; 
    }

    [[nodiscard]] const double & getXsecOrLumiNano() const { 
        return nanoXssOrLumi_; 
    }

    [[nodiscard]] const long int & getEventsNano() const { 
        return nanoEvents_; 
    }

private:
    // Member variables
    std::string outName_;
    std::string loadedSampKey_ = "MC_Year_Channel_Name";
    int loadedNthJob_ = 1;
    int loadedTotJob_ = 100;
    std::string inputJsonPath_ = "./FilesSkim_2022_GamJet.json";
    std::vector<std::string> loadedAllFileNames_;
    std::vector<std::string> loadedJobFileNames_;

    // Reference to GlobalFlag instance and related constant members
    GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Era era_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;
    const bool isData_;
    const bool isMC_;

    double nanoXssOrLumi_ = 1.0;
    long int nanoEvents_ = 1;  // Changed from 1.0 to 1 for type consistency
};

