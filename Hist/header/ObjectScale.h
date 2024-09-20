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
        ObjectScale(TString oName): GlobalFlag(oName){
            debug  = false;
            jetVetoKey      ="jetvetomap";
            jetVetoName     = "Winter22Run3_RunCD_V1";
            jetVetoJsonPath = "./jetvetomaps.json";
            jetL2L3Names    = {};
            jetL2L3JsonPath = "./jet_jerc.json";
            phoSsName       = "2022Re-recoE+PromptFG_ScaleJSON";
            phoSsJsonPath   = "./photonSS.json";
            eleSsName       = "2022Re-recoE+PromptFG_ScaleJSON";
            eleSsJsonPath   = "./electronSS.json";
            lumiJsonPath    = "./Cert_Collisions2022_355100_362760_Golden.json";
            minbXsec        = 69200;
            bThresh         = 0.0;
            cThresh         = 0.0;
        };
        ~ObjectScale();
        // Jet veto
        string jetVetoKey;
        string jetVetoName;
        string jetVetoJsonPath;
        void setJetVetoKey(TString oName);
        void setJetVetoName(TString oName);
        void setJetVetoJsonPath(TString oName);
        void loadJetVetoRef();
        correction::Correction::Ref loadedJetVetoRef;
        bool checkJetVetoMap(SkimTree *tree);

        // L1 Offset (aka PU or L1RC) correction
        string jetL1FastJetName;
        string jetL1FastJetJsonPath;
        void setJetL1FastJetName(TString oName);
        void setJetL1FastJetJsonPath(TString oName);
        void loadJetL1FastJetRef();
        correction::Correction::Ref loadedJetL1FastJetRef;
        double getL1FastJetCorrection(double jetArea, double jetEta, double jetPt, double rho);

        // L2Relative (aka MCTruth or L2Relative) correction
        string jetL2RelativeName;
        string jetL2RelativeJsonPath;
        void setJetL2RelativeName(TString oName);
        void setJetL2RelativeJsonPath(TString oName);
        void loadJetL2RelativeRef();
        correction::Correction::Ref loadedJetL2RelativeRef;
        double getL2RelativeCorrection(double jetEta, double jetPhi, double jetPt);

        // L2L3Residual (aka L2Residual + L3Residual) correction
        string jetL2L3ResidualName;
        string jetL2L3ResidualJsonPath;
        void setJetL2L3ResidualName(TString oName);
        void setJetL2L3ResidualJsonPath(TString oName);
        void loadJetL2L3ResidualRef();
        correction::Correction::Ref loadedJetL2L3ResidualRef;
        double getL2L3ResidualCorrection(double jetEta, double jetPt);

        // Jet L2L3
        vector<string> jetL2L3Names;
        string jetL2L3JsonPath;
        void setJetL2L3Names(TString oName);
        void setJetL2L3JsonPath(TString oName);
        void loadJetL2L3Refs();
        vector<correction::Correction::Ref> loadedJetL2L3Refs;

        // resoJER 
        string JERResoName;
        string JERResoJsonPath;
        void setJERResoName(TString oName);
        void setJERResoJsonPath(TString oName);
        void loadJERResoRef();
        correction::Correction::Ref loadedJERResoRef;
        double getJERResolution(SkimTree *tree, int index);

        // sfJER 
        string JERSFName;
        string JERSFJsonPath;
        void setJERSFName(TString oName);
        void setJERSFJsonPath(TString oName);
        void loadJERSFRef();
        correction::Correction::Ref loadedJERSFRef;
        double getJERScaleFactor(SkimTree *tree, int index, string syst);

        // now corrJER using resoJER and sfJER
        TRandom* generator = new TRandom3(0);
        double getJERCorrection(SkimTree *tree, int index, string syst);


        // Photon Scale and Smearing (Ss)
        string phoSsName;
        string phoSsJsonPath;
        void setPhoSsName(TString oName);
        void setPhoSsJsonPath(TString oName);
        void loadPhoSsRef();
        correction::Correction::Ref loadedPhoSsRef;
        
        // Electron Scale and Smearing (Ss)
        string eleSsName;
        string eleSsJsonPath;
        void setEleSsName(TString oName);
        void setEleSsJsonPath(TString oName);
        void loadEleSsRef();
        correction::Correction::Ref loadedEleSsRef;

        // Lumi
        void PrintInfo(string info, bool printcout);
        string lumiJsonPath;
        void setLumiJsonPath(TString oName);
        void loadLumiJson();
        nlohmann::json loadedLumiJson;
        bool checkGoodLumi(unsigned int &run, unsigned int &lumi);

        double bThresh;
        double cThresh;
        void setThresh(TString oName);

    private:
        bool debug;
        double minbXsec;
				double DELTAPHI(double phi1, double phi2);
				double DELTAR(double phi1, double phi2, double eta1, double eta2);
};


#endif
