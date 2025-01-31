// HistMultiJet.cpp
#include "HistMultiJet.h"
#include <TDirectory.h>
#include <iostream>
#include "Helper.h"

// Constructor
HistMultiJet::HistMultiJet(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin) {
    InitializeHistograms(origDir, directoryName, varBin);
}

// Destructor
HistMultiJet::~HistMultiJet() {
    // Clean up histograms if needed
}

// Method to initialize histograms
void HistMultiJet::InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin) {
    // Use the Helper method to get or create the directory
    std::string dirName = directoryName + "/HistMultiJet";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    newDir->cd();

    std::vector<double> binsPt  = varBin.getBinsPt();
    int nPt = binsPt.size() - 1;
    // Initialize histograms and profiles
    h1EventInAvgProjPt = new TH1D("h1EventInAvgProjPt", "", nPt, binsPt.data());
    p1RespInAvgProjPt = new TProfile("p1RespInAvgProjPt", "", nPt, binsPt.data());
    p1LeadPtInAvgProjPt = new TProfile("p1LeadPtInAvgProjPt", "", nPt, binsPt.data());
    p1CrecoilInAvgProjPt = new TProfile("p1CrecoilInAvgProjPt", "", nPt, binsPt.data());

    p1MpfResp0InAvgProjPt = new TProfile("p1MpfResp0InAvgProjPt", "", nPt, binsPt.data());
    p1MpfResp3InAvgProjPt = new TProfile("p1MpfResp3InAvgProjPt", "", nPt, binsPt.data());
    p1MpfRespnInAvgProjPt = new TProfile("p1MpfRespnInAvgProjPt", "", nPt, binsPt.data());
    p1MpfRespuInAvgProjPt = new TProfile("p1MpfRespuInAvgProjPt", "", nPt, binsPt.data());

    h1EventInAvgPt = new TH1D("h1EventInAvgPt", "", nPt, binsPt.data());
    p1RespInAvgPt = new TProfile("p1RespInAvgPt", "", nPt, binsPt.data());
    p1LeadPtInAvgPt = new TProfile("p1LeadPtInAvgPt", "", nPt, binsPt.data());
    p1CrecoilInAvgPt = new TProfile("p1CrecoilInAvgPt", "", nPt, binsPt.data());

    p1MpfResp0InAvgPt = new TProfile("p1MpfResp0InAvgPt", "", nPt, binsPt.data());
    p1MpfResp3InAvgPt = new TProfile("p1MpfResp3InAvgPt", "", nPt, binsPt.data());
    p1MpfRespnInAvgPt = new TProfile("p1MpfRespnInAvgPt", "", nPt, binsPt.data());
    p1MpfRespuInAvgPt = new TProfile("p1MpfRespuInAvgPt", "", nPt, binsPt.data());

    h1EventInLeadPt = new TH1D("h1EventInLeadPt", "", nPt, binsPt.data());
    p1RespInLeadPt = new TProfile("p1RespInLeadPt", "", nPt, binsPt.data());
    p1LeadPtInLeadPt = new TProfile("p1LeadPtInLeadPt", "", nPt, binsPt.data());
    p1CrecoilInLeadPt = new TProfile("p1CrecoilInLeadPt", "", nPt, binsPt.data());

    p1MpfResp0InLeadPt = new TProfile("p1MpfResp0InLeadPt", "", nPt, binsPt.data());
    p1MpfResp3InLeadPt = new TProfile("p1MpfResp3InLeadPt", "", nPt, binsPt.data());
    p1MpfRespnInLeadPt = new TProfile("p1MpfRespnInLeadPt", "", nPt, binsPt.data());
    p1MpfRespuInLeadPt = new TProfile("p1MpfRespuInLeadPt", "", nPt, binsPt.data());

    h1EventInRecoilPt = new TH1D("h1EventInRecoilPt", "", nPt, binsPt.data());
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

    h2JetsPtInAvgProjPt = new TH2D("h2JetsPtInAvgProjPt", "", nPt, binsPt.data(), nPt, binsPt.data());
    h2JetsPtInAvgPt = new TH2D("h2JetsPtInAvgPt", "", nPt, binsPt.data(), nPt, binsPt.data());
    h2JetsPtInLeadPt = new TH2D("h2JetsPtInLeadPt", "", nPt, binsPt.data(), nPt, binsPt.data());
    h2JetsPtInRecoilPt = new TH2D("h2JetsPtInRecoilPt", "", nPt, binsPt.data(), nPt, binsPt.data());

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

    std::cout << "Initialized HistMultiJet histograms in directory: " << dirName << std::endl;
    origDir->cd();
}

void HistMultiJet::setInputs(const HistMultiJetInputs& inputs){
    fInputs_ = inputs;
}

