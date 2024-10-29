#ifndef HISTTIME_H
#define HISTTIME_H

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>

// ROOT includes
#include <TH1D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TLorentzVector.h>

// User-defined includes
#include "SkimTree.h"
#include "ObjectScale.h"
#include "GlobalFlag.h"

/**
 * @brief Struct to hold all histograms for a specific pTRef threshold.
 */
struct RunHistograms {
    std::unique_ptr<TH1D> h1EventInRun;             ///< Histogram for event counts per run
    std::unique_ptr<TProfile> p1DbRespInRun;       ///< Profile histogram for balance factor per run
    std::unique_ptr<TProfile> p1MpfRespInRun;      ///< Profile histogram for momentum fraction per run
    std::unique_ptr<TProfile> p1JetChhefInRun;     ///< Profile histogram for jet chHEF per run
    std::unique_ptr<TProfile> p1JetNehefInRun;     ///< Profile histogram for jet neHEF per run
    std::unique_ptr<TProfile> p1JetNeemefInRun;    ///< Profile histogram for jet neEmEF per run
};

/**
 * @brief Class to handle run stability histograms.
 * 
 * This class encapsulates the creation and filling of histograms that monitor
 * the stability of variables across different runs, based on pTRef thresholds.
 */
class HistTime {
public:
    /**
     * @brief Constructs a HistTime object and initializes histograms.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param pTRefs Vector of pTRef thresholds to create histograms for.
     * @param runN Number of bins for the run histograms.
     * @param runMin Minimum run number.
     * @param runMax Maximum run number.
     */
    HistTime(TFile* fout, const std::string& directoryName, const std::vector<int>& pTRefs, int runN, double runMin, double runMax);
    
    /**
     * @brief Default destructor.
     */
    ~HistTime() = default;
    
    /**
     * @brief Fills the run stability histograms based on event data.
     * 
     * This function evaluates which pTRef thresholds the event satisfies and fills
     * the corresponding histograms with the provided data.
     * 
     * @param skimT Pointer to the SkimTree containing event data.
     * @param objS Pointer to the ObjectScale instance for scaling factors.
     * @param iJet1 Index of the first jet in the event.
     * @param bal Balance factor.
     * @param mpf Momentum fraction.
     * @param p4Jet1 TLorentzVector of the first jet.
     * @param weight Event weight.
     * @param ptRef Reference pT value for determining which histograms to fill.
     */
    void Fill(SkimTree* skimT, int iJet1, double bal, double mpf, double ptRef, double weight);

private:
    // Map from pTRef threshold to its corresponding RunHistograms
    std::map<int, RunHistograms> histMap_;
    
    // List of pTRef thresholds
    std::vector<int> pTRefs_;
    
    // Run histogram parameters
    int runN_;
    double runMin_;
    double runMax_;
    
    /**
     * @brief Initializes histograms for each pTRef threshold.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     */
    void InitializeHistograms(TFile* fout, const std::string& directoryName);
};

#endif // HISTTIME_H

