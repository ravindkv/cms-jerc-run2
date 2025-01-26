#include "HistMain.h"
#include "Helper.h"

HistMain::HistMain(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin)
{
    InitializeHistograms(origDir, directoryName, varBin);
}

void HistMain::InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin)
{

    // Use the Helper method to get or create the directory
    std::string dirName = directoryName + "/HistMain";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    newDir->cd();

    const double* binsPt  = varBin.getBinsPt().data();
    const double* binsEta = varBin.getBinsEta().data();
    const double* binsPhi = varBin.getBinsPhi().data();

    const int nPt  = varBin.getBinsPt().size()  - 1;
    const int nEta = varBin.getBinsEta().size() - 1;
    const int nPhi = varBin.getBinsPhi().size() - 1;
    
    std::vector<double> rangeIndex  = varBin.getRangeIndex();
    int indexN = rangeIndex.at(0);
    int indexMin = rangeIndex.at(1);
    int indexMax = rangeIndex.at(2);

    // Initialize 1D Histograms
    hist_.h1EventInJet1Index = std::make_unique<TH1D>("h1EventInJet1Index", "", indexN, indexMin, indexMax);
    hist_.p1Jet1EtaInRefPt = std::make_unique<TProfile>("p1Jet1EtaInRefPt", "", nPt, binsPt);
    hist_.h2EventInRefPtJet1Eta = std::make_unique<TH2D>(
        "h2EventInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    
    // 1D Composition and Response
    hist_.p1DbRespInRefPt = std::make_unique<TProfile>("p1DbRespInRefPt", "", nPt, binsPt);
    hist_.p1MpfRespInRefPt = std::make_unique<TProfile>("p1MpfRespInRefPt", "", nPt, binsPt);
    hist_.p1Jet1ChHefInRefPt = std::make_unique<TProfile>("p1Jet1ChHefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1NeHefInRefPt = std::make_unique<TProfile>("p1Jet1NeHefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1NeEmefInRefPt = std::make_unique<TProfile>("p1Jet1NeEmefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1ChEmefInRefPt = std::make_unique<TProfile>("p1Jet1ChEmefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1MuefInRefPt = std::make_unique<TProfile>("p1Jet1MuefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1Chpv0efInRefPt = std::make_unique<TProfile>("p1Jet1Chpv0efInRefPt", "", nPt, binsPt);
    
    // 2D Composition and Response
    hist_.p2DbRespInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2DbRespInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2MpfRespInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2MpfRespInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetChHefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChHefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetNeHefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetNeHefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetNeEmefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetNeEmefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetChEmefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChEmefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetMuefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetMuefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetChPv0efInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChPv0efInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    
    // 2D Composition and Response with Phi
    hist_.p2DbRespInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2DbRespInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2MpfRespInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2MpfRespInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChHefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChHefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetNeHefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetNeHefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetNeEmefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetNeEmefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChEmefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChEmefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetMuefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetMuefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChPv0efInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChPv0efInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    
    // 2D Composition and Response for RefPt230
    hist_.p2DbRespInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2DbRespInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2MpfRespInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2MpfRespInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChHefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetChHefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetNeHefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetNeHefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetNeEmefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetNeEmefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChEmefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetChEmefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetMuefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetMuefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChPv0efInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetChPv0efInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    
    // Control Plots
    hist_.p1RhoInRefPt = std::make_unique<TProfile>("p1RhoInRefPt", "", nPt, binsPt);
    hist_.p1NpvGoodInRefPt = std::make_unique<TProfile>("p1NpvGoodInRefPt", "", nPt, binsPt);
    hist_.p1NpvInRefPt = std::make_unique<TProfile>("p1NpvInRefPt", "", nPt, binsPt);
    
    std::cout << "Initialized HistMain histograms in directory: " << dirName << std::endl;
    origDir->cd();
}

void HistMain::Fill(SkimTree* skimT, int iJet1, double bal, double mpf, double ptRef, double weight)
{
    double eta = skimT->Jet_eta[iJet1];
    double phi = skimT->Jet_phi[iJet1];
    double chHEF  =  skimT->Jet_chHEF[iJet1];
    double neHEF  =  skimT->Jet_neHEF[iJet1];
    double neEmEF =  skimT->Jet_neEmEF[iJet1];
    double chEmEF =  skimT->Jet_chEmEF[iJet1];
    double muEF   =  skimT->Jet_muEF[iJet1];
    //double chFPV0EF =  skimT->Jet_chFPV0EF[iJet1];
    
    // Control plots for Ref-jet 
    // PF composition plots
    hist_.h1EventInJet1Index->Fill(iJet1, weight);
    hist_.p1Jet1EtaInRefPt->Fill(ptRef, eta, weight);
    hist_.h2EventInRefPtJet1Eta->Fill(eta, ptRef, weight);
    
    // 1D composition and response
    hist_.p1DbRespInRefPt->Fill(ptRef, bal, weight);
    hist_.p1MpfRespInRefPt->Fill(ptRef, mpf, weight);
    hist_.p1Jet1ChHefInRefPt->Fill(ptRef, chHEF, weight);
    hist_.p1Jet1NeHefInRefPt->Fill(ptRef, neHEF, weight);
    hist_.p1Jet1NeEmefInRefPt->Fill(ptRef, neEmEF, weight);
    hist_.p1Jet1ChEmefInRefPt->Fill(ptRef, chEmEF, weight);
    hist_.p1Jet1MuefInRefPt->Fill(ptRef, muEF, weight);
    // hist_.p1Jet1Chpv0efInRefPt->Fill(ptRef, chFPV0EF, weight);
    
    hist_.p1RhoInRefPt->Fill(ptRef, skimT->Rho, weight);
    hist_.p1NpvGoodInRefPt->Fill(ptRef, skimT->PV_npvsGood, weight);
    hist_.p1NpvInRefPt->Fill(ptRef, skimT->PV_npvs, weight);
    
    // 2D Composition and Response
    hist_.p2DbRespInRefPtJet1Eta->Fill(eta, ptRef, bal, weight);
    hist_.p2MpfRespInRefPtJet1Eta->Fill(eta, ptRef, mpf, weight);
    hist_.p2JetChHefInRefPtJet1Eta->Fill(eta, ptRef, chHEF, weight);
    hist_.p2JetNeHefInRefPtJet1Eta->Fill(eta, ptRef, neHEF, weight);
    hist_.p2JetNeEmefInRefPtJet1Eta->Fill(eta, ptRef, neEmEF, weight);
    hist_.p2JetChEmefInRefPtJet1Eta->Fill(eta, ptRef, chEmEF, weight);
    hist_.p2JetMuefInRefPtJet1Eta->Fill(eta, ptRef, muEF, weight);
    //hist_.p2JetChPv0efInRefPtJet1Eta->Fill(eta, ptRef, chFPV0EF, weight);
    
    // 2D Composition and Response with Phi
    hist_.p2DbRespInJet1PhiJet1Eta->Fill(eta, phi, bal, weight);
    hist_.p2MpfRespInJet1PhiJet1Eta->Fill(eta, phi, mpf, weight);
    hist_.p2JetChHefInJet1PhiJet1Eta->Fill(eta, phi, chHEF, weight);
    hist_.p2JetNeHefInJet1PhiJet1Eta->Fill(eta, phi, neHEF, weight);
    hist_.p2JetNeEmefInJet1PhiJet1Eta->Fill(eta, phi, neEmEF, weight);
    hist_.p2JetChEmefInJet1PhiJet1Eta->Fill(eta, phi, chEmEF, weight);
    hist_.p2JetMuefInJet1PhiJet1Eta->Fill(eta, phi, muEF, weight);
    //hist_.p2JetChPv0efInJet1PhiJet1Eta->Fill(eta, phi, chFPV0EF, weight);
    
    // 2D Composition and Response for RefPt230
    if (ptRef > 230) {
        hist_.p2DbRespInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, bal, weight);
        hist_.p2MpfRespInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, mpf, weight);
        hist_.p2JetChHefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, chHEF, weight);
        hist_.p2JetNeHefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, neHEF, weight);
        hist_.p2JetNeEmefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, neEmEF, weight);
        hist_.p2JetChEmefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, chEmEF, weight);
        hist_.p2JetMuefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, muEF, weight);
        //hist_.p2JetChPv0efInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, chFPV0EF, weight);
    }
    
}

