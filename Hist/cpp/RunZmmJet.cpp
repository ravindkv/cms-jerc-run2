#include "RunZmmJet.h"
#include "HistTag.h"
#include "HistTime.h"
#include "HistRef.h"
#include "HistMain.h"
#include "Helper.h"
   
// Constructor implementation
RunZmmJet::RunZmmJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}

auto RunZmmJet::Run(std::shared_ptr<SkimTree>& skimT, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout) -> int{
   
    TDirectory *curdir = gDirectory;
    assert(fout && !fout->IsZombie());
    
    // pT binning
    std::vector<double> binsPt = {15, 20, 25, 30, 35, 40, 50, 60, 75, 90, 110, 130, 175, 230,
    								300, 400, 500, 600, 700, 850, 1000, 1200, 1500};
    const int nPt = binsPt.size() - 1;
    
    // Eta binning
    std::vector<double> binsEta = {-5.191, -3.839, -3.489, -3.139, -2.964, -2.853, -2.650,
    								 -2.500, -2.322, -2.172, -1.930, -1.653, -1.479, -1.305,
    								 -1.044, -0.783, -0.522, -0.261, 0.000, 0.261, 0.522, 0.783,
    								 1.044, 1.305, 1.479, 1.653, 1.930, 2.172, 2.322, 2.500,
    								 2.650, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
    const int nEta = binsEta.size() - 1;
    
    //------------------------------------
    // Cutflow histograms
    //------------------------------------
    std::vector<std::string> cuts = {
    		"passSkim", "passHLT", "passGoodLumi", "passMetFilter", "passAtleast1Ref",
    		"passAtleast1Jet", "passJetVetoMap", "passDPhiRefJet1", "passRefBarrel",
    		"passJet1EtaJet2Pt", "passResponse"
    };
    
    // Create the histogram with the number of bins equal to the number of cuts
    auto h1EventInCutflow = std::make_unique<TH1D>(
         "h1EventInCutflow", "", cuts.size(), 0.5, cuts.size() + 0.5);
    
    // Map each cut name to a bin number (1-based bin number for ROOT)
    std::map<std::string, int> cutToBinMap;
    for (size_t i = 0; i < cuts.size(); ++i) {
    		cutToBinMap[cuts[i]] = i + 1;  // Bin numbers are 1-based in ROOT
    		h1EventInCutflow->GetXaxis()->SetBinLabel(i + 1, cuts[i].c_str());  // Set bin labels
    }
    
    //------------------------------------
    // Variables after at least 1 Ref
    //------------------------------------
    // Initialize HistRef instance
    HistRef coreRefHists(fout, "passAtleast1Ref", nPt, binsPt.data());
    curdir->cd();
    
    
    //------------------------------------
    // Variables after leading Ref in barrel (eta < 1.33)
    //------------------------------------
    fout->mkdir("passRefBarrel");
    fout->cd("passRefBarrel");
    
    // Follow up on problematic cuts
    auto h1EventInDphiJetRef = std::make_unique<TH1D>(
         "h1EventInDphiJetRef", "", 126, -TMath::TwoPi(), TMath::TwoPi());
    auto h1EventInDrJetRef = std::make_unique<TH1D>("h1EventInDrJetRef", "", 100, 0, 10);
    auto h2EventInRefPtDbResp = std::make_unique<TH2D>(
         "h2EventInRefPtDbResp", "", nPt, binsPt.data(), 200, 0, 4);
    auto h2EventInRefPtMpfResp = std::make_unique<TH2D>(
         "h2EventInRefPtMpfResp", "", nPt, binsPt.data(), 300, -2, 4);
    
    // Plots for jet properties
    auto p1GenJ1PtOverProbePtInRefPt = std::make_unique<TProfile>(
         "p1GenJ1PtOverProbePtInRefPt", "", nPt, binsPt.data());
    auto p1J1PtOverProbePtInRefPt = std::make_unique<TProfile>(
         "p1J1PtOverProbePtInRefPt", "", nPt, binsPt.data());
    auto p1J1PtOverGenJ1PtInGenJ1Pt = std::make_unique<TProfile>(
         "p1J1PtOverGenJ1PtInGenJ1Pt", "", nPt, binsPt.data());
    
    //------------------------------------
    // Variables in 'flavourXTagY' directory
    //------------------------------------
    // Initialize HistTag instance
    HistTag coreTagHists(fout, "passRefBarrel/flavourXTagY", nPt, binsPt, globalFlags_);
    
    //------------------------------------
    // Variables after Jet1Eta, Jet2Pt cut
    //------------------------------------
    const int nPhi = 72;
    const double binsPhi[nPhi + 1] = {
    		-3.142, -3.054, -2.967, -2.880, -2.793, -2.705, -2.618, -2.531, -2.443,
    		-2.356, -2.269, -2.182, -2.094, -2.007, -1.920, -1.833, -1.745, -1.658,
    		-1.571, -1.484, -1.396, -1.309, -1.222, -1.134, -1.047, -0.960, -0.873,
    		-0.785, -0.698, -0.611, -0.524, -0.436, -0.349, -0.262, -0.175, -0.087,
    		0.000, 0.087, 0.175, 0.262, 0.349, 0.436, 0.524, 0.611, 0.698, 0.785,
    		0.873, 0.960, 1.047, 1.134, 1.222, 1.309, 1.396, 1.484, 1.571, 1.658,
    		1.745, 1.833, 1.920, 2.007, 2.094, 2.182, 2.269, 2.356, 2.443, 2.531,
    		2.618, 2.705, 2.793, 2.880, 2.967, 3.054, 3.142
    };
    
    // Initialize HistMain instance
    HistMain coreMainHists(fout, "passJet1EtaJet2Pt", nPt, binsPt.data(), nEta, binsEta.data(), nPhi, binsPhi);
    curdir->cd();
    fout->cd("passJet1EtaJet2Pt");
    
    auto p1GenJ2PtOverProbePtInRefPt = std::make_unique<TProfile>(
         "p1GenJ2PtOverProbePtInRefPt", "", nPt, binsPt.data());
    auto p1J2PtOverProbePtInRefPt = std::make_unique<TProfile>(
         "p1J2PtOverProbePtInRefPt", "", nPt, binsPt.data());
    auto p1J2PtOverGenJ2PtInGenJ2Pt = std::make_unique<TProfile>(
         "p1J2PtOverGenJ2PtInGenJ2Pt", "", nPt, binsPt.data());
    
    auto h2EventInRefPtDbRespPassMpf = std::make_unique<TH2D>(
         "h2EventInRefPtDbRespPassMpf", "", nPt, binsPt.data(), 200, 0, 4);
    auto h2EventInRefPtMpfRespPassDb = std::make_unique<TH2D>(
         "h2EventInRefPtMpfRespPassDb", "", nPt, binsPt.data(), 300, -2, 4);
    auto h2EventInRefPtDbRespPassBoth = std::make_unique<TH2D>(
         "h2EventInRefPtDbRespPassBoth", "", nPt, binsPt.data(), 200, 0, 4);
    auto h2EventInRefPtMpfRespPassBoth = std::make_unique<TH2D>(
         "h2EventInRefPtMpfRespPassBoth", "", nPt, binsPt.data(), 300, -2, 4);
    
    //------------------------------------
    // Response in data-taking Runs
    //------------------------------------
    Int_t runN = 20000;
    Double_t runMin = 370000.5;
    Double_t runMax = 390000.5;
    std::vector<int> pTRefs = {30, 110, 230};
    // Initialize HistTime instance
    HistTime coreRun(fout, "passResponse", pTRefs, runN, runMin, runMax);
    curdir->cd();


    //------------------------------------
    // Event loop
    //------------------------------------
    TLorentzVector p4Ref, p4GenRef, p4Jet1, p4Jet2, p4Jetn;
    TLorentzVector p4Met, p4Met1, p4Metn, p4Metu, p4Metnu, p4RawMet, p4CorrMet, p4RawRef;
    TLorentzVector p4Jeti, p4CorrJets, p4RawJet, p4RawJets, p4RcJet, p4RcJets, p4RcOffsets;
    TLorentzVector p4GenJeti, p4GenJet1, p4GenJet2;
    TLorentzVector p4Refx; // for MPFX

    Long64_t nentries = skimT->getEntries(); 
    std::cout << "\nStarting loop over " << nentries << " entries" << '\n';

    std::cout << "---------------------------" << '\n';
    std::cout << std::setw(10) << "Progress" << std::setw(10) << "Time" << '\n';
    std::cout << "---------------------------" << '\n';
    double totTime = 0.0;
    auto startClock = std::chrono::high_resolution_clock::now();

    for (Long64_t jentry = 0; jentry < nentries; ++jentry) {
        if (globalFlags_.isDebug() && jentry > globalFlags_.getNDebug()) break;
        Helper::printProgress(jentry, nentries, startClock, totTime);
       
        Long64_t ientry = skimT->loadEntry(jentry);
        if (ientry < 0) break; 
        skimT->getChain()->GetTree()->GetEntry(ientry);
        h1EventInCutflow->Fill(cutToBinMap["passSkim"]);

        //------------------------------------
        // Trigger and golden lumi, MET filter selection 
        //------------------------------------
        if (!eventP->passHLT(skimT)) continue; 
        h1EventInCutflow->Fill(cutToBinMap["passHLT"]);

        bool passGoodLumi = true; 
        if (globalFlags_.isData()){
            passGoodLumi = objS->checkGoodLumi(skimT->run, skimT->luminosityBlock);
        }
        if (!passGoodLumi) continue; 
        h1EventInCutflow->Fill(cutToBinMap["passGoodLumi"]);

        if (!eventP->passFilter(skimT)) continue; 
        h1EventInCutflow->Fill(cutToBinMap["passMetFilter"]);

        //------------------------------------------
        // Select objects
        //------------------------------------------
        // Reco objects
        objP->clearObjects();
        objP->pickMuons();
        if (objP->getPickedMuons().size() < 2) continue;
        if (objP->getPickedMuons().size() > 3) continue;
        objP->pickRefs();
        std::vector<TLorentzVector> p4Refs = objP->getPickedRefs();

        if (p4Refs.empty()) continue; 
        h1EventInCutflow->Fill(cutToBinMap["passAtleast1Ref"]);

        // Weight
        double weight = (globalFlags_.isMC() ? skimT->genWeight : 1.0);
        if (globalFlags_.isMC()) weight *= objS->getPuCorrection(skimT->Pileup_nTrueInt, "nominal");

        p4Ref = p4Refs.at(0);
        p4RawRef = p4Refs.at(0);
        double ptRef = p4Ref.Pt();

        // Gen objects
        p4GenRef.SetPtEtaPhiM(0, 0, 0, 0);
        if (globalFlags_.isMC()) {
            objP->pickGenMuons();
            objP->pickGenRefs();
            std::vector<TLorentzVector> p4GenRefs = objP->getPickedGenRefs();
            if (p4GenRefs.empty()) continue;
            p4GenRef = p4GenRefs.at(0);
        }
        // Fill HistRef histograms
        coreRefHists.Fill(p4Refs.size(), p4Ref, p4GenRef, weight); 
        

        //------------------------------------------------
        // Jet loop: Apply JEC
        //------------------------------------------------
        // Select leading jets. Just exclude Ref, don't apply JetID yet
        int iJet1(-1), iJet2(-1), nJets(0);
        p4Jet1.SetPtEtaPhiM(0, 0, 0, 0);
        p4Jet2.SetPtEtaPhiM(0, 0, 0, 0);
        p4Jetn.SetPtEtaPhiM(0, 0, 0, 0);
        // Also calculate corrected type-I chsMET and HDM inputs
        p4RawJets.SetPtEtaPhiM(0, 0, 0, 0);
        p4RcJets.SetPtEtaPhiM(0, 0, 0, 0);
        p4RcOffsets.SetPtEtaPhiM(0, 0, 0, 0);
        p4CorrJets.SetPtEtaPhiM(0, 0, 0, 0);

        for (int i = 0; i != skimT->nJet; ++i) {
            if (!(skimT->Jet_jetId[i] & (1 << 2))) continue; // tightLepVeto

            // Data+MC: undo the correction already applied in NanoAOD
            skimT->Jet_pt[i] *= (1.0 - skimT->Jet_rawFactor[i]);
            skimT->Jet_mass[i] *= (1.0 - skimT->Jet_rawFactor[i]);
            p4RawJet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i], skimT->Jet_phi[i], skimT->Jet_mass[i]);
            p4RawJets += p4RawJet;

            // Data+MC: L1RC correction is applied to the uncorrected jet
            double corrL1FastJet = objS->getL1FastJetCorrection(skimT->Jet_area[i], skimT->Jet_eta[i], 
                                                                skimT->Jet_pt[i], skimT->Rho);
            skimT->Jet_pt[i] *= corrL1FastJet;
            skimT->Jet_mass[i] *= corrL1FastJet;
            p4RcJet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i], skimT->Jet_phi[i], skimT->Jet_mass[i]);
            p4RcJets += p4RcJet;
            p4RcOffsets += (p4RawJet - p4RcJet);

            // Data+MC: MCTruth correction is applied to the L1RC corrected jet
            double corrL2Relative = objS->getL2RelativeCorrection(skimT->Jet_eta[i], skimT->Jet_pt[i]);
            skimT->Jet_pt[i] *= corrL2Relative;
            skimT->Jet_mass[i] *= corrL2Relative;

            // Only Data: L2Residual+L3Residual correction is applied to the MCTruth corrected jet
            if (globalFlags_.isData()) {
                double corrL2L3Residual = objS->getL2L3ResidualCorrection(skimT->Jet_eta[i], skimT->Jet_pt[i]);
                skimT->Jet_pt[i] *= corrL2L3Residual;
                skimT->Jet_mass[i] *= corrL2L3Residual;
            }

            p4Jeti.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i], skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (smearJets_) {
                // Apply JER correction if necessary
                // double corrJER = objS->getJERCorrection(skimT, i, "nom");
                // skimT->Jet_pt[i] *= corrJER;
            }
           
            if (skimT->Jet_pt[i] < 12) continue;
            if (p4Ref.DeltaR(p4Jeti) < 0.2) continue; // should not happen, but does?
            ++nJets;
            if (iJet1 == -1) { // Leading jet for balance
                iJet1 = i;
                p4Jet1 = p4Jeti;
            } else { // Subleading jets 
                p4Jetn += p4Jeti;
                if (iJet2 == -1) { // First subleading jet for alpha
                    iJet2 = i;
                    p4Jet2 = p4Jeti;
                }
            }
            p4CorrJets += p4Jeti;
        }//nJet

        if (nJets < 1) continue; 
        h1EventInCutflow->Fill(cutToBinMap["passAtleast1Jet"]);

        if (objS->checkJetVetoMap()) continue; // expensive function
        h1EventInCutflow->Fill(cutToBinMap["passJetVetoMap"]);
        
        //------------------------------------------------
        // Set MET vectors
        //------------------------------------------------
        p4RawMet.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);
        p4RawMet += p4RawRef - p4Ref; // replace PF Ref with Reco Ref
        p4Met1 = -p4Jet1 - p4Ref;
        p4Metn = -p4Jetn;
        p4CorrMet = p4RawMet + p4RcJets - p4CorrJets; // Corrected MET
        // Unclustered MET from corrMET
        p4Metu = p4CorrMet + p4Ref + p4CorrJets;
        p4Metnu = p4Metn + 1.1 * p4Metu;
        p4Met = p4CorrMet;
        
        // Make MET transverse
        p4CorrMet.SetPz(0);
        p4Met.SetPz(0);
        p4Metn.SetPz(0);
        p4Met1.SetPz(0);
        p4Metu.SetPz(0);
        
        //------------------------------------------------
        // Calculate basic variables
        //------------------------------------------------
        double bal = p4Jet1.Pt() / ptRef;
        double mpf = 1 + p4Met.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);
        double mpf1 = 1 + p4Met1.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);
        double mpfn = p4Metn.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);
        double mpfu = p4Metu.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);
        double mpfnu = p4Metnu.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);

        // For MPFX
        p4Refx.SetPtEtaPhiM(p4Ref.Pt(), p4Ref.Eta(), p4Ref.Phi() + 0.5 * TMath::Pi(), 0.0);
        double mpfx = 1 + p4Met.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
        double mpf1x = 1 + p4Met1.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
        double mpfnPt = p4Metn.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
        double mpfux = p4Metu.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
        double mpfnux = p4Metnu.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
         
        // Sanity checks for HDM inputs
        if (!(fabs(mpf1 + mpfn + mpfu - mpf) < 1e-4)) {
            std::cout << "\nHDM input error: mpf=" << mpf << " mpf1=" << mpf1
                      << " mpfn=" << mpfn << " mpfu=" << mpfu << '\n';
            std::cout << "Difference = " << mpf1 + mpfn + mpfu - mpf << '\n' << std::flush;
            std::cout << "Skip entry " << jentry
                      << " (" << skimT->run << "," << skimT->luminosityBlock << "," << skimT->event << ")"
                      << " in file " << fout->GetName() << '\n' << std::flush;
            continue;
        }
       
        if (fabs(p4Ref.DeltaPhi(p4Jet1) - TMath::Pi()) >= 0.44) continue; 
        h1EventInCutflow->Fill(cutToBinMap["passDPhiRefJet1"]);

        if (fabs(p4Ref.Eta()) > 1.3) continue; 
        h1EventInCutflow->Fill(cutToBinMap["passRefBarrel"]);

        h1EventInDphiJetRef->Fill(p4Ref.DeltaPhi(p4Jet1), weight);
        h1EventInDrJetRef->Fill(p4Ref.DeltaR(p4Jet1), weight);

        //------------------------------------------------
        // GenJet loop
        //------------------------------------------------
        // Select p4GenJet1 matching leading and subleading reco jet
        int iGenJet(-1), iGenJet2(-1);
        p4GenJet1.SetPtEtaPhiM(0, 0, 0, 0);
        p4GenJet2.SetPtEtaPhiM(0, 0, 0, 0);
        if (globalFlags_.isMC()) {
            for (Int_t i = 0; i != skimT->nGenJet; ++i) {
                p4GenJeti.SetPtEtaPhiM(skimT->GenJet_pt[i], skimT->GenJet_eta[i],
                                       skimT->GenJet_phi[i], skimT->GenJet_mass[i]);
                if (iJet1 != -1 && p4GenJeti.DeltaR(p4Jet1) < 0.4 && iGenJet == -1) {
                    iGenJet = i;
                    p4GenJet1 = p4GenJeti;
                } else if (iJet2 != -1 && p4GenJeti.DeltaR(p4Jet2) < 0.4 && iGenJet2 == -1) {
                    iGenJet2 = i;
                    p4GenJet2 = p4GenJeti;
                }
            }
        }
        bool pass_gen = (iGenJet != -1);
       
        if (pass_gen || globalFlags_.isData()) {
            p1J1PtOverProbePtInRefPt->Fill(ptRef, p4Jet1.Pt() / ptRef, weight);
        }
        if (pass_gen) {
            p1GenJ1PtOverProbePtInRefPt->Fill(ptRef, p4GenJet1.Pt() / ptRef, weight);
            p1J1PtOverGenJ1PtInGenJ1Pt->Fill(p4GenJet1.Pt(), p4Jet1.Pt() / p4GenJet1.Pt(), weight);
        }
      
        coreTagHists.mvar["h1EventInRefPt"] = 1.0;
        coreTagHists.mvar["p1MpfRespInRefPt"] = mpf;
        coreTagHists.mvar["p1DbRespInRefPt"] = bal;
        coreTagHists.mvar["p1MpfResp1InRefPt"] = mpf1;
        coreTagHists.mvar["p1MpfRespNInRefPt"] = mpfn;
        coreTagHists.mvar["p1MpfRespUInRefPt"] = mpfu;
        coreTagHists.mvar["p1RhoInRefPt"] = skimT->Rho;
        coreTagHists.mvar["p1Jet1PtORefPtInRefPt"] = (p4Jet1.Pt() / ptRef);
        coreTagHists.mvar["p1GenJet1PtORefPtInRefPt"] = (p4GenJet1.Pt() / ptRef);
        coreTagHists.mvar["p1Jet1PtOGenJet1PtInGenJet1Pt"] = (p4GenJet1.Pt() != 0 ? p4Jet1.Pt() / p4GenJet1.Pt() : 0.0);
        coreTagHists.FillHistograms(skimT.get(), ptRef, iJet1, iGenJet, weight);


        if (!((fabs(p4Jet1.Eta()) < 1.3) && (p4Jet2.Pt() < ptRef || p4Jet2.Pt() < 30))) continue; 
        h1EventInCutflow->Fill(cutToBinMap["passJet1EtaJet2Pt"]);

        // Fill HistMain histograms
        coreMainHists.Fill(skimT.get(), iJet1, bal, mpf, ptRef, weight);
        
        h2EventInRefPtDbResp->Fill(ptRef, bal, weight);
        h2EventInRefPtMpfResp->Fill(ptRef, mpf, weight);
        if (p4Jet2.Pt() > 0) {
            if (iGenJet2 != -1 || !globalFlags_.isMC()) {
                p1J2PtOverProbePtInRefPt->Fill(ptRef, p4Jet2.Pt() / ptRef, weight);
            }
            if (iGenJet2 != -1) {
                p1GenJ2PtOverProbePtInRefPt->Fill(ptRef, p4GenJet2.Pt() / ptRef, weight);
                p1J2PtOverGenJ2PtInGenJ2Pt->Fill(p4GenJet2.Pt(), p4Jet2.Pt() / p4GenJet2.Pt(), weight);
            }
        }

        bool pass_DbResp = (fabs(1 - bal) < 0.7);
        bool pass_MpfResp = (fabs(1 - mpf) < 0.7);

        if (pass_MpfResp) h2EventInRefPtDbRespPassMpf->Fill(ptRef, bal, weight);
        if (pass_DbResp) h2EventInRefPtMpfRespPassDb->Fill(ptRef, mpf, weight);
        if (pass_MpfResp && pass_DbResp) {
            h2EventInRefPtDbRespPassBoth->Fill(ptRef, bal, weight);
            h2EventInRefPtMpfRespPassBoth->Fill(ptRef, mpf, weight);
        }

        if (!(pass_DbResp && pass_MpfResp)) continue;
        h1EventInCutflow->Fill(cutToBinMap["passResponse"]);
        coreRun.Fill(skimT.get(), iJet1, bal, mpf, ptRef, weight);

    }  // end of event loop

fout->Write();
Helper::printCutflow(h1EventInCutflow.get());
//Helper::scanTFile(fout);
std::cout << "Output file: " << fout->GetName() << '\n';
return 0;

}
   
