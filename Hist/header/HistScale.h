#ifndef HISTSCALE_H
#define HISTSCALE_H

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
#include "ScaleElectron.h"
#include "ScaleMuon.h"
#include "ScalePhoton.h"
#include "ScaleJetMet.h"

/**
 * @brief Class to handle histograms related to jet energy corrections.
 * 
 * This class encapsulates the creation and filling of histograms that monitor
 * variables after applying jet energy corrections. It manages histograms using ROOT's TH1D.
 */
class HistScale {
public:
    /**
     * @brief Constructs a HistScale object and initializes histograms.
     * 
     * @param origDir Pointer to the original ROOT directory.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param varBin Object defining variable binning.
     */
    HistScale(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin);
    
    /**
     * @brief Default destructor.
     */
    ~HistScale() = default;
    
    void FillElectron(const ScaleElectron& scaleElectron);
    void FillMuon(const ScaleMuon& scaleMuon);
    void FillPhoton(const ScalePhoton& scalePhoton);
    void FillJetMet(const ScaleJetMet& scaleJetMet);
    
private:
    // Map to hold histograms for Electron
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histElectron1Pt_;

    // Map to hold histograms for Muon
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histMuon1Pt_;

    // Map to hold histograms for Photon
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histPhoton1Pt_;

    // Map to hold histograms for Jet
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histJet1Pt_;
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histJetSumPt_;

    // Map to hold histograms for Met
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histMetPt_;
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histMetPhi_;
    
    // Map to hold histograms for JetMet
    std::unordered_map<std::string, std::unique_ptr<TH1D>> histJetMetSumPt_;

    // Correction names
    std::vector<std::string> jecNames_;
    std::vector<std::string> corrNames_;
    
    void InitializeHistograms(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin);
};

#endif // HISTSCALE_H

