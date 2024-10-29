#ifndef OBJECTSCALE_H
#define OBJECTSCALE_H

#include <set>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "SkimTree.h"
#include "correction.h"
#include "RoccoR.h"
#include "GlobalFlag.h"

#include "TH2D.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include <nlohmann/json.hpp>

class ObjectScale{
public: 
    // Constructor accepting a reference to GlobalFlag
    explicit ObjectScale(GlobalFlag& globalFlags);
    ~ObjectScale(){}
    
    // Set the SkimTree pointer
    void setTree(const std::shared_ptr<SkimTree>& skimTree);
    
    void setInputs();

    // Jet veto
    void loadJetVetoRef();
    bool checkJetVetoMap() const;
    
    // L1 Offset (aka PU or L1RC) correction
    void loadJetL1FastJetRef();
    double getL1FastJetCorrection(double jetArea, double jetEta, double jetPt, double rho) const;
    
    // L2Relative (aka MCTruth or L2Relative) correction
    void loadJetL2RelativeRef();
    double getL2RelativeCorrection(double jetEta, double jetPt) const;
    
    // L2L3Residual (aka L2Residual + L3Residual) correction
    void loadJetL2L3ResidualRef();
    double getL2L3ResidualCorrection(double jetEta, double jetPt) const;
    
    // resoJER 
    void loadJERResoRef();
    double getJERResolution( int index) const;
    
    // sfJER 
    void loadJERSFRef();
    double getJERScaleFactor( int index, const std::string& syst) const;

    // now corrJER using resoJER and sfJER
    double getJERCorrection( int index, const std::string& syst) const;
    
    // Photon Scale and Smearing (Ss)
    void loadPhoSsRef();
    double getPhoScaleCorrection(const std::string& nomOrSyst,  int indexPho) const;
    double getPhoSmearCorrection(const std::string& nomOrSyst,  int indexPho) const;

    // Muon Rochester correction 
    void loadMuRochRef();
    double getMuRochCorrection( int index, const std::string& syst) const;
    
    // Electron Scale and Smearing (Ss)
    void loadEleSsRef();
    double getEleSsCorrection( int index, const std::string& syst) const;
    
    // Lumi
    void loadLumiJson();
    bool checkGoodLumi(const unsigned int& run, const unsigned int& lumi) const;

    // Pileup
    void loadPuRef();
    double getPuCorrection(Float_t nTrueInt, const std::string& nomOrSyst) const;

private:
    // Pointer to SkimTree
    std::shared_ptr<SkimTree> skimTree_;

    // Jet veto
    std::string jetVetoJsonPath_;
    std::string jetVetoName_;
    std::string jetVetoKey_;
    correction::Correction::Ref loadedJetVetoRef_;
    
    std::string jercJsonPath_;
    
    // L1 Offset (aka PU or L1RC) correction
    std::string jetL1FastJetName_;
    correction::Correction::Ref loadedJetL1FastJetRef_;
    
    // L2Relative (aka MCTruth or L2Relative) correction
    std::string jetL2RelativeName_;
    correction::Correction::Ref loadedJetL2RelativeRef_;

    // L3Absolute (aka MCTruth or L3Absolute) correction
    std::string jetL3AbsoluteName_;
    correction::Correction::Ref loadedJetL3AbsoluteRef_;
    
    // L2L3Residual (aka L2Residual + L3Residual) correction
    std::string jetL2L3ResidualName_;
    correction::Correction::Ref loadedJetL2L3ResidualRef_;
    
    // resoJER 
    std::string JERResoName_;
    correction::Correction::Ref loadedJERResoRef_;
    
    // sfJER 
    std::string JERSFName_;
    correction::Correction::Ref loadedJERSFRef_;
	std::unique_ptr<TRandom> randomNumGen = std::make_unique<TRandom3>(0);
    
    // Photon Scale and Smearing (Ss)
    std::string phoSsJsonPath_;
    std::string phoSsName_;
    correction::Correction::Ref loadedPhoSsRef_;
    
    // Electron Scale and Smearing (Ss)
    std::string eleSsJsonPath_;
    std::string eleSsName_;
    correction::Correction::Ref loadedEleSsRef_;
    
    // Muon rochester corrections 
    std::string muRochJsonPath_;
    RoccoR loadedRochRef_; 

    // Lumi
    std::string lumiJsonPath_;
    nlohmann::json loadedLumiJson_;

    // Pileup
    std::string puJsonPath_;
    std::string puName_;
    correction::Correction::Ref loadedPuRef_;
    
    double minbXsec_{};

    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Era era_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;
    const bool isData_;
    const bool isMC_;
};

#endif

