#include "RunGamJetFake.h"

#include "HistCutflow.h"
#include "HistObjectVar.h"
#include "HistTag.h"
#include "HistTime.h"
#include "HistRef.h"
#include "HistBarrel.h"
#include "HistMain.h"
#include "HistFinal.h"

#include "ScalePhoton.h"
#include "ScaleJetMet.h"
#include "HistScale.h"

#include "Helper.h"
#include "VarBin.h"
#include "VarCut.h"
#include "MathHdm.h"
   
// Constructor implementation
RunGamJetFake::RunGamJetFake(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}

auto RunGamJetFake::Run(std::shared_ptr<SkimTree>& skimT, PickEvent *pickEvent, PickObject *pickObject, ScaleEvent* scaleEvent,  ScaleObject *scaleObject, TFile *fout) -> int{
   
    assert(fout && !fout->IsZombie());
    fout->mkdir("Base");
    fout->cd("Base");
 
    TDirectory *origDir = gDirectory;
    //------------------------------------
    // Initialise hists and directories 
    //------------------------------------
    
    // Cutflow histograms
    std::vector<std::string> cuts = {
    		"passSkim", "passAtleast3Jet",  
            "passExactly1Ref", "passJetVetoMap", "passDPhiRefJet1",
            "passRefBarrel", "passAlpha", "passResponse"
    };
    auto h1EventInCutflow = std::make_unique<HistCutflow>(origDir, "", cuts, globalFlags_);
      
    // Variable binning
    VarBin varBin(globalFlags_);
    std::vector<int> pTRefs = {30, 110, 230};

    HistObjectVar histObjectVar(origDir, "passSkim", varBin);

    HistRef histRef(origDir, "passExactly1Ref", varBin);

    HistScale histScale(origDir, "passAtleast1Jet", varBin);
    
    HistBarrel histBarrel(origDir, "passRefBarrel", varBin);

    HistTag histTag(origDir, "passRefBarrel", varBin, globalFlags_);
    
    HistRef histRef2(origDir,  "passAlpha", varBin);

    HistMain histMain(origDir, "passAlpha", varBin);

    HistFinal histFinal(origDir, "passAlpha", varBin);
    HistTime histTime(origDir, "passResponse", varBin, pTRefs);

    // Scale constructor
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
        //if (ientry < 0) break; 
        skimT->getChain()->GetTree()->GetEntry(ientry);
        h1EventInCutflow->fill("passSkim");

        // Weight
        double weight = 1.0;
        histObjectVar.FillJet(*skimT, weight, 0);

        //------------------------------------------------
        // Apply jet energy scale
        //------------------------------------------------
        scaleJetMet->applyCorrections(skimT, ScaleJetMet::CorrectionLevel::L2L3Res);
        if(globalFlags_.isDebug()) scaleJetMet->print();
        histScale.FillJetMet(*scaleJetMet);

        //------------------------------------------------
        // Select jets 
        //------------------------------------------------
        pickObject->clearObjects();
        pickObject->pickJetsForFakeGamma(*skimT);

        //Pick index of jets
        std::vector<int> jetsIndex = pickObject->getPickedJetsIndex();
        if (jetsIndex.size()!=3) continue; //sanity 
        int iJ1 = jetsIndex.at(0);
        int iJ2 = jetsIndex.at(1);
        int iJ3 = jetsIndex.at(2);
        if (iJ1==-1) continue; //Make sure the events have 2 leading jet
        if (iJ2==-1) continue; 
        if (iJ3==-1) continue; 
        h1EventInCutflow->fill("passAtleast3Jet");

        //Pick p4 of jets
        TLorentzVector p4J1, p4J2, p4J3, p4Jn;
        std::vector<TLorentzVector> jetsP4 = pickObject->getPickedJetsP4();
        p4J1  = jetsP4.at(0);
        p4J2  = jetsP4.at(1);
        p4J3  = jetsP4.at(2);
        p4Jn  = jetsP4.at(3);

        TLorentzVector p4Jet1, p4Jet2, p4Jet3, p4Jetn;
        TLorentzVector p4JetForFakeGamma; 
        int iJetForFakeGamma = -1;
        int iJet1 = -1;
        int iJet2 = -1;
        //Randomly assign one of the leading jets to photon
        if(jentry%2==0){
            iJetForFakeGamma = iJ1;
            p4JetForFakeGamma = p4Jet1;
            iJet1 = iJ2;// since the first jet is set for gamma
            iJet2 = iJ3;
            p4Jet1 = p4J2;
            p4Jet2 = p4J3;
        }
        else{
            iJetForFakeGamma = iJ2;
            p4JetForFakeGamma = p4J2;
            iJet1 = iJ1;
            p4Jet1 = p4J1;
            iJet2 = iJ3;
            p4Jet2 = p4J3;
        }
        p4Jetn+= p4Jn; 
        
        //------------------------------------------
        // Select objects
        //------------------------------------------
        pickObject->pickRefForFakeGamma(*skimT, iJetForFakeGamma);
        std::vector<TLorentzVector> p4Refs = pickObject->getPickedRefs();

        if (p4Refs.size()!=1) continue; 
        h1EventInCutflow->fill("passExactly1Ref");

        p4Ref = p4Refs.at(0);
        p4RawRef = p4Refs.at(0);
        double ptRef = p4Ref.Pt();

        double lumiPerHlt = 1.0;
        if (globalFlags_.isMC()){
            weight *= skimT->genWeight;
            weight *= scaleEvent->getPuCorrection(skimT->Pileup_nTrueInt, "nominal");
        }

        // Gen objects
        p4GenRef.SetPtEtaPhiM(0, 0, 0, 0);

        // Fill HistRef histograms
        histRef.Fill(p4Refs.size(), p4Ref, p4GenRef, weight); 
        

        if (scaleEvent->checkJetVetoMap(*skimT)) continue; // expensive function
        h1EventInCutflow->fill("passJetVetoMap");
        
        //------------------------------------------------
        // Set MET vectors
        //------------------------------------------------
        p4Jetn+= p4Jet2; // except leading
        p4CorrMet.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);//CRITICAL
        p4CorrMet += p4JetForFakeGamma; 
        //https://github.com/blehtela/gamjet-analysis/blob/919921427e6a1ec96df677bd774f0f819a19d108/GamHistosFill.C#L2582
        // QCD_CP5 has about 2.5 GeV/A of UE offset at FullSim level
        double area = skimT->Jet_area[iJetForFakeGamma];
    	p4JetForFakeGamma *= (p4JetForFakeGamma.Pt()>0 ? 1 - 2.5*area/p4JetForFakeGamma.Pt() : 1.);

        // Replace PF Ref with Reco Ref
        p4CorrMet -= p4JetForFakeGamma;//CHECK
        //p4CorrMet += p4JetForFakeGamma - p4Ref;//p4JetForFakeGamma=rawjet-PU, p4Ref=genjet 

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

        //double alpha = p4Jet2.Pt()/ptRef;
        double pt2Min = 30;
        double ptJet2 = p4Jet2.Pt();
        bool passAlpha = (ptJet2 < ptRef || ptJet2 < pt2Min); 
        if(!passAlpha) continue;
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
   
