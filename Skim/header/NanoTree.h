#ifndef NANOTREE_H
#define NANOTREE_H

#include<iostream>
#include <fstream>
#include<TFile.h>
#include<TTree.h>
#include<TChain.h>
#include<TMath.h>
#include <map>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "GlobalFlag.h"

#include<vector>

using namespace std;
const Int_t maxP = 600;

class NanoTree: public GlobalFlag{
  public:
  NanoTree(string oName): GlobalFlag(oName){
    fCurrent = -1;
  };
  ~NanoTree();
  Long64_t GetEntries();
  Int_t GetEntry(Long64_t entry);
  Int_t           fCurrent; //!current Tree number in a TChain
  virtual Long64_t loadEntry(Long64_t entry);
  
  UInt_t    run_;
  ULong64_t event_;
  UInt_t    lumis_;
  //-----------------------------
  // Branch values
  //-----------------------------
  //2022, GamJet
  Bool_t HLT_Photon300_NoHE = false;
  Bool_t HLT_Photon20 = false;
  Bool_t HLT_Photon33 = false;
  Bool_t HLT_Photon50 = false;
  Bool_t HLT_Photon75 = false;
  Bool_t HLT_Photon90 = false;
  Bool_t HLT_Photon120 = false;
  Bool_t HLT_Photon150 = false;
  Bool_t HLT_Photon175 = false;
  Bool_t HLT_Photon200 = false;
  Bool_t HLT_Photon30EB_TightID_TightIso = false;
  Bool_t HLT_Photon100EB_TightID_TightIso = false;
  Bool_t HLT_Photon110EB_TightID_TightIso = false;
  Bool_t HLT_Photon120EB_TightID_TightIso = false;
  Bool_t HLT_Photon100EBHE10 = false;
  Bool_t HLT_Photon100EEHE10 = false;
  Bool_t HLT_Photon100EE_TightID_TightIso = false;
  Bool_t HLT_Photon50_R9Id90_HE10_IsoM = false;
  Bool_t HLT_Photon75_R9Id90_HE10_IsoM = false;
  Bool_t HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3 = false;
  Bool_t HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3 = false;
  Bool_t HLT_Photon90_R9Id90_HE10_IsoM = false;
  Bool_t HLT_Photon120_R9Id90_HE10_IsoM = false;
  Bool_t HLT_Photon165_R9Id90_HE10_IsoM = false;
  Bool_t HLT_Photon35_TwoProngs35 = false;
  Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL = false;
  Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL = false;
  Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15 = false;
  Bool_t HLT_Photon20_HoverELoose = false;
  Bool_t HLT_Photon30_HoverELoose = false;
  Bool_t HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50 = false;
  Bool_t HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3 = false;
  Bool_t HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3 = false;
  
  TBranch *b_HLT_Photon300_NoHE;
  TBranch *b_HLT_Photon20;
  TBranch *b_HLT_Photon33;
  TBranch *b_HLT_Photon50;
  TBranch *b_HLT_Photon75;
  TBranch *b_HLT_Photon90;
  TBranch *b_HLT_Photon120;
  TBranch *b_HLT_Photon150;
  TBranch *b_HLT_Photon175;
  TBranch *b_HLT_Photon200;
  TBranch *b_HLT_Photon30EB_TightID_TightIso;
  TBranch *b_HLT_Photon100EB_TightID_TightIso;
  TBranch *b_HLT_Photon110EB_TightID_TightIso;
  TBranch *b_HLT_Photon120EB_TightID_TightIso;
  TBranch *b_HLT_Photon100EBHE10;
  TBranch *b_HLT_Photon100EEHE10;
  TBranch *b_HLT_Photon100EE_TightID_TightIso;
  TBranch *b_HLT_Photon50_R9Id90_HE10_IsoM;
  TBranch *b_HLT_Photon75_R9Id90_HE10_IsoM;
  TBranch *b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3;
  TBranch *b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3;
  TBranch *b_HLT_Photon90_R9Id90_HE10_IsoM;
  TBranch *b_HLT_Photon120_R9Id90_HE10_IsoM;
  TBranch *b_HLT_Photon165_R9Id90_HE10_IsoM;
  TBranch *b_HLT_Photon35_TwoProngs35;
  TBranch *b_HLT_Photon60_R9Id90_CaloIdL_IsoL;
  TBranch *b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL;
  TBranch *b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15;
  TBranch *b_HLT_Photon20_HoverELoose;
  TBranch *b_HLT_Photon30_HoverELoose;
  TBranch *b_HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50;
  TBranch *b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3;
  TBranch *b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3;
  
  //additional for 2023
  Bool_t HLT_Photon50EB_TightID_TightIso = false;
  Bool_t HLT_Photon75EB_TightID_TightIso = false;
  Bool_t HLT_Photon90EB_TightID_TightIso = false;
  Bool_t HLT_Photon130EB_TightID_TightIso = false;
  Bool_t HLT_Photon150EB_TightID_TightIso = false;
  Bool_t HLT_Photon175EB_TightID_TightIso = false;
  Bool_t HLT_Photon200EB_TightID_TightIso = false;
  Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350 = false;
  Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380 = false;
  Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400 = false;
  Bool_t HLT_Photon32_OneProng32_M50To105 = false;
  
