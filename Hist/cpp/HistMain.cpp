#include "HistMain.h"

HistMain::HistMain(TFile* fout, const std::string& directoryName,
                     int nPtBins, const double* binsPt,
                     int nEtaBins, const double* binsEta,
                     int nPhiBins, const double* binsPhi)
{
    InitializeHistograms(fout, directoryName, nPtBins, binsPt, nEtaBins, binsEta, nPhiBins, binsPhi);
}

void HistMain::InitializeHistograms(TFile* fout, const std::string& directoryName,
                                     int nPtBins, const double* binsPt,
                                     int nEtaBins, const double* binsEta,
                                     int nPhiBins, const double* binsPhi)
{
    // Create the directory within the ROOT file
    fout->mkdir(directoryName.c_str());
    fout->cd(directoryName.c_str());
    
    // Initialize 1D Histograms
    hist_.p1JetEtaInRefPt = std::make_unique<TProfile>("p1JetEtaInRefPt", ";Reference #it{p}_{T} (GeV);|#eta|", nPtBins, binsPt);
    hist_.h2EventInJetEtaRefPt = std::make_unique<TH2D>("h2EventInJetEtaRefPt", ";Reference #it{p}_{T} (GeV);#eta;Events", nPtBins, binsPt, 100, -5.0, 5.0);
    
    // 1D Composition and Response
    hist_.p1DbRespInRefPt = std::make_unique<TProfile>("p1DbRespInRefPt", ";Reference #it{p}_{T} (GeV);Balance Factor", nPtBins, binsPt);
    hist_.p1MpfRespInRefPt = std::make_unique<TProfile>("p1MpfRespInRefPt", ";Reference #it{p}_{T} (GeV);Momentum Fraction", nPtBins, binsPt);
    hist_.p1JetChhefInRefPt = std::make_unique<TProfile>("p1JetChhefInRefPt", ";Reference #it{p}_{T} (GeV);Jet chHEF", nPtBins, binsPt);
    hist_.p1JetNehefInRefPt = std::make_unique<TProfile>("p1JetNehefInRefPt", ";Reference #it{p}_{T} (GeV);Jet neHEF", nPtBins, binsPt);
    hist_.p1JetNeemefInRefPt = std::make_unique<TProfile>("p1JetNeemefInRefPt", ";Reference #it{p}_{T} (GeV);Jet neEmEF", nPtBins, binsPt);
    hist_.p1JetChemefInRefPt = std::make_unique<TProfile>("p1JetChemefInRefPt", ";Reference #it{p}_{T} (GeV);Jet chEmEF", nPtBins, binsPt);
    hist_.p1JetMuefInRefPt = std::make_unique<TProfile>("p1JetMuefInRefPt", ";Reference #it{p}_{T} (GeV);Jet muEF", nPtBins, binsPt);
    hist_.p1JetChpv0efInRefPt = std::make_unique<TProfile>("p1JetChpv0efInRefPt", ";Reference #it{p}_{T} (GeV);Jet chpv0EF", nPtBins, binsPt);
    
    // 2D Composition and Response
    hist_.p2DbRespInJet1EtaRefPt = std::make_unique<TProfile2D>("p2DbRespInJet1EtaRefPt", ";#eta;Reference #it{p}_{T} (GeV);Balance Factor", nEtaBins, binsEta, nPtBins, binsPt);
    hist_.p2MpfRespInJet1EtaRefPt = std::make_unique<TProfile2D>("p2MpfRespInJet1EtaRefPt", ";#eta;Reference #it{p}_{T} (GeV);Momentum Fraction", nEtaBins, binsEta, nPtBins, binsPt);
    hist_.p2JetChhefInJet1EtaRefPt = std::make_unique<TProfile2D>("p2JetChhefInJet1EtaRefPt", ";#eta;Reference #it{p}_{T} (GeV);Jet chHEF", nEtaBins, binsEta, nPtBins, binsPt);
    hist_.p2JetNehefInJet1EtaRefPt = std::make_unique<TProfile2D>("p2JetNehefInJet1EtaRefPt", ";#eta;Reference #it{p}_{T} (GeV);Jet neHEF", nEtaBins, binsEta, nPtBins, binsPt);
    hist_.p2JetNeemefInJet1EtaRefPt = std::make_unique<TProfile2D>("p2JetNeemefInJet1EtaRefPt", ";#eta;Reference #it{p}_{T} (GeV);Jet neEmEF", nEtaBins, binsEta, nPtBins, binsPt);
    hist_.p2JetChemefInJet1EtaRefPt = std::make_unique<TProfile2D>("p2JetChemefInJet1EtaRefPt", ";#eta;Reference #it{p}_{T} (GeV);Jet chEmEF", nEtaBins, binsEta, nPtBins, binsPt);
    hist_.p2JetMuefInJet1EtaRefPt = std::make_unique<TProfile2D>("p2JetMuefInJet1EtaRefPt", ";#eta;Reference #it{p}_{T} (GeV);Jet muEF", nEtaBins, binsEta, nPtBins, binsPt);
    hist_.p2JetChfpv0efInJet1EtaRefPt = std::make_unique<TProfile2D>("p2JetChfpv0efInJet1EtaRefPt", ";#eta;Reference #it{p}_{T} (GeV);Jet Chfpv0EF", nEtaBins, binsEta, nPtBins, binsPt);
    
    // 2D Composition and Response with Phi
    hist_.p2DbRespInJet1EtaJet1Phi = std::make_unique<TProfile2D>("p2DbRespInJet1EtaJet1Phi", ";#eta;#phi;Balance Factor", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2MpfRespInJet1EtaJet1Phi = std::make_unique<TProfile2D>("p2MpfRespInJet1EtaJet1Phi", ";#eta;#phi;Momentum Fraction", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetChhefInJet1EtaJet1Phi = std::make_unique<TProfile2D>("p2JetChhefInJet1EtaJet1Phi", ";#eta;#phi;Jet chHEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetNehefInJet1EtaJet1Phi = std::make_unique<TProfile2D>("p2JetNehefInJet1EtaJet1Phi", ";#eta;#phi;Jet neHEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetNeemefInJet1EtaJet1Phi = std::make_unique<TProfile2D>("p2JetNeemefInJet1EtaJet1Phi", ";#eta;#phi;Jet neEmEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetChemefInJet1EtaJet1Phi = std::make_unique<TProfile2D>("p2JetChemefInJet1EtaJet1Phi", ";#eta;#phi;Jet chEmEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetMuefInJet1EtaJet1Phi = std::make_unique<TProfile2D>("p2JetMuefInJet1EtaJet1Phi", ";#eta;#phi;Jet muEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetChfpv0efInJet1EtaJet1Phi = std::make_unique<TProfile2D>("p2JetChfpv0efInJet1EtaJet1Phi", ";#eta;#phi;Jet Chfpv0EF", nEtaBins, binsEta, nPhiBins, binsPhi);
    
    // 2D Composition and Response for RefPt230
    hist_.p2DbRespInJet1EtaJet1PhiForRefPt230 = std::make_unique<TProfile2D>("p2DbRespInJet1EtaJet1PhiForRefPt230", ";#eta;#phi;Balance Factor", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2MpfRespInJet1EtaJet1PhiForRefPt230 = std::make_unique<TProfile2D>("p2MpfRespInJet1EtaJet1PhiForRefPt230", ";#eta;#phi;Momentum Fraction", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetChhefInJet1EtaJet1PhiForRefPt230 = std::make_unique<TProfile2D>("p2JetChhefInJet1EtaJet1PhiForRefPt230", ";#eta;#phi;Jet chHEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetNehefInJet1EtaJet1PhiForRefPt230 = std::make_unique<TProfile2D>("p2JetNehefInJet1EtaJet1PhiForRefPt230", ";#eta;#phi;Jet neHEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetNeemefInJet1EtaJet1PhiForRefPt230 = std::make_unique<TProfile2D>("p2JetNeemefInJet1EtaJet1PhiForRefPt230", ";#eta;#phi;Jet neEmEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetChemefInJet1EtaJet1PhiForRefPt230 = std::make_unique<TProfile2D>("p2JetChemefInJet1EtaJet1PhiForRefPt230", ";#eta;#phi;Jet chEmEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetMuefInJet1EtaJet1PhiForRefPt230 = std::make_unique<TProfile2D>("p2JetMuefInJet1EtaJet1PhiForRefPt230", ";#eta;#phi;Jet muEF", nEtaBins, binsEta, nPhiBins, binsPhi);
    hist_.p2JetChfpv0efInJet1EtaJet1PhiForRefPt230 = std::make_unique<TProfile2D>("p2JetChfpv0efInJet1EtaJet1PhiForRefPt230", ";#eta;#phi;Jet Chfpv0EF", nEtaBins, binsEta, nPhiBins, binsPhi);
    
    // Control Plots
    hist_.p1RhoInRefPt = std::make_unique<TProfile>("p1RhoInRefPt", ";Reference #it{p}_{T} (GeV);#rho", nPtBins, binsPt);
    hist_.p1NpvgoodInRefPt = std::make_unique<TProfile>("p1NpvgoodInRefPt", ";Reference #it{p}_{T} (GeV);N_{PV}^{good}", nPtBins, binsPt);
    hist_.p1NpvInRefPt = std::make_unique<TProfile>("p1NpvInRefPt", ";Reference #it{p}_{T} (GeV);N_{PV}", nPtBins, binsPt);
    
    std::cout << "Initialized HistMain histograms in directory: " << directoryName << std::endl;
}

void HistMain::Fill(SkimTree* skimT, int iJet1, double bal, double mpf, double ptRef, double weight)
{
    // Control plots for Ref-jet 
    // PF composition plots
    hist_.h2EventInJetEtaRefPt->Fill(ptRef, skimT->Jet_eta[iJet1], weight);
    hist_.p1JetEtaInRefPt->Fill(ptRef, fabs(skimT->Jet_eta[iJet1]), weight);
    
    // 1D composition and response
    hist_.p1DbRespInRefPt->Fill(ptRef, bal, weight);
    hist_.p1MpfRespInRefPt->Fill(ptRef, mpf, weight);
    hist_.p1JetChhefInRefPt->Fill(ptRef, skimT->Jet_chHEF[iJet1], weight);
    hist_.p1JetNehefInRefPt->Fill(ptRef, skimT->Jet_neHEF[iJet1], weight);
    hist_.p1JetNeemefInRefPt->Fill(ptRef, skimT->Jet_neEmEF[iJet1], weight);
    hist_.p1JetChemefInRefPt->Fill(ptRef, skimT->Jet_chEmEF[iJet1], weight);
    hist_.p1JetMuefInRefPt->Fill(ptRef, skimT->Jet_muEF[iJet1], weight);
    // hist_.p1JetChpv0efInRefPt->Fill(ptRef, skimT->Jet_chFPV0EF[iJet1], weight);
    
    hist_.p1RhoInRefPt->Fill(ptRef, skimT->Rho, weight);
    hist_.p1NpvgoodInRefPt->Fill(ptRef, skimT->PV_npvsGood, weight);
    hist_.p1NpvInRefPt->Fill(ptRef, skimT->PV_npvs, weight);
    
    double eta = skimT->Jet_eta[iJet1];
    double phi = skimT->Jet_phi[iJet1];
    
    // 2D Composition and Response
    hist_.p2DbRespInJet1EtaRefPt->Fill(eta, ptRef, bal, weight);
    hist_.p2MpfRespInJet1EtaRefPt->Fill(eta, ptRef, mpf, weight);
    hist_.p2JetChhefInJet1EtaRefPt->Fill(eta, ptRef, skimT->Jet_chHEF[iJet1], weight);
    hist_.p2JetNehefInJet1EtaRefPt->Fill(eta, ptRef, skimT->Jet_neHEF[iJet1], weight);
    hist_.p2JetNeemefInJet1EtaRefPt->Fill(eta, ptRef, skimT->Jet_neEmEF[iJet1], weight);
    hist_.p2JetChemefInJet1EtaRefPt->Fill(eta, ptRef, skimT->Jet_chEmEF[iJet1], weight);
    hist_.p2JetMuefInJet1EtaRefPt->Fill(eta, ptRef, skimT->Jet_muEF[iJet1], weight);
    //hist_.p2JetChfpv0efInJet1EtaRefPt->Fill(eta, ptRef, skimT->Jet_chFPV0EF[iJet1], weight);
    
    // 2D Composition and Response with Phi
    hist_.p2DbRespInJet1EtaJet1Phi->Fill(eta, phi, bal, weight);
    hist_.p2MpfRespInJet1EtaJet1Phi->Fill(eta, phi, mpf, weight);
    hist_.p2JetChhefInJet1EtaJet1Phi->Fill(eta, phi, skimT->Jet_chHEF[iJet1], weight);
    hist_.p2JetNehefInJet1EtaJet1Phi->Fill(eta, phi, skimT->Jet_neHEF[iJet1], weight);
    hist_.p2JetNeemefInJet1EtaJet1Phi->Fill(eta, phi, skimT->Jet_neEmEF[iJet1], weight);
    hist_.p2JetChemefInJet1EtaJet1Phi->Fill(eta, phi, skimT->Jet_chEmEF[iJet1], weight);
    hist_.p2JetMuefInJet1EtaJet1Phi->Fill(eta, phi, skimT->Jet_muEF[iJet1], weight);
    //hist_.p2JetChfpv0efInJet1EtaJet1Phi->Fill(eta, phi, skimT->Jet_chFPV0EF[iJet1], weight);
    
    // 2D Composition and Response for RefPt230
    if (ptRef > 230) {
        hist_.p2DbRespInJet1EtaJet1PhiForRefPt230->Fill(eta, phi, bal, weight);
        hist_.p2MpfRespInJet1EtaJet1PhiForRefPt230->Fill(eta, phi, mpf, weight);
        hist_.p2JetChhefInJet1EtaJet1PhiForRefPt230->Fill(eta, phi, skimT->Jet_chHEF[iJet1], weight);
        hist_.p2JetNehefInJet1EtaJet1PhiForRefPt230->Fill(eta, phi, skimT->Jet_neHEF[iJet1], weight);
        hist_.p2JetNeemefInJet1EtaJet1PhiForRefPt230->Fill(eta, phi, skimT->Jet_neEmEF[iJet1], weight);
        hist_.p2JetChemefInJet1EtaJet1PhiForRefPt230->Fill(eta, phi, skimT->Jet_chEmEF[iJet1], weight);
        hist_.p2JetMuefInJet1EtaJet1PhiForRefPt230->Fill(eta, phi, skimT->Jet_muEF[iJet1], weight);
        //hist_.p2JetChfpv0efInJet1EtaJet1PhiForRefPt230->Fill(eta, phi, skimT->Jet_chFPV0EF[iJet1], weight);
    }
    
}

