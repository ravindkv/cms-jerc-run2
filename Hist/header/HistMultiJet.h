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
    TH1D* h1EventsInPtAvpAll;
    TH1D* h1EventsInPtAveAll;
    TH1D* h1EventsInPtLeadAll;
    TH1D* h1EventsInPtRecoilAll;
    TH1D* h1EventsInPtAvpSel;
    TH1D* h1EventsInPtAveSel;
    TH1D* h1EventsInPtLeadSel;
    TH1D* h1EventsInPtRecoilSel;

    TProfile* p1RespInPtAvp;
    TProfile* p1RespInPtAve;
    TProfile* p1RespInPtLead;
    TProfile* p1RespInPtRecoil;

    TProfile* p1PtLeadInPtAvp;
    TProfile* p1PtLeadInPtAve;
    TProfile* p1PtLeadInPtLead;
    TProfile* p1PtLeadInPtRecoil;

    TProfile* p1CRecoilInPtAvp;
    TProfile* p1CRecoilInPtAve;
    TProfile* p1CRecoilInPtLead;
    TProfile* p1CRecoilInPtRecoil;

    TProfile* p1M0InPtAvp;
    TProfile* p1M3InPtAvp;
    TProfile* p1MnInPtAvp;
    TProfile* p1MuInPtAvp;

    TProfile* p1M0InPtAve;
    TProfile* p1M3InPtAve;
    TProfile* p1MnInPtAve;
    TProfile* p1MuInPtAve;

    TProfile* p1M0InPtLead;
    TProfile* p1M3InPtLead;
    TProfile* p1MnInPtLead;
    TProfile* p1MuInPtLead;

    TProfile* p1M0InPtRecoil;
    TProfile* p1M3InPtRecoil;
    TProfile* p1MnInPtRecoil;
    TProfile* p1MuInPtRecoil;

    // 2D recoils
    TH2D* h2RecoilInPtAvp;
    TH2D* h2RecoilInPtAve;
    TH2D* h2RecoilInPtLead;
    TH2D* h2RecoilInPtRecoil;

    // PF composition plots
    TProfile* p1Pt13InPtAvp;
    TProfile* p1Rho13InPtAvp;
    TProfile* p1Chf13InPtAvp;
    TProfile* p1Nhf13InPtAvp;
    TProfile* p1Nef13InPtAvp;
    TProfile* p1Cef13InPtAvp;
    TProfile* p1Muf13InPtAvp;

    TProfile* p1Pt25InPtAvp;
    TProfile* p1Rho25InPtAvp;
    TProfile* p1Chf25InPtAvp;
    TProfile* p1Nhf25InPtAvp;
    TProfile* p1Nef25InPtAvp;
    TProfile* p1Cef25InPtAvp;
    TProfile* p1Muf25InPtAvp;

    // Controls
    TH2D* h2M0InPtAvp;
    TH2D* h2M3InPtAvp;
    TH1D* h1CosDeltaPhiInEvents;

    // Method to initialize histograms
    void initializeHistograms(const std::vector<double>& binsPt, TDirectory* dirOut);

private:
};

#endif // HISTMULTIJET_H

