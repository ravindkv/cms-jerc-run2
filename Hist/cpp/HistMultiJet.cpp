// HistMultiJet.cpp
#include "HistMultiJet.h"
#include <TDirectory.h>
#include <iostream>

// Constructor
HistMultiJet::HistMultiJet(const std::string& triggerName_, double ptMin_, double ptMax_, double absEtaMin_, double absEtaMax_)
    : triggerName(triggerName_), ptMin(ptMin_), ptMax(ptMax_), absEtaMin(absEtaMin_), absEtaMax(absEtaMax_) {
    // You can initialize other members here if needed
}

// Destructor
HistMultiJet::~HistMultiJet() {
    // Clean up histograms if needed
}

// Method to initialize histograms
void HistMultiJet::initializeHistograms(const std::vector<double>& binsPt, TDirectory* dirOut) {
    int nPt = binsPt.size() - 1;
    dirOut->mkdir("HistMultijet");
    dirOut->cd("HistMultijet");

    // Initialize histograms and profiles
    h1EventInAvgProjPt = new TH1D("h1EventInAvgProjPt", "", nPt, binsPt.data());
    h1EventInAvgProjPtForTrigCut = new TH1D("h1EventInAvgProjPtForTrigCut", "", nPt, binsPt.data());
    p1RespInAvgProjPt = new TProfile("p1RespInAvgProjPt", "", nPt, binsPt.data());
    p1LeadPtInAvgProjPt = new TProfile("p1LeadPtInAvgProjPt", "", nPt, binsPt.data());
    p1CrecoilInAvgProjPt = new TProfile("p1CrecoilInAvgProjPt", "", nPt, binsPt.data());

    p1MpfResp0InAvgProjPt = new TProfile("p1MpfResp0InAvgProjPt", "", nPt, binsPt.data());
    p1MpfResp3InAvgProjPt = new TProfile("p1MpfResp3InAvgProjPt", "", nPt, binsPt.data());
    p1MpfRespnInAvgProjPt = new TProfile("p1MpfRespnInAvgProjPt", "", nPt, binsPt.data());
    p1MpfRespuInAvgProjPt = new TProfile("p1MpfRespuInAvgProjPt", "", nPt, binsPt.data());

    h1EventInAvgPt = new TH1D("h1EventInAvgPt", "", nPt, binsPt.data());
    h1EventInAvgPtForTrigCut = new TH1D("h1EventInAvgPtForTrigCut", "", nPt, binsPt.data());
    p1RespInAvgPt = new TProfile("p1RespInAvgPt", "", nPt, binsPt.data());
    p1LeadPtInAvgPt = new TProfile("p1LeadPtInAvgPt", "", nPt, binsPt.data());
    p1CrecoilInAvgPt = new TProfile("p1CrecoilInAvgPt", "", nPt, binsPt.data());

    p1MpfResp0InAvgPt = new TProfile("p1MpfResp0InAvgPt", "", nPt, binsPt.data());
    p1MpfResp3InAvgPt = new TProfile("p1MpfResp3InAvgPt", "", nPt, binsPt.data());
    p1MpfRespnInAvgPt = new TProfile("p1MpfRespnInAvgPt", "", nPt, binsPt.data());
    p1MpfRespuInAvgPt = new TProfile("p1MpfRespuInAvgPt", "", nPt, binsPt.data());

    h1EventInLeadPt = new TH1D("h1EventInLeadPt", "", nPt, binsPt.data());
    h1EventInLeadPtForTrigCut = new TH1D("h1EventInLeadPtForTrigCut", "", nPt, binsPt.data());
    p1RespInLeadPt = new TProfile("p1RespInLeadPt", "", nPt, binsPt.data());
    p1LeadPtInLeadPt = new TProfile("p1LeadPtInLeadPt", "", nPt, binsPt.data());
    p1CrecoilInLeadPt = new TProfile("p1CrecoilInLeadPt", "", nPt, binsPt.data());

    p1MpfResp0InLeadPt = new TProfile("p1MpfResp0InLeadPt", "", nPt, binsPt.data());
    p1MpfResp3InLeadPt = new TProfile("p1MpfResp3InLeadPt", "", nPt, binsPt.data());
    p1MpfRespnInLeadPt = new TProfile("p1MpfRespnInLeadPt", "", nPt, binsPt.data());
    p1MpfRespuInLeadPt = new TProfile("p1MpfRespuInLeadPt", "", nPt, binsPt.data());

    h1EventInRecoilPt = new TH1D("h1EventInRecoilPt", "", nPt, binsPt.data());
    h1EventInRecoilPtForTrigCut = new TH1D("h1EventInRecoilPtForTrigCut", "", nPt, binsPt.data());
    p1RespInRecoilPt = new TProfile("p1RespInRecoilPt", "", nPt, binsPt.data());
    p1LeadPtInRecoilPt = new TProfile("p1LeadPtInRecoilPt", "", nPt, binsPt.data());
    p1CrecoilInRecoilPt = new TProfile("p1CrecoilInRecoilPt", "", nPt, binsPt.data());

    p1MpfResp0InRecoilPt = new TProfile("p1MpfResp0InRecoilPt", "", nPt, binsPt.data());
    p1MpfResp3InRecoilPt = new TProfile("p1MpfResp3InRecoilPt", "", nPt, binsPt.data());
    p1MpfRespnInRecoilPt = new TProfile("p1MpfRespnInRecoilPt", "", nPt, binsPt.data());
    p1MpfRespuInRecoilPt = new TProfile("p1MpfRespuInRecoilPt", "", nPt, binsPt.data());

    // Control histograms
    h2MpfResp0InAvgProjPt = new TH2D("h2MpfResp0InAvgProjPt", "", nPt, binsPt.data(), 200, -1, 3);
    h2MpfResp3InAvgProjPt = new TH2D("h2MpfResp3InAvgProjPt", "", nPt, binsPt.data(), 200, -1, 3);
    h1EventInCosDeltaPhiLeadRecoil = new TH1D("h1EventInCosDeltaPhiLeadRecoil", "", 102, -1.01, 1.01);

    // 2D recoil histograms
    dirOut->mkdir("HistMultijet/2Drecoil");
    dirOut->cd("HistMultijet/2Drecoil");
    h2JetsPtInAvgProjPt = new TH2D("h2JetsPtInAvgProjPt", "", nPt, binsPt.data(), nPt, binsPt.data());
    h2JetsPtInAvgPt = new TH2D("h2JetsPtInAvgPt", "", nPt, binsPt.data(), nPt, binsPt.data());
    h2JetsPtInLeadPt = new TH2D("h2JetsPtInLeadPt", "", nPt, binsPt.data(), nPt, binsPt.data());
    h2JetsPtInRecoilPt = new TH2D("h2JetsPtInRecoilPt", "", nPt, binsPt.data(), nPt, binsPt.data());

    // PF composition histograms
    dirOut->mkdir("HistMultijet/PFcomposition");
    dirOut->cd("HistMultijet/PFcomposition");

    p1RhoInAvgProjPt = new TProfile("p1RhoInAvgProjPt", "", nPt, binsPt.data());
    p1JetsPtInAvgProjPt = new TProfile("p1JetsPtInAvgProjPt", "", nPt, binsPt.data());
    p1JetsChfInAvgProjPt = new TProfile("p1JetsChfInAvgProjPt", "", nPt, binsPt.data());
    p1JetsNhfInAvgProjPt = new TProfile("p1JetsNhfInAvgProjPt", "", nPt, binsPt.data());
    p1JetsNefInAvgProjPt = new TProfile("p1JetsNefInAvgProjPt", "", nPt, binsPt.data());
    p1JetsCefInAvgProjPt = new TProfile("p1JetsCefInAvgProjPt", "", nPt, binsPt.data());
    p1JetsMufInAvgProjPt = new TProfile("p1JetsMufInAvgProjPt", "", nPt, binsPt.data());


    p1RhoInAvgProjPtForLeadEta1p3 = new TProfile("p1RhoInAvgProjPtForLeadEta1p3", "", nPt, binsPt.data());
    p1Jet1PtInAvgProjPtForLeadEta1p3 = new TProfile("p1Jet1PtInAvgProjPtForLeadEta1p3", "", nPt, binsPt.data());
    p1Jet1ChfInAvgProjPtForLeadEta1p3 = new TProfile("p1Jet1ChfInAvgProjPtForLeadEta1p3", "", nPt, binsPt.data());
    p1Jet1NhfInAvgProjPtForLeadEta1p3 = new TProfile("p1Jet1NhfInAvgProjPtForLeadEta1p3", "", nPt, binsPt.data());
    p1Jet1NefInAvgProjPtForLeadEta1p3 = new TProfile("p1Jet1NefInAvgProjPtForLeadEta1p3", "", nPt, binsPt.data());
    p1Jet1CefInAvgProjPtForLeadEta1p3 = new TProfile("p1Jet1CefInAvgProjPtForLeadEta1p3", "", nPt, binsPt.data());
    p1Jet1MufInAvgProjPtForLeadEta1p3 = new TProfile("p1Jet1MufInAvgProjPtForLeadEta1p3", "", nPt, binsPt.data());

    dirOut->cd(".."); // Go back to the parent directory
}


