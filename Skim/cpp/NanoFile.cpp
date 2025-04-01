#include "NanoFile.h"
#include <iostream>
#include <stdexcept>
#include <regex>

NanoFile::NanoFile(GlobalFlag& globalFlags, const std::string& outName, const std::string& inputJsonPath)
    : globalFlags_(globalFlags), inputJsonPath_(inputJsonPath) {
    setInput(outName);
    loadInput();
    loadInputJson();
    loadJobFileNames();
}

NanoFile::~NanoFile() {
}

void NanoFile::setInput(const std::string& outputName) {
    if (outputName.empty()) {
        throw std::invalid_argument("Output name cannot be empty");
    }
    outputName_ = outputName;
    std::cout << "+ setInput() = " << outputName_ << '\n';
}

void NanoFile::loadInput() {
    std::cout << "==> loadInput()" << '\n';
    // Use regex to parse the expected format:
    // Expected format: <sampleKey>_Skim_<nth>of<total>.root
    std::regex pattern(R"(^(.+)_Skim_(\d+)of(\d+)\.root$)");
    std::smatch matches;
    if (!std::regex_match(outputName_, matches, pattern)) {
        throw std::runtime_error("Invalid output name format: " + outputName_ +
            "\nExpected format: DataOrMC_Year_Channel_Sample_Skim_<nth>of<total>.root");
    }
    if (matches.size() != 4) { // full match + 3 capture groups
        throw std::runtime_error("Output name did not match expected format groups: " + outputName_);
    }
    loadedSampleKey_ = matches[1];
    try {
        loadedNthJob_ = std::stoul(matches[2]);
        loadedTotalJobs_ = std::stoul(matches[3]);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing job numbers from output name: " + std::string(e.what()));
    }
    std::cout << "Loaded sample key: " << loadedSampleKey_ << '\n';
}

void NanoFile::loadInputJson() {
    std::cout << "==> loadInputJson()" << '\n';
    std::ifstream inputFile(inputJsonPath_);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Cannot open JSON file: " + inputJsonPath_);
    }
    nlohmann::json jsonContent;
    try {
        inputFile >> jsonContent;
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing JSON at " + inputJsonPath_ + ": " + e.what());
    }
    
    try {
        jsonContent.at(loadedSampleKey_).get_to(loadedAllFileNames_);
    } catch (const std::exception& e) {
        std::string availableKeys;
        for (const auto& element : jsonContent.items()) {
            availableKeys += element.key() + " ";
        }
        throw std::runtime_error("Invalid sample key: " + loadedSampleKey_ +
            "\nAvailable keys: " + availableKeys);
    }
}

std::vector<std::vector<std::string>> NanoFile::splitVector(const std::vector<std::string>& strings, std::size_t n) {
    if (n == 0) {
        throw std::invalid_argument("Number of splits cannot be zero.");
    }
    std::size_t size = strings.size() / n;  // Base size of each small vector
    std::size_t remainder = strings.size() % n;  // Remaining elements
    std::vector<std::vector<std::string>> smallVectors;
    std::size_t index = 0;

    using diff_t = std::vector<std::string>::difference_type;
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t count = (i < remainder) ? size + 1 : size;
        if (index + count > strings.size()) {
            throw std::runtime_error("Index out of bounds during vector splitting.");
        }
        smallVectors.emplace_back(strings.begin() + static_cast<diff_t>(index),
                                  strings.begin() + static_cast<diff_t>(index + count));
        index += count;
    }
    return smallVectors;
}

std::vector<std::string> NanoFile::splitString(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    if (delimiter.empty()) {
        throw std::invalid_argument("Delimiter cannot be empty in splitString.");
    }
    size_t start = 0, end = 0;
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start)); // Add the final token
    return tokens;
}

void NanoFile::loadJobFileNames() {
    std::cout << "==> loadJobFileNames()" << '\n';
    std::size_t totalFiles = loadedAllFileNames_.size();
    std::cout << "Total files: " << totalFiles << '\n';

    if (totalFiles == 0) {
        throw std::runtime_error("No files found in JSON for sample key: " + loadedSampleKey_);
    }

    if (loadedTotalJobs_ > totalFiles) {
        std::cerr << "Warning: loadedTotalJobs exceeds total files; adjusting to " << totalFiles << '\n';
        loadedTotalJobs_ = totalFiles;
    }

    if (loadedNthJob_ > loadedTotalJobs_) {
        throw std::runtime_error("loadedNthJob (" + std::to_string(loadedNthJob_) +
                                   ") exceeds loadedTotalJobs (" + std::to_string(loadedTotalJobs_) + ")");
    }

    if (loadedNthJob_ == 0 || loadedTotalJobs_ == 0) {
        throw std::runtime_error("loadedNthJob and loadedTotalJobs must be greater than zero");
    }

    std::cout << "Processing job " << loadedNthJob_ << " of " << loadedTotalJobs_ << '\n';
    std::cout << "Approximately " << totalFiles / loadedTotalJobs_ << " file(s) per job" << '\n';

    auto splitFiles = splitVector(loadedAllFileNames_, loadedTotalJobs_);
    if (loadedNthJob_ - 1 >= splitFiles.size()) {
        throw std::runtime_error("Job index out of range after splitting files.");
    }
    loadedJobFileNames_ = splitFiles.at(loadedNthJob_ - 1);
}

