
#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "GlobalFlag.h"

class NanoFile { 
public:
    explicit NanoFile(GlobalFlag& globalFlags, const std::string& outName, const std::string& inJsonDir);
    ~NanoFile();

    void setInput(const std::string& outputName);
    void loadInput();

    // JSON management
    void setInputJsonPath(const std::string& inputDir);
    void loadInputJson();

    // File loading
    void loadJobFileNames();

    [[nodiscard]] const std::vector<std::string>& getJobFileNames() const { 
        return loadedJobFileNames_; 
    }

private:
    // Member variables
    GlobalFlag& globalFlags_;

    std::string outputName_;
    std::string loadedSampleKey_ = "MC_Year_Channel_Name";
    std::string inputJsonPath_ = "./FilesNano_2022_GamJet.json";
    std::size_t loadedNthJob_ = 1;
    std::size_t loadedTotalJobs_ = 100;

    std::vector<std::string> loadedAllFileNames_;
    std::vector<std::string> loadedJobFileNames_;
};

