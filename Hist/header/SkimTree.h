#ifndef EVENTTREE_H
#define EVENTTREE_H

#include<TFile.h>
#include<TTree.h>
#include<TChain.h>
#include<TH1D.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "GlobalFlag.h"
#include<vector>

using namespace std;
const Int_t maxP = 600;

class SkimTree: public GlobalFlag{
 public:
    SkimTree(TString oName): GlobalFlag(oName){
        fCurrent = -1;
    }; 
    ~SkimTree();
    Long64_t GetEntries();
    Int_t GetEntry(Long64_t entry);
    Int_t           fCurrent; //!current Tree number in a TChain
    virtual Long64_t loadEntry(Long64_t entry);

    // nCorrT1MetJet, nFatJet, nJet, nSoftActivityJet, nSubJet
    static const int nJetMax = 200;
    // nPhoton, nFsrPhoton, nGenIsolatedPhoton
    static const int nPhotonMax = 200;//100;//20;
    // nElectron, nGenDressedLepton
    static const int nElectronMax = 10;
    static const int nTauMax = 10;
    static const int nMuonMax = 20;

    // nGenJet, nGenJetAK8, nSubGenJetAK8
    static const int nGenJetMax = 100;
    static const int nGenPartMax = 200;
    static const int nIsoTrackMax = 10;
    static const int nLHEPartMax = 10;

    static const int nTrigObjMax = 50;
    static const int nSVMax = 100;

    static const int nPSWeightMax = 400;

//  Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    UInt_t          run;
    UInt_t          luminosityBlock;
    ULong64_t       event;
    UInt_t          bunchCrossing; // NanoV12
    
    Float_t         ChsMET_phi; // Run2
    Float_t         ChsMET_pt;  // Run2
    Float_t         RawPuppiMET_phi; // Run3
    Float_t         RawPuppiMET_pt;  // Run3


    //--------------------------------------- 
    // Photon (for Jets )
    //--------------------------------------- 
    //UInt_t          nJet; // NanoV11,10
    Int_t           nJet; // NanoV12
    Float_t         Jet_pt[nJetMax];
    Float_t         Jet_eta[nJetMax];
    Float_t         Jet_phi[nJetMax];
    Float_t         Jet_mass[nJetMax];

    Float_t         Jet_rawFactor[nJetMax];
    Float_t         Jet_area[nJetMax];
    //Int_t           Jet_jetId[nJetMax]; // NanoV10,11
    UChar_t         Jet_jetId[nJetMax];   // NanoV12

    Float_t         Jet_btagDeepB[nJetMax]; // Run2
    Float_t         Jet_btagDeepC[nJetMax]; // Run2
    Float_t         Jet_qgl[nJetMax];       // Run2

    Float_t         Jet_btagDeepFlavB[nJetMax];   // Run3
    Float_t         Jet_btagDeepFlavCvB[nJetMax]; // Run3
    Float_t         Jet_btagDeepFlavCvL[nJetMax]; // Run3
    Float_t         Jet_btagDeepFlavQG[nJetMax];  // Run3

    Float_t         Jet_chHEF[nJetMax];
    Float_t         Jet_neHEF[nJetMax];
    Float_t         Jet_neEmEF[nJetMax];
    Float_t         Jet_chEmEF[nJetMax];
    Float_t         Jet_muEF[nJetMax];
    //Float_t         Jet_chFPV0EF[nJetMax];   //[nJet]
    
    //Int_t           Jet_nConstituents[nJetMax];   //[nJet]
    //Int_t           Jet_nElectrons[nJetMax];   //[nJet]
    //Int_t           Jet_nMuons[nJetMax];   //[nJet]

    //Int_t           Jet_genJetIdx[nJetMax]; // NanoV11,10
    Short_t           Jet_genJetIdx[nJetMax]; // NanoV12

