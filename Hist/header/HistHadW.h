#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <cassert>
#include <cmath>

// ROOT includes
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TLorentzVector.h>

#include "VarBin.h"

struct HadWHistograms {
    // 1D Histograms
    std::unique_ptr<TH1D> h1EventInHadWPt;
    std::unique_ptr<TH1D> h1EventInAvgHadPt;
    std::unique_ptr<TH1D> h1EventInHadWEta;
    std::unique_ptr<TH1D> h1EventInHadWPhi;
    std::unique_ptr<TH1D> h1EventInHadWMass;
    std::unique_ptr<TH1D> h1EventInChiSqrW;

    // TProfile Histograms
    std::unique_ptr<TProfile> p1HadWMassInAvgHadPt;
    std::unique_ptr<TProfile> p1HadWMassInHadWPt;
    std::unique_ptr<TProfile> p1HadWMassInHadWEta;
    std::unique_ptr<TProfile> p1HadWMassInHadWPhi;
    std::unique_ptr<TProfile> p1HadWMassInRun;

};

/**
 * This class encapsulates the creation and filling of histograms that monitor
 * variables after at least one reference. It manages both 1D and 2D histograms using ROOT's TH1D, TH2D, and TProfile classes.
 */
class HistHadW {
public:
    /**
     * @brief Constructs a HistHadW object and initializes histograms.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     */
    HistHadW(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin);
    
    /**
     * @brief Default destructor.
     */
    ~HistHadW() = default;
    
    /**
     * This function fills various histograms related to reference pT, jet eta, phi,
     * composition, and response. It handles both 1D and 2D histograms.
     * 
     * @param p4HadW TLorentzVector of the raw reference jet.
     * @param p4GenHadW TLorentzVector of the generated reference jet (if applicable).
     * @param ptHadW HadWerence pT value for scaling.
     * @param weight Event weight.
     */
    void Fill(const TLorentzVector& p4HadW, double avgHadPt, double chiSqr, int run, double weight);

private:
    // Struct holding all histograms
    HadWHistograms hist_;
    
    /**
     * @brief Initializes all histograms and sets up the internal structures.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     */
    void InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin);
};
