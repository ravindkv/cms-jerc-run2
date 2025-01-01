#ifndef SCALEOBJECT_H
#define SCALEOBJECT_H

#include <set>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "SkimTree.h"
#include "correction.h"
#include "RoccoR.h"
#include "GlobalFlag.h"

#include "TRandom3.h"
#include <nlohmann/json.hpp>

class ScaleObject{
public: 
    // Constructor accepting a reference to GlobalFlag
    explicit ScaleObject(GlobalFlag& globalFlags);
    ~ScaleObject(){}
    
    void setInputs();

    // L1 Offset (aka PU or L1RC) correction
    void loadJetL1FastJetRef();
    double getL1FastJetCorrection(double jetArea, double jetEta, double jetPt, double rho) const;
    
    // L2Relative (aka MCTruth or L2Relative) correction
    void loadJetL2RelativeRef();
    double getL2RelativeCorrection(double jetEta, double jetPt) const;
    
    // L2L3Residual (aka L2Residual + L3Residual) correction
    void loadJetL2L3ResidualRef();
    double getL2L3ResidualCorrection(double jetEta, double jetPt) const;
    
    // resoJer 
    void loadJerResoRef();
    double getJerResolution(const SkimTree& skimT, int index) const;
    
    // sfJer 
    void loadJerSfRef();
    double getJerScaleFactor(const SkimTree& skimT, int index, const std::string& syst) const;

    // now corrJer using resoJer and sfJer
    double getJerCorrection(const SkimTree& skimT, int index, const std::string& syst) const;
    
    // Photon Scale and Smearing (Ss)
    void loadPhoSsRef();
    double getPhoScaleCorrection(const SkimTree& skimT, const std::string& nomOrSyst,  int indexPho) const;
    double getPhoSmearCorrection(const SkimTree& skimT, const std::string& nomOrSyst,  int indexPho) const;

    // Muon Rochester correction 
    void loadMuRochRef();
    double getMuRochCorrection(const SkimTree& skimT, int index, const std::string& syst) const;
    
    // Electron Scale and Smearing (Ss):  ONLY Syst. Nominal corrections are already applied in Nano
    // https://cms-talk.web.cern.ch/t/electron-scale-and-smearing-uncertainties-in-nanoaod/9311
    void loadEleSsRef();
    double getEleSsCorrection(const SkimTree& skimT, int index, const std::string& syst) const;
    
private:

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
    
    // resoJer 
    std::string JerResoName_;
    correction::Correction::Ref loadedJerResoRef_;
    
    // sfJer 
    std::string JerSfName_;
    correction::Correction::Ref loadedJerSfRef_;
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

