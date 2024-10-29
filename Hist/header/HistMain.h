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
#include <TFile.h>
#include <TLorentzVector.h>

// User-defined includes
#include "SkimTree.h"

/**
 * @brief Struct to hold all final histograms for specific directories.
 */
struct MainHistograms {
    // 1D Histograms
    std::unique_ptr<TProfile> p1JetEtaInRefPt;
    std::unique_ptr<TH2D> h2EventInJetEtaRefPt;
    
    // 1D Composition and Response
    std::unique_ptr<TProfile> p1DbRespInRefPt;
    std::unique_ptr<TProfile> p1MpfRespInRefPt;
    std::unique_ptr<TProfile> p1JetChhefInRefPt;
    std::unique_ptr<TProfile> p1JetNehefInRefPt;
    std::unique_ptr<TProfile> p1JetNeemefInRefPt;
    std::unique_ptr<TProfile> p1JetChemefInRefPt;
    std::unique_ptr<TProfile> p1JetMuefInRefPt;
    std::unique_ptr<TProfile> p1JetChpv0efInRefPt;
    
    // 2D Composition and Response
    std::unique_ptr<TProfile2D> p2DbRespInJet1EtaRefPt;
    std::unique_ptr<TProfile2D> p2MpfRespInJet1EtaRefPt;
    std::unique_ptr<TProfile2D> p2JetChhefInJet1EtaRefPt;
    std::unique_ptr<TProfile2D> p2JetNehefInJet1EtaRefPt;
    std::unique_ptr<TProfile2D> p2JetNeemefInJet1EtaRefPt;
    std::unique_ptr<TProfile2D> p2JetChemefInJet1EtaRefPt;
    std::unique_ptr<TProfile2D> p2JetMuefInJet1EtaRefPt;
    std::unique_ptr<TProfile2D> p2JetChfpv0efInJet1EtaRefPt;
    
    // 2D Composition and Response with Phi
    std::unique_ptr<TProfile2D> p2DbRespInJet1EtaJet1Phi;
    std::unique_ptr<TProfile2D> p2MpfRespInJet1EtaJet1Phi;
    std::unique_ptr<TProfile2D> p2JetChhefInJet1EtaJet1Phi;
    std::unique_ptr<TProfile2D> p2JetNehefInJet1EtaJet1Phi;
    std::unique_ptr<TProfile2D> p2JetNeemefInJet1EtaJet1Phi;
    std::unique_ptr<TProfile2D> p2JetChemefInJet1EtaJet1Phi;
    std::unique_ptr<TProfile2D> p2JetMuefInJet1EtaJet1Phi;
    std::unique_ptr<TProfile2D> p2JetChfpv0efInJet1EtaJet1Phi;
    
    // 2D Composition and Response for RefPt230
    std::unique_ptr<TProfile2D> p2DbRespInJet1EtaJet1PhiForRefPt230;
    std::unique_ptr<TProfile2D> p2MpfRespInJet1EtaJet1PhiForRefPt230;
    std::unique_ptr<TProfile2D> p2JetChhefInJet1EtaJet1PhiForRefPt230;
    std::unique_ptr<TProfile2D> p2JetNehefInJet1EtaJet1PhiForRefPt230;
    std::unique_ptr<TProfile2D> p2JetNeemefInJet1EtaJet1PhiForRefPt230;
    std::unique_ptr<TProfile2D> p2JetChemefInJet1EtaJet1PhiForRefPt230;
    std::unique_ptr<TProfile2D> p2JetMuefInJet1EtaJet1PhiForRefPt230;
    std::unique_ptr<TProfile2D> p2JetChfpv0efInJet1EtaJet1PhiForRefPt230;
    
    // Control Plots
    std::unique_ptr<TProfile> p1RhoInRefPt;
    std::unique_ptr<TProfile> p1NpvgoodInRefPt;
    std::unique_ptr<TProfile> p1NpvInRefPt;
    
    // 2D Event Control Plots
    std::unique_ptr<TH2D> h2EventInRefPtDbRespPassMpf;
    std::unique_ptr<TH2D> h2EventInRefPtMpfRespPassDb;
    std::unique_ptr<TH2D> h2EventInRefPtDbRespPassBoth;
    std::unique_ptr<TH2D> h2EventInRefPtMpfRespPassBoth;
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
     * @param nPtBins Number of pT bins.
     * @param binsPt Array of pT bin edges.
     * @param nEtaBins Number of eta bins.
     * @param binsEta Array of eta bin edges.
     * @param nPhiBins Number of phi bins.
     * @param binsPhi Array of phi bin edges.
     */
    HistMain(TFile* fout, const std::string& directoryName,
              int nPtBins, const double* binsPt,
              int nEtaBins, const double* binsEta,
              int nPhiBins, const double* binsPhi);
    
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
    void Fill(SkimTree* skimT, int iJet1, double bal, double mpf,  double ptRef, double weight);
    
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
     * @param nPtBins Number of pT bins.
     * @param binsPt Array of pT bin edges.
     * @param nEtaBins Number of eta bins.
     * @param binsEta Array of eta bin edges.
     * @param nPhiBins Number of phi bins.
     * @param binsPhi Array of phi bin edges.
     */
    void InitializeHistograms(TFile* fout, const std::string& directoryName,
                              int nPtBins, const double* binsPt,
                              int nEtaBins, const double* binsEta,
                              int nPhiBins, const double* binsPhi);
};

#endif // HISTMAIN_H

