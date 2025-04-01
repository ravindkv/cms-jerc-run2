
#pragma once 

#include <set>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "SkimTree.h"
#include "correction.h"
#include "GlobalFlag.h"

#include <nlohmann/json.hpp>
#include <TLorentzVector.h>

class ScaleEvent{
public: 
    // Constructor accepting a reference to GlobalFlag
    explicit ScaleEvent(GlobalFlag& globalFlags);
    ~ScaleEvent(){}
    
    void setNormGenEventSumw(Double_t normGenEventSumw);
    void setLumiWeightInput(double lumiPerYear, double xsec, double nEventsNano);

    Double_t getNormGenEventSumw(){return normGenEventSumw_;} 
    Double_t getLumiWeight(){return lumiWeight_;} 

    // Jet veto
    void loadJetVetoRef();
    bool checkJetVetoMap(const SkimTree& skimT) const;
    bool checkJetVetoMapOnJet1(const TLorentzVector& p4Jet1) const;
    
    // Lumi
    void loadGoldenLumiJson();
    bool checkGoodLumi(const unsigned int& run, const unsigned int& lumi) const;

    void loadHltLumiJson();
    double getHltLumiPerRun(const std::string& hltPathBase, const std::string& runNumber) const;
    double cacheHltLumiPerRun(const std::shared_ptr<SkimTree>& skimT, const double& pt) const;
    // Debugging method to print the cache contents
    void printRunTriggerLumiCache() const;

    // Pileup
    void loadPuRef();
    double getPuCorrection(Float_t nTrueInt, const std::string& nomOrSyst) const;

private:

    // Jet veto
    std::string jetVetoJsonPath_;
    std::string jetVetoName_;
    std::string jetVetoKey_;
    correction::Correction::Ref loadedJetVetoRef_;
    
    // Lumi
    double lumiWeight_;
    std::string goldenLumiJsonPath_;
    nlohmann::json loadedGoldenLumiJson_;

    // Lumi
    std::string hltLumiJsonPath_;
    nlohmann::json loadedHltLumiJson_;

    // Two-level cache: run -> (trigger -> recorded luminosity)
    mutable std::unordered_map<int, std::unordered_map<std::string, double>> run2Trig2Lumi_;


    // Pileup
    std::string puJsonPath_;
    std::string puName_;
    correction::Correction::Ref loadedPuRef_;
    
    double minbXsec_{};
    Double_t normGenEventSumw_;

    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Era era_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;
    const bool isData_;
    const bool isMC_;

    // Load configuration from external JSON file
    void loadConfig(const std::string& filename);
};

