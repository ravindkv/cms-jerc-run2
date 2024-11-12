#include "RunMultiJet.h"
#include "HistCutflow.h"
#include "HistMultiJet.h"
#include "TrigList.h"
#include "Helper.h"
#include "VarBin.h"

// Constructor implementation
RunMultiJet::RunMultiJet(GlobalFlag& globalFlags)
    : globalFlags_(globalFlags) {
}

int RunMultiJet::Run(std::shared_ptr<SkimTree>& skimT, EventPick* eventP, ObjectPick* objP, ObjectScale* objS, TFile* fout) {
    TDirectory* currentDir = gDirectory;
    assert(fout && !fout->IsZombie());

    //------------------------------------
    // Initialise hists and directories 
    //------------------------------------
    // Cutflow histogram
    std::vector<std::string> cuts = {
        "passSkim", "passHLT", "passGoodLumi", "passMetFilter", "passExactly1Lead",
        "passAtleast2Recoil", "passJetVetoMap", "passDPhiLeadRecoil", "passVetoNearByJets",
        "passLeadRecoilEtaPt", "passResponse"
    };
    auto h1EventInCutflow = std::make_unique<HistCutflow>("h1EventInCutflow", cuts, fout->mkdir("Cutflow"));

    fout->mkdir("Refs");
    fout->cd("Refs");
    TH1D* h1NumJetsInEvents = new TH1D("h1NumJetsInEvents", "Number of Jets", 500, 0, 500);
    fout->cd();

    // Variable binning 
    std::vector<double> binsPt  = VarBin::getBinsPt(); 
    std::vector<double> binsEta = VarBin::getBinsEta();

    // Initialize TrigList with skimT
    TrigList trigList(skimT.get());

    // Retrieve the trigDetails map from TrigList
    const auto& trigDetails = trigList.getTrigDetails();

    std::map<std::string, HistMultiJet*> mapTriggerToHistos;
    for (const auto& trigPair : trigDetails) {
        const std::string& trigName = trigPair.first;

        if (globalFlags_.isDebug()) {
            std::cout << "Processing Trigger: " << trigName << '\n';
        }

        fout->mkdir(trigName.c_str());
        fout->cd(trigName.c_str());
        TDirectory* dirOut = gDirectory;

        const Range& r = trigPair.second.second;
        // Create an instance of HistMultiJet
        HistMultiJet* h = new HistMultiJet(trigName, r.ptMin, r.ptMax, r.absEtaMin, r.absEtaMax);

        mapTriggerToHistos[trigName] = h;
        h->triggerPt = static_cast<int>(r.ptMin);  // Assuming triggerPt corresponds to ptMin

        // Initialize histograms
        h->initializeHistograms(binsPt, dirOut);

        // Return to the main directory
        fout->cd();
    } // End of trig loop

    //------------------------------------
    // Event loop
    //------------------------------------
    double jetL1RCFactor[skimT->nJetMax];
    TLorentzVector p4RawMET, p4Type1MET, p4MHT, p4L1RC, p4DiJet;
    TLorentzVector p4Jet, p4GenJet, p4SmearedJet, p4TagJet, p4ProbeJet;
    TLorentzVector p4LeadJet, p4RecoilJet;
    TLorentzVector p4LeadJetRES, p4RecoilJetRES;
    TLorentzVector p4Bisector, p4BisectorRecoil, p4BisectorLead, p4M;
    TLorentzVector p4L, p4R;
    TLorentzVector p4M0, p4M3, p4Mn3, p4Mu3;

    TLorentzVector p4Lead;
    TLorentzVector p4Recoil;
    TLorentzVector p4LeadRES;
    TLorentzVector p4RecoilRES;

    TLorentzVector p4CorrectedJetsSum, p4L1RCJetsSum, p4RawJetsSum;
    TLorentzVector p4CorrectedJets, p4RawJet, p4RawJets, p4L1RCJet, p4L1RCJets, p4L1RCOffsets;

    double totalTime = 0.0;
    auto startClock = std::chrono::high_resolution_clock::now();
    Long64_t nEntries = skimT->getEntries();
    Helper::initProgress(nEntries);

    for (Long64_t jEntry = 0; jEntry < nEntries; jEntry++) {
        if (globalFlags_.isDebug() && jEntry > globalFlags_.getNDebug()) break;
        Helper::printProgress(jEntry, nEntries, startClock, totalTime);

        Long64_t iEntry = skimT->loadEntry(jEntry);
        if (iEntry < 0) break;
        skimT->getChain()->GetTree()->GetEntry(iEntry);
        h1EventInCutflow->fill("passSkim");

        //------------------------------------
        // Trigger and golden lumi, MET filter selection
        //------------------------------------
        // if (!eventP->passHLT(skimT)) continue;
        h1EventInCutflow->fill("passHLT");

        bool passGoodLumi = true;
        if (globalFlags_.isData()) {
            passGoodLumi = objS->checkGoodLumi(skimT->run, skimT->luminosityBlock);
        }
        if (!passGoodLumi) continue;
        h1EventInCutflow->fill("passGoodLumi");

        if (!eventP->passFilter(skimT)) continue;
        h1EventInCutflow->fill("passMetFilter");

        // Weight
        double weight = (globalFlags_.isMC() ? skimT->genWeight : 1.0);
        if (globalFlags_.isMC()) {
            weight *= objS->getPuCorrection(skimT->Pileup_nTrueInt, "nominal");
        }

	
        //------------------------------------------------
        // Jet loop: Apply JEC
        //------------------------------------------------
        // Select leading jets. Just exclude Probe, don't apply JetID yet
        bool allJetsGood = true;
        Float_t jetResolutionFactors[skimT->nJetMax];  // Custom addition
        Float_t jetDeltaJES[skimT->nJetMax];           // Custom addition
        Float_t jetCorrectionFactors[skimT->nJetMax];  // Custom addition

        int indexLeadingJet = -1, indexSecondJet = -1, numJets = 0;
        double deltaJES = 1.0, jetEnergyScale = 1.0, resolution = 1.0;

        TLorentzVector p4CorrectedJetsSum;
        TLorentzVector p4RawJetsSum;
        TLorentzVector p4L1RCJetsSum;
        TLorentzVector p4L1RCOffsetsSum;

        // Also calculate corrected type-I chsMET and HDM inputs
        p4CorrectedJetsSum.SetPtEtaPhiM(0, 0, 0, 0);
        p4RawJetsSum.SetPtEtaPhiM(0, 0, 0, 0);
        p4L1RCJetsSum.SetPtEtaPhiM(0, 0, 0, 0);
        p4L1RCOffsetsSum.SetPtEtaPhiM(0, 0, 0, 0);

        int numLead = 0;
        int numRecoil = 0;
        bool multiJetVetoNear = false;
        bool multiJetVetoFwd = false;

        // Reset multi-jet vectors
        p4Lead.SetPtEtaPhiM(0, 0, 0, 0);
        p4Recoil.SetPtEtaPhiM(0, 0, 0, 0);
        p4M3.SetPtEtaPhiM(0, 0, 0, 0);
        p4Mn3.SetPtEtaPhiM(0, 0, 0, 0);
        p4Mu3.SetPtEtaPhiM(0, 0, 0, 0);
        p4LeadRES.SetPtEtaPhiM(0, 0, 0, 0);
        p4RecoilRES.SetPtEtaPhiM(0, 0, 0, 0);

        for (int i = 0; i != skimT->nJet; ++i) {
            if (!(skimT->Jet_jetId[i] & (1 << 2))) continue; // tightLepVeto
            // Data+MC: Undo the correction already applied in NanoAOD
            skimT->Jet_pt[i] *= (1.0 - skimT->Jet_rawFactor[i]);
            skimT->Jet_mass[i] *= (1.0 - skimT->Jet_rawFactor[i]);
            p4RawJet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                                  skimT->Jet_phi[i], skimT->Jet_mass[i]);
            p4RawJetsSum += p4RawJet;

            // Data+MC: L1RC correction is applied to the uncorrected jet
            double corrL1FastJet = objS->getL1FastJetCorrection(
                skimT->Jet_area[i], skimT->Jet_eta[i], skimT->Jet_pt[i], skimT->Rho);
            skimT->Jet_pt[i] *= corrL1FastJet;
            skimT->Jet_mass[i] *= corrL1FastJet;
            p4L1RCJet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                                   skimT->Jet_phi[i], skimT->Jet_mass[i]);
            p4L1RCJetsSum += p4L1RCJet;
            p4L1RCOffsetsSum += (p4RawJet - p4L1RCJet);

            // Data+MC: MCTruth correction is applied to the L1RC corrected jet
            double corrL2Relative = objS->getL2RelativeCorrection(
                skimT->Jet_eta[i], skimT->Jet_pt[i]);
            skimT->Jet_pt[i] *= corrL2Relative;
            skimT->Jet_mass[i] *= corrL2Relative;

            // Only Data: L2Residual+L3Residual correction is applied to the MCTruth corrected jet
            if (globalFlags_.isData()) {
                double corrL2L3Residual = objS->getL2L3ResidualCorrection(
                    skimT->Jet_eta[i], skimT->Jet_pt[i]);
                skimT->Jet_pt[i] *= corrL2L3Residual;
                skimT->Jet_mass[i] *= corrL2L3Residual;
            }
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (smearJets_) {
                // Apply JER correction if necessary
                // double corrJER = objS->getJERCorrection(skimT, i, "nom");
                // skimT->Jet_pt[i] *= corrJER;
            }
            p4CorrectedJetsSum += p4Jet;

            // Calculate type-I MET (L1L2L3-RC) and MHT
            if (p4Jet.Pt() > 15.0) {
                p4MHT -= p4Jet;
                // Note: p4L1RCJet must be defined properly
                p4Type1MET += p4L1RCJet - p4Jet; // same as (+raw - rcoff) - corr
                p4M0 += p4L1RCJet - p4Jet;       // same as (+raw - rcoff) - corr
            }

            // L3Res HDM (multi-jet)
            if (i == 0 && p4Jet.Pt() > 30.0) { // leading jet
                p4Lead += p4Jet;
                p4M3 -= p4Jet;
                p4LeadRES += p4Jet;
                //p4LeadRES += jetResolutionFactors[i] * p4Jet;//FIXME
                ++numLead;
            } else if (i > 0 && p4Jet.Pt() > 30.0 && fabs(p4Jet.Eta()) < 2.5 &&
                       Helper::DELTAPHI(p4Jet.Phi(), p4Lead.Phi()) > 1.0) { // recoil jets
                p4Recoil += p4Jet;
                p4M3 -= p4Jet;
                p4RecoilRES += p4Jet;
                //p4RecoilRES += jetResolutionFactors[i] * p4Jet;//FIXME
                ++numRecoil;
            } else if (p4Jet.Pt() > 15.0) { // all other jets
                p4Mn3 -= p4Jet;
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

        if (objS->checkJetVetoMap()) continue; // expensive function
        h1EventInCutflow->fill("passJetVetoMap");

        if(!(fabs(Helper::DELTAPHI(p4Lead.Phi(), p4Recoil.Phi()) - TMath::Pi()) < 0.3)) continue;
        h1EventInCutflow->fill("passDPhiLeadRecoil");

        if(multiJetVetoNear) continue;
        if(multiJetVetoFwd)  continue; 
        h1EventInCutflow->fill("passVetoNearByJets");

        double ptRecoil = p4Recoil.Pt();
        if (globalFlags_.isDebug()){
            std::cout<<
                     ", ptRecoil = "<<ptRecoil <<
                     ", pt[0] = "<<skimT->Jet_pt[0] <<
                     ", pt[1] = "<<skimT->Jet_pt[1] <<
                     ", pt[2] = "<<skimT->Jet_pt[2] <<
                     ", eta[0] = "<<skimT->Jet_eta[0] <<
                     ", eta[1] = "<<skimT->Jet_eta[1] <<
                     ", eta[2] = "<<skimT->Jet_eta[2] <<
            std::endl;
        }
        bool passMultiJet =
            skimT->Jet_pt[0] > 30.0 && fabs(skimT->Jet_eta[0]) < 1.3 &&
            skimT->Jet_pt[1] > 30.0 && fabs(skimT->Jet_eta[1]) < 2.5 &&
            skimT->Jet_pt[2] > 30.0 && fabs(skimT->Jet_eta[2]) < 2.5;
            //skimT->Jet_pt[1] < 0.6 * ptRecoil && skimT->Jet_pt[2] < 0.6 * ptRecoil;//FIXME

        if(!passMultiJet) continue;
        h1EventInCutflow->fill("passLeadRecoilEtaPt");

        //------------------------------------------------
        // Set MET vectors
        //------------------------------------------------
        p4RawMET.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);
        p4Type1MET.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);
        p4M0.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);


        // Calculate Crecoil
        double logCRecoil = 0.0;
        double ptAverageProjection = 0.0;

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
        ptAverageProjection = 0.5 * (p4Recoil.Vect().Dot(p4Bisector.Vect()) -
                                     p4Lead.Vect().Dot(p4Bisector.Vect()));

        double ptLead = p4Lead.Pt();
        double ptAverage = 0.5 * (ptLead + ptRecoil);
        for (int i = 0; i != skimT->nJet; ++i) {
            // Ensure selection matches the one above for p4Recoil
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
                               skimT->Jet_phi[i], skimT->Jet_mass[i]);
            if (i > 0 && p4Jet.Pt() > 30.0 && fabs(p4Jet.Eta()) < 2.5 &&
                Helper::DELTAPHI(p4Jet.Phi(), p4Lead.Phi()) > 1.0) {
                double pt_i = p4Jet.Pt();
                double f_i = pt_i / ptRecoil;
                double F_i = f_i * cos(Helper::DELTAPHI(p4Lead.Phi(), p4Recoil.Phi()));
                logCRecoil += F_i * log(f_i);
                if (globalFlags_.isDebug()){
                    std::cout<<
                            "ptLead = "<<ptLead<<
                            ", ptRecoil = "<<ptRecoil<<
                            ", ptAverageProjection = "<<ptAverageProjection<<
                            ", pt_i = "<<pt_i << 
                            ", f_i = "<<f_i<<
                            ", F_i = "<<F_i<<
                            ", weight = "<<weight<<
                    std::endl; 
                }
                // Fill histograms per trig
                for (const auto& trigPair : trigDetails) {
                    if (!(*trigPair.second.first)) continue;
                    HistMultiJet* h = mapTriggerToHistos[trigPair.first];

                    // Assumption is that sum_i F_i = 1, but should check?
                    h->h2RecoilInPtAvp->Fill(ptAverageProjection, pt_i, weight * F_i);
                    h->h2RecoilInPtAve->Fill(ptAverage, pt_i, weight * F_i);
                    h->h2RecoilInPtLead->Fill(ptLead, pt_i, weight * F_i);
                    h->h2RecoilInPtRecoil->Fill(ptRecoil, pt_i, weight * F_i);

                    // Fill PF composition histograms
                    h->p1Pt25InPtAvp->Fill(ptAverageProjection, skimT->Jet_pt[i], weight * F_i);
                    h->p1Rho25InPtAvp->Fill(ptAverageProjection, skimT->Rho, weight * F_i);
                    h->p1Chf25InPtAvp->Fill(ptAverageProjection, skimT->Jet_chHEF[i], weight * F_i);
                    h->p1Nhf25InPtAvp->Fill(ptAverageProjection, skimT->Jet_neHEF[i], weight * F_i);
                    h->p1Nef25InPtAvp->Fill(ptAverageProjection, skimT->Jet_neEmEF[i], weight * F_i);
                    h->p1Cef25InPtAvp->Fill(ptAverageProjection, skimT->Jet_chEmEF[i], weight * F_i);
                    h->p1Muf25InPtAvp->Fill(ptAverageProjection, skimT->Jet_muEF[i], weight * F_i);
                } // End of trig loop
            } // End of good recoil jet condition
        } // End of jet loop

        double cRecoil = exp(logCRecoil);

        h1NumJetsInEvents->Fill(skimT->nJet, weight);

        // Multijet selection
        if (globalFlags_.isDebug()) std::cout << "Analyze multiJet" << '\n';

        // pTave binning
        ptLead = p4Lead.Pt();
        ptRecoil = p4Recoil.Pt();
        ptAverage = 0.5 * (ptLead + ptRecoil);

        // Projection to transverse plane (is this necessary?)
        p4M0.SetPtEtaPhiM(p4M0.Pt(), 0.0, p4M0.Phi(), 0.0);
        p4M3.SetPtEtaPhiM(p4M3.Pt(), 0.0, p4M3.Phi(), 0.0);
        p4Mn3.SetPtEtaPhiM(p4Mn3.Pt(), 0.0, p4Mn3.Phi(), 0.0);

        // Bisector axis p4Bisector defined earlier (equal angles)
        double m0b = 1 + (p4M0.Vect().Dot(p4Bisector.Vect())) / ptAverage;
        double m3b = 1 + (p4M3.Vect().Dot(p4Bisector.Vect())) / ptAverage;
        double mnb = 0 + (p4Mn3.Vect().Dot(p4Bisector.Vect())) / ptAverage;
        double mub = 0 + (p4Mu3.Vect().Dot(p4Bisector.Vect())) / ptAverage;

        // Dijet axis (not equal angles)
        p4M.SetPtEtaPhiM(0, 0, 0, 0);
        p4M -= p4Lead;
        p4M += p4Recoil;
        p4M.SetPtEtaPhiM(p4M.Pt(), 0.0, p4M.Phi(), 0.0);
        p4M *= 1.0 / p4M.Pt();

        double m0m = 1 + (p4M0.Vect().Dot(p4M.Vect())) / ptAverage;
        double m3m = 1 + (p4M3.Vect().Dot(p4M.Vect())) / ptAverage;
        double mnm = 0 + (p4Mn3.Vect().Dot(p4M.Vect())) / ptAverage;
        double mum = 0 + (p4Mu3.Vect().Dot(p4M.Vect())) / ptAverage;

        p4L.SetPtEtaPhiM(0, 0, 0, 0);
        p4L -= p4Lead;
        p4L.SetPtEtaPhiM(p4L.Pt(), 0.0, p4L.Phi(), 0.0);
        p4L *= 1.0 / p4L.Pt();

        double m0l = 1 + (p4M0.Vect().Dot(p4L.Vect())) / ptLead;
        double m3l = 1 + (p4M3.Vect().Dot(p4L.Vect())) / ptLead;
        double mnl = 0 + (p4Mn3.Vect().Dot(p4L.Vect())) / ptLead;
        double mul = 0 + (p4Mu3.Vect().Dot(p4L.Vect())) / ptLead;

        p4R.SetPtEtaPhiM(0, 0, 0, 0);
        p4R += p4Recoil;
        p4R.SetPtEtaPhiM(p4R.Pt(), 0.0, p4R.Phi(), 0.0);
        p4R *= 1.0 / p4R.Pt();

        double m0r = 1 + (p4M0.Vect().Dot(p4R.Vect())) / ptRecoil;
        double m3r = 1 + (p4M3.Vect().Dot(p4R.Vect())) / ptRecoil;
        double mnr = 0 + (p4Mn3.Vect().Dot(p4R.Vect())) / ptRecoil;
        double mur = 0 + (p4Mu3.Vect().Dot(p4R.Vect())) / ptRecoil;

        for (const auto& trigPair : trigDetails) {
            if (!(*trigPair.second.first)) continue;
            HistMultiJet* h = mapTriggerToHistos[trigPair.first];

            h->h1EventsInPtAvpAll->Fill(ptAverageProjection, weight);
            h->h1EventsInPtAveAll->Fill(ptAverage, weight);
            h->h1EventsInPtLeadAll->Fill(ptLead, weight);
            h->h1EventsInPtRecoilAll->Fill(ptRecoil, weight);

            if (ptAverageProjection >= h->ptMin && ptAverageProjection < h->ptMax)
                h->h1EventsInPtAvpSel->Fill(ptAverageProjection, weight);
            if (ptAverage >= h->ptMin && ptAverage < h->ptMax)
                h->h1EventsInPtAveSel->Fill(ptAverage, weight);
            if (ptLead >= h->ptMin && ptLead < h->ptMax)
                h->h1EventsInPtLeadSel->Fill(ptLead, weight);
            if (ptRecoil >= h->ptMin && ptRecoil < h->ptMax)
                h->h1EventsInPtRecoilSel->Fill(ptRecoil, weight);

            double response = (p4LeadRES.Pt() / p4RecoilRES.Pt()) /
                              (p4Lead.Pt() / p4Recoil.Pt());
            h->p1RespInPtAvp->Fill(ptAverageProjection, response, weight);
            h->p1RespInPtAve->Fill(ptAverage, response, weight);
            h->p1RespInPtLead->Fill(ptLead, response, weight);
            h->p1RespInPtRecoil->Fill(ptRecoil, response, weight);

            h->p1PtLeadInPtAvp->Fill(ptAverageProjection, ptLead, weight);
            h->p1PtLeadInPtAve->Fill(ptAverage, ptLead, weight);
            h->p1PtLeadInPtLead->Fill(ptLead, ptLead, weight);
            h->p1PtLeadInPtRecoil->Fill(ptRecoil, ptLead, weight);

            h->p1CRecoilInPtAvp->Fill(ptAverageProjection, cRecoil, weight);
            h->p1CRecoilInPtAve->Fill(ptAverage, cRecoil, weight);
            h->p1CRecoilInPtLead->Fill(ptLead, cRecoil, weight);
            h->p1CRecoilInPtRecoil->Fill(ptRecoil, cRecoil, weight);

            h->p1M0InPtAvp->Fill(ptAverageProjection, m0b, weight);
            h->p1M3InPtAvp->Fill(ptAverageProjection, m3b, weight);
            h->p1MnInPtAvp->Fill(ptAverageProjection, mnb, weight);
            h->p1MuInPtAvp->Fill(ptAverageProjection, mub, weight);

            h->p1M0InPtAve->Fill(ptAverage, m0m, weight);
            h->p1M3InPtAve->Fill(ptAverage, m3m, weight);
            h->p1MnInPtAve->Fill(ptAverage, mnm, weight);
            h->p1MuInPtAve->Fill(ptAverage, mum, weight);

            h->p1M0InPtLead->Fill(ptLead, m0l, weight);
            h->p1M3InPtLead->Fill(ptLead, m3l, weight);
            h->p1MnInPtLead->Fill(ptLead, mnl, weight);
            h->p1MuInPtLead->Fill(ptLead, mul, weight);

            h->p1M0InPtRecoil->Fill(ptRecoil, m0r, weight);
            h->p1M3InPtRecoil->Fill(ptRecoil, m3r, weight);
            h->p1MnInPtRecoil->Fill(ptRecoil, mnr, weight);
            h->p1MuInPtRecoil->Fill(ptRecoil, mur, weight);

            // Multi-jet control
            h->h2M0InPtAvp->Fill(ptAverageProjection, m0b, weight);
            h->h2M3InPtAvp->Fill(ptAverageProjection, m3b, weight);
            if (ptAverage > 1.25 * h->triggerPt)
                h->h1CosDeltaPhiInEvents->Fill(cos(Helper::DELTAPHI(p4Lead.Phi(), p4Recoil.Phi())), weight);

            // PF composition
            h->p1Pt13InPtAvp->Fill(ptAverageProjection, skimT->Jet_pt[0], weight);
            h->p1Rho13InPtAvp->Fill(ptAverageProjection, skimT->Rho, weight);
            h->p1Chf13InPtAvp->Fill(ptAverageProjection, skimT->Jet_chHEF[0], weight);
            h->p1Nhf13InPtAvp->Fill(ptAverageProjection, skimT->Jet_neHEF[0], weight);
            h->p1Nef13InPtAvp->Fill(ptAverageProjection, skimT->Jet_neEmEF[0], weight);
            h->p1Cef13InPtAvp->Fill(ptAverageProjection, skimT->Jet_chEmEF[0], weight);
            h->p1Muf13InPtAvp->Fill(ptAverageProjection, skimT->Jet_muEF[0], weight);
        } // End of trig loop

    } // End of event loop

    Helper::printCutflow(h1EventInCutflow->getHistogram());
    fout->Write();
    //Helper::scanTFile(fout);
    std::cout << "Output file: " << fout->GetName() << '\n';
    return 0;
}

