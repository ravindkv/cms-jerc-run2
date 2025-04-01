#ifndef HISTMAIN_H
#define HISTMAIN_H

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>

// ROOT includes
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TLorentzVector.h>

// User-defined includes
#include "SkimTree.h"
#include "VarBin.h"

/**
 * @brief Struct to hold all final histograms for specific directories.
 */
struct MainHistograms {
    // 1D Histograms
    std::unique_ptr<TH1D> h1EventInJet1Index;
    std::unique_ptr<TProfile> p1Jet1EtaInRefPt;
    std::unique_ptr<TH2D> h2EventInRefPtJet1Eta;
    
    // 1D Composition and Response
    std::unique_ptr<TProfile> p1DbRespInRefPt;
    std::unique_ptr<TProfile> p1MpfRespInRefPt;
    std::unique_ptr<TProfile> p1Mpf1RespInRefPt;
    std::unique_ptr<TProfile> p1MpfnRespInRefPt;
    std::unique_ptr<TProfile> p1MpfuRespInRefPt;
    std::unique_ptr<TProfile> p1MpfnuRespInRefPt;
    std::unique_ptr<TProfile> p1Jet1ChHefInRefPt;
    std::unique_ptr<TProfile> p1Jet1NeHefInRefPt;
    std::unique_ptr<TProfile> p1Jet1NeEmefInRefPt;
    std::unique_ptr<TProfile> p1Jet1ChEmefInRefPt;
    std::unique_ptr<TProfile> p1Jet1MuefInRefPt;
    std::unique_ptr<TProfile> p1Jet1Chpv0efInRefPt;
    
    // 2D Composition and Response
    std::unique_ptr<TProfile2D> p2DbRespInRefPtJet1Eta;
    std::unique_ptr<TProfile2D> p2MpfRespInRefPtJet1Eta;
    std::unique_ptr<TProfile2D> p2JetChHefInRefPtJet1Eta;
    std::unique_ptr<TProfile2D> p2JetNeHefInRefPtJet1Eta;
    std::unique_ptr<TProfile2D> p2JetNeEmefInRefPtJet1Eta;
    std::unique_ptr<TProfile2D> p2JetChEmefInRefPtJet1Eta;
    std::unique_ptr<TProfile2D> p2JetMuefInRefPtJet1Eta;
    std::unique_ptr<TProfile2D> p2JetChPv0efInRefPtJet1Eta;
    
    // 2D Composition and Response with Phi
    std::unique_ptr<TProfile2D> p2DbRespInJet1PhiJet1Eta;
    std::unique_ptr<TProfile2D> p2MpfRespInJet1PhiJet1Eta;
    std::unique_ptr<TProfile2D> p2JetChHefInJet1PhiJet1Eta;
    std::unique_ptr<TProfile2D> p2JetNeHefInJet1PhiJet1Eta;
    std::unique_ptr<TProfile2D> p2JetNeEmefInJet1PhiJet1Eta;
    std::unique_ptr<TProfile2D> p2JetChEmefInJet1PhiJet1Eta;
    std::unique_ptr<TProfile2D> p2JetMuefInJet1PhiJet1Eta;
    std::unique_ptr<TProfile2D> p2JetChPv0efInJet1PhiJet1Eta;
    
    // 2D Composition and Response for RefPt230
    std::unique_ptr<TProfile2D> p2DbRespInJet1PhiJet1EtaForRefPt230;
    std::unique_ptr<TProfile2D> p2MpfRespInJet1PhiJet1EtaForRefPt230;
    std::unique_ptr<TProfile2D> p2JetChHefInJet1PhiJet1EtaForRefPt230;
    std::unique_ptr<TProfile2D> p2JetNeHefInJet1PhiJet1EtaForRefPt230;
    std::unique_ptr<TProfile2D> p2JetNeEmefInJet1PhiJet1EtaForRefPt230;
    std::unique_ptr<TProfile2D> p2JetChEmefInJet1PhiJet1EtaForRefPt230;
    std::unique_ptr<TProfile2D> p2JetMuefInJet1PhiJet1EtaForRefPt230;
    std::unique_ptr<TProfile2D> p2JetChPv0efInJet1PhiJet1EtaForRefPt230;
    
    // Control Plots
    std::unique_ptr<TProfile> p1RhoInRefPt;
    std::unique_ptr<TProfile> p1NpvGoodInRefPt;
    std::unique_ptr<TProfile> p1NpvInRefPt;
    
};

/**
 * @brief Class to handle final histograms in the analysis.
 * 
 * This class encapsulates the creation and filling of various histograms related to jet eta, pTRef,
 * composition, and response. It manages both 1D and 2D histograms using ROOT's TH1D, TProfile,
 * TH2D, and TProfile2D classes.
 */
class HistMain {
public:
    /**
     * @brief Constructs a HistMain object and initializes histograms.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     */
    HistMain(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin);
    
    /**
     * @brief Default destructor.
     */
    ~HistMain() = default;
    
    /**
     * @brief Fills the final histograms based on event data.
     * 
     * This function fills various histograms related to jet eta, pTRef, composition, and response.
     * It handles both 1D and 2D histograms.
     * 
     * @param skimT Pointer to the SkimTree containing event data.
     * @param iJet1 Index of the first jet in the event.
     * @param bal Balance factor.
     * @param mpf Momentum fraction.
     * @param ptRef Reference pT value for scaling.
     * @param weight Event weight.
     */
    void Fill(SkimTree* skimT, int iJet1, double bal, double mpf, double ptRef, double weight);
    void FillOtherResp(double mpf1, double mpfn, double mpfu, double mpfnu, double ptRef, double weight);
    
private:
    // Map from pTRef thresholds to corresponding histograms (if needed)
    // Alternatively, manage histograms directly as member variables
    
    // Struct holding all final histograms
    MainHistograms hist_;

    // Run histogram parameters (if needed)
    // int runN_;
    // double runMin_;
    // double runMax_;
    
    /**
     * @brief Initializes all final histograms and sets up the internal structures.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     */
    void InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin);
};

#endif // HISTMAIN_H

