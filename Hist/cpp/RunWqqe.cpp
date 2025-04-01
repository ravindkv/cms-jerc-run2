#include "RunWqqe.h"

#include "HistCutflow.h"
#include "HistHadW.h"
#include "HistTTbar.h"

#include "ScaleElectron.h"
#include "ScaleJetMet.h"

#include "Helper.h"
#include "VarBin.h"
#include "VarCut.h"
#include "MathHadW.h"
#include "MathTTbar.h"
   
// Constructor implementation
RunWqqe::RunWqqe(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}

auto RunWqqe::Run(std::shared_ptr<SkimTree>& skimT, PickEvent *pickEvent, PickObject *pickObject, ScaleEvent* scaleEvent, ScaleObject *scaleObject, TFile *fout) -> int{
   
    assert(fout && !fout->IsZombie());
    fout->mkdir("Base");
    fout->cd("Base");
 
    TDirectory *origDir = gDirectory;
    //------------------------------------
    // Initialise hists and directories 
    //------------------------------------
    
    // Cutflow histograms
    std::vector<std::string> cuts = {
    		"passSkim", "passHlt", "passGoodLumi", "passExactly1Lep", 
            "passAtleast4Jet", "passExactly2bJet", "passJetVetoMap" 
    };
    auto h1EventInCutflow = std::make_unique<HistCutflow>(origDir, "", cuts, globalFlags_);
      
    // Variable binning
    VarBin varBin(globalFlags_);

    VarCut varCut(globalFlags_);
    const auto& jetCuts = varCut.getJetCuts();
    const auto& bJetCuts = varCut.getBJetCuts();
    const auto& leptonCuts = varCut.getLeptonCuts();

    HistHadW histHadW(origDir, "passExactly2bJet", varBin);
    HistTTbar histTTbar(origDir, "passExactly2bJet", varBin);

    //------------------------------------
    //Resolutions: FIXME
    //------------------------------------
    double resLep  = 0.0;
    double resMet  = 0.0;
    double resHadW = 24.0;
    double resHadT = 34.0;
    double resLepT = 30.0;
    bool useReso   = false;

    MathHadW mathHadW(globalFlags_, bJetCuts.minDisc, VarCut::MassW, resHadW);
    MathTTbar mathTTbar(globalFlags_, bJetCuts.minDisc);
    mathTTbar.setMass(leptonCuts.mass, VarCut::MassW, VarCut::MassT);

    // Scale constructor
    auto scaleElectron = std::make_shared<ScaleElectron>(scaleObject, globalFlags_.isData());
    auto scaleJetMet = std::make_shared<ScaleJetMet>(scaleObject, globalFlags_.isData(), VarCut::applyJer);

    //------------------------------------
    // Event loop
    //------------------------------------
    TLorentzVector p4RawRef, p4Ref, p4GenRef;
    TLorentzVector p4Jeti, p4CorrMet;

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
        //scaleElectron->applyCorrections(skimT);
        if(globalFlags_.isDebug()) scaleElectron->print();

        pickObject->clearObjects();
        pickObject->pickElectrons(*skimT);
        std::vector<int> pickedElectrons = pickObject->getPickedElectrons();
        if (pickedElectrons.size() !=1) continue;
        TLorentzVector p4Lep;
        int eleInd = pickedElectrons[0];
        p4Lep.SetPtEtaPhiM(skimT->Electron_pt[eleInd], skimT->Electron_eta[eleInd],
                            skimT->Electron_phi[eleInd],skimT->Electron_mass[eleInd]);
        h1EventInCutflow->fill("passExactly1Lep");

        // Weight
        double weight = (globalFlags_.isMC() ? skimT->genWeight : 1.0);
        if (globalFlags_.isMC()) weight *= scaleEvent->getPuCorrection(skimT->Pileup_nTrueInt, "nominal");

        p4CorrMet.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);

        // Apply jet energy scaleions
        scaleJetMet->applyCorrections(skimT, ScaleJetMet::CorrectionLevel::L2L3Res);
        if(globalFlags_.isDebug()) scaleJetMet->print();

        std::vector<int> indexJets; 
        std::vector<int> indexBjets;
        for (int i = 0; i < skimT->nJet; ++i) {
            float pt = skimT->Jet_pt[i];
            if (pt < jetCuts.minPt) continue; 
            if (std::abs(skimT->Jet_eta[i]) >= jetCuts.maxEta) continue; 
            if (skimT->Jet_jetId[i] < jetCuts.minId) continue;
            indexJets.push_back(i);
            if(skimT->Jet_btagDeepFlavB[i] > bJetCuts.minDisc){
                indexBjets.push_back(i);
            }
        }
        if(indexJets.size() < jetCuts.nJet) continue;
        h1EventInCutflow->fill("passAtleast4Jet");

        if(indexBjets.size() != bJetCuts.nBJet) continue;
        h1EventInCutflow->fill("passExactly2bJet");

        h1EventInCutflow->fill("passJetVetoMap");
        
        //------------------------
        // W->qq
        //------------------------
        TLorentzVector p4HadW = mathHadW.getP4HadW(*skimT, indexJets);
        int index1ForW = mathHadW.getIndex1ForW();
        int index2ForW = mathHadW.getIndex2ForW();
        double chiSqrW  = mathHadW.getChiSqr();
        if(index1ForW != -1 && index2ForW !=-1 
            && std::abs(skimT->Jet_eta[index1ForW] < jetCuts.maxEtaLight)
            && std::abs(skimT->Jet_eta[index2ForW] < jetCuts.maxEtaLight)
            && chiSqrW < VarCut::MinChiSqr){
            double avgHadPt = 0.5*(skimT->Jet_pt[index1ForW] + skimT->Jet_pt[index2ForW]);
            histHadW.Fill(p4HadW, avgHadPt, chiSqrW, skimT->run, weight); 
        }

        //------------------------
        // TTbar
        //------------------------
        mathTTbar.setEventObjects(p4Lep, p4CorrMet, indexJets);
        mathTTbar.setResolution(resLep, resMet, resHadW, resHadT, resLepT, {}, useReso);
        mathTTbar.minimizeChiSqr(*skimT);
        index1ForW = mathTTbar.getIndex1ForW();
        index2ForW = mathTTbar.getIndex2ForW();
        double chiSqrTT  = mathTTbar.getChiSqr();
        if(index1ForW != -1 && index2ForW !=-1 
            && std::abs(skimT->Jet_eta[index1ForW] < jetCuts.maxEtaLight)
            && std::abs(skimT->Jet_eta[index2ForW] < jetCuts.maxEtaLight)
            && chiSqrW < VarCut::MinChiSqr){
            double avgHadPt = 0.5*(skimT->Jet_pt[index1ForW] + skimT->Jet_pt[index2ForW]);
            TLorentzVector p4HadW = mathTTbar.getP4HadW();
            TLorentzVector p4HadT = mathTTbar.getP4HadT();
            TLorentzVector p4LepT = mathTTbar.getP4LepT();
            histTTbar.Fill(p4HadW, p4HadT, p4LepT, avgHadPt, chiSqrTT, skimT->run, weight); 
        }

    }  // end of event loop

    h1EventInCutflow->printCutflow();
    h1EventInCutflow->fillFractionCutflow();
    fout->Write();
    //Helper::scanTFile(fout);
    std::cout << "Output file: " << fout->GetName() << '\n';
    return 0;
}
   
