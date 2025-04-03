#include "RunZeeJet.h"
#include "HistCutflow.h"
#include "HistTag.h"
#include "HistTime.h"
#include "HistRef.h"
#include "HistBarrel.h"
#include "HistMain.h"
#include "HistFinal.h"
#include "HistAlpha.h"

#include "PickObjectZeeJet.h"

#include "ScaleElectron.h"
#include "ScaleJetMet.h"
#include "HistScale.h"

#include "Helper.h"
#include "VarBin.h"
#include "MathHdm.h"

#include "ReadConfig.h"

// Constructor implementation: load configuration from JSON file
RunZeeJet::RunZeeJet(GlobalFlag& globalFlags)
    : globalFlags_(globalFlags) {
    loadConfig("config/RunZeeJet.json");
}

void RunZeeJet::loadConfig(const std::string& filename) {
    ReadConfig config(filename);
    
    // Read the configuration values using getValue() for proper error handling.
    cutflows_          = config.getValue<std::vector<std::string>>({"cutflows"});
    minRefPts_         = config.getValue<std::vector<int>>({"minRefPts"});
    nElectronMin_      = config.getValue<int>({"nElectronMin"});
    nElectronMax_      = config.getValue<int>({"nElectronMax"});
    maxDeltaPhiRefJet1_= config.getValue<double>({"maxDeltaPhiRefJet1"});
    maxAlpha_          = config.getValue<double>({"maxAlpha"});
    alphaCuts_         = config.getValue<std::vector<double>>({"alphaCuts"});
    maxRefEta_         = config.getValue<double>({"maxRefEta"});
    minResp_           = config.getValue<double>({"minResp"});
    maxResp_           = config.getValue<double>({"maxResp"});
}

// Main run method updated to use configuration parameters
auto RunZeeJet::Run(std::shared_ptr<SkimTree>& skimT, PickEvent *pickEvent, ScaleEvent* scaleEvent, ScaleObject *scaleObject, TFile *fout) -> int{

    assert(fout && !fout->IsZombie());
    fout->mkdir("Base");
    fout->cd("Base");
 
    TDirectory *origDir = gDirectory;
    //------------------------------------
    // Initialise histograms using config cutflows
    //------------------------------------
    
    auto h1EventInCutflow = std::make_unique<HistCutflow>(origDir, "", cutflows_, globalFlags_);
      
    // Variable binning
    VarBin varBin(globalFlags_);
    
    // Define histos in cutflows directories 
    HistRef histRef(origDir, "passExactly1Ref", varBin);
    HistScale histScale(origDir, "passAtleast1Jet", varBin);
    HistBarrel histBarrel(origDir, "passRefBarrel", varBin);
    HistTag histTag(origDir, "passRefBarrel", varBin, globalFlags_);
    HistAlpha histAlpha(origDir, "passRefBarrel", varBin, alphaCuts_);
    HistRef histRef2(origDir,  "passAlpha", varBin);
    HistMain histMain(origDir, "passAlpha", varBin);
    HistFinal histFinal(origDir, "passAlpha", varBin);
    HistTime histTime(origDir, "passResponse", varBin, minRefPts_);
    
    auto pickObjectZeeJet = std::make_shared<PickObjectZeeJet>(globalFlags_);

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

        //------------------------------------------
        // Select objects
        //------------------------------------------
        // Apply Electron Corrections 
        scaleElectron->applyCorrections(skimT);
        if(globalFlags_.isDebug()) scaleElectron->print();

        pickObjectZeeJet->pickElectrons(*skimT);
        int nPickedElectron = pickObjectZeeJet->getPickedElectrons().size();
        if (nPickedElectron < nElectronMin_) continue;
        if (nPickedElectron > nElectronMax_) continue;

        pickObjectZeeJet->pickRefs(*skimT);
        std::vector<TLorentzVector> p4Refs = pickObjectZeeJet->getPickedRefs();
        if (p4Refs.size() != 1) continue; 
        h1EventInCutflow->fill("passExactly1Ref");

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

        p4Ref = p4Refs.at(0);
        p4RawRef = p4Refs.at(0);
        double ptRef = p4Ref.Pt();

        // Gen objects
        p4GenRef.SetPtEtaPhiM(0, 0, 0, 0);
        if (globalFlags_.isMC()) {
            pickObjectZeeJet->pickGenElectrons(*skimT);
            pickObjectZeeJet->pickGenRefs(*skimT, p4Ref);
            std::vector<TLorentzVector> p4GenRefs = pickObjectZeeJet->getPickedGenRefs();
            if (p4GenRefs.empty()) continue;
            p4GenRef = p4GenRefs.at(0);
        }
        // Fill HistRef histograms
        histRef.Fill(p4Refs.size(), p4Ref, p4GenRef, weight); 
        
        // Apply jet energy scale corrections
        scaleJetMet->applyCorrections(skimT, ScaleJetMet::CorrectionLevel::L2L3Res);
        if(globalFlags_.isDebug()) scaleJetMet->print();

        //------------------------------------------------
        // Select jets 
        //------------------------------------------------
        pickObjectZeeJet->pickJets(*skimT, p4Ref);

        // Pick index of jets
        std::vector<int> jetsIndex = pickObjectZeeJet->getPickedJetsIndex();
        int iJet1 = jetsIndex.at(0);
        int iJet2 = jetsIndex.at(1);
        if (iJet1 == -1) continue; // make sure the events have a valid leading jet
        h1EventInCutflow->fill("passAtleast1Jet");

        // Pick p4 of jets
        TLorentzVector p4Jet1, p4Jet2, p4Jetn;
        std::vector<TLorentzVector> jetsP4 = pickObjectZeeJet->getPickedJetsP4();
        p4Jet1 = jetsP4.at(0);
        p4Jet2 = jetsP4.at(1);
        p4Jetn = jetsP4.at(2);
        p4Jetn += p4Jet2; // except leading jet

        histScale.FillElectron(*scaleElectron);
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
            pickObjectZeeJet->pickGenJets(*skimT, iJet1, iJet2, p4Jet1, p4Jet2);
            std::vector<TLorentzVector> genJetsP4 = pickObjectZeeJet->getPickedGenJetsP4();
            p4GenJet1 = genJetsP4.at(0);
            p4GenJet2 = genJetsP4.at(1);
            std::vector<int> genJetsIndex = pickObjectZeeJet->getPickedGenJetsIndex();
            iGenJet1 = genJetsIndex.at(0);
            iGenJet2 = genJetsIndex.at(1);
        }
        histBarrel.Fill(p4Ref, p4Jet1, p4GenJet1, weight);
 
        // Flavor-tag histograms
        histTag.SetResponse(bal, mpf, mpf1, mpfn, mpfu);
        histTag.FillHistograms(skimT.get(), ptRef, iJet1, iGenJet1, weight);

        double alpha = p4Jet2.Pt() / ptRef;
        histAlpha.Fill(alpha, ptRef, skimT->Rho, bal, mpf, weight);

        // Use maxAlpha_ from configuration
        if (alpha > maxAlpha_) continue;
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
    std::cout << "Output file: " << fout->GetName() << '\n';
    return 0;
}

