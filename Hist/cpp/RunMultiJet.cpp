#include "RunMultiJet.h"
#include "HistCutflow.h"
#include "HistMultiJet.h"
#include "TrigDetail.h"

#include "ScaleJetMet.h"

#include "Helper.h"
#include "VarBin.h"
#include "VarCut.h"
#include "MathHdm.h"

// Constructor implementation
RunMultiJet::RunMultiJet(GlobalFlag& globalFlags)
    : globalFlags_(globalFlags) {
}

auto RunMultiJet::Run(std::shared_ptr<SkimTree>& skimT, PickEvent *pickEvent, PickObject *pickObject, ScaleEvent* scaleEvent,  ScaleObject *scaleObject, TFile *fout) -> int{
    assert(fout && !fout->IsZombie());
 
    TDirectory *origDir = gDirectory;
    //------------------------------------
    // Initialise hists and directories 
    //------------------------------------
    // Cutflow histogram
    std::vector<std::string> cuts = {
        "passSkim", "passHLT", "passGoodLumi", "passMetFilter", "passExactly1Lead",
        "passAtleast2Recoil", "passJetVetoMap", "passDPhiLeadRecoil", "passVetoNearByJets",
        "passMultiJet"
    };
    auto h1EventInCutflow = std::make_unique<HistCutflow>(origDir, "", cuts, globalFlags_);

    fout->mkdir("Refs");
    fout->cd("Refs");
    TH1D* h1NumJetsInEvents = new TH1D("h1NumJetsInEvents", "Number of Jets", 500, 0, 500);
    fout->cd();

    // Variable binning
    VarBin varBin(globalFlags_);
    std::vector<double> binsPt  = varBin.getBinsPt(); 
    std::vector<double> binsEta = varBin.getBinsEta(); 

    // Initialize TrigDetail
    TrigDetail trigDetail(globalFlags_);
    const auto& trigDetails = trigDetail.getTrigMapRangePtEta();

    std::map<std::string, HistMultiJet*> mapTriggerToHistos;
    for (const auto& trigPair : trigDetails) {
        const std::string& trigName = trigPair.first;

        if (globalFlags_.isDebug()) {
            std::cout << "Processing Trigger: " << trigName << '\n';
        }

        fout->mkdir(trigName.c_str());
        fout->cd(trigName.c_str());
        TDirectory* dirOut = gDirectory;

        const TrigRangePtEta& r = trigPair.second;
        // Create an instance of HistMultiJet
        HistMultiJet* h = new HistMultiJet(trigName, r.ptMin, r.ptMax, r.absEtaMin, r.absEtaMax);

        mapTriggerToHistos[trigName] = h;
        h->triggerPt = static_cast<int>(r.ptMin);  // Assuming triggerPt corresponds to ptMin

        // Initialize histograms
        h->initializeHistograms(binsPt, dirOut);

        // Return to the main directory
        fout->cd();
    } // End of trig loop

    auto scaleJetMet = std::make_shared<ScaleJetMet>(scaleObject, globalFlags_.isData(), VarCut::applyJer);
    //------------------------------------
    // Event loop
    //------------------------------------
    TLorentzVector p4Jet;
    TLorentzVector p4LeadJet, p4RecoilJet;
    TLorentzVector p4LeadJetRES, p4RecoilJetRES;
    TLorentzVector p4Bisector, p4BisectorRecoil, p4BisectorLead, p4M;
    TLorentzVector p4L, p4R;
    TLorentzVector p4CorrMet, p4NegSum_LeadRecoil, p4NegSum_Other, p4Unclustered;

    TLorentzVector p4Lead;
    TLorentzVector p4Recoil;
    TLorentzVector p4LeadRES;
    TLorentzVector p4RecoilRES;

    TLorentzVector p4CorrectedJets, p4RawJet, p4RawJets, p4L1RCJet, p4L1RCJets, p4L1RCOffsets;

    double totalTime = 0.0;
    auto startClock = std::chrono::high_resolution_clock::now();
    Long64_t nentries = skimT->getEntries();
    Helper::initProgress(nentries);

    for (Long64_t jentry = 0; jentry < nentries; ++jentry) {
        if (globalFlags_.isDebug() && jentry > globalFlags_.getNDebug()) break;
        Helper::printProgress(jentry, nentries, startClock, totalTime, globalFlags_.isDebug());
        Long64_t ientry = skimT->loadEntry(jentry);
        if (ientry < 0) break; 
        skimT->getChain()->GetTree()->GetEntry(ientry);
        h1EventInCutflow->fill("passSkim");

        //------------------------------------
        // Trigger and golden lumi, MET filter selection
        //------------------------------------
        if (!pickEvent->passHlt(skimT)) continue;
        std::vector<std::string> passedHlts = pickEvent->getPassedHlts();
        h1EventInCutflow->fill("passHLT");

        bool passGoodLumi = true;
        if (globalFlags_.isData()) {
            passGoodLumi = scaleEvent->checkGoodLumi(skimT->run, skimT->luminosityBlock);
        }
        if (!passGoodLumi) continue;
        h1EventInCutflow->fill("passGoodLumi");

        if (!pickEvent->passFilter(skimT)) continue;
        h1EventInCutflow->fill("passMetFilter");

        // Weight
        double weight = (globalFlags_.isMC() ? skimT->genWeight : 1.0);
        if (globalFlags_.isMC()) {
            weight *= scaleEvent->getPuCorrection(skimT->Pileup_nTrueInt, "nominal");
        }

	
        //------------------------------------------------
        // Apply JEC
        //------------------------------------------------
        scaleJetMet->applyCorrections(skimT, ScaleJetMet::CorrectionLevel::L2L3Res);
        if(globalFlags_.isDebug()) scaleJetMet->print();

        // Select leading jets. Just exclude Probe, don't apply JetID yet
        int indexLeadingJet = -1, indexSecondJet = -1, numJets = 0;
        double deltaJES = 1.0, jetEnergyScale = 1.0, resolution = 1.0;

        int numLead = 0;
        int numRecoil = 0;
        bool multiJetVetoNear = false;
        bool multiJetVetoFwd = false;

        // Reset multi-jet vectors
        p4Lead.SetPtEtaPhiM(0, 0, 0, 0);
        p4Recoil.SetPtEtaPhiM(0, 0, 0, 0);
        p4NegSum_LeadRecoil.SetPtEtaPhiM(0, 0, 0, 0);
        p4NegSum_Other.SetPtEtaPhiM(0, 0, 0, 0);
        p4Unclustered.SetPtEtaPhiM(0, 0, 0, 0);
        p4LeadRES.SetPtEtaPhiM(0, 0, 0, 0);
        p4RecoilRES.SetPtEtaPhiM(0, 0, 0, 0);

        double ptHardestInRecoil = 0.0;
        for (int i = 0; i != skimT->nJet; ++i) {
            if(skimT->Jet_jetId[i] < 6)  continue;
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);

            // L3Res HDM (multi-jet)
            if (i == 0 && p4Jet.Pt() > 30.0) { // leading jet
                p4Lead += p4Jet;
                p4NegSum_LeadRecoil -= p4Jet;
                p4LeadRES += p4Jet;
                ++numLead;
            } else if (i > 0 && p4Jet.Pt() > 30.0 && fabs(p4Jet.Eta()) < 2.5 &&
                       Helper::DELTAPHI(p4Jet.Phi(), p4Lead.Phi()) > 1.0) { // recoil jets
                p4Recoil += p4Jet;
                p4NegSum_LeadRecoil -= p4Jet;
                p4RecoilRES += p4Jet;
                ++numRecoil;
                if(ptHardestInRecoil < p4Jet.Pt()) ptHardestInRecoil = p4Jet.Pt();
            } else if (p4Jet.Pt() > 15.0) { // all other jets
                p4NegSum_Other -= p4Jet;
            }

            // Veto nearby jets for multi-jet topology
            if (i > 0 && p4Jet.Pt() > 30.0 && fabs(p4Jet.Eta()) < 2.5 &&
                Helper::DELTAPHI(p4Jet.Phi(), p4Lead.Phi()) <= 1.0)
                multiJetVetoNear = true;

            // Veto forward jets for multi-jet topology
            if (i > 0 && p4Jet.Pt() > 30.0 && fabs(p4Jet.Eta()) >= 2.5)
                multiJetVetoFwd = true;

        } // End of jet loop

        if (numLead != 1) continue;
        h1EventInCutflow->fill("passExactly1Lead");

        if (numRecoil < 2) continue;
        h1EventInCutflow->fill("passAtleast2Recoil");

        //if (scaleEvent->checkJetVetoMap(*skimT)) continue; // expensive function
        h1EventInCutflow->fill("passJetVetoMap");

        if(!(fabs(Helper::DELTAPHI(p4Lead.Phi(), p4Recoil.Phi()) - TMath::Pi()) < 0.3)) continue;
        h1EventInCutflow->fill("passDPhiLeadRecoil");

        if(multiJetVetoNear) continue;
        if(multiJetVetoFwd)  continue; 
        h1EventInCutflow->fill("passVetoNearByJets");

        bool passMultiJet  = p4Lead.Pt() > 0.7 * ptHardestInRecoil;
        if(!passMultiJet) continue;
        h1EventInCutflow->fill("passMultiJet");


        // Calculate Crecoil
        double logCrecoil = 0.0;
        double ptAvgProj = 0.0;

        // Proper bisector axis (equal angles to each jet)
        p4Bisector.SetPtEtaPhiM(0, 0, 0, 0);
        p4BisectorRecoil.SetPtEtaPhiM(1, 0, p4Recoil.Phi(), 0);
        p4BisectorLead.SetPtEtaPhiM(1, 0, p4Lead.Phi(), 0);
        p4Bisector += p4BisectorRecoil;
        p4Bisector -= p4BisectorLead;
        p4Bisector.SetPtEtaPhiM(p4Bisector.Pt(), 0.0, p4Bisector.Phi(), 0.0);
        p4Bisector *= 1.0 / p4Bisector.Pt();

        // Average projection pT to bisector axis, pT_avp
        // as explained in JME-21-001 (HDM method: bisector extension)
        ptAvgProj = 0.5 * (p4Recoil.Vect().Dot(p4Bisector.Vect()) -
                                     p4Lead.Vect().Dot(p4Bisector.Vect()));

        double ptLead = p4Lead.Pt();
        double ptRecoil = p4Recoil.Pt();
        double sumF_i = 0.0;
        double ptAverage = 0.5 * (ptLead + ptRecoil);
        for (int i = 0; i != skimT->nJet; ++i) {
            // Ensure selection matches the one above for p4Recoil
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (i > 0 && p4Jet.Pt() > 30.0 && fabs(p4Jet.Eta()) < 2.5 &&
                Helper::DELTAPHI(p4Jet.Phi(), p4Lead.Phi()) > 1.0) {
                double pt_i = p4Jet.Pt();
                double f_i = pt_i / ptRecoil;
                double F_i = f_i * cos(Helper::DELTAPHI(p4Jet.Phi(), p4Recoil.Phi()));
                sumF_i +=F_i;
                logCrecoil += F_i * log(f_i);
                if (globalFlags_.isDebug()){
                    std::cout<<
                            "ptLead = "<<ptLead<<
                            ", ptRecoil = "<<ptRecoil<<
                            ", ptAvgProj = "<<ptAvgProj<<
                            ", pt_i = "<<pt_i << 
                            ", f_i = "<<f_i<<
                            ", F_i = "<<F_i<<
                            ", weight = "<<weight<<
                    std::endl; 
                }
                // Fill histograms per trig
                for (const auto& trigPair : trigDetails) {
                    bool isPresent = std::find(passedHlts.begin(), passedHlts.end(),
                                               trigPair.first) != passedHlts.end();

                    if (!isPresent) continue;
                    HistMultiJet* h = mapTriggerToHistos[trigPair.first];

                    // Assumption is that sum_i F_i = 1, but should check?
                    h->h2JetsPtInAvgProjPt->Fill(ptAvgProj, pt_i, weight * F_i);
                    h->h2JetsPtInAvgPt->Fill(ptAverage, pt_i, weight * F_i);
                    h->h2JetsPtInLeadPt->Fill(ptLead, pt_i, weight * F_i);
                    h->h2JetsPtInRecoilPt->Fill(ptRecoil, pt_i, weight * F_i);

                    // Fill PF composition histograms
                    h->p1RhoInAvgProjPt->Fill(ptAvgProj, skimT->Rho, weight * F_i);
                    h->p1JetsPtInAvgProjPt->Fill(ptAvgProj, skimT->Jet_pt[i], weight * F_i);
                    h->p1JetsChfInAvgProjPt->Fill(ptAvgProj, skimT->Jet_chHEF[i], weight * F_i);
                    h->p1JetsNhfInAvgProjPt->Fill(ptAvgProj, skimT->Jet_neHEF[i], weight * F_i);
                    h->p1JetsNefInAvgProjPt->Fill(ptAvgProj, skimT->Jet_neEmEF[i], weight * F_i);
                    h->p1JetsCefInAvgProjPt->Fill(ptAvgProj, skimT->Jet_chEmEF[i], weight * F_i);
                    h->p1JetsMufInAvgProjPt->Fill(ptAvgProj, skimT->Jet_muEF[i], weight * F_i);
                } // End of trig loop
            } // End of good recoil jet condition
        } // End of jet loop
        if(globalFlags_.isDebug()){
            std::cout<<"Summ F_i = "<<sumF_i<<'\n'; //sumF_i should be 1.0
        }

        double cRecoil = exp(logCrecoil);

        h1NumJetsInEvents->Fill(skimT->nJet, weight);

        // Multijet selection
        if (globalFlags_.isDebug()) std::cout << "Analyze multiJet" << '\n';

        //------------------------------------------------
        // Set MET vectors
        //------------------------------------------------
        p4CorrMet.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);

        // Projection to transverse plane (is this necessary?)
        p4CorrMet.SetPtEtaPhiM(p4CorrMet.Pt(), 0.0, p4CorrMet.Phi(), 0.0);
        p4NegSum_LeadRecoil.SetPtEtaPhiM(p4NegSum_LeadRecoil.Pt(), 0.0, p4NegSum_LeadRecoil.Phi(), 0.0);
        p4NegSum_Other.SetPtEtaPhiM(p4NegSum_Other.Pt(), 0.0, p4NegSum_Other.Phi(), 0.0);
        p4Unclustered = p4CorrMet - p4NegSum_LeadRecoil - p4NegSum_Other;

        // Bisector axis 
        double m0b = 1 + (p4CorrMet.Vect().Dot(p4Bisector.Vect())) / ptAverage;
        double m3b = 1 + (p4NegSum_LeadRecoil.Vect().Dot(p4Bisector.Vect())) / ptAverage;
        double mnb = 0 + (p4NegSum_Other.Vect().Dot(p4Bisector.Vect())) / ptAverage;
        double mub = 0 + (p4Unclustered.Vect().Dot(p4Bisector.Vect())) / ptAverage;

        // Lead axis 
        p4L.SetPtEtaPhiM(0, 0, 0, 0);
        p4L -= p4Lead;
        p4L.SetPtEtaPhiM(p4L.Pt(), 0.0, p4L.Phi(), 0.0);
        p4L *= 1.0 / p4L.Pt();

        double m0l = 1 + (p4CorrMet.Vect().Dot(p4L.Vect())) / ptLead;
        double m3l = 1 + (p4NegSum_LeadRecoil.Vect().Dot(p4L.Vect())) / ptLead;
        double mnl = 0 + (p4NegSum_Other.Vect().Dot(p4L.Vect())) / ptLead;
        double mul = 0 + (p4Unclustered.Vect().Dot(p4L.Vect())) / ptLead;

        // Recoil axis 
        p4R.SetPtEtaPhiM(0, 0, 0, 0);
        p4R += p4Recoil;
        p4R.SetPtEtaPhiM(p4R.Pt(), 0.0, p4R.Phi(), 0.0);
        p4R *= 1.0 / p4R.Pt();

        double m0r = 1 + (p4CorrMet.Vect().Dot(p4R.Vect())) / ptRecoil;
        double m3r = 1 + (p4NegSum_LeadRecoil.Vect().Dot(p4R.Vect())) / ptRecoil;
        double mnr = 0 + (p4NegSum_Other.Vect().Dot(p4R.Vect())) / ptRecoil;
        double mur = 0 + (p4Unclustered.Vect().Dot(p4R.Vect())) / ptRecoil;

        // (Lead - Recoil) axis 
        p4M.SetPtEtaPhiM(0, 0, 0, 0);
        p4M -= p4Lead;
        p4M += p4Recoil;
        p4M.SetPtEtaPhiM(p4M.Pt(), 0.0, p4M.Phi(), 0.0);
        p4M *= 1.0 / p4M.Pt();

        double m0m = 1 + (p4CorrMet.Vect().Dot(p4M.Vect())) / ptAverage;
        double m3m = 1 + (p4NegSum_LeadRecoil.Vect().Dot(p4M.Vect())) / ptAverage;
        double mnm = 0 + (p4NegSum_Other.Vect().Dot(p4M.Vect())) / ptAverage;
        double mum = 0 + (p4Unclustered.Vect().Dot(p4M.Vect())) / ptAverage;

        for (const auto& trigPair : trigDetails) {
            bool isPresent = std::find(passedHlts.begin(), passedHlts.end(),
                                       trigPair.first) != passedHlts.end();

            if (!isPresent) continue;
            HistMultiJet* h = mapTriggerToHistos[trigPair.first];

            h->h1EventInAvgProjPt->Fill(ptAvgProj, weight);
            h->h1EventInAvgPt->Fill(ptAverage, weight);
            h->h1EventInLeadPt->Fill(ptLead, weight);
            h->h1EventInRecoilPt->Fill(ptRecoil, weight);

            if (ptAvgProj >= h->ptMin && ptAvgProj < h->ptMax)
                h->h1EventInAvgProjPtForTrigCut->Fill(ptAvgProj, weight);
            if (ptAverage >= h->ptMin && ptAverage < h->ptMax)
                h->h1EventInAvgPtForTrigCut->Fill(ptAverage, weight);
            if (ptLead >= h->ptMin && ptLead < h->ptMax)
                h->h1EventInLeadPtForTrigCut->Fill(ptLead, weight);
            if (ptRecoil >= h->ptMin && ptRecoil < h->ptMax)
                h->h1EventInRecoilPtForTrigCut->Fill(ptRecoil, weight);

            double response = (p4LeadRES.Pt() / p4RecoilRES.Pt()) /
                              (p4Lead.Pt() / p4Recoil.Pt());
            h->p1RespInAvgProjPt->Fill(ptAvgProj, response, weight);
            h->p1RespInAvgPt->Fill(ptAverage, response, weight);
            h->p1RespInLeadPt->Fill(ptLead, response, weight);
            h->p1RespInRecoilPt->Fill(ptRecoil, response, weight);

            h->p1LeadPtInAvgProjPt->Fill(ptAvgProj, ptLead, weight);
            h->p1LeadPtInAvgPt->Fill(ptAverage, ptLead, weight);
            h->p1LeadPtInLeadPt->Fill(ptLead, ptLead, weight);
            h->p1LeadPtInRecoilPt->Fill(ptRecoil, ptLead, weight);

            h->p1CrecoilInAvgProjPt->Fill(ptAvgProj, cRecoil, weight);
            h->p1CrecoilInAvgPt->Fill(ptAverage, cRecoil, weight);
            h->p1CrecoilInLeadPt->Fill(ptLead, cRecoil, weight);
            h->p1CrecoilInRecoilPt->Fill(ptRecoil, cRecoil, weight);

            h->p1MpfResp0InAvgProjPt->Fill(ptAvgProj, m0b, weight);
            h->p1MpfResp3InAvgProjPt->Fill(ptAvgProj, m3b, weight);
            h->p1MpfRespnInAvgProjPt->Fill(ptAvgProj, mnb, weight);
            h->p1MpfRespuInAvgProjPt->Fill(ptAvgProj, mub, weight);

            h->p1MpfResp0InAvgPt->Fill(ptAverage, m0m, weight);
            h->p1MpfResp3InAvgPt->Fill(ptAverage, m3m, weight);
            h->p1MpfRespnInAvgPt->Fill(ptAverage, mnm, weight);
            h->p1MpfRespuInAvgPt->Fill(ptAverage, mum, weight);

            h->p1MpfResp0InLeadPt->Fill(ptLead, m0l, weight);
            h->p1MpfResp3InLeadPt->Fill(ptLead, m3l, weight);
            h->p1MpfRespnInLeadPt->Fill(ptLead, mnl, weight);
            h->p1MpfRespuInLeadPt->Fill(ptLead, mul, weight);

            h->p1MpfResp0InRecoilPt->Fill(ptRecoil, m0r, weight);
            h->p1MpfResp3InRecoilPt->Fill(ptRecoil, m3r, weight);
            h->p1MpfRespnInRecoilPt->Fill(ptRecoil, mnr, weight);
            h->p1MpfRespuInRecoilPt->Fill(ptRecoil, mur, weight);

            // Multi-jet control
            h->h2MpfResp0InAvgProjPt->Fill(ptAvgProj, m0b, weight);
            h->h2MpfResp3InAvgProjPt->Fill(ptAvgProj, m3b, weight);
            if (ptAverage > 1.25 * h->triggerPt)
                h->h1EventInCosDeltaPhiLeadRecoil->Fill(cos(Helper::DELTAPHI(p4Lead.Phi(), p4Recoil.Phi())), weight);

            // PF composition
            if (std::abs(p4Lead.Eta()) < 1.3){
                h->p1RhoInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Rho, weight);
                h->p1Jet1PtInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_pt[0], weight);
                h->p1Jet1ChfInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_chHEF[0], weight);
                h->p1Jet1NhfInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_neHEF[0], weight);
                h->p1Jet1NefInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_neEmEF[0], weight);
                h->p1Jet1CefInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_chEmEF[0], weight);
                h->p1Jet1MufInAvgProjPtForLeadEta1p3->Fill(ptAvgProj, skimT->Jet_muEF[0], weight);
            }
        } // End of trig loop

    } // End of event loop

    h1EventInCutflow->printCutflow();
    h1EventInCutflow->fillFractionCutflow();
    fout->Write();
    //Helper::scanTFile(fout);
    std::cout << "Output file: " << fout->GetName() << '\n';
    return 0;
}

