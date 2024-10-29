#ifndef HISTREF_H
#define HISTREF_H

#include <memory>
#include <string>
#include <iostream>
#include <cassert>
#include <cmath>

// ROOT includes
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TLorentzVector.h>

/**
 * @brief Struct to hold all histograms related to "passAtleast1Ref".
 */
struct RefHistograms {
    // 1D Histograms
    std::unique_ptr<TH1D> h1EventInRefPt;
    std::unique_ptr<TH1D> h1EventInRefMass;
    std::unique_ptr<TH1D> h1EventInRefPtBarrel;
    std::unique_ptr<TH1D> h1EventInGenRefPt;

    // 2D Histograms
    std::unique_ptr<TH2D> h2EventInRefEtaRefPhi;
    std::unique_ptr<TH2D> h2EventInRefEtaRefPhiRebin;
    std::unique_ptr<TH2D> h2EventInRefEtaRefPhiForRefPt110;
    std::unique_ptr<TH2D> h2EventInRefEtaRefPhiForRefPt110Rebin;
    std::unique_ptr<TH2D> h2EventInRefPtCountRef;

    // TProfile Histograms
    std::unique_ptr<TProfile> p1RefMassInRefPt;
    std::unique_ptr<TProfile> p1RefPtOverGenRefPtInGenRefPt;
    std::unique_ptr<TProfile> p1GenRefPtOverRefPtInRefPt;

};

/**
 * @brief Class to handle histograms related to "passAtleast1Ref".
 * 
 * This class encapsulates the creation and filling of histograms that monitor
 * variables after at least one reference. It manages both 1D and 2D histograms using ROOT's TH1D, TH2D, and TProfile classes.
 */
class HistRef {
public:
    /**
     * @brief Constructs a HistRef object and initializes histograms.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param nPtBins Number of pT bins.
     * @param binsPt Array of pT bin edges.
     */
    HistRef(TFile* fout, const std::string& directoryName,
            int nPtBins, const double* binsPt);
    
    /**
     * @brief Default destructor.
     */
    ~HistRef() = default;
    
    /**
     * @brief Fills the "passAtleast1Ref" histograms based on event data.
     * 
     * This function fills various histograms related to reference pT, jet eta, phi,
     * composition, and response. It handles both 1D and 2D histograms.
     * 
     * @param p4Ref TLorentzVector of the raw reference jet.
     * @param p4GenRef TLorentzVector of the generated reference jet (if applicable).
     * @param ptRef Reference pT value for scaling.
     * @param weight Event weight.
     */
    void Fill(const int& nRef, const TLorentzVector& p4Ref, const TLorentzVector& p4GenRef, double weight);

private:
    // Struct holding all histograms
    RefHistograms hist_;
    
    /**
     * @brief Initializes all histograms and sets up the internal structures.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param nPtBins Number of pT bins.
     * @param binsPt Array of pT bin edges.
     */
    void InitializeHistograms(TFile* fout, const std::string& directoryName,
                              int nPtBins, const double* binsPt);
};

#endif // HISTREF_H

