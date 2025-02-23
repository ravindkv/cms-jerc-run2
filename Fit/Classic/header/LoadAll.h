#pragma once

#include "Config.h"
#include "LoadBase.h"
#include "LoadSyst.h"
#include "LoadShape.h"
#include "LoadNorm.h"
#include "GlobalFlag.h"  // Include GlobalFlag

#include <TGraphErrors.h>
#include <TH1D.h>
#include <TFile.h>
#include <string>
#include <map>
#include <memory>

/**
 * @brief Loads all components (base, systematics, shapes, and normalization)
 *        using the provided configuration.
 */
class LoadAll {
public:
    // Updated constructor to accept a GlobalFlag reference.
    explicit LoadAll(const Config& config, GlobalFlag& globalFlags);
    ~LoadAll();

    void loadBase();
    void loadSyst();
    void loadShape();
    void loadNorm();
    
    // Getters for loaded containers.
    const std::map<std::string, std::unique_ptr<LoadBase>>& getBase() const { return loadedBase_; }
    const std::map<std::string, std::unique_ptr<LoadSyst>>& getSyst() const { return loadedSyst_; }
    const std::map<std::string, std::unique_ptr<LoadShape>>& getShape() const { return loadedShape_; }
    const std::map<std::string, std::unique_ptr<LoadNorm>>& getNorm() const { return loadedNorm_; }
    
    // Delete copy and assignment operators.
    LoadAll(const LoadAll&) = delete;
    LoadAll& operator=(const LoadAll&) = delete;
    
private:
    const Config& config;
    std::unique_ptr<TFile> inFile_; // Managed ROOT file.
    std::map<std::string, std::unique_ptr<LoadBase>> loadedBase_;
    std::map<std::string, std::unique_ptr<LoadSyst>> loadedSyst_;
    std::map<std::string, std::unique_ptr<LoadShape>> loadedShape_;
    std::map<std::string, std::unique_ptr<LoadNorm>> loadedNorm_;
    
    GlobalFlag& globalFlags_; // Global flags for debug and configuration.
};

