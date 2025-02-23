#pragma once

#include <memory>
#include <cassert>
#include <iostream>
#include <string>
#include "TF1.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "Helper.h"

/**
 * @brief Manages systematic uncertainties loaded as TH1D histograms.
 */
class LoadSyst {
public:
    LoadSyst(const TString& name, TH1D* hist);

    // Delete copy and assignment operators.
    LoadSyst(const LoadSyst&) = delete;
    LoadSyst& operator=(const LoadSyst&) = delete;

    // Setters.
    void setName(const TString& x) { m_name = x; }
    void setHist(TH1D* x) {
        m_hist.reset(x);
        if (m_hist) m_hist->SetDirectory(0);
    }

    // Getters.
    std::string getColor() const { return colCyan; }
    TString getName() const { return m_name; }
    TH1D* getHist() const { return m_hist.get(); }

    int getN() const {
        return m_hist ? m_hist->GetNbinsX() : 0;
    }

private:
    TString m_name;
    std::unique_ptr<TH1D> m_hist;
};

