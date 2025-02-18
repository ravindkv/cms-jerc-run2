#include "RunWqqe.h"
#include "HistCutflow.h"
#include "Helper.h"
   
// Constructor implementation
RunWqqe::RunWqqe(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}


auto RunWqqe::Run(std::shared_ptr<NanoTree>& nanoT, TFile *fout) -> int{
    fout->cd();

	//----------------------------------
	// Add channel specific branches 
	//----------------------------------
    nanoT->fChain->SetBranchStatus("nElectron", true);
    nanoT->fChain->SetBranchStatus("Electron_charge", true);
    nanoT->fChain->SetBranchStatus("Electron_pt", true);
    nanoT->fChain->SetBranchStatus("Electron_deltaEtaSC", true);
    nanoT->fChain->SetBranchStatus("Electron_eta", true);
    nanoT->fChain->SetBranchStatus("Electron_phi", true);
    nanoT->fChain->SetBranchStatus("Electron_mass", true);
    nanoT->fChain->SetBranchStatus("Electron_cutBased", true);
    nanoT->fChain->SetBranchStatus("Electron_seedGain", true);
    nanoT->fChain->SetBranchStatus("Electron_eCorr", true);
    nanoT->fChain->SetBranchStatus("Electron_dE*", true);
    nanoT->fChain->SetBranchStatus("Electron_pfRelIso03_all", true);
    nanoT->fChain->SetBranchStatus("Electron_sieie", true);
    nanoT->fChain->SetBranchStatus("Electron_photonIdx", true);
    nanoT->fChain->SetBranchStatus("Electron_mvaFall17V2Iso_WP*", true);
  	if(globalFlags_.isMC){
      nanoT->fChain->SetBranchStatus("GenDressedLepton_*",true);
      nanoT->fChain->SetBranchStatus("nGenDressedLepton",true);
    }

	//----------------------------------
	// Set trigger list
	//----------------------------------
    if(globalFlags_.is2016Pre || globalFlags_.is2016Post){
        trigList_ = {"HLT_Ele27_WPTight_Gsf", "HLT_Photon175", "HLT_Ele115_CaloIdVT_GsfTrkIdT"};
    }
    if(globalFlags_.is2017){
        trigList_ = {"HLT_Ele35_WPTight_Gsf", "HLT_Photon200", "HLT_Ele115_CaloIdVT_GsfTrkIdT"};
    }
    if(globalFlags_.is2018){
        trigList_ = {"HLT_Ele32_WPTight_Gsf", "HLT_Photon200", "HLT_Ele115_CaloIdVT_GsfTrkIdT"};
    }

    if (trigList_.empty()) {
        std::cerr << "No triggers found for channel: ZeeJet" << '\n';
        exit(EXIT_FAILURE);
    }
	for (const auto& trigN : trigList_) {
		nanoT->fChain->SetBranchStatus(trigN.c_str(), true);
	    nanoT->fChain->SetBranchAddress(trigN.c_str(), &trigVals_[trigN], &trigTBranches_[trigN]);
	} 

    TTree* newTree = nanoT->fChain->GetTree()->CloneTree(0);
    newTree->SetCacheSize(Helper::tTreeCatchSize);
    Long64_t nentries = nanoT->getEntries();
    std::cout << "\nSample has "<<nentries << " entries" << '\n';


    //------------------------------------
    // Cutflow histograms
    //------------------------------------
    std::vector<std::string> cuts = { "NanoAOD", "Trigger"};
    auto h1EventInCutflow = std::make_unique<HistCutflow>("h1EventInCutflow", cuts, fout->mkdir("Cutflow"));
    
    
    //--------------------------------
    //Event loop
    //--------------------------------
    double totalTime = 0.0;
	auto startClock = std::chrono::high_resolution_clock::now();
    Helper::initProgress();
	for(Long64_t i= 0; i < nentries; i++){
        //if(i>10000) break;
        Helper::printProgress(i, nentries, startClock, totalTime);
        
        Long64_t entry = nanoT->loadEntry(i);
        h1EventInCutflow->fill("NanoAOD");

		for (const auto& trigN : trigList_) {
            if (!trigTBranches_[trigN]) continue;
		    trigTBranches_[trigN]->GetEntry(entry);//Read only content of HLT branches from NanoTree
		    if (trigVals_[trigN]) {
            	nanoT->fChain->GetTree()->GetEntry(entry);// Then read content of ALL branches
                h1EventInCutflow->fill("Trigger");
            	newTree->Fill();
		        break; // Event passes if any trigger is true
		    }
		}
    }
    Helper::printCutflow(h1EventInCutflow->getHistogram());
    std::cout<<"nEvents_Skim = "<<newTree->GetEntries()<<'\n';
    std::cout << "Output file path = "<<fout->GetName()<<'\n';
    //newTree->Write();
    fout->Write();
    return EXIT_SUCCESS;
}
