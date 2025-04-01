#include "RunMultiJet.h"

#include "HistCutflow.h"
#include "HistMultiJet.h"
#include "HistRef.h"
#include "HistTime.h"

#include "ScaleJetMet.h"

#include "Helper.h"
#include "VarBin.h"
#include "VarCut.h"
#include "MathHdm.h"
#include "TrigDetail.h"

// Constructor implementation
RunMultiJet::RunMultiJet(GlobalFlag& globalFlags)
    : globalFlags_(globalFlags) {
}

auto RunMultiJet::Run(std::shared_ptr<SkimTree>& skimT, PickEvent *pickEvent, PickObject *pickObject, ScaleEvent* scaleEvent,  ScaleObject *scaleObject, TFile *fout) -> int{
    assert(fout && !fout->IsZombie());
    fout->mkdir("Base");
    fout->cd("Base");
 
    TDirectory *origDir = gDirectory;
    //------------------------------------
    // Initialise hists and directories 
    //------------------------------------
    // Cutflow histogram
    std::vector<std::string> cuts = {
        "passSkim", "passHLT", "passGoodLumi", "passExactly1Lead",
        "passAtleast2Recoil", "passJetVetoMap", "passDPhiLeadAndRecoil", "passVetoNearByJets",
        "passMultiJet", "passHltWithPtEta"
    };
    auto h1EventInCutflow = std::make_unique<HistCutflow>(origDir, "", cuts, globalFlags_);

    // Variable binning
    VarBin varBin(globalFlags_);
    std::vector<int> pTRefs = {30, 110, 230};

    HistRef histRef(origDir, "passMultiJet", varBin);
    HistTime histTime(origDir, "passMultiJet", varBin, pTRefs);
    
    // MultiJet Histograms per HLT
    HistMultiJet histMultiJet(origDir, "passMultiJet", varBin);

    // Initialize TrigDetail
    TrigDetail trigDetail(globalFlags_);
    const auto& trigDetails = trigDetail.getTrigMapRangePtEta();
    std::map<std::string, HistMultiJet*> mapHistMultiJet;
    for (const auto& trigPair : trigDetails) {
        const std::string& trigName = trigPair.first;
        const TrigRangePtEta& r = trigPair.second;
        HistMultiJet* hMultiJet = new HistMultiJet(origDir, "passMultiJet/"+trigName, varBin);
        hMultiJet->trigPt = r.trigPt; 
        mapHistMultiJet[trigName] = hMultiJet;
    } // End of trig loop

    auto scaleJetMet = std::make_shared<ScaleJetMet>(scaleObject, globalFlags_.isData(), VarCut::applyJer);

    //------------------------------------
    // Event loop
    //------------------------------------
    TLorentzVector p4Jet;
    TLorentzVector p4L, p4R;
    TLorentzVector p4Bisector, p4BisectorRecoil, p4BisectorLead, p4M;
    TLorentzVector p4CorrMet, p4SumLeadAndRecoil, p4SumOther, p4Unclustered;

    TLorentzVector p4LeadJet, p4LeadGenJet;
    TLorentzVector p4SumRecoiledJets;
    TLorentzVector p4LeadJetRES;
    TLorentzVector p4SumRecoiledJetsRES;

    TLorentzVector p4CorrectedJets, p4RawJet, p4RawJets, p4L1RCJet, p4L1RCJets, p4L1RCOffsets;
    MathHdm mathHdm(globalFlags_);

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
        // Trigger and golden lumi
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

        //------------------------------------------------
        // Select JEC
        //------------------------------------------------
		bool multiJetVetoNear = false;
		bool multiJetVetoFwd  = false;
		
		// We'll store which jets are "recoil" later
		std::vector<int> recoilIndices;
		
		// Reset multi-jet vectors to zero
		p4LeadJet.SetPtEtaPhiM(0, 0, 0, 0);
		p4SumRecoiledJets.SetPtEtaPhiM(0, 0, 0, 0);
		p4SumLeadAndRecoil.SetPtEtaPhiM(0, 0, 0, 0);
		p4SumOther.SetPtEtaPhiM(0, 0, 0, 0);
		
		int iJet1 = -1;      // index of the leading jet
		double maxPtLead = 0; 
		double ptHardestInRecoil = 0.0;
		
		// ─────────────────────────────────────────────────────────────
		// 1) First pass: find the index of the true highest-pT jet
		// ─────────────────────────────────────────────────────────────
		for (int i = 0; i < skimT->nJet; ++i) {
			double pt = skimT->Jet_pt[i];
			if (pt > maxPtLead) {
				maxPtLead = pt;
				iJet1     = i;
			}
		}
		
		// If we found no valid jet or the max pT < 30, skip this event
		if (iJet1 < 0 || maxPtLead < 30.0) {
			// no "leading" jet by your threshold → reject event
			continue;
		}
	    if (iJet1  !=-1 && skimT->Jet_jetId[iJet1] < 6) continue;
		h1EventInCutflow->fill("passExactly1Lead");
		
		// ─────────────────────────────────────────────────────────────
		// 2) Build 4-vector for the leading jet using iJet1
		// ─────────────────────────────────────────────────────────────
		p4Jet.SetPtEtaPhiM(skimT->Jet_pt[iJet1], skimT->Jet_eta[iJet1],
						   skimT->Jet_phi[iJet1], skimT->Jet_mass[iJet1]);
		p4LeadJet = p4Jet;        // Use =, not +=
		p4SumLeadAndRecoil += p4Jet;   
		
		
		// ─────────────────────────────────────────────────────────────
		// 3) Second pass: classify the rest of the jets
		// ─────────────────────────────────────────────────────────────
		bool passId = true;
		for (int i = 0; i < skimT->nJet; ++i) {
			// Skip the leading jet itself and any failing ID
			if (i == iJet1) continue;
			
			p4Jet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i],
							   skimT->Jet_phi[i], skimT->Jet_mass[i]);
			
			double pt  = p4Jet.Pt();
			double eta = p4Jet.Eta();
			double dphi = Helper::DELTAPHI(p4Jet.Phi(), p4LeadJet.Phi());
		
			// Classify as "recoil" jets
			if (pt > 30.0 && fabs(eta) < 2.5 && dphi > 1.0) {
			    if (skimT->Jet_jetId[i] < 6) passId = false;
				p4SumRecoiledJets += p4Jet;
				p4SumLeadAndRecoil += p4Jet;
				recoilIndices.push_back(i);
				if (ptHardestInRecoil < pt) ptHardestInRecoil = pt;
			}
			else if (pt > 15.0) {
				p4SumOther += p4Jet;
			}
			// Veto conditions:
			if (pt > 30.0 && fabs(eta) < 2.5 && dphi <= 1.0) {
				multiJetVetoNear = true;
			}
			if (pt > 30.0 && fabs(eta) >= 2.5) {
				multiJetVetoFwd = true;
			}
		}
	    if (!passId) continue;
		
		// At least 2 recoil jets
		if (recoilIndices.size() < 2) {
			continue;
		}
		h1EventInCutflow->fill("passAtleast2Recoil");
		
		if (scaleEvent->checkJetVetoMap(*skimT)) continue;
		h1EventInCutflow->fill("passJetVetoMap");
		
		// Check Δφ(leading, sumRecoil) ~ π
		double deltaPhi = Helper::DELTAPHI(p4LeadJet.Phi(), p4SumRecoiledJets.Phi());
		if (!(fabs(deltaPhi - TMath::Pi()) < 0.3)) {
			continue;
		}
		h1EventInCutflow->fill("passDPhiLeadAndRecoil");
		
		// Veto near & forward jets
		if (multiJetVetoNear) continue;
		if (multiJetVetoFwd ) continue;
		h1EventInCutflow->fill("passVetoNearByJets");
		
        bool passMultiJet  = ptHardestInRecoil < 0.7 * p4SumRecoiledJets.Pt();
        if(!passMultiJet) continue;
        h1EventInCutflow->fill("passMultiJet");

        double ptAvgProj = 0.0;
        // Proper bisector axis (equal angles to each jet)
        auto p4Bisector = mathHdm.buildUnitAxisForBisector(p4SumRecoiledJets, p4LeadJet);
        // Average projection pT to bisector axis, pT_avp
        // as explained in JME-21-001 (HDM method: bisector extension)
        ptAvgProj = 0.5 * (p4SumRecoiledJets.Vect().Dot(p4Bisector.Vect()) -
                                     p4LeadJet.Vect().Dot(p4Bisector.Vect()));

        double ptLead = p4LeadJet.Pt();
        double ptRecoil = p4SumRecoiledJets.Pt();
        double sumF_i = 0.0;
        double ptAverage = 0.5 * (ptLead + ptRecoil);

        // Calculate Crecoil
        double logCrecoil = 0.0;
		std::vector<double> recoilFs;
        for (const auto& iRecoil: recoilIndices) {
            p4Jet.SetPtEtaPhiM(skimT->Jet_pt[iRecoil], skimT->Jet_eta[iRecoil],
                               skimT->Jet_phi[iRecoil], skimT->Jet_mass[iRecoil]);
            double f_i = p4Jet.Pt() / ptRecoil;
            double F_i = f_i * cos(Helper::DELTAPHI(p4Jet.Phi(), p4SumRecoiledJets.Phi()));
            sumF_i +=F_i;
            logCrecoil += F_i * log(f_i);
            recoilFs.push_back(F_i);
        } // End of jet loop
        if(globalFlags_.isDebug()){
            std::cout<<"Summ F_i = "<<sumF_i<<'\n'; //sumF_i should be 1.0
        }

        double cRecoil = exp(logCrecoil);

        //------------------------------------------------
        // Set MET vectors
        //------------------------------------------------
        p4CorrMet.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);  //CRTICAL

        // Projection to transverse plane (is this necessary?)
        p4CorrMet.SetPtEtaPhiM(p4CorrMet.Pt(), 0.0, p4CorrMet.Phi(), 0.0);
        p4SumLeadAndRecoil.SetPtEtaPhiM(p4SumLeadAndRecoil.Pt(), 0.0, p4SumLeadAndRecoil.Phi(), 0.0);
        p4SumOther.SetPtEtaPhiM(p4SumOther.Pt(), 0.0, p4SumOther.Phi(), 0.0);
        p4Unclustered = -(p4CorrMet + p4SumLeadAndRecoil + p4SumOther);

        //-------------------------------
        // Compute imputs for histograms
        //-------------------------------
        HistMultiJetInputs fillInputs;
        fillInputs.ptAvgProj = ptAvgProj;
        fillInputs.ptAverage = ptAverage;
        fillInputs.ptLead    = ptLead;
        fillInputs.etaLead    = p4LeadJet.Eta();
        fillInputs.phiLead    = p4LeadJet.Phi();
        fillInputs.ptRecoil  = ptRecoil;
        fillInputs.phiRecoil  = p4SumRecoiledJets.Phi();
        fillInputs.cRecoil   = std::exp(logCrecoil);
        fillInputs.weight    = weight;
        fillInputs.response  = 1.0;

        double offsetOne  = 1.0;
        double offsetZero = 0.0;
        // Bisector axis 
        fillInputs.m0b       = mathHdm.mpfResponse(p4CorrMet, p4Bisector, ptAverage, offsetOne);
        fillInputs.m3b       = mathHdm.mpfResponse(p4SumLeadAndRecoil, p4Bisector, ptAverage, offsetOne);
        fillInputs.mnb       = mathHdm.mpfResponse(p4SumOther, p4Bisector, ptAverage, offsetZero);
        fillInputs.mub       = mathHdm.mpfResponse(p4Unclustered, p4Bisector, ptAverage, offsetZero);

        // (Recoil - Lead) axis 
        auto p4M = mathHdm.buildUnitAxis(-p4LeadJet, p4SumRecoiledJets);
        fillInputs.m0m       = mathHdm.mpfResponse(p4CorrMet, p4M, ptAverage, offsetOne);
        fillInputs.m3m       = mathHdm.mpfResponse(p4SumLeadAndRecoil, p4M, ptAverage, offsetOne);
        fillInputs.mnm       = mathHdm.mpfResponse(p4SumOther, p4M, ptAverage, offsetZero);
        fillInputs.mum       = mathHdm.mpfResponse(p4Unclustered, p4M, ptAverage, offsetZero);

        // -Lead axis 
        auto p4L = mathHdm.buildUnitAxis(-p4LeadJet, TLorentzVector());
        fillInputs.m0l       = mathHdm.mpfResponse(p4CorrMet, p4L, ptLead, offsetOne);
        fillInputs.m3l       = mathHdm.mpfResponse(p4SumLeadAndRecoil, p4L, ptLead, offsetOne);
        fillInputs.mnl       = mathHdm.mpfResponse(p4SumOther, p4L, ptLead, offsetZero);
        fillInputs.mul       = mathHdm.mpfResponse(p4Unclustered, p4L, ptLead, offsetZero);

        // Recoil axis 
        auto p4R = mathHdm.buildUnitAxis(p4SumRecoiledJets, TLorentzVector());
        fillInputs.m0r       = mathHdm.mpfResponse(p4CorrMet, p4R, ptRecoil, offsetOne);
        fillInputs.m3r       = mathHdm.mpfResponse(p4SumLeadAndRecoil, p4R, ptRecoil, offsetOne);
        fillInputs.mnr       = mathHdm.mpfResponse(p4SumOther, p4R, ptRecoil, offsetZero);
        fillInputs.mur       = mathHdm.mpfResponse(p4Unclustered, p4R, ptRecoil, offsetZero);

        for (const auto& trigPair : trigDetails) {
            const std::string& trigName = trigPair.first;
            bool isPresent = std::find(passedHlts.begin(), passedHlts.end(),
                                       trigName) != passedHlts.end();
            if (!isPresent) continue;
            //Fill HistMultiJet
            HistMultiJet* h = mapHistMultiJet[trigName];
            h->setInputs(fillInputs);
            for (int i = 0; i != recoilIndices.size(); ++i) {
                histMultiJet.fillJetLevelHistos(skimT.get(), recoilIndices.at(i), weight * recoilFs.at(i));
            }    
            h->fillEventLevelHistos(skimT.get(), iJet1, h->trigPt);
        } // End of trig loop


        //------------------------------------------------
        // Exclusive HLT for a given pT 
        //------------------------------------------------
        if (!pickEvent->passHltWithPtEta(skimT, ptLead, p4LeadJet.Eta())) continue; 
        h1EventInCutflow->fill("passHltWithPtEta");

        histMultiJet.setInputs(fillInputs);
        for (int i = 0; i != recoilIndices.size(); ++i) {
            histMultiJet.fillJetLevelHistos(skimT.get(), recoilIndices.at(i), weight* recoilFs.at(i));
        } 
        double trigPt = 1.0;
        auto it = trigDetails.find(pickEvent->getPassedHlt());
        if (it != trigDetails.end()) {
            trigPt = it->second.trigPt;
        }
        histMultiJet.fillEventLevelHistos(skimT.get(), iJet1, trigPt);

        //Fill other histograms
        histRef.Fill(1.0, p4LeadJet, p4LeadGenJet, weight); 
        if(globalFlags_.isData()){
            histTime.Fill(skimT.get(), iJet1, fillInputs.m0l, fillInputs.m3l, p4LeadJet.Pt(), weight);
        }

    } // End of event loop

    h1EventInCutflow->printCutflow();
    h1EventInCutflow->fillFractionCutflow();
    fout->Write();
    //Helper::scanTFile(fout);
    std::cout << "Output file: " << fout->GetName() << '\n';
    return 0;
}

