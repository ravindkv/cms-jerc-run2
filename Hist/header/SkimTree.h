#ifndef SKIMTREE_H
#define SKIMTREE_H

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "GlobalFlag.h"
#include "TrigDetail.h"

class SkimTree{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit SkimTree(GlobalFlag& globalFlags);
    ~SkimTree();

    // Tree operations
    Long64_t getEntries() const;
    TChain* getChain() const;  // Getter function to access fChain_
    Int_t getEntry(Long64_t entry);
    Long64_t loadEntry(Long64_t entry);

    // Input handling
    void setInput(const std::string& outName);
    void loadInput();

    void setInputJsonPath(const std::string& inDir);
    void loadInputJson();

    void loadJobFileNames();
    void loadTree();

    // Accessors for tree variables (public for direct access)
    // {} in the end is to initialise
    // Event information
    UInt_t run{};
    UInt_t luminosityBlock{};
    ULong64_t event{};
    UInt_t bunchCrossing{}; // NanoV12

    // MET information
    Float_t ChsMET_phi{};      // Run2
    Float_t ChsMET_pt{};       // Run2

    // Jet variables
    static const int nJetMax = 200;
    UInt_t nJet{}; // NanoV12
    Float_t Jet_pt[nJetMax]{};
    Float_t Jet_eta[nJetMax]{};
    Float_t Jet_phi[nJetMax]{};
    Float_t Jet_mass[nJetMax]{};

    Float_t Jet_rawFactor[nJetMax]{};
    Float_t Jet_area[nJetMax]{};
    UChar_t Jet_jetId[nJetMax]{}; // NanoV12

    Float_t Jet_btagDeepFlavB[nJetMax]{}; 
    Float_t Jet_btagDeepFlavCvL[nJetMax]{};
    Float_t Jet_btagDeepFlavCvB[nJetMax]{};
    Float_t Jet_btagDeepFlavG[nJetMax]{};
    Float_t Jet_btagDeepFlavQG[nJetMax]{};
    Float_t Jet_btagDeepFlavUDS[nJetMax]{};

    Float_t Jet_chHEF[nJetMax]{};
    Float_t Jet_neHEF[nJetMax]{};
    Float_t Jet_neEmEF[nJetMax]{};
    Float_t Jet_chEmEF[nJetMax]{};
    Float_t Jet_muEF[nJetMax]{};

    Short_t Jet_genJetIdx[nJetMax]{}; // NanoV12

    // Photon variables
    static const int nPhotonMax = 200;
    UInt_t nPhoton{}; // NanoV11,10
    Float_t Photon_pt[nPhotonMax]{};
    Float_t Photon_eta[nPhotonMax]{};
    Float_t Photon_phi[nPhotonMax]{};
    Float_t Photon_mass[nPhotonMax]{}; // Run2
    Float_t Photon_hoe[nPhotonMax]{};
    Int_t Photon_cutBased[nPhotonMax]{}; // NanoV11,10
    Short_t Photon_jetIdx[nPhotonMax]{}; // NanoV12
    UChar_t Photon_seedGain[nPhotonMax]{};
    Float_t Photon_r9[nPhotonMax]{};
    Float_t Photon_eCorr[nPhotonMax]{};
    Float_t Photon_energyErr[nPhotonMax]{};

    //HLT and MET Filters
    std::vector<std::string> getTrigNames() const {return trigNames_;}
    Bool_t getTrigValue(const std::string& trigName) const;
    std::vector<std::string> getFilterNames() const {return filterNames_;}
    Bool_t getFilterValue(const std::string& filterName) const;

    // Gen photon variables
    UInt_t nGenIsolatedPhoton{};
    Float_t GenIsolatedPhoton_pt[nPhotonMax]{};
    Float_t GenIsolatedPhoton_eta[nPhotonMax]{};
    Float_t GenIsolatedPhoton_phi[nPhotonMax]{};
    Float_t GenIsolatedPhoton_mass[nPhotonMax]{};

