#ifndef HISTTIME_H
#define HISTTIME_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>

// ROOT includes
#include <TH1D.h>
#include <TProfile.h>
#include <TLorentzVector.h>

// User-defined includes
#include "SkimTree.h"
#include "ScaleObject.h"
#include "VarBin.h"

/**
 * @brief Struct to hold all histograms for a specific pTRef threshold.
 */
struct RunHistograms {
    std::unique_ptr<TH1D> h1EventInRun;          
    std::unique_ptr<TProfile> p1DbRespInRun;     
    std::unique_ptr<TProfile> p1MpfRespInRun;    
    std::unique_ptr<TProfile> p1Jet1ChHefInRun;   
    std::unique_ptr<TProfile> p1Jet1NeHefInRun;   
    std::unique_ptr<TProfile> p1Jet1NeEmefInRun;  
    std::unique_ptr<TProfile> p1Jet1ChEmefInRun;  
    std::unique_ptr<TProfile> p1Jet1MuefInRun;  
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
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param pTRefs Vector of pTRef thresholds to create histograms for.
     * @param runN Number of bins for the run histograms.
     * @param runMin Minimum run number.
     * @param runMax Maximum run number.
     */
    HistTime(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin, const std::vector<int>& pTRefs);
    
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
     * @param scaleObj Pointer to the ScaleObject instance for scaling factors.
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
    std::unordered_map<int, RunHistograms> histMap_;
    
    // List of pTRef thresholds
    std::vector<int> pTRefs_;
    
    // Run histogram parameters
    int runN_;
    double runMin_;
    double runMax_;
    
    /**
     * @brief Initializes histograms for each pTRef threshold.
     * 
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     */
    void InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin);
};

#endif // HISTTIME_H

