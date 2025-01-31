// HistMultiJet.h
#ifndef HISTMULTIJET_H
#define HISTMULTIJET_H

#include <string>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>

#include "VarBin.h"
#include "SkimTree.h"

// In HistMultiJet.h (near the top or in a separate header if you prefer):

/// A struct to hold all per-event quantities that we want to fill into HistMultiJet.
/// Add or remove fields as needed for your analysis.
struct HistMultiJetInputs {
    double ptAvgProj   = 0.0;
    double ptAverage   = 0.0;
    double ptLead      = 0.0;
    double etaLead     = 0.0;
    double phiLead     = 0.0;
    double ptRecoil    = 0.0;
    double phiRecoil   = 0.0;
    double cRecoil     = 0.0;
    double response    = 0.0;
    double m0b         = 0.0;
    double m3b         = 0.0;
    double mnb         = 0.0;
    double mub         = 0.0;

    double m0m         = 0.0;
    double m3m         = 0.0;
    double mnm         = 0.0;
    double mum         = 0.0;

    double m0l         = 0.0;
    double m3l         = 0.0;
    double mnl         = 0.0;
    double mul         = 0.0;

    double m0r         = 0.0;
    double m3r         = 0.0;
    double mnr         = 0.0;
    double mur         = 0.0;
    double weight      = 1.0;
};

class HistMultiJet {
public:
    // Constructor
    HistMultiJet(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin);

    // Destructor
    ~HistMultiJet();

    // 1) A method to store all user inputs:
    void setInputs(const HistMultiJetInputs& inputs);

    // 2) A method to fill the histograms, using the stored inputs:
    void fillHistos();

    // Basic information about the trigger
    std::string triggerName;
    int trigPt;
    double ptMin, ptMax, absEtaMin, absEtaMax;

    // Histograms and profiles
    TH1D* h1EventInAvgProjPt;
    TH1D* h1EventInAvgPt;
    TH1D* h1EventInLeadPt;
    TH1D* h1EventInRecoilPt;

    TProfile* p1RespInAvgProjPt;
    TProfile* p1RespInAvgPt;
    TProfile* p1RespInLeadPt;
    TProfile* p1RespInRecoilPt;

    TProfile* p1LeadPtInAvgProjPt;
    TProfile* p1LeadPtInAvgPt;
    TProfile* p1LeadPtInLeadPt;
    TProfile* p1LeadPtInRecoilPt;

    TProfile* p1CrecoilInAvgProjPt;
    TProfile* p1CrecoilInAvgPt;
    TProfile* p1CrecoilInLeadPt;
    TProfile* p1CrecoilInRecoilPt;

    TProfile* p1MpfResp0InAvgProjPt;
    TProfile* p1MpfResp3InAvgProjPt;
    TProfile* p1MpfRespnInAvgProjPt;
    TProfile* p1MpfRespuInAvgProjPt;

    TProfile* p1MpfResp0InAvgPt;
    TProfile* p1MpfResp3InAvgPt;
    TProfile* p1MpfRespnInAvgPt;
    TProfile* p1MpfRespuInAvgPt;

    TProfile* p1MpfResp0InLeadPt;
    TProfile* p1MpfResp3InLeadPt;
    TProfile* p1MpfRespnInLeadPt;
    TProfile* p1MpfRespuInLeadPt;

    TProfile* p1MpfResp0InRecoilPt;
    TProfile* p1MpfResp3InRecoilPt;
    TProfile* p1MpfRespnInRecoilPt;
    TProfile* p1MpfRespuInRecoilPt;

    // 2D recoils
    TH2D* h2JetsPtInAvgProjPt;
    TH2D* h2JetsPtInAvgPt;
    TH2D* h2JetsPtInLeadPt;
    TH2D* h2JetsPtInRecoilPt;

    // PF composition plots
    TProfile* p1RhoInAvgProjPt;
    TProfile* p1JetsPtInAvgProjPt;
    TProfile* p1JetsChfInAvgProjPt;
    TProfile* p1JetsNhfInAvgProjPt;
    TProfile* p1JetsNefInAvgProjPt;
    TProfile* p1JetsCefInAvgProjPt;
    TProfile* p1JetsMufInAvgProjPt;

    TProfile* p1RhoInAvgProjPtForLeadEta1p3;
    TProfile* p1Jet1PtInAvgProjPtForLeadEta1p3;
    TProfile* p1Jet1ChfInAvgProjPtForLeadEta1p3;
    TProfile* p1Jet1NhfInAvgProjPtForLeadEta1p3;
    TProfile* p1Jet1NefInAvgProjPtForLeadEta1p3;
    TProfile* p1Jet1CefInAvgProjPtForLeadEta1p3;
    TProfile* p1Jet1MufInAvgProjPtForLeadEta1p3;


    // Controls
    TH2D* h2MpfResp0InAvgProjPt;
    TH2D* h2MpfResp3InAvgProjPt;
    TH1D* h1EventInCosDeltaPhiLeadRecoil;

    // Method to initialize histograms
    void InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin);
    void fillJetLevelHistos(SkimTree* skimT, int iJet,  const double& weightFi);
    void fillEventLevelHistos(SkimTree* skimT, const double& trigPt);

private:
    // Add a private data member to hold the current event’s inputs:
    HistMultiJetInputs fInputs_;
};

#endif // HISTMULTIJET_H