void HistMultiJet::fillEventLevelHistos(SkimTree* skimT, const double& trigPt)
{
    // Here, read from fInputs_ and fill all relevant histos.
    // This is the code that used to be repeated in your big loops.
    const double& ptAvgProj = fInputs_.ptAvgProj;
    const double& ptAverage = fInputs_.ptAverage;
    const double& ptLead    = fInputs_.ptLead;
    const double& ptRecoil  = fInputs_.ptRecoil;
    const double& cRecoil   = fInputs_.cRecoil;
    const double& response  = fInputs_.response;
    const double& weight    = fInputs_.weight;

    h1EventInAvgProjPt->Fill(ptAvgProj, weight);
    h1EventInAvgPt->Fill(ptAverage, weight);
    h1EventInLeadPt->Fill(ptLead, weight);
    h1EventInRecoilPt->Fill(ptRecoil, weight);

    p1RespInAvgProjPt->Fill(ptAvgProj, response, weight);
    p1RespInAvgPt->Fill(ptAverage, response, weight);
    p1RespInLeadPt->Fill(ptLead, response, weight);
    p1RespInRecoilPt->Fill(ptRecoil, response, weight);

    p1LeadPtInAvgProjPt->Fill(ptAvgProj, ptLead, weight);
    p1LeadPtInAvgPt->Fill(ptAverage, ptLead, weight);
    p1LeadPtInLeadPt->Fill(ptLead, ptLead, weight);
    p1LeadPtInRecoilPt->Fill(ptRecoil, ptLead, weight);

    p1CrecoilInAvgProjPt->Fill(ptAvgProj, cRecoil, weight);
    p1CrecoilInAvgPt->Fill(ptAverage, cRecoil, weight);
    p1CrecoilInLeadPt->Fill(ptLead, cRecoil, weight);
    p1CrecoilInRecoilPt->Fill(ptRecoil, cRecoil, weight);

    p1MpfResp0InAvgProjPt->Fill(ptAvgProj, fInputs_.m0b, weight);
    p1MpfResp3InAvgProjPt->Fill(ptAvgProj, fInputs_.m3b, weight);
    p1MpfRespnInAvgProjPt->Fill(ptAvgProj, fInputs_.mnb, weight);
    p1MpfRespuInAvgProjPt->Fill(ptAvgProj, fInputs_.mub, weight);

    p1MpfResp0InAvgPt->Fill(ptAverage, fInputs_.m0m, weight);
    p1MpfResp3InAvgPt->Fill(ptAverage, fInputs_.m3m, weight);
    p1MpfRespnInAvgPt->Fill(ptAverage, fInputs_.mnm, weight);
    p1MpfRespuInAvgPt->Fill(ptAverage, fInputs_.mum, weight);

    p1MpfResp0InLeadPt->Fill(ptLead, fInputs_.m0l, weight);
    p1MpfResp3InLeadPt->Fill(ptLead, fInputs_.m3l, weight);
    p1MpfRespnInLeadPt->Fill(ptLead, fInputs_.mnl, weight);
    p1MpfRespuInLeadPt->Fill(ptLead, fInputs_.mul, weight);

    p1MpfResp0InRecoilPt->Fill(ptRecoil, fInputs_.m0r, weight);
    p1MpfResp3InRecoilPt->Fill(ptRecoil, fInputs_.m3r, weight);
    p1MpfRespnInRecoilPt->Fill(ptRecoil, fInputs_.mnr, weight);
    p1MpfRespuInRecoilPt->Fill(ptRecoil, fInputs_.mur, weight);

    h2MpfResp0InAvgProjPt->Fill(ptAvgProj, fInputs_.m0b, weight);
    h2MpfResp3InAvgProjPt->Fill(ptAvgProj, fInputs_.m3b, weight);
    if (skimT->nJet > 0 && std::abs(fInputs_.etaLead) < 1.3){
        p1RhoInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Rho, weight);
        p1Jet1PtInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_pt[0], weight);
        p1Jet1ChfInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_chHEF[0], weight);
        p1Jet1NhfInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_neHEF[0], weight);
        p1Jet1NefInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_neEmEF[0], weight);
        p1Jet1CefInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_chEmEF[0], weight);
        p1Jet1MufInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_muEF[0], weight);
    }
    if (ptAverage > 1.25 *trigPt){
        h1EventInCosDeltaPhiLeadRecoil->Fill(
        cos(Helper::DELTAPHI(fInputs_.phiLead, fInputs_.phiRecoil)), weight);
    }
}

void HistMultiJet::fillJetLevelHistos(SkimTree* skimT, int iJet, const double& weightFi){
    
    double pt_i = skimT->Jet_pt[iJet];
    const double& ptAverage = fInputs_.ptAverage;
    const double& ptAvgProj = fInputs_.ptAvgProj;
    const double& ptLead    = fInputs_.ptLead;
    const double& ptRecoil  = fInputs_.ptRecoil;

    h2JetsPtInAvgProjPt->Fill(ptAvgProj, pt_i, weightFi);
    h2JetsPtInAvgPt->Fill(ptAverage, pt_i, weightFi);
    h2JetsPtInLeadPt->Fill(ptLead, pt_i, weightFi);
    h2JetsPtInRecoilPt->Fill(ptRecoil, pt_i, weightFi);

    // Fill PF composition histograms
    p1RhoInAvgProjPt->Fill(ptAvgProj, skimT->Rho, weightFi);
    p1JetsPtInAvgProjPt->Fill(ptAvgProj, skimT->Jet_pt[iJet], weightFi);
    p1JetsChfInAvgProjPt->Fill(ptAvgProj, skimT->Jet_chHEF[iJet], weightFi);
    p1JetsNhfInAvgProjPt->Fill(ptAvgProj, skimT->Jet_neHEF[iJet], weightFi);
    p1JetsNefInAvgProjPt->Fill(ptAvgProj, skimT->Jet_neEmEF[iJet], weightFi);
    p1JetsCefInAvgProjPt->Fill(ptAvgProj, skimT->Jet_chEmEF[iJet], weightFi);
    p1JetsMufInAvgProjPt->Fill(ptAvgProj, skimT->Jet_muEF[iJet], weightFi);
}

