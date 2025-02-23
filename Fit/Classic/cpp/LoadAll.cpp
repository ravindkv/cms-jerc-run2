#include "LoadAll.h"
#include <iostream>
#include <cassert>

// Constructor: open the ROOT file using the filename from the configuration.
LoadAll::LoadAll(const Config& cfg, GlobalFlag& flags)
    : config(cfg), globalFlags_(flags) {
    std::string filename = config.getRootFile().get<std::string>();
    if (globalFlags_.isDebug) {
        std::cout << "[LoadAll] Attempting to open ROOT file: " << filename << std::endl;
    }
    inFile_ = std::unique_ptr<TFile>(TFile::Open(filename.c_str(), "UPDATE"));
    if (!inFile_ || inFile_->IsZombie()) {
        throw std::runtime_error("Failed to open ROOT file: " + filename);
    }
    if (globalFlags_.isDebug) {
        std::cout << "[LoadAll] Successfully opened ROOT file: " << inFile_->GetName() << std::endl;
    }
}

// Destructor: unique_ptr members clean up automatically.
LoadAll::~LoadAll() {
    if (globalFlags_.isDebug) {
        std::cout << "[LoadAll] Cleaning up and closing the ROOT file." << std::endl;
    }
}

// Load base histograms.
void LoadAll::loadBase() {
    if (globalFlags_.isDebug) {
        std::cout << "[LoadAll] Starting to load base histogram..." << std::endl;
    }
    auto baseName = config.getBase();
    std::string name = baseName.get<std::string>();
    TGraphErrors* graph = static_cast<TGraphErrors*>(inFile_->Get(name.c_str()));
    if (!graph) {
        std::cerr << "[LoadAll] Could not load base histogram: " << name
                  << " from " << inFile_->GetName() << std::endl;
        return;
    }
    loadedBase_[name] = std::make_unique<LoadBase>(name, graph);
    if (globalFlags_.isDebug) {
        std::cout << "[LoadAll] Successfully loaded base histogram: " << name << std::endl;
    }
}

// Load systematic uncertainties.
void LoadAll::loadSyst() {
    if (globalFlags_.isDebug) {
        std::cout << "[LoadAll] Starting to load systematic uncertainties..." << std::endl;
    }
    auto systs = config.getSyst();
    for (const auto& systName : systs) {
        std::string name = systName.get<std::string>();
        TH1D* hist = static_cast<TH1D*>(inFile_->Get(name.c_str()));
        if (!hist) {
            std::cerr << "[LoadAll] Could not load systematic histogram: " << name
                      << " from " << inFile_->GetName() << std::endl;
            continue;
        }
        loadedSyst_[name] = std::make_unique<LoadSyst>(name, hist);
        if (globalFlags_.isDebug) {
            std::cout << "[LoadAll] Successfully loaded systematic histogram: " << name << std::endl;
        }
    }
}

// Load shape definitions.
void LoadAll::loadShape() {
    if (globalFlags_.isDebug) {
        std::cout << "[LoadAll] Starting to load shape definitions..." << std::endl;
    }
    auto shapesMap = config.getShape();
    for (auto& item : shapesMap.items()) {
        std::string name = item.key();
        auto info = item.value();
        std::string form = info["form"].get<std::string>();
        bool isPositive = (info["ispositive"].get<int>() != 0);
        int index = static_cast<int>(loadedShape_.size());
        loadedShape_[name] = std::make_unique<LoadShape>(name, form, index, isPositive);
        if (globalFlags_.isDebug) {
            std::cout << "[LoadAll] Successfully loaded shape: " << name 
                      << " with form: " << form 
                      << " (isPositive: " << (isPositive ? "true" : "false") << ")" << std::endl;
        }
    }
}

// Load normalization histograms.
void LoadAll::loadNorm() {
    if (globalFlags_.isDebug) {
        std::cout << "[LoadAll] Starting to load normalization histograms..." << std::endl;
    }
    auto norms = config.getNorm();
    for (const auto& normName : norms) {
        std::string name = normName.get<std::string>();
        TH1D* hist = static_cast<TH1D*>(inFile_->Get(name.c_str()));
        if (!hist) {
            std::cerr << "[LoadAll] Could not load normalization histogram: " << name
                      << " from " << inFile_->GetName() << std::endl;
            continue;
        }
        loadedNorm_[name] = std::make_unique<LoadNorm>(name, hist);
        if (globalFlags_.isDebug) {
            std::cout << "[LoadAll] Successfully loaded normalization histogram: " << name << std::endl;
        }
    }
}

