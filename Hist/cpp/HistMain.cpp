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
    
    // Initialize 1D Histograms
    hist_.p1Jet1EtaInRefPt = std::make_unique<TProfile>("p1Jet1EtaInRefPt", "", nPt, binsPt);
    hist_.h2EventInRefPtJet1Eta = std::make_unique<TH2D>(
        "h2EventInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    
    // 1D Composition and Response
    hist_.p1DbRespInRefPt = std::make_unique<TProfile>("p1DbRespInRefPt", "", nPt, binsPt);
    hist_.p1MpfRespInRefPt = std::make_unique<TProfile>("p1MpfRespInRefPt", "", nPt, binsPt);
    hist_.p1Jet1ChhefInRefPt = std::make_unique<TProfile>("p1Jet1ChhefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1NehefInRefPt = std::make_unique<TProfile>("p1Jet1NehefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1NeemefInRefPt = std::make_unique<TProfile>("p1Jet1NeemefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1ChemefInRefPt = std::make_unique<TProfile>("p1Jet1ChemefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1MuefInRefPt = std::make_unique<TProfile>("p1Jet1MuefInRefPt", "", nPt, binsPt);
    hist_.p1Jet1Chpv0efInRefPt = std::make_unique<TProfile>("p1Jet1Chpv0efInRefPt", "", nPt, binsPt);
    
    // 2D Composition and Response
    hist_.p2DbRespInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2DbRespInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2MpfRespInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2MpfRespInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetChhefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChhefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetNehefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetNehefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetNeemefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetNeemefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetChemefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChemefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetMuefInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetMuefInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    hist_.p2JetChfpv0efInRefPtJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChfpv0efInRefPtJet1Eta", "", nEta, binsEta, nPt, binsPt);
    
    // 2D Composition and Response with Phi
    hist_.p2DbRespInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2DbRespInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2MpfRespInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2MpfRespInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChhefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChhefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetNehefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetNehefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetNeemefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetNeemefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChemefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChemefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetMuefInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetMuefInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChfpv0efInJet1PhiJet1Eta = std::make_unique<TProfile2D>(
        "p2JetChfpv0efInJet1PhiJet1Eta", "", nEta, binsEta, nPhi, binsPhi);
    
    // 2D Composition and Response for RefPt230
    hist_.p2DbRespInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2DbRespInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2MpfRespInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2MpfRespInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChhefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetChhefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetNehefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetNehefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetNeemefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetNeemefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChemefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetChemefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetMuefInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetMuefInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    hist_.p2JetChfpv0efInJet1PhiJet1EtaForRefPt230 = std::make_unique<TProfile2D>(
        "p2JetChfpv0efInJet1PhiJet1EtaForRefPt230", "", nEta, binsEta, nPhi, binsPhi);
    
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
    hist_.p1Jet1EtaInRefPt->Fill(ptRef, eta, weight);
    hist_.h2EventInRefPtJet1Eta->Fill(eta, ptRef, weight);
    
    // 1D composition and response
    hist_.p1DbRespInRefPt->Fill(ptRef, bal, weight);
    hist_.p1MpfRespInRefPt->Fill(ptRef, mpf, weight);
    hist_.p1Jet1ChhefInRefPt->Fill(ptRef, chHEF, weight);
    hist_.p1Jet1NehefInRefPt->Fill(ptRef, neHEF, weight);
    hist_.p1Jet1NeemefInRefPt->Fill(ptRef, neEmEF, weight);
    hist_.p1Jet1ChemefInRefPt->Fill(ptRef, chEmEF, weight);
    hist_.p1Jet1MuefInRefPt->Fill(ptRef, muEF, weight);
    // hist_.p1Jet1Chpv0efInRefPt->Fill(ptRef, chFPV0EF, weight);
    
    hist_.p1RhoInRefPt->Fill(ptRef, skimT->Rho, weight);
    hist_.p1NpvGoodInRefPt->Fill(ptRef, skimT->PV_npvsGood, weight);
    hist_.p1NpvInRefPt->Fill(ptRef, skimT->PV_npvs, weight);
    
    // 2D Composition and Response
    hist_.p2DbRespInRefPtJet1Eta->Fill(eta, ptRef, bal, weight);
    hist_.p2MpfRespInRefPtJet1Eta->Fill(eta, ptRef, mpf, weight);
    hist_.p2JetChhefInRefPtJet1Eta->Fill(eta, ptRef, chHEF, weight);
    hist_.p2JetNehefInRefPtJet1Eta->Fill(eta, ptRef, neHEF, weight);
    hist_.p2JetNeemefInRefPtJet1Eta->Fill(eta, ptRef, neEmEF, weight);
    hist_.p2JetChemefInRefPtJet1Eta->Fill(eta, ptRef, chEmEF, weight);
    hist_.p2JetMuefInRefPtJet1Eta->Fill(eta, ptRef, muEF, weight);
    //hist_.p2JetChfpv0efInRefPtJet1Eta->Fill(eta, ptRef, chFPV0EF, weight);
    
    // 2D Composition and Response with Phi
    hist_.p2DbRespInJet1PhiJet1Eta->Fill(eta, phi, bal, weight);
    hist_.p2MpfRespInJet1PhiJet1Eta->Fill(eta, phi, mpf, weight);
    hist_.p2JetChhefInJet1PhiJet1Eta->Fill(eta, phi, chHEF, weight);
    hist_.p2JetNehefInJet1PhiJet1Eta->Fill(eta, phi, neHEF, weight);
    hist_.p2JetNeemefInJet1PhiJet1Eta->Fill(eta, phi, neEmEF, weight);
    hist_.p2JetChemefInJet1PhiJet1Eta->Fill(eta, phi, chEmEF, weight);
    hist_.p2JetMuefInJet1PhiJet1Eta->Fill(eta, phi, muEF, weight);
    //hist_.p2JetChfpv0efInJet1PhiJet1Eta->Fill(eta, phi, chFPV0EF, weight);
    
    // 2D Composition and Response for RefPt230
    if (ptRef > 230) {
        hist_.p2DbRespInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, bal, weight);
        hist_.p2MpfRespInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, mpf, weight);
        hist_.p2JetChhefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, chHEF, weight);
        hist_.p2JetNehefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, neHEF, weight);
        hist_.p2JetNeemefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, neEmEF, weight);
        hist_.p2JetChemefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, chEmEF, weight);
        hist_.p2JetMuefInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, muEF, weight);
        //hist_.p2JetChfpv0efInJet1PhiJet1EtaForRefPt230->Fill(eta, phi, chFPV0EF, weight);
    }
    
}

