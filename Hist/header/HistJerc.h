#ifndef HISTJERC_H
#define HISTJERC_H

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

// ROOT includes
#include <TH1D.h>
#include <TDirectory.h>

// Custom includes
#include "VarBin.h"
#include "Helper.h"
#include "ApplyJerc.h"

/**
 * @brief Class to handle histograms related to jet energy corrections.
 * 
 * This class encapsulates the creation and filling of histograms that monitor
 * variables after applying jet energy corrections. It manages histograms using ROOT's TH1D.
 */
class HistJerc {
public:
    /**
     * @brief Constructs a HistJerc object and initializes histograms.
     * 
     * @param origDir Pointer to the original ROOT directory.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param varBin Object defining variable binning.
     */
    HistJerc(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin);
    
    /**
     * @brief Default destructor.
     */
    ~HistJerc() = default;
    
    /**
     * @brief Fills the JEC histograms based on the corrected jet variables.
     * 
     * @param appliedJerc Instance of ApplyJerc containing corrected jet variables.
     * @param weight Event weight.
     */
    void Fill(const ApplyJerc& appliedJerc, double weight);
    
private:
    // Map to hold histograms for jet1 and jetSum corrections
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histJet1Pt_;
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histJet1Mass_;
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histJetSumPt_;
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histJetSumMass_;

    std::unordered_map<std::string, std::unique_ptr<TH1D>> histMetPt_;
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histMetPhi_;
    
    // Correction names
    std::vector<std::string> jecNames_;
    std::vector<std::string> metNames_;
    
    /**
     * @brief Initializes all histograms and sets up the internal structures.
     * 
     * @param origDir Pointer to the original ROOT directory.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param varBin Object defining variable binning.
     */
    void InitializeHistograms(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin);
};

#endif // HISTJERC_H

