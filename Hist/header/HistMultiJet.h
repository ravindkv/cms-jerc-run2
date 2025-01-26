// HistMultiJet.h
#ifndef HISTMULTIJET_H
#define HISTMULTIJET_H

#include <string>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>

class HistMultiJet {
public:
    // Constructor
    HistMultiJet(const std::string& triggerName, double ptMin, double ptMax, double absEtaMin, double absEtaMax);

    // Destructor
    ~HistMultiJet();

    // Basic information about the trigger
    std::string triggerName;
    int triggerPt;
    double ptMin, ptMax, absEtaMin, absEtaMax;

    // Histograms and profiles
    TH1D* h1EventInAvgProjPt;
    TH1D* h1EventInAvgPt;
    TH1D* h1EventInLeadPt;
    TH1D* h1EventInRecoilPt;
    TH1D* h1EventInAvgProjPtForTrigCut;
    TH1D* h1EventInAvgPtForTrigCut;
    TH1D* h1EventInLeadPtForTrigCut;
    TH1D* h1EventInRecoilPtForTrigCut;

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
    void initializeHistograms(const std::vector<double>& binsPt, TDirectory* dirOut);

private:
};

#endif // HISTMULTIJET_H

