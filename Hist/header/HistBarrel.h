#ifndef HISTBARREL_H
#define HISTBARREL_H

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

// Custom includes
#include "VarBin.h"

/**
 * @brief Struct to hold all histograms related to the barrel region.
 */
struct BarrelHistograms {
    // 1D Histograms
    std::unique_ptr<TH1D> h1EventInDeltaPhiJetRef;
    std::unique_ptr<TH1D> h1EventInDeltaRadiusJetRef;

    // TProfile Histograms
    std::unique_ptr<TProfile> p1GenJet1PtOverRefPtInRefPt;
    std::unique_ptr<TProfile> p1Jet1PtOverRefPtInRefPt;
    std::unique_ptr<TProfile> p1Jet1PtOverGenJet1PtInGenJet1Pt;
};

/**
 * @brief Class to handle histograms related to the barrel region (|eta| < 1.33).
 * 
 * This class encapsulates the creation and filling of histograms that monitor
 * variables after selecting jets in the barrel region. It manages 1D, 2D, and TProfile histograms using ROOT's TH1D, TH2D, and TProfile classes.
 */
class HistBarrel {
public:
    /**
     * @brief Constructs a HistBarrel object and initializes histograms.
     * 
     * @param origDir Pointer to the original ROOT directory.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param varBin Reference to the VarBin object containing binning configurations.
     */
    HistBarrel(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin);
    
    /**
     * @brief Default destructor.
     */
    ~HistBarrel() = default;
    
    /**
     * @brief Fills the barrel histograms based on event data.
     * 
     * It handles both 1D and 2D histograms as well as TProfile histograms.
     * 
     * @param p4Ref TLorentzVector of the reference jet.
     * @param p4Jet1 TLorentzVector of the first jet.
     * @param p4GenJet1 TLorentzVector of the generated first jet.
     * @param weight Event weight.
     */
    void Fill(const TLorentzVector& p4Ref, const TLorentzVector& p4Jet1, const TLorentzVector& p4GenJet1, double weight);

private:
    // Struct holding all histograms
    BarrelHistograms hist_;

    /**
     * @brief Initializes all histograms and sets up the internal structures.
     * 
     * @param origDir Pointer to the original ROOT directory.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param varBin Reference to the VarBin object containing binning configurations.
     */
    void InitializeHistograms(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin);
};

#endif // HISTBARREL_H

