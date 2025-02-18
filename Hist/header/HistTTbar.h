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

struct TTbarHistograms {
    // 1D Histograms
    std::unique_ptr<TH1D> h1EventInHadWPt;
    std::unique_ptr<TH1D> h1EventInAvgHadPt;
    std::unique_ptr<TH1D> h1EventInHadWEta;
    std::unique_ptr<TH1D> h1EventInHadWPhi;
    std::unique_ptr<TH1D> h1EventInHadWMass;

    std::unique_ptr<TH1D> h1EventInHadTPt;
    std::unique_ptr<TH1D> h1EventInHadTEta;
    std::unique_ptr<TH1D> h1EventInHadTPhi;
    std::unique_ptr<TH1D> h1EventInHadTMass;

    std::unique_ptr<TH1D> h1EventInLepTPt;
    std::unique_ptr<TH1D> h1EventInLepTEta;
    std::unique_ptr<TH1D> h1EventInLepTPhi;
    std::unique_ptr<TH1D> h1EventInLepTMass;

    std::unique_ptr<TH1D> h1EventInChiSqr;
    // TProfile Histograms
    std::unique_ptr<TProfile> p1HadWMassInHadWPt;
    std::unique_ptr<TProfile> p1HadWMassInAvgHadPt;
    std::unique_ptr<TProfile> p1HadWMassInHadWEta;
    std::unique_ptr<TProfile> p1HadWMassInHadWPhi;

    std::unique_ptr<TProfile> p1HadTMassInHadTPt;
    std::unique_ptr<TProfile> p1HadTMassInHadTEta;
    std::unique_ptr<TProfile> p1HadTMassInHadTPhi;

    std::unique_ptr<TProfile> p1LepTMassInLepTPt;
    std::unique_ptr<TProfile> p1LepTMassInLepTEta;
    std::unique_ptr<TProfile> p1LepTMassInLepTPhi;

    std::unique_ptr<TProfile> p1HadWMassInRun;
    std::unique_ptr<TProfile> p1HadTMassInRun;
    std::unique_ptr<TProfile> p1LepTMassInRun;

};

/**
 * This class encapsulates the creation and filling of histograms that monitor
 * variables after at least one reference. It manages both 1D and 2D histograms using ROOT's TH1D, TH2D, and TProfile classes.
 */
class HistTTbar {
public:
    /**
     * @brief Constructs a HistTTbar object and initializes histograms.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     */
    HistTTbar(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin);
    
    /**
     * @brief Default destructor.
     */
    ~HistTTbar() = default;
    
    void Fill(const TLorentzVector& p4HadW, const TLorentzVector& p4HadT,
                const TLorentzVector& p4LepT, double avgHadPt, double chiSqr, double run, double weight);

private:
    // Struct holding all histograms
    TTbarHistograms hist_;
    
    /**
     * @brief Initializes all histograms and sets up the internal structures.
     * 
     * @param fout Pointer to the output ROOT file.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     */
    void InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin);
};

