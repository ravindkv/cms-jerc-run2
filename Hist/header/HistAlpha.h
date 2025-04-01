#ifndef HISTALPHA_H
#define HISTALPHA_H

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

// ROOT includes
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile2D.h>
#include <TProfile.h>
#include <TDirectory.h>

// Custom includes
#include "VarBin.h"

/**
 * @brief Class to create and fill histograms/TProfiles for different α‐cut values.
 *
 * For each value in alphaCuts, the following histograms are created:
 *  - h1EventInRefPtForAlphaXX
 *  - p1RhoInRefPtForAlphaXX
 *  - p1DpbRespInRefPtForAlphaXX
 *  - p1MpfRespInRefPtForAlphaXX
 *
 * where "XX" is the formatted α value.
 */
class HistAlpha {
public:
    /**
     * @brief Constructs a HistAlpha object and initializes histograms.
     * 
     * @param origDir Pointer to the output ROOT directory.
     * @param directoryName Directory name within the ROOT file.
     * @param varBin VarBin object for binning configurations.
     * @param alphaCuts Vector of α‐cut values.
     */
    HistAlpha(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin, const std::vector<double>& alphaCuts);
    
    ~HistAlpha() = default;
    
    /**
     * @brief Fills the histograms based on the event's α value and other parameters.
     * 
     * For each α‐cut histogram, if eventAlpha is below the cut value, the histograms are filled.
     * 
     * @param eventAlpha The event α value.
     * @param ptRef Reference pT value.
     * @param rho Rho value.
     * @param dbResp DB response value.
     * @param mpfResp MPF response value.
     * @param weight Event weight.
     */
    void Fill(double eventAlpha, double ptRef, double rho, double dbResp, double mpfResp, double weight);

private:
    // Maps keyed by the α‐cut value.
    std::map<double, std::unique_ptr<TH1D>> h1EventInRefPtForAlpha_;

    std::unique_ptr<TProfile> p1DbRespInAlphaForRefPt175to230_;
    std::unique_ptr<TProfile> p1MpfRespInAlphaForRefPt175to230_;

    std::unique_ptr<TProfile> p1DbRespInMaxAlphaForRefPt175to230_;
    std::unique_ptr<TProfile> p1MpfRespInMaxAlphaForRefPt175to230_;

    std::map<double, std::unique_ptr<TProfile>> p1RhoInRefPtForAlpha_;
    std::map<double, std::unique_ptr<TProfile>> p1DpbRespInRefPtForAlpha_;
    std::map<double, std::unique_ptr<TProfile>> p1MpfRespInRefPtForAlpha_;

    // Histogram for the event α distribution.
    std::unique_ptr<TH1D> h1EventInAlpha_;
    std::unique_ptr<TH2D> h2EventInRefPtAlpha_;
    std::unique_ptr<TProfile2D> p2DbRespInRefPtAlpha_;
    std::unique_ptr<TProfile2D> p2MpfRespInRefPtAlpha_;

    // Binning configuration (assumed common for all histograms)
    std::vector<double> binsPt_;
    int nPt_;

    // The α‐cut values used for creating the histograms
    std::vector<double> alphaCuts_;

    /**
     * @brief Initializes histograms for a specific α‐cut.
     * 
     * @param alphaCut The α‐cut value.
     * @param dir Pointer to the directory in which to create histograms.
     */
    void InitializeHistogramsForAlpha(double alphaCut, TDirectory* dir);
};

#endif // HISTALPHA_H