    Bool_t HLT_MC;
    Bool_t HLT_ZeroBias;
    Bool_t HLT_DiPFJetAve40;
    Bool_t HLT_DiPFJetAve60;
    Bool_t HLT_DiPFJetAve80;
    Bool_t HLT_DiPFJetAve140;
    Bool_t HLT_DiPFJetAve200;
    Bool_t HLT_DiPFJetAve260;
    Bool_t HLT_DiPFJetAve320;
    Bool_t HLT_DiPFJetAve400;
    Bool_t HLT_DiPFJetAve500;
    
    Bool_t HLT_PFJet40;
    Bool_t HLT_PFJet60;
    Bool_t HLT_PFJet80;
    Bool_t HLT_PFJet140;
    Bool_t HLT_PFJet200;
    Bool_t HLT_PFJet260;
    Bool_t HLT_PFJet320;
    Bool_t HLT_PFJet400; // v14
    Bool_t HLT_PFJet450;
    Bool_t HLT_PFJet500;
    Bool_t HLT_PFJet550;
    
    Bool_t HLT_DiPFJetAve60_HFJEC;
    Bool_t HLT_DiPFJetAve80_HFJEC;
    Bool_t HLT_DiPFJetAve100_HFJEC;
    Bool_t HLT_DiPFJetAve160_HFJEC;
    Bool_t HLT_DiPFJetAve220_HFJEC;
    Bool_t HLT_DiPFJetAve300_HFJEC;

    Bool_t HLT_PFJetFwd40;
    Bool_t HLT_PFJetFwd60;
    Bool_t HLT_PFJetFwd80;
    Bool_t HLT_PFJetFwd140;
    Bool_t HLT_PFJetFwd200;
    Bool_t HLT_PFJetFwd260;
    Bool_t HLT_PFJetFwd320;
    Bool_t HLT_PFJetFwd400;
    Bool_t HLT_PFJetFwd450;
    Bool_t HLT_PFJetFwd500;

   
    //--------------------------------------- 
    // Photon (for GamJet)
    //--------------------------------------- 
    //UInt_t          nPhoton; // NanoV11,10
    Int_t           nPhoton; // NanoV12
    Float_t         Photon_pt[nPhotonMax];
    Float_t         Photon_eta[nPhotonMax];
    Float_t         Photon_phi[nPhotonMax];
    Float_t         Photon_mass[nPhotonMax]; // Run2
    Float_t         Photon_hoe[nPhotonMax];
    //Int_t           Photon_cutBased[nPhotonMax]; // NanoV11,10
    UChar_t         Photon_cutBased[nPhotonMax];   // NanoV12
    Int_t           Photon_cutBasedBitmap[nPhotonMax];   // EXTRA
    //Int_t           Photon_jetIdx[nPhotonMax];   // NanoV10,V11
    Short_t         Photon_jetIdx[nPhotonMax];     // NanoV12
    UChar_t         Photon_seedGain[nPhotonMax];
    Float_t         Photon_r9[nPhotonMax];
    Float_t         Photon_eCorr[nPhotonMax];
    Float_t         Photon_energyErr[nPhotonMax];
    //Gen photon
    Int_t           nGenIsolatedPhoton;
    Float_t         GenIsolatedPhoton_pt[nPhotonMax];
    Float_t         GenIsolatedPhoton_eta[nPhotonMax];
    Float_t         GenIsolatedPhoton_phi[nPhotonMax];
    Float_t         GenIsolatedPhoton_mass[nPhotonMax];
    Int_t           GenIsolatedPhoton_pdgId[nPhotonMax];