  TBranch *b_HLT_Photon50EB_TightID_TightIso;
  TBranch *b_HLT_Photon75EB_TightID_TightIso;
  TBranch *b_HLT_Photon90EB_TightID_TightIso;
  TBranch *b_HLT_Photon130EB_TightID_TightIso;
  TBranch *b_HLT_Photon150EB_TightID_TightIso;
  TBranch *b_HLT_Photon175EB_TightID_TightIso;
  TBranch *b_HLT_Photon200EB_TightID_TightIso;
  TBranch *b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350;
  TBranch *b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380;
  TBranch *b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400;
  TBranch *b_HLT_Photon32_OneProng32_M50To105;
  
  //2022+23, DiJet
  Bool_t HLT_PFJet40 = false;
  Bool_t HLT_PFJet60 = false;
  Bool_t HLT_PFJet80 = false;
  Bool_t HLT_PFJet140 = false;
  Bool_t HLT_PFJet200 = false;
  Bool_t HLT_PFJet260 = false;
  Bool_t HLT_PFJet320 = false;
  Bool_t HLT_PFJet400 = false;
  Bool_t HLT_PFJet450 = false;
  Bool_t HLT_PFJet500 = false;
  Bool_t HLT_DiPFJetAve40 = false;
  Bool_t HLT_DiPFJetAve60 = false;
  Bool_t HLT_DiPFJetAve80 = false;
  Bool_t HLT_DiPFJetAve140 = false;
  Bool_t HLT_DiPFJetAve200 = false;
  Bool_t HLT_DiPFJetAve260 = false;
  Bool_t HLT_DiPFJetAve320 = false;
  Bool_t HLT_DiPFJetAve400 = false;
  Bool_t HLT_DiPFJetAve500 = false;
  Bool_t HLT_DiPFJetAve60_HFJEC = false;
  Bool_t HLT_DiPFJetAve80_HFJEC = false;
  Bool_t HLT_DiPFJetAve100_HFJEC = false;
  Bool_t HLT_DiPFJetAve160_HFJEC = false;
  Bool_t HLT_DiPFJetAve220_HFJEC = false;
  Bool_t HLT_DiPFJetAve300_HFJEC = false;
  
  TBranch *b_HLT_PFJet40;
  TBranch *b_HLT_PFJet60;
  TBranch *b_HLT_PFJet80;
  TBranch *b_HLT_PFJet140;
  TBranch *b_HLT_PFJet200;
  TBranch *b_HLT_PFJet260;
  TBranch *b_HLT_PFJet320;
  TBranch *b_HLT_PFJet400;
  TBranch *b_HLT_PFJet450;
  TBranch *b_HLT_PFJet500;
  TBranch *b_HLT_DiPFJetAve40;
  TBranch *b_HLT_DiPFJetAve60;
  TBranch *b_HLT_DiPFJetAve80;
  TBranch *b_HLT_DiPFJetAve140;
  TBranch *b_HLT_DiPFJetAve200;
  TBranch *b_HLT_DiPFJetAve260;
  TBranch *b_HLT_DiPFJetAve320;
  TBranch *b_HLT_DiPFJetAve400;
  TBranch *b_HLT_DiPFJetAve500;
  TBranch *b_HLT_DiPFJetAve60_HFJEC;
  TBranch *b_HLT_DiPFJetAve80_HFJEC;
  TBranch *b_HLT_DiPFJetAve100_HFJEC;
  TBranch *b_HLT_DiPFJetAve160_HFJEC;
  TBranch *b_HLT_DiPFJetAve220_HFJEC;
  TBranch *b_HLT_DiPFJetAve300_HFJEC;
  
  Bool_t HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL = false;
  TBranch * b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
  
  Bool_t HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = false;
  TBranch * b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  
  Bool_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ = false; 
  TBranch *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ; 
  
  Bool_t HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8 = false; 
  TBranch *b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8; 
  
  //Inputs
  string outName;
  void setInput(string oName);
  void loadInput();
  string loadedSampKey = "MC_Year_Channel_Name";
  int loadedNthJob = 1;
  int loadedTotJob =100;
  
  //Inputs json
  string inputJsonPath = "./FilesNano_2022_GamJet.json";
  void setInputJsonPath(string inDir); 
  void loadInputJson();
  vector<string> loadedAllFileNames;
  
  //Nano files and tree
  vector<string> loadedJobFileNames;
  void loadJobFileNames();
  TChain *fChain = new TChain("Events");   
  void loadTree();
  std::vector<std::vector<std::string>> splitVector(const std::vector<std::string>& strings, int n);
  std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);

};

#endif
