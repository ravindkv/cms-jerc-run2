#pragma once

#include "FitModel.h"
#include "GlobalFlag.h"  // Include GlobalFlag for debug control
#include <TFitter.h>
#include <TMatrixD.h>
#include <vector>

struct FitResults {
    std::vector<double> parameters;
    TMatrixD errorMatrix;
    double chi2;
    int nDataPoints;
};

class Minimizer {
public:
    // Modified constructor to accept a GlobalFlag reference.
    Minimizer(FitModel& model, GlobalFlag& globalFlags);
    ~Minimizer();
    void runFit(int iterations = 100);
    const FitResults& getResults() const;
    
private:
    FitModel& fitModel_;
    GlobalFlag& globalFlags_; // Global flags for debug output.
    TFitter* fitter_;
    FitResults results_;
    static void jesFitterWrapper(Int_t &npar, Double_t *grad, Double_t &fval, Double_t *par, Int_t flag);
};