    // Triggers from 2016
    Bool_t          HLT_Photon250_NoHE;
    Bool_t          HLT_Photon300_NoHE;
    // Triggers from 2016
    Bool_t          HLT_Photon22;
    Bool_t          HLT_Photon30;
    Bool_t          HLT_Photon36;
    Bool_t          HLT_Photon50;
    Bool_t          HLT_Photon75;
    Bool_t          HLT_Photon90;
    Bool_t          HLT_Photon120;
    Bool_t          HLT_Photon175;
    Bool_t          HLT_Photon165_HE10;
    Bool_t          HLT_Photon22_R9Id90_HE10_IsoM;
    Bool_t          HLT_Photon30_R9Id90_HE10_IsoM;
    Bool_t          HLT_Photon36_R9Id90_HE10_IsoM;
    Bool_t          HLT_Photon50_R9Id90_HE10_IsoM;
    Bool_t          HLT_Photon75_R9Id90_HE10_IsoM;
    Bool_t          HLT_Photon90_R9Id90_HE10_IsoM;
    Bool_t          HLT_Photon120_R9Id90_HE10_IsoM;
    Bool_t          HLT_Photon165_R9Id90_HE10_IsoM;
    // Triggers and branches from 22-23
    Bool_t          HLT_Photon100EBHE10;
    Bool_t          HLT_Photon30EB_TightID_TightIso;
    // Triggers and branches from 2018
    Bool_t          HLT_Photon100EB_TightID_TightIso;
    Bool_t          HLT_Photon110EB_TightID_TightIso;
    Bool_t          HLT_Photon120EB_TightID_TightIso;
    Bool_t          HLT_Photon200;
    Bool_t          HLT_Photon20_HoverELoose;
    Bool_t          HLT_Photon30_HoverELoose;
    Bool_t          HLT_Photon150;
    Bool_t          HLT_Photon33;
    Bool_t          HLT_Photon20;
    // Triggers from 2017
    Bool_t          HLT_Photon40_HoverELoose;
    Bool_t          HLT_Photon50_HoverELoose;
    Bool_t          HLT_Photon60_HoverELoose;

    //--------------------------------------- 
    // Electron (for DiEleJet)
    //--------------------------------------- 
    Int_t           nEle_;
    Float_t         elePhi_[100];
    Float_t         elePt_[100];
    Float_t         eleEta_[100];
    Float_t         eleDeltaEtaSC_[100];
    Int_t           eleCharge_[100];
    Float_t         eleMass_[100];
    Int_t           eleID_[100];
    Bool_t  HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;

    Int_t  nGenDressedLepton;
    Float_t GenDressedLepton_pt[100];
    Float_t GenDressedLepton_phi[100];
    Float_t GenDressedLepton_mass[100];
    Float_t GenDressedLepton_eta[100];
    Int_t   GenDressedLepton_pdgId[100];

    //--------------------------------------- 
    // Muon (for DiMuJet)
    //--------------------------------------- 
    Int_t          nMuon_;
    Float_t         muPhi_[100];
    Float_t         muPt_[100];
    Float_t         muEta_[100];
    Int_t           muCharge_[100];
    Float_t         muMass_[100];
    Float_t         muPFRelIso_[100];
    Float_t         muTkRelIso_[100];
    Bool_t          muMediumId_[100];
    Bool_t          muTightId_[100];
    Bool_t          muHighPurity_[100];
    Float_t         muDxy_[100];
    Float_t         muDz_[100];
    Bool_t  HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;

    Float_t         Rho; // Run2,Run3
    //UInt_t          nTrigObj; // NanoV11,10
    //Int_t          nTrigObj; // NanoV12
    //Float_t         TrigObj_pt[nTrigObjMax];   //[nTrigObj]
    //Float_t         TrigObj_eta[nTrigObjMax];   //[nTrigObj]
    //Float_t         TrigObj_phi[nTrigObjMax];   //[nTrigObj]
    //Float_t         TrigObj_l1pt[nTrigObjMax];   //[nTrigObj]
    //Float_t         TrigObj_l1pt_2[nTrigObjMax];   //[nTrigObj]
    //Float_t         TrigObj_l2pt[nTrigObjMax];   //[nTrigObj]
    //Int_t           TrigObj_id[nTrigObjMax];   //[nTrigObj]
    //Int_t           TrigObj_l1iso[nTrigObjMax];   //[nTrigObj]
    //Int_t           TrigObj_l1charge[nTrigObjMax];   //[nTrigObj]
    //Int_t           TrigObj_filterBits[nTrigObjMax];   //[nTrigObj]
    //UInt_t          nOtherPV;
    //Float_t         OtherPV_z[nSVMax];   //[nOtherPV]
    //Float_t         PV_ndof;
    //Float_t         PV_x;
    //Float_t         PV_y;
    //Float_t         PV_z;
    //Float_t         PV_chi2;
    //Float_t         PV_score;

