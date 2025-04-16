#include "RunGamJet.h"

#include "HistCutflow.h"
#include "HistObjectVar.h"
#include "HistTag.h"
#include "HistTime.h"
#include "HistRef.h"
#include "HistBarrel.h"
#include "HistMain.h"
#include "HistFinal.h"
#include "HistAlpha.h"

#include "PickObjectGamJet.h"

#include "ScalePhoton.h"
#include "ScaleJetMet.h"
#include "HistScale.h"

#include "Helper.h"
#include "VarBin.h"
#include "MathHdm.h"
   
#include "ReadConfig.h"

// Constructor implementation
RunGamJet::RunGamJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
    loadConfig("config/RunGamJet.json");
}

void RunGamJet::loadConfig(const std::string& filename) {
    ReadConfig config(filename);
    
    // Read the configuration values using getValue() for proper error handling.
    cutflows_          = config.getValue<std::vector<std::string>>({"cutflows"});
    minRefPts_         = config.getValue<std::vector<int>>({"minRefPts"});
    nPhotonMin_        = config.getValue<int>({"nPhotonMin"});
    nPhotonMax_        = config.getValue<int>({"nPhotonMax"});
    maxDeltaPhiRefJet1_= config.getValue<double>({"maxDeltaPhiRefJet1"});
    maxAlpha_          = config.getValue<double>({"maxAlpha"});
    minPtJet2InAlpha_  = config.getValue<double>({"minPtJet2InAlpha"});
    alphaCuts_         = config.getValue<std::vector<double>>({"alphaCuts"});
    maxRefEta_         = config.getValue<double>({"maxRefEta"});
    minResp_           = config.getValue<double>({"minResp"});
    maxResp_           = config.getValue<double>({"maxResp"});
}