    // Electron variables
    static const int nElectronMax = 150;
    UInt_t nElectron{};
    Float_t Electron_phi[nElectronMax]{};
    Float_t Electron_pt[nElectronMax]{};
    Float_t Electron_eta[nElectronMax]{};
    Float_t Electron_deltaEtaSC[nElectronMax]{};
    Int_t Electron_charge[nElectronMax]{};
    Float_t Electron_mass[nElectronMax]{};
    Float_t Electron_eCorr[nElectronMax]{};
    Int_t Electron_cutBased[nElectronMax]{};

    // Gen lepton variables
    UInt_t nGenDressedLepton{};
    Float_t GenDressedLepton_pt[nElectronMax]{};
    Float_t GenDressedLepton_phi[nElectronMax]{};
    Float_t GenDressedLepton_mass[nElectronMax]{};
    Float_t GenDressedLepton_eta[nElectronMax]{};
    Int_t GenDressedLepton_pdgId[nElectronMax]{};

    // Muon variables
    static const int nMuonMax = 100;
    UInt_t nMuon{};
    Int_t Muon_nTrackerLayers[nMuonMax]{};
    Float_t Muon_phi[nMuonMax]{};
    Float_t Muon_pt[nMuonMax]{};
    Float_t Muon_eta[nMuonMax]{};
    Int_t Muon_charge[nMuonMax]{};
    Float_t Muon_mass[nMuonMax]{};
    Float_t Muon_pfRelIso04_all[nMuonMax]{};
    Float_t Muon_tkRelIso[nMuonMax]{};
    Bool_t Muon_mediumId[nMuonMax]{};
    Bool_t Muon_tightId[nMuonMax]{};
    Bool_t Muon_highPurity[nMuonMax]{};
    Float_t Muon_dxy[nMuonMax]{};
    Float_t Muon_dz[nMuonMax]{};

    // Other variables
    Float_t Rho{}; // Run2, Run3

    Int_t PV_npvs{};     // NanoV12
    Int_t PV_npvsGood{}; // NanoV12
    Float_t PV_z{};
    Float_t GenVtx_z{};

    // MC-specific variables
    static const int nGenJetMax = 100;
    UInt_t nGenJet{}; // NanoV11,10
    Float_t GenJet_eta[nGenJetMax]{};
    Float_t GenJet_mass[nGenJetMax]{};
    Float_t GenJet_phi[nGenJetMax]{};
    Float_t GenJet_pt[nGenJetMax]{};
    Short_t GenJet_partonFlavour[nGenJetMax]{}; // NanoV12

    Float_t LHE_HT{};

    Float_t genWeight{};
    Float_t Pileup_nTrueInt{};
    UInt_t nPSWeight{}; // NanoV11,10
    static const int nPSWeightMax = 400;
    Float_t PSWeight[nPSWeightMax]{}; // [nPSWeight]

private:
    // Member variables
    std::string outName_;
    std::string loadedSampKey_ = "MC_Year_Channel_Name";
    int loadedNthJob_ = 1;
    int loadedTotJob_ = 100;
    std::string inputJsonPath_ = "./FilesSkim_2022_GamJet.json";
    std::vector<std::string> loadedAllFileNames_;
    std::vector<std::string> loadedJobFileNames_;

    Int_t fCurrent_; // Current Tree number in a TChain

    // ROOT TChain
    std::unique_ptr<TChain> fChain_;

    // Disable copying and assignment
    SkimTree(const SkimTree&) = delete;
    SkimTree& operator=(const SkimTree&) = delete;

    // HLT
    TrigDetail trigDetail_;
    std::vector<std::string> trigNames_;
    std::vector<char> trigValues_; // Changed from std::vector<Bool_t> to std::vector<char>
    std::unordered_map<std::string, size_t> trigNameToIndex_;
    void initializeTriggers();

    // MET Filter
    std::vector<std::string> filterNames_;
    std::vector<char> filterValues_; // Changed from std::vector<Bool_t> to std::vector<char>
    std::unordered_map<std::string, size_t> filterNameToIndex_;
    void initializeFilters();
    
    //std::unordered_map<std::string, Bool_t> metFilterName2Values_;

    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Era era_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;
    const bool isData_;
    const bool isMC_;
};

#endif // SKIMTREE_H

