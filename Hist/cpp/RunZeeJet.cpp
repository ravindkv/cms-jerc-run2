#include "RunZeeJet.h"

#include "HistCutflow.h"
#include "HistTag.h"
#include "HistTime.h"
#include "HistRef.h"
#include "HistBarrel.h"
#include "HistMain.h"
#include "HistFinal.h"

#include "ScaleElectron.h"
#include "ScaleJetMet.h"
#include "HistScale.h"

#include "Helper.h"
#include "VarBin.h"
#include "MathHdm.h"
   
// Constructor implementation
RunZeeJet::RunZeeJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}

auto RunZeeJet::Run(std::shared_ptr<SkimTree>& skimT, PickEvent *pickEvent, PickObject *pickObject, ScaleEvent* scaleEvent, ScaleObject *scaleObject, TFile *fout) -> int{
   
    assert(fout && !fout->IsZombie());
    fout->mkdir("Base");
    fout->cd("Base");
 
    TDirectory *origDir = gDirectory;
    //------------------------------------
    // Initialise hists and directories 
    //------------------------------------
    
    // Cutflow histograms
    std::vector<std::string> cuts = {
    		"passSkim", "passHlt", "passGoodLumi", "passMetFilter", "passExactly1Ref",
    		"passAtleast1Jet", "passJetVetoMap", "passDPhiRefJet1", "passRefBarrel",
    		"passAlpha", "passResponse"
    };
    auto h1EventInCutflow = std::make_unique<HistCutflow>(origDir, "", cuts, globalFlags_);
      
    // Variable binning
    VarBin varBin(globalFlags_);
    std::vector<int> pTRefs = {30, 110, 230};

    HistRef histRef(origDir, "passExactly1Ref", varBin);

    HistScale histScale(origDir, "passAtleast1Jet", varBin);
    
    HistBarrel histBarrel(origDir, "passRefBarrel", varBin);

    HistTag histTag(origDir, "passRefBarrel", varBin, globalFlags_);
    
    HistRef histRef2(origDir,  "passAlpha", varBin);

    HistMain histMain(origDir, "passAlpha", varBin);

    HistFinal histFinal(origDir, "passAlpha", varBin);

    HistTime histTime(origDir, "passResponse", varBin, pTRefs);

    // Scale constructor
    auto scaleElectron = std::make_shared<ScaleElectron>(scaleObject, globalFlags_.isData());
    auto scaleJetMet = std::make_shared<ScaleJetMet>(scaleObject, globalFlags_.isData(), VarCut::applyJer);

    //------------------------------------
    // Event loop
    //------------------------------------
    TLorentzVector p4RawRef, p4Ref, p4GenRef;
    TLorentzVector p4Met1, p4Metn, p4Metu, p4Metnu;
    TLorentzVector p4Jeti, p4CorrMet;
    TLorentzVector p4GenJeti, p4GenJet1, p4GenJet2;
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
        // Trigger and golden lumi, MET filter selection 
        //------------------------------------
        if (!pickEvent->passHlt(skimT)) continue; 
        h1EventInCutflow->fill("passHlt");

        bool passGoodLumi = true; 
        if (globalFlags_.isData()){
            passGoodLumi = scaleEvent->checkGoodLumi(skimT->run, skimT->luminosityBlock);
        }
        if (!passGoodLumi) continue; 
        h1EventInCutflow->fill("passGoodLumi");

        if (!pickEvent->passFilter(skimT)) continue; 
        h1EventInCutflow->fill("passMetFilter");

        //------------------------------------------
        // Select objects
        //------------------------------------------
        // Apply Electron Corrections 
        scaleElectron->applyCorrections(skimT);
        if(globalFlags_.isDebug()) scaleElectron->print();

        pickObject->clearObjects();
        pickObject->pickElectrons(*skimT);
        if (pickObject->getPickedElectrons().size() < 2) continue;
        if (pickObject->getPickedElectrons().size() > 3) continue;
        pickObject->pickRefs(*skimT);
        std::vector<TLorentzVector> p4Refs = pickObject->getPickedRefs();

        if (p4Refs.size()!=1) continue; 
        h1EventInCutflow->fill("passExactly1Ref");

        // Weight
        double weight = (globalFlags_.isMC() ? skimT->genWeight : 1.0);
        if (globalFlags_.isMC()) weight *= scaleEvent->getPuCorrection(skimT->Pileup_nTrueInt, "nominal");

        p4Ref = p4Refs.at(0);
        p4RawRef = p4Refs.at(0);
        double ptRef = p4Ref.Pt();

        // Gen objects
        p4GenRef.SetPtEtaPhiM(0, 0, 0, 0);
        if (globalFlags_.isMC()) {
            pickObject->pickGenElectrons(*skimT);
            pickObject->pickGenRefs(*skimT, p4Ref);
            std::vector<TLorentzVector> p4GenRefs = pickObject->getPickedGenRefs();
            if (p4GenRefs.empty()) continue;
            p4GenRef = p4GenRefs.at(0);
        }
        // Fill HistRef histograms
        histRef.Fill(p4Refs.size(), p4Ref, p4GenRef, weight); 
        
        // Apply jet energy scaleions
        scaleJetMet->applyCorrections(skimT, ScaleJetMet::CorrectionLevel::L2L3Res);
        if(globalFlags_.isDebug()) scaleJetMet->print();

        //------------------------------------------------
        // Select jets 
        //------------------------------------------------
        pickObject->pickJets(*skimT, p4Ref);

        //Pick index of jets
        std::vector<int> jetsIndex = pickObject->getPickedJetsIndex();
        if (jetsIndex.size()!=2) continue; //sanity 
        int iJet1 = jetsIndex.at(0);
        int iJet2 = jetsIndex.at(1);
        if (iJet1==-1) continue; //Make sure the events have first leading jet
        if (iJet2==-1) continue; //Make sure the events have second leading jet

        //Pick p4 of jets
        TLorentzVector p4Jet1, p4Jet2, p4Jetn;
        std::vector<TLorentzVector> jetsP4 = pickObject->getPickedJetsP4();
        p4Jet1  = jetsP4.at(0);
        p4Jet2  = jetsP4.at(1);
        p4Jetn  = jetsP4.at(2);
        p4Jetn+= p4Jet2; // except leading jet

        if (fabs(p4Jet1.Eta()) >= 1.3) continue; 
        h1EventInCutflow->fill("passAtleast1Jet");
        histScale.FillElectron(*scaleElectron);
        histScale.FillJetMet(*scaleJetMet);

        if (scaleEvent->checkJetVetoMap(*skimT)) continue; // expensive function
        h1EventInCutflow->fill("passJetVetoMap");
        
        //------------------------------------------------
        // Set MET vectors
        //------------------------------------------------
        p4CorrMet.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);
        // Replace PF Ref with Reco Ref
        p4CorrMet += p4RawRef - p4Ref; 

        mathHdm.calcResponse(p4CorrMet, p4Ref, p4Jet1, p4Jetn);
        double bal   = mathHdm.getBal();  
        double mpf   = mathHdm.getMpf();  
        double mpf1  = mathHdm.getMpf1(); 
        double mpfn  = mathHdm.getMpfn(); 
        double mpfu  = mathHdm.getMpfu(); 
        double mpfnu = mathHdm.getMpfnu();
       
        double deltaPhi = Helper::DELTAPHI(p4Ref.Phi(), p4Jet1.Phi());
        if (fabs(deltaPhi - TMath::Pi()) >= 0.44) continue; 
        h1EventInCutflow->fill("passDPhiRefJet1");

        if (fabs(p4Ref.Eta()) > 1.3) continue; 
        h1EventInCutflow->fill("passRefBarrel");

        //------------------------------------------------
        // GenJet
        //------------------------------------------------
        int iGenJet1 = -1;
        int iGenJet2 = -1;
        if (globalFlags_.isMC()){
            pickObject->pickGenJets(*skimT, iJet1, iJet2, p4Jet1, p4Jet2);
            std::vector<TLorentzVector> genJetsP4 = pickObject->getPickedGenJetsP4();
            p4GenJet1  = genJetsP4.at(0);
            p4GenJet2  = genJetsP4.at(1);
            std::vector<int> genJetsIndex = pickObject->getPickedGenJetsIndex();
            iGenJet1 = genJetsIndex.at(0);
            iGenJet2 = genJetsIndex.at(1);
        }
        histBarrel.Fill(p4Ref, p4Jet1, p4GenJet1, weight);
 
        //flavorXTaggedY
        histTag.SetResponse(bal, mpf, mpf1, mpfn, mpfu);
        histTag.FillHistograms(skimT.get(), ptRef, iJet1, iGenJet1, weight);

        double alpha = p4Jet2.Pt()/ptRef;
        if(alpha > 1.0) continue;
        h1EventInCutflow->fill("passAlpha");
        histRef2.Fill(p4Refs.size(), p4Ref, p4GenRef, weight); 

        histFinal.Fill(ptRef, bal, mpf, p4Jet2, p4GenJet2, iGenJet2, globalFlags_.isMC(), weight);

        histMain.Fill(skimT.get(), iJet1, bal, mpf, ptRef, weight);
       
        bool pass_DbResp = (fabs(1 - bal) < 0.7);
        bool pass_MpfResp = (fabs(1 - mpf) < 0.7); 
        if (!(pass_DbResp && pass_MpfResp)) continue;
        h1EventInCutflow->fill("passResponse");
        if(globalFlags_.isData()) histTime.Fill(skimT.get(), iJet1, bal, mpf, ptRef, weight);

    }  // end of event loop

    h1EventInCutflow->printCutflow();
    h1EventInCutflow->fillFractionCutflow();
    fout->Write();
    //Helper::scanTFile(fout);
    std::cout << "Output file: " << fout->GetName() << '\n';
    return 0;
}
   