auto RunGamJet::Run(std::shared_ptr<SkimTree>& skimT, PickEvent *pickEvent, ScaleEvent* scaleEvent,  ScaleObject *scaleObject, TFile *fout) -> int{
   
    assert(fout && !fout->IsZombie());
    fout->mkdir("Base");
    fout->cd("Base");
 
    TDirectory *origDir = gDirectory;
    //------------------------------------
    // Initialise hists and directories 
    //------------------------------------
    
    auto h1EventInCutflow = std::make_unique<HistCutflow>(origDir, "", cutflows_, globalFlags_);
      
    // Variable binning
    VarBin varBin(globalFlags_);
    HistObjectVar histObjectVar(origDir, "passSkim", varBin);
    HistRef histRef(origDir, "passExactly1Ref", varBin);
    HistScale histScale(origDir, "passAtleast2Jet", varBin);
    HistBarrel histBarrel(origDir, "passRefBarrel", varBin);
    HistTag histTag(origDir, "passRefBarrel", varBin, globalFlags_);
    HistAlpha histAlpha(origDir, "passRefBarrel", varBin, alphaCuts_);
    HistRef histRef2(origDir,  "passAlpha", varBin);
    HistMain histMain(origDir, "passAlpha", varBin);
    HistFinal histFinal(origDir, "passAlpha", varBin);
    HistTime histTime(origDir, "passResponse", varBin, minRefPts_);

    auto pickObjectGamJet = std::make_shared<PickObjectGamJet>(globalFlags_);

    // Scale constructor
    auto scalePhoton = std::make_shared<ScalePhoton>(scaleObject, globalFlags_.isData());
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
        // Trigger and golden lumi
        //------------------------------------
        if (!pickEvent->passHlt(skimT)) continue; 
        h1EventInCutflow->fill("passHlt");
        
        bool passGoodLumi = true; 
        if (globalFlags_.isData()){
            passGoodLumi = scaleEvent->checkGoodLumi(skimT->run, skimT->luminosityBlock);
        }
        if (!passGoodLumi) continue; 
        h1EventInCutflow->fill("passGoodLumi");

        // Weight
        Double_t weight = 1.0;
        if (globalFlags_.isMC()){
            double genWeight    = skimT->genWeight; 
            weight *= genWeight/scaleEvent->getNormGenEventSumw();
            weight *= scaleEvent->getLumiWeight();
            weight *= scaleEvent->getPuCorrection(skimT->Pileup_nTrueInt, "nominal");
            if(globalFlags_.isDebug()){
                std::cout << "genWeight     = " << genWeight << std::endl;
                std::cout << "weight        = " << weight << std::endl;
            }
        }
        histObjectVar.FillPhoton(*skimT, weight);
        histObjectVar.FillJet(*skimT, weight, 0);

        //------------------------------------------
        // Select objects
        //------------------------------------------
        // Apply Photon Corrections 
        scalePhoton->applyCorrections(skimT);
        if(globalFlags_.isDebug()) scalePhoton->print();

        pickObjectGamJet->pickPhotons(*skimT);
        int nPickedPhoton = pickObjectGamJet->getPickedPhotons().size();
        if (nPickedPhoton < nPhotonMin_) continue;
        if (nPickedPhoton > nPhotonMax_) continue;

        pickObjectGamJet->pickRefs(*skimT);
        std::vector<TLorentzVector> p4Refs = pickObjectGamJet->getPickedRefs();
        if (p4Refs.size()!=1) continue; 
        h1EventInCutflow->fill("passExactly1Ref");
        p4Ref = p4Refs.at(0);
        p4RawRef = p4Refs.at(0);
        double ptRef = p4Ref.Pt();
        if (!pickEvent->passHltWithPt(skimT, ptRef)) continue; 
        h1EventInCutflow->fill("passHltWithPt");

        double weightLumiPerHlt = 1.0;
        if (globalFlags_.isData()){
            double lumiPerHlt = 1.0;
            lumiPerHlt = scaleEvent->cacheHltLumiPerRun(pickEvent->getPassedHlt(), skimT->run);
            lumiPerHlt *= 10e-9; //convert ub-1 to fb-1
            //Scale each HLT to the lumi of full Era
            if(lumiPerHlt>0.0) weightLumiPerHlt *= (scaleEvent->getLumiPerEra()/lumiPerHlt);
        }
        //weight *= weightLumiPerHlt;
        //std::cout<<pickEvent->getPassedHlt()<<" = "<<weightLumiPerHlt<<'\n';

        // Gen objects
        p4GenRef.SetPtEtaPhiM(0, 0, 0, 0);
        if (globalFlags_.isMC()) {
            pickObjectGamJet->pickGenPhotons(*skimT);
            pickObjectGamJet->pickGenRefs(*skimT, p4Ref);
            std::vector<TLorentzVector> p4GenRefs = pickObjectGamJet->getPickedGenRefs();
            if (p4GenRefs.empty()) continue;
            p4GenRef = p4GenRefs.at(0);
        }
        // Fill HistRef histograms
        histRef.Fill(p4Refs.size(), p4Ref, p4GenRef, weight); 
        
        // Apply jet energy scale
        scaleJetMet->applyCorrections(skimT, ScaleJetMet::CorrectionLevel::L2L3Res);
        if(globalFlags_.isDebug()) scaleJetMet->print();

        //------------------------------------------------
        // Select jets 
        //------------------------------------------------
        pickObjectGamJet->pickJets(*skimT, p4Ref);

        //Pick index of jets
        std::vector<int> jetsIndex = pickObjectGamJet->getPickedJetsIndex();
        int iJet1 = jetsIndex.at(0);
        int iJet2 = jetsIndex.at(1);
        if (iJet1==-1) continue; 
        h1EventInCutflow->fill("passAtleast1Jet");

        //Pick p4 of jets
        TLorentzVector p4Jet1, p4Jet2, p4Jetn;
        std::vector<TLorentzVector> jetsP4 = pickObjectGamJet->getPickedJetsP4();
        p4Jet1  = jetsP4.at(0);
        p4Jet2  = jetsP4.at(1);
        p4Jetn  = jetsP4.at(2);
        p4Jetn+= p4Jet2; // except leading jet

        histScale.FillPhoton(*scalePhoton);
        histScale.FillJetMet(*scaleJetMet);

        if (scaleEvent->checkJetVetoMapOnJet1(p4Jet1)) continue; 
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
        // Use maxDeltaPhiRefJet1_ from configuration
        if (fabs(deltaPhi - TMath::Pi()) >= maxDeltaPhiRefJet1_) continue; 
        h1EventInCutflow->fill("passDPhiRefJet1");

        if (fabs(p4Ref.Eta()) > maxRefEta_) continue; 
        h1EventInCutflow->fill("passRefBarrel");

        //------------------------------------------------
        // GenJet
        //------------------------------------------------
        int iGenJet1 = -1;
        int iGenJet2 = -1;
        if (globalFlags_.isMC()){
            pickObjectGamJet->pickGenJets(*skimT, iJet1, iJet2, p4Jet1, p4Jet2);
            std::vector<TLorentzVector> genJetsP4 = pickObjectGamJet->getPickedGenJetsP4();
            p4GenJet1  = genJetsP4.at(0);
            p4GenJet2  = genJetsP4.at(1);
            std::vector<int> genJetsIndex = pickObjectGamJet->getPickedGenJetsIndex();
            iGenJet1 = genJetsIndex.at(0);
            iGenJet2 = genJetsIndex.at(1);
        }
        histBarrel.Fill(p4Ref, p4Jet1, p4GenJet1, weight);
 
        //flavorXTaggedY
        histTag.SetResponse(bal, mpf, mpf1, mpfn, mpfu);
        histTag.FillHistograms(skimT.get(), ptRef, iJet1, iGenJet1, weight);

        //double alpha = p4Jet2.Pt()/ptRef;
        double ptJet2 = p4Jet2.Pt();
        double alpha = ptJet2/ptRef;
        bool passAlpha = ( alpha  < maxAlpha_ || ptJet2 < minPtJet2InAlpha_); 
        histAlpha.Fill(alpha, ptRef, skimT->Rho, bal, mpf, weight);
        if(!passAlpha) continue;
        h1EventInCutflow->fill("passAlpha");

        histRef2.Fill(p4Refs.size(), p4Ref, p4GenRef, weight); 

        histMain.Fill(skimT.get(), iJet1, bal, mpf, ptRef, weight);
        histMain.FillOtherResp(mpf1, mpfn, mpfu, mpfnu, ptRef, weight);
       
        bool passDbResp  = bal > minResp_ && bal < maxResp_;
        bool passMpfResp = mpf > minResp_ && mpf < maxResp_;

        histFinal.FillCommon(ptRef, bal, mpf, passDbResp, passMpfResp, weight);
        if(globalFlags_.isMC())histFinal.FillGen(ptRef, p4Jet2, p4GenJet2, weight);

        if (!(passDbResp && passMpfResp)) continue;
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
   