    UChar_t           PV_npvs; // NanoV12
    UChar_t           PV_npvsGood; // NanoV12
    Float_t           PV_z;
    Float_t           GenVtx_z;
 
    Bool_t          Flag_HBHENoiseFilter;
    Bool_t          Flag_HBHENoiseIsoFilter;
    Bool_t          Flag_CSCTightHaloFilter;
    Bool_t          Flag_CSCTightHaloTrkMuUnvetoFilter;
    Bool_t          Flag_CSCTightHalo2015Filter;
    Bool_t          Flag_globalTightHalo2016Filter;
    Bool_t          Flag_globalSuperTightHalo2016Filter;
    Bool_t          Flag_HcalStripHaloFilter;
    Bool_t          Flag_hcalLaserEventFilter;
    Bool_t          Flag_EcalDeadCellTriggerPrimitiveFilter;
    Bool_t          Flag_EcalDeadCellBoundaryEnergyFilter;
    Bool_t          Flag_ecalBadCalibFilter;
    Bool_t          Flag_goodVertices;
    Bool_t          Flag_eeBadScFilter;
    Bool_t          Flag_ecalLaserCorrFilter;
    Bool_t          Flag_trkPOGFilters;
    Bool_t          Flag_chargedHadronTrackResolutionFilter;
    Bool_t          Flag_muonBadTrackFilter;
    Bool_t          Flag_BadChargedCandidateFilter;
    Bool_t          Flag_BadPFMuonFilter;
    Bool_t          Flag_BadPFMuonDzFilter;
    Bool_t          Flag_hfNoisyHitsFilter;
    Bool_t          Flag_BadChargedCandidateSummer16Filter;
    Bool_t          Flag_BadPFMuonSummer16Filter;
    Bool_t          Flag_trkPOG_manystripclus53X;
    Bool_t          Flag_trkPOG_toomanystripclus53X;
    Bool_t          Flag_trkPOG_logErrorTooManyClusters;
    Bool_t          Flag_METFilters;

    // Extras for MC
    //UInt_t          nGenJet; // NanoV11,10
    Int_t           nGenJet; // NanoV12
    Float_t         GenJet_eta[nGenJetMax];   //[nGenJet]
    Float_t         GenJet_mass[nGenJetMax];   //[nGenJet]
    Float_t         GenJet_phi[nGenJetMax];   //[nGenJet]
    Float_t         GenJet_pt[nGenJetMax];   //[nGenJet]
    //Int_t           GenJet_partonFlavour[nGenJetMax]; //[nGenJet] // NanoV10,11
    Short_t         GenJet_partonFlavour[nGenJetMax]; //[nGenJet] // NanoV12

    Float_t         LHE_HT;
    
    Float_t         genWeight;
    Float_t         Pileup_nTrueInt;
    //UInt_t          nPSWeight; // NanoV11,10
    Int_t           nPSWeight; // NanoV12
    Float_t         PSWeight[nPSWeightMax];   //[nPSWeight]

    //Inputs
    string iName;
    void setInput(string oName);
    void loadInput();
    string loadedSampKey = "MC_Year_Channel_Name";
    int loadedNthJob = 1;
    int loadedTotJob =100;

    //Inputs json
    string inputJsonPath = "./FilesSkim_2022_GamJet.json";
    void setInputJsonPath(string inDir); 
    void loadInputJson();
    vector<string> loadedAllFileNames;

    //Skim files and tree
    vector<string> loadedJobFileNames;
    void loadJobFileNames();
    TChain *fChain = new TChain("Events");   
    void loadTree();
    
    std::vector<std::vector<std::string>> splitVector(const std::vector<std::string>& strings, int n);
    std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);

};
#endif
