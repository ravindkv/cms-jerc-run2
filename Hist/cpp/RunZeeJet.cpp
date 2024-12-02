#include "RunZeeJet.h"

#include "HistCutflow.h"
#include "HistTag.h"
#include "HistTime.h"
#include "HistRef.h"
#include "HistJerc.h"
#include "HistBarrel.h"
#include "HistMain.h"
#include "HistFinal.h"

#include "ApplyJerc.h"
#include "Helper.h"
#include "VarBin.h"
   
// Constructor implementation
RunZeeJet::RunZeeJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}

auto RunZeeJet::Run(std::shared_ptr<SkimTree>& skimT, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout) -> int{
   
    assert(fout && !fout->IsZombie());
    fout->mkdir("Base");
    fout->cd("Base");
 
    TDirectory *origDir = gDirectory;
    //------------------------------------
    // Initialise hists and directories 
    //------------------------------------
    
    // Cutflow histograms
    std::vector<std::string> cuts = {
    		"passSkim", "passHLT", "passGoodLumi", "passMetFilter", "passAtleast1Ref",
    		"passAtleast1Jet", "passJetVetoMap", "passDPhiRefJet1", "passRefBarrel",
    		"passJet1EtaJet2Pt", "passResponse"
    };
    auto h1EventInCutflow = std::make_unique<HistCutflow>(origDir, "", cuts);
      
    // Variable binning
    VarBin varBin(globalFlags_);
    std::vector<int> pTRefs = {30, 110, 230};

    HistRef histRef(origDir, "passAtleast1Ref", varBin);

    HistJerc histJerc(origDir, "passAtleast1Jet", varBin);
    
    HistBarrel histBarrel(origDir, "passRefBarrel", varBin);

    HistTag histTag(origDir, "passRefBarrel", varBin, globalFlags_);
    
    HistRef histRef2(origDir,  "passJet1EtaJet2Pt", varBin);

    HistMain histMain(origDir, "passJet1EtaJet2Pt", varBin);

    HistFinal histFinal(origDir, "passJet1EtaJet2Pt", varBin);

    HistTime histTime(origDir, "passResponse", varBin, pTRefs);

    //------------------------------------
    // Event loop
    //------------------------------------
    TLorentzVector p4Ref, p4GenRef, p4Jet1, p4Jet2, p4Jetn;
    TLorentzVector p4Met, p4Met1, p4Metn, p4Metu, p4Metnu, p4RawRef;
    TLorentzVector p4Jeti;
    TLorentzVector p4GenJeti, p4GenJet1, p4GenJet2;
    TLorentzVector p4Refx; // for MPFX

    // ApplyJerc constructor
    auto appliedJerc = std::make_shared<ApplyJerc>(objS, globalFlags_.isData(), VarCut::applyJer);

    double totalTime = 0.0;
    auto startClock = std::chrono::high_resolution_clock::now();
    Long64_t nentries = skimT->getEntries();
    Helper::initProgress(nentries);

    for (Long64_t jentry = 0; jentry < nentries; ++jentry) {
        if (globalFlags_.isDebug() && jentry > globalFlags_.getNDebug()) break;
        Helper::printProgress(jentry, nentries, startClock, totalTime);
       
        Long64_t ientry = skimT->loadEntry(jentry);
        if (ientry < 0) break; 
        skimT->getChain()->GetTree()->GetEntry(ientry);
        h1EventInCutflow->fill("passSkim");

        //------------------------------------
        // Trigger and golden lumi, MET filter selection 
        //------------------------------------
        if (!eventP->passHLT(skimT)) continue; 
        h1EventInCutflow->fill("passHLT");

        bool passGoodLumi = true; 
        if (globalFlags_.isData()){
            passGoodLumi = objS->checkGoodLumi(skimT->run, skimT->luminosityBlock);
        }
        if (!passGoodLumi) continue; 
        h1EventInCutflow->fill("passGoodLumi");

        if (!eventP->passFilter(skimT)) continue; 
        h1EventInCutflow->fill("passMetFilter");

        //------------------------------------------
        // Select objects
        //------------------------------------------
        // Reco objects
        objP->clearObjects();
        objP->pickElectrons();
        if (objP->getPickedElectrons().size() < 2) continue;
        if (objP->getPickedElectrons().size() > 3) continue;
        objP->pickRefs();
        std::vector<TLorentzVector> p4Refs = objP->getPickedRefs();

        if (p4Refs.empty()) continue; 
        h1EventInCutflow->fill("passAtleast1Ref");

        // Weight
        double weight = (globalFlags_.isMC() ? skimT->genWeight : 1.0);
        if (globalFlags_.isMC()) weight *= objS->getPuCorrection(skimT->Pileup_nTrueInt, "nominal");

        p4Ref = p4Refs.at(0);
        p4RawRef = p4Refs.at(0);
        double ptRef = p4Ref.Pt();

        // Gen objects
        p4GenRef.SetPtEtaPhiM(0, 0, 0, 0);
        if (globalFlags_.isMC()) {
            objP->pickGenElectrons();
            objP->pickGenRefs();
            std::vector<TLorentzVector> p4GenRefs = objP->getPickedGenRefs();
            if (p4GenRefs.empty()) continue;
            p4GenRef = p4GenRefs.at(0);
        }
        // Fill HistRef histograms
        histRef.Fill(p4Refs.size(), p4Ref, p4GenRef, weight); 
        
        // Apply jet energy corrections
        appliedJerc->applyCorrections(skimT, ApplyJerc::CorrectionLevel::L2L3Res);
        if(globalFlags_.isDebug()) appliedJerc->print();

        //------------------------------------------------
        // Jet loop: Apply JEC
        //------------------------------------------------
        // Select leading jets. Just exclude Ref, don't apply JetID yet
        int iJet1(-1), iJet2(-1), nJets(0);
        p4Jet1.SetPtEtaPhiM(0, 0, 0, 0);
        p4Jet2.SetPtEtaPhiM(0, 0, 0, 0);
        p4Jetn.SetPtEtaPhiM(0, 0, 0, 0);
        for (int i = 0; i != skimT->nJet; ++i) {
            if (!(skimT->Jet_jetId[i] & (1 << 2))) continue; // tightLepVeto
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
        }//nJet

        if (nJets < 1) continue; 
        h1EventInCutflow->fill("passAtleast1Jet");
        histJerc.Fill(*appliedJerc, weight);

        if (objS->checkJetVetoMap()) continue; // expensive function
        h1EventInCutflow->fill("passJetVetoMap");
        

        //------------------------------------------------
        // Set MET vectors
        //------------------------------------------------
        TLorentzVector p4CorrJetSum = appliedJerc->getP4MapJetSum().at("Corr");
        TLorentzVector p4CorrMet    = appliedJerc->getP4MapMet().at("Type1Corr");

        p4CorrMet += p4RawRef - p4Ref; // replace PF Ref with Reco Ref
        p4Met1 = -p4Jet1 - p4Ref;
        p4Metn = -p4Jetn;
        // Unclustered MET from corrMET
        p4Metu = p4CorrMet + p4Ref + p4CorrJetSum;
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
       
        double deltaPhi = Helper::DELTAPHI(p4Ref.Phi(), p4Jet1.Phi());
        if (fabs(deltaPhi - TMath::Pi()) >= 0.44) continue; 
        h1EventInCutflow->fill("passDPhiRefJet1");

        if (fabs(p4Ref.Eta()) > 1.3) continue; 
        h1EventInCutflow->fill("passRefBarrel");

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
        histBarrel.Fill(p4Ref, p4Jet1, p4GenJet1, weight);
 
        //flavorXTaggedY
        histTag.SetResponse(bal, mpf, mpf1, mpfn, mpfu);
        histTag.FillHistograms(skimT.get(), ptRef, iJet1, iGenJet, weight);

        if (!((fabs(p4Jet1.Eta()) < 1.3) && (p4Jet2.Pt() < ptRef || p4Jet2.Pt() < 30))) continue; 
        h1EventInCutflow->fill("passJet1EtaJet2Pt");
        histRef2.Fill(p4Refs.size(), p4Ref, p4GenRef, weight); 

        histFinal.Fill(ptRef, bal, mpf, p4Jet2, p4GenJet2, iGenJet2, globalFlags_.isMC(), weight);

        histMain.Fill(skimT.get(), iJet1, bal, mpf, ptRef, weight);
       
        bool pass_DbResp = (fabs(1 - bal) < 0.7);
        bool pass_MpfResp = (fabs(1 - mpf) < 0.7); 
        if (!(pass_DbResp && pass_MpfResp)) continue;
        h1EventInCutflow->fill("passResponse");
        if(globalFlags_.isData()) histTime.Fill(skimT.get(), iJet1, bal, mpf, ptRef, weight);

    }  // end of event loop

    Helper::printCutflow(h1EventInCutflow->getHistogram());
    fout->Write();
    //Helper::scanTFile(fout);
    std::cout << "Output file: " << fout->GetName() << '\n';
    return 0;
}
   
