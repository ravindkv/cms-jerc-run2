#pragma once

#include <iostream>
#include "GlobalFlag.h"

struct JetCuts {
    float minPt;
    float maxEta;
    float maxEtaLight;
    int   minId;
    int   nJet;
};

struct LeptonCuts {
    double mass;
};

struct BJetCuts {
    float minDisc;
    int   nBJet;
};

class VarCut {
public:
    explicit VarCut(const GlobalFlag& globalFlags);
    void InitializeCuts();
    void print() const;

    // All magic numbers with clear comments
    static constexpr int tTreeCatchSize = 52428800; // Buffer size for TTree cache (50 MB)
    static constexpr unsigned long cutFlowBinMargin = 0.5; // Bin margin for cut flow
    static constexpr int coutTabWidth = 5; // Tab width for console output

    static constexpr bool applyJer = false; // Apply JER correction
    static constexpr double MassW = 80.4; 
    static constexpr double MassT = 172.0; 
    static constexpr double MinChiSqr = 50; 

    // Accessors for each group of cuts
    const JetCuts& getJetCuts() const { return jetCuts_; }
    const LeptonCuts& getLeptonCuts() const { return leptonCuts_; }
    const BJetCuts& getBJetCuts() const { return bJetCuts_; }

private:
    const GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;

    // Grouped configuration for each object type
    JetCuts jetCuts_;
    LeptonCuts leptonCuts_;
    BJetCuts bJetCuts_;
};
