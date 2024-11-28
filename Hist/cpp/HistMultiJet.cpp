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
    dirOut->mkdir("Multijet");
    dirOut->cd("Multijet");

    // Initialize histograms and profiles
    h1EventsInPtAvpAll = new TH1D("h1EventsInPtAvpAll", "", nPt, binsPt.data());
    h1EventsInPtAvpSel = new TH1D("h1EventsInPtAvpSel", "", nPt, binsPt.data());
    p1RespInPtAvp = new TProfile("p1RespInPtAvp", "", nPt, binsPt.data());
    p1PtLeadInPtAvp = new TProfile("p1PtLeadInPtAvp", "", nPt, binsPt.data());
    p1CRecoilInPtAvp = new TProfile("p1CRecoilInPtAvp", "", nPt, binsPt.data());

    p1M0InPtAvp = new TProfile("p1M0InPtAvp", "", nPt, binsPt.data());
    p1M3InPtAvp = new TProfile("p1M3InPtAvp", "", nPt, binsPt.data());
    p1MnInPtAvp = new TProfile("p1MnInPtAvp", "", nPt, binsPt.data());
    p1MuInPtAvp = new TProfile("p1MuInPtAvp", "", nPt, binsPt.data());

    h1EventsInPtAveAll = new TH1D("h1EventsInPtAveAll", "", nPt, binsPt.data());
    h1EventsInPtAveSel = new TH1D("h1EventsInPtAveSel", "", nPt, binsPt.data());
    p1RespInPtAve = new TProfile("p1RespInPtAve", "", nPt, binsPt.data());
    p1PtLeadInPtAve = new TProfile("p1PtLeadInPtAve", "", nPt, binsPt.data());
    p1CRecoilInPtAve = new TProfile("p1CRecoilInPtAve", "", nPt, binsPt.data());

    p1M0InPtAve = new TProfile("p1M0InPtAve", "", nPt, binsPt.data());
    p1M3InPtAve = new TProfile("p1M3InPtAve", "", nPt, binsPt.data());
    p1MnInPtAve = new TProfile("p1MnInPtAve", "", nPt, binsPt.data());
    p1MuInPtAve = new TProfile("p1MuInPtAve", "", nPt, binsPt.data());

    h1EventsInPtLeadAll = new TH1D("h1EventsInPtLeadAll", "", nPt, binsPt.data());
    h1EventsInPtLeadSel = new TH1D("h1EventsInPtLeadSel", "", nPt, binsPt.data());
    p1RespInPtLead = new TProfile("p1RespInPtLead", "", nPt, binsPt.data());
    p1PtLeadInPtLead = new TProfile("p1PtLeadInPtLead", "", nPt, binsPt.data());
    p1CRecoilInPtLead = new TProfile("p1CRecoilInPtLead", "", nPt, binsPt.data());

    p1M0InPtLead = new TProfile("p1M0InPtLead", "", nPt, binsPt.data());
    p1M3InPtLead = new TProfile("p1M3InPtLead", "", nPt, binsPt.data());
    p1MnInPtLead = new TProfile("p1MnInPtLead", "", nPt, binsPt.data());
    p1MuInPtLead = new TProfile("p1MuInPtLead", "", nPt, binsPt.data());

    h1EventsInPtRecoilAll = new TH1D("h1EventsInPtRecoilAll", "", nPt, binsPt.data());
    h1EventsInPtRecoilSel = new TH1D("h1EventsInPtRecoilSel", "", nPt, binsPt.data());
    p1RespInPtRecoil = new TProfile("p1RespInPtRecoil", "", nPt, binsPt.data());
    p1PtLeadInPtRecoil = new TProfile("p1PtLeadInPtRecoil", "", nPt, binsPt.data());
    p1CRecoilInPtRecoil = new TProfile("p1CRecoilInPtRecoil", "", nPt, binsPt.data());

    p1M0InPtRecoil = new TProfile("p1M0InPtRecoil", "", nPt, binsPt.data());
    p1M3InPtRecoil = new TProfile("p1M3InPtRecoil", "", nPt, binsPt.data());
    p1MnInPtRecoil = new TProfile("p1MnInPtRecoil", "", nPt, binsPt.data());
    p1MuInPtRecoil = new TProfile("p1MuInPtRecoil", "", nPt, binsPt.data());

    // Control histograms
    h2M0InPtAvp = new TH2D("h2M0InPtAvp", "", nPt, binsPt.data(), 200, -1, 3);
    h2M3InPtAvp = new TH2D("h2M3InPtAvp", "", nPt, binsPt.data(), 200, -1, 3);
    h1CosDeltaPhiInEvents = new TH1D("h1CosDeltaPhiInEvents", "", 102, -1.01, 1.01);

    // 2D recoil histograms
    dirOut->mkdir("Multijet/2Drecoil");
    dirOut->cd("Multijet/2Drecoil");
    h2RecoilInPtAvp = new TH2D("h2RecoilInPtAvp", "", nPt, binsPt.data(), nPt, binsPt.data());
    h2RecoilInPtAve = new TH2D("h2RecoilInPtAve", "", nPt, binsPt.data(), nPt, binsPt.data());
    h2RecoilInPtLead = new TH2D("h2RecoilInPtLead", "", nPt, binsPt.data(), nPt, binsPt.data());
    h2RecoilInPtRecoil = new TH2D("h2RecoilInPtRecoil", "", nPt, binsPt.data(), nPt, binsPt.data());

    // PF composition histograms
    dirOut->mkdir("Multijet/PFcomposition");
    dirOut->cd("Multijet/PFcomposition");

    p1Pt13InPtAvp = new TProfile("p1Pt13InPtAvp", "", nPt, binsPt.data());
    p1Rho13InPtAvp = new TProfile("p1Rho13InPtAvp", "", nPt, binsPt.data());
    p1Chf13InPtAvp = new TProfile("p1Chf13InPtAvp", "", nPt, binsPt.data());
    p1Nhf13InPtAvp = new TProfile("p1Nhf13InPtAvp", "", nPt, binsPt.data());
    p1Nef13InPtAvp = new TProfile("p1Nef13InPtAvp", "", nPt, binsPt.data());
    p1Cef13InPtAvp = new TProfile("p1Cef13InPtAvp", "", nPt, binsPt.data());
    p1Muf13InPtAvp = new TProfile("p1Muf13InPtAvp", "", nPt, binsPt.data());

    p1Pt25InPtAvp = new TProfile("p1Pt25InPtAvp", "", nPt, binsPt.data());
    p1Rho25InPtAvp = new TProfile("p1Rho25InPtAvp", "", nPt, binsPt.data());
    p1Chf25InPtAvp = new TProfile("p1Chf25InPtAvp", "", nPt, binsPt.data());
    p1Nhf25InPtAvp = new TProfile("p1Nhf25InPtAvp", "", nPt, binsPt.data());
    p1Nef25InPtAvp = new TProfile("p1Nef25InPtAvp", "", nPt, binsPt.data());
    p1Cef25InPtAvp = new TProfile("p1Cef25InPtAvp", "", nPt, binsPt.data());
    p1Muf25InPtAvp = new TProfile("p1Muf25InPtAvp", "", nPt, binsPt.data());

    dirOut->cd(".."); // Go back to the parent directory
}


