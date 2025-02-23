#include "FitModel.h"
#include "Helper.h"
#include <iostream>
#include <cassert>
#include <algorithm>

FitModel::FitModel(const Config& cfg,
                   const std::map<std::string, std::unique_ptr<LoadShape>>& shapes,
                   const std::map<std::string, std::unique_ptr<LoadNorm>>& norms,
                   GlobalFlag& flags)
    : config(cfg), jesFit(nullptr), globalFlags_(flags)
{
    // Extract raw pointers from the unique_ptr maps and store them.
    for (const auto &pair : shapes) {
        shapes_[pair.first] = pair.second.get();
    }
    for (const auto &pair : norms) {
        norms_[pair.first] = pair.second.get();
    }
    if (globalFlags_.isDebug) {
        std::cout << "[FitModel] Initialized with " << shapes_.size() << " shapes and "
                  << norms_.size() << " norms." << std::endl;
    }
}

FitModel::~FitModel() {
    // jesFit is managed by unique_ptr.
    // Note: The raw pointers in shapes_ and norms_ are not owned by FitModel.
}

std::function<double(Double_t*, Double_t*)> FitModel::jesFitWrapper() {
    // For example, use a reference histogram "norm" if available.
    TH1D* hjesref = nullptr;
    if (norms_.find("norm") != norms_.end())
        hjesref = norms_.at("norm")->getHist();
    
    if (globalFlags_.isDebug) {
        if (hjesref) {
            std::cout << "[FitModel] Found 'norm' histogram for JES reference interpolation." << std::endl;
        } else {
            std::cout << "[FitModel] 'norm' histogram not found; proceeding without JES reference." << std::endl;
        }
    }
    
    // Lambda that computes the model prediction at pt.
    return [this, hjesref](Double_t* x, Double_t* p) -> double {
        double pt = x[0];
        double var = 0.0;
        bool useJESref = true; // Could be read from config.
        if (useJESref && hjesref) {
            double localJESref = hjesref->Interpolate(pt);
            var = 1.0;
            for (auto& kv : shapes_) {
                int index = kv.second->getIndex();
                TF1* f1 = kv.second->getFunc();
                double parVal = p[index];
                if (kv.second->isPositive())
                    parVal = std::max(parVal, 0.0);
                var += parVal * f1->Eval(pt);
            }
            return var / localJESref;
        } else {
            for (auto& kv : shapes_) {
                TF1* f1 = kv.second->getFunc();
                int index = kv.second->getIndex();
                var += p[index] * f1->Eval(pt);
            }
            return var;
        }
    };
}

void FitModel::setupFitFunction(double min, double max) {
    fitMin = min;
    fitMax = max;
    int nFitPars = shapes_.size();
    if (globalFlags_.isDebug) {
        std::cout << "[FitModel] Setting up fit function in range [" << fitMin << ", " << fitMax
                  << "] with " << nFitPars << " parameters." << std::endl;
    }
    auto funcWrapper = jesFitWrapper();
    jesFit = std::make_unique<TF1>("jesFit", funcWrapper, fitMin, fitMax, nFitPars);
}

TF1* FitModel::getFitFunction() const {
    return jesFit.get();
}

