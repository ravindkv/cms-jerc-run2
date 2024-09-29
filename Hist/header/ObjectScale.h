#ifndef OBJECTSCALE_H
#define OBJECTSCALE_H
        
#include <set>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "SkimTree.h"
#include "correction.h"
#include "GlobalFlag.h"

#include "TH2D.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include <nlohmann/json.hpp>

class ObjectScale: public GlobalFlag{
  public: 
    ObjectScale(TString oName): GlobalFlag(oName){};
    ~ObjectScale();
    
    void setInputs();
    
    // Jet veto
    void loadJetVetoRef();
    bool checkJetVetoMap(SkimTree *tree);
    
    // L1 Offset (aka PU or L1RC) correction
    void loadJetL1FastJetRef();
    double getL1FastJetCorrection(double jetArea, double jetEta, double jetPt, double rho);
    
    // L2Relative (aka MCTruth or L2Relative) correction
    void loadJetL2RelativeRef();
    double getL2RelativeCorrection(double jetEta, double jetPhi, double jetPt);
    
    // L2L3Residual (aka L2Residual + L3Residual) correction
    void loadJetL2L3ResidualRef();
    double getL2L3ResidualCorrection(double jetEta, double jetPt);
    
    // resoJER 
    void loadJERResoRef();
    double getJERResolution(SkimTree *tree, int index);
    // sfJER 
    void loadJERSFRef();
    double getJERScaleFactor(SkimTree *tree, int index, string syst);
    // now corrJER using resoJER and sfJER
    double getJERCorrection(SkimTree *tree, int index, string syst);
    
    // Photon Scale and Smearing (Ss)
    void loadPhoSsRef();
    double getPhoScaleCorrection(string nomOrSyst, SkimTree *tree, int indexPho);
    double getPhoSmearCorrection(string nomOrSyst, SkimTree *tree, int indexPho);
    
    // Electron Scale and Smearing (Ss)
    void loadEleSsRef();
    
    // Lumi
    void loadLumiJson();
    bool checkGoodLumi(unsigned int &run, unsigned int &lumi);

    void PrintInfo(string info, bool printcout);
    
    double bThresh;
    double cThresh;
    void setThresh();
  
  private:
    // Jet veto
    string jetVetoJsonPath;
    string jetVetoName;
    string jetVetoKey;
    correction::Correction::Ref loadedJetVetoRef;
    
    string jercJsonPath;
    // L1 Offset (aka PU or L1RC) correction
    string jetL1FastJetName;
    correction::Correction::Ref loadedJetL1FastJetRef;
    
    // L2Relative (aka MCTruth or L2Relative) correction
    string jetL2RelativeName;
    correction::Correction::Ref loadedJetL2RelativeRef;

    // L3Absolute (aka MCTruth or L3Absolute) correction
    string jetL3AbsoluteName;
    correction::Correction::Ref loadedJetL3AbsoluteRef;
    
    // L2L3Residual (aka L2Residual + L3Residual) correction
    string jetL2L3ResidualName;
    correction::Correction::Ref loadedJetL2L3ResidualRef;
    
    // resoJER 
    string JERResoName;
    correction::Correction::Ref loadedJERResoRef;
    
    // sfJER 
    string JERSFName;
    correction::Correction::Ref loadedJERSFRef;
    TRandom* generator = new TRandom3(0);
    
    // Photon Scale and Smearing (Ss)
    string phoSsJsonPath;
    string phoSsName;
    correction::Correction::Ref loadedPhoSsRef;
    
    // Electron Scale and Smearing (Ss)
    string eleSsJsonPath;
    string eleSsName;
    correction::Correction::Ref loadedEleSsRef;
    
    // Lumi
    string lumiJsonPath;
    nlohmann::json loadedLumiJson;
    
    double minbXsec;
    double DELTAPHI(double phi1, double phi2);
    double DELTAR(double phi1, double phi2, double eta1, double eta2);
};


#endif
