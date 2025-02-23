#pragma once

#include "Config.h"
#include "LoadShape.h"
#include "LoadNorm.h"
#include "GlobalFlag.h"  // Include GlobalFlag for debug control
#include <TGraphErrors.h>
#include <TF1.h>
#include <map>
#include <string>
#include <functional>
#include <memory>

class FitModel {
public:
    // Modified constructor to accept maps of unique_ptr and a GlobalFlag reference.
    FitModel(const Config& config,
             const std::map<std::string, std::unique_ptr<LoadShape>>& shapes,
             const std::map<std::string, std::unique_ptr<LoadNorm>>& norms,
             GlobalFlag& globalFlags);
    ~FitModel();
    
    void setupFitFunction(double fitMin, double fitMax);
    TF1* getFitFunction() const;
    // Returns a lambda wrapper for the fit function.
    std::function<double(Double_t*, Double_t*)> jesFitWrapper();
    
private:
    const Config& config;
    // These members now store raw pointers, extracted from the unique_ptr maps.
    std::map<std::string, LoadShape*> shapes_;
    std::map<std::string, LoadNorm*> norms_;
    std::unique_ptr<TF1> jesFit; // the fit function
    double fitMin, fitMax;
    
    GlobalFlag& globalFlags_; // Global flags for controlling debug output.
};

