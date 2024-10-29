#ifndef TAGGED_H
#define TAGGED_H

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
#include "GlobalFlag.h"

class HistTag {
public:
    /**
     * @brief Constructs a HistTag object and initializes histograms.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param nPtBins Number of bins in pT.
     * @param binsPt Array of pT bin edges.
     */
    HistTag(TFile* fout, const std::string& directoryName, int nPtBins, const std::vector<double>binsPt, GlobalFlag& globalFlags);
    
    /**
     * @brief Default destructor.
     */
    ~HistTag() = default;

    /**
     * @brief Fills the histograms based on the event data.
     * 
     * This function computes the necessary variables, evaluates tagging conditions,
     * and fills the corresponding histograms.
     * 
     * @param skimT Pointer to the SkimTree containing event data.
     * @param iJet1 Index of the first jet in the event.
     * @param iGenJet Index of the generated jet (for MC simulations).
     */
    // Map to hold variable values
    std::map<std::string, double> mvar;

    void FillHistograms(
        SkimTree* skimT,
        double ptRef,
        int iJet1,
        int iGenJet,
        double weight
    );

private:
    // Nested map to store histograms: var -> tag -> flavor -> histogram
    std::map<std::string, std::map<std::string, std::map<std::string, std::unique_ptr<TH1>>>> varTagFlvMap_;
    
    // Tag and flavor identifiers
    std::vector<std::string> atag_;
    std::vector<std::string> aflv_;

    //https://btv-wiki.docs.cern.ch/ScaleFactors/UL2016postVFP/
    double threshBtagDeepFlavB_; 
    double threshBtagDeepFlavCvL_;
    double threshBtagDeepFlavCvB_;
    double threshBtagDeepFlavG_;
    double threshBtagDeepFlavQG_;
    double threshBtagDeepFlavUDS_;


    /**
     * @brief Initializes all histograms and sets up the internal maps.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param nPtBins Number of bins in pT.
     * @param binsPt Array of pT bin edges.
     */
    const GlobalFlag::Year year_;
    const bool isMC_;
    const bool isDebug_;
    void InitializeHistograms(TFile* fout, const std::string& directoryName, int nPtBins, const std::vector<double> binsPt);
};

#endif // TAGGED_H

