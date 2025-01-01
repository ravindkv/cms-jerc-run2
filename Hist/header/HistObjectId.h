#ifndef HISTOBJECTID_H
#define HISTOBJECTID_H

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

// ROOT includes
#include <TH1D.h>
#include <TH2D.h>
#include <TDirectory.h>

// Custom includes
#include "VarBin.h"
#include "Helper.h"
#include "SkimTree.h"

/**
 * @brief Struct to hold all final histograms for specific directories.
 */
struct HistElectronId {
    std::unique_ptr<TH2D> h2EventInElePtEleEta;
};

struct HistPhotonId {
    std::unique_ptr<TH2D> h2EventInPhoEtaPhoPt;
    std::unique_ptr<TH2D> h2EventInPhoCountPhoPt;
    std::unique_ptr<TH2D> h2EventInPhoR9PhoPt;
    std::unique_ptr<TH2D> h2EventInPhoHoePhoPt;
    std::unique_ptr<TH2D> h2EventInPhoIdPhoPt;
};

struct HistMuonId {
    std::unique_ptr<TH2D> h2EventInMuPtMuEta;
};

struct HistJetId {
    std::unique_ptr<TH2D> h2EventInJetPtJetEta;
};

struct HistMetId {
    std::unique_ptr<TH2D> h2EventInMetPtMetPhi;
};
/**
 * @brief Class to handle histograms related to jet energy corrections.
 * 
 * This class encapsulates the creation and filling of histograms that monitor
 * variables after applying jet energy corrections. It manages histograms using ROOT's TH1D.
 */
class HistObjectId {
public:
    /**
     * @brief Constructs a HistObjectId object and initializes histograms.
     * 
     * @param origDir Pointer to the original ROOT directory.
     * @param directoryName Name of the directory within the ROOT file to store histograms.
     * @param varBin Object defining variable binning.
     */
    HistObjectId(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin);
    
    /**
     * @brief Default destructor.
     */
    ~HistObjectId() = default;
    
    void FillElectron(const SkimTree& skimT, double weight);
    void FillPhoton(const SkimTree& skimT, double weight);
    void FillMuon(const SkimTree& skimT, double weight);
    void FillJet(const SkimTree& skimT, double weight);
    void FillMet(const SkimTree& skimT, double weight);

private:
    // Struct holding all final histograms
    HistElectronId histEle_;
    HistPhotonId histPho_;
    HistMuonId histMu_;
    HistJetId histJet_;
    HistMetId histMet_;
    
    void InitializeHistograms(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin);
};

#endif // HISTOBJECTID_H

