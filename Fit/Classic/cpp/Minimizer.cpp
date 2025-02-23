#include "Minimizer.h"
#include "Helper.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <TMinuit.h>

Minimizer::Minimizer(FitModel& model, GlobalFlag& flags)
    : fitModel_(model), globalFlags_(flags), fitter_(nullptr) {
    int nPars = fitModel_.getFitFunction()->GetNpar();
    if (globalFlags_.isDebug) {
        std::cout << "[Minimizer] Initializing with " << nPars << " fit parameters." << std::endl;
    }
    fitter_ = new TFitter(nPars);
    fitter_->SetFCN(jesFitterWrapper);
    // Suppress Minuit output:
    fitter_->GetMinuit()->SetPrintLevel(-1);
    for (int i = 0; i < nPars; ++i) {
        fitter_->SetParameter(i, Form("p%d", i), 1.0, 0.01, -100, 100);
        if (globalFlags_.isDebug) {
            std::cout << "[Minimizer] Parameter " << i << " set with initial value 1.0." << std::endl;
        }
    }
}

Minimizer::~Minimizer() {
    if (globalFlags_.isDebug) {
        std::cout << "[Minimizer] Destroying Minimizer and cleaning up TFitter." << std::endl;
    }
    delete fitter_;
}

void Minimizer::runFit(int iterations) {
    if (globalFlags_.isDebug) {
        std::cout << "[Minimizer] Starting fit with " << iterations << " iterations." << std::endl;
    }
    for (int i = 0; i < iterations; ++i) {
        fitter_->ExecuteCommand("MINI", nullptr, 0);
        if (globalFlags_.isDebug) {
            std::cout << "[Minimizer] Completed iteration " << (i + 1) << " of " << iterations << std::endl;
        }
    }
    int nPars = fitModel_.getFitFunction()->GetNpar();
    results_.parameters.resize(nPars);
    for (int i = 0; i < nPars; ++i) {
        results_.parameters[i] = fitter_->GetParameter(i);
        if (globalFlags_.isDebug) {
            std::cout << "[Minimizer] Parameter " << i << " value: " << results_.parameters[i] << std::endl;
        }
    }
    // For demonstration, fill the error matrix as identity.
    results_.errorMatrix.ResizeTo(nPars, nPars);
    for (int i = 0; i < nPars; ++i) {
        for (int j = 0; j < nPars; ++j) {
            results_.errorMatrix[i][j] = (i == j ? 1.0 : 0.0);
        }
    }
    Double_t amin, edm, errdef;
    Int_t npari, nparx, istat;
    fitter_->GetMinuit()->mnstat(amin, edm, errdef, npari, nparx, istat);
    results_.chi2 = amin;
    if (globalFlags_.isDebug) {
        std::cout << "[Minimizer] Fit completed. Chi2 = " << results_.chi2 << std::endl;
    }
    results_.nDataPoints = 100; // Placeholder for demonstration.
}

void Minimizer::jesFitterWrapper(Int_t &npar, Double_t *grad, Double_t &chi2, Double_t *par, Int_t flag) {
    // For demonstration, use a dummy chi2: sum of squares of parameters.
    chi2 = 0.0;
    for (int i = 0; i < npar; ++i) {
        chi2 += par[i] * par[i];
    }
    // No gradient is computed.
}

const FitResults& Minimizer::getResults() const {
    return results_;
}

