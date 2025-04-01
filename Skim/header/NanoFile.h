#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "GlobalFlag.h"

class NanoFile { 
public:
    explicit NanoFile(GlobalFlag& globalFlags, const std::string& outName, const std::string& inputJsonPath);
    ~NanoFile();

    void setInput(const std::string& outputName);
    void loadInput();

    void loadInputJson();

    // File loading
    void loadJobFileNames();

    [[nodiscard]] const std::vector<std::string>& getJobFileNames() const { 
        return loadedJobFileNames_; 
    }

    // Utility functions to split strings and vectors.
    std::vector<std::vector<std::string>> splitVector(const std::vector<std::string>& strings, std::size_t n);
    std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);

private:
    // Member variables
    GlobalFlag& globalFlags_;

    std::string outputName_;
    std::string loadedSampleKey_; 
    std::string inputJsonPath_;
    std::size_t loadedNthJob_ = 1;
    std::size_t loadedTotalJobs_ = 100;

    std::vector<std::string> loadedAllFileNames_;
    std::vector<std::string> loadedJobFileNames_;
};

