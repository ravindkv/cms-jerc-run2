#include "RunZmmJet.h"
#include "HistCutflow.h"
#include "Helper.h"
   
// Constructor implementation
RunZmmJet::RunZmmJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}


auto RunZmmJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile *fout) -> int{
   fout->cd();

	//----------------------------------
	// Add channel specific branches 
	//----------------------------------
    nanoT->fChain->SetBranchStatus("Muon_charge", true);
    nanoT->fChain->SetBranchStatus("Muon_pt", true);
    nanoT->fChain->SetBranchStatus("Muon_eta", true);
    nanoT->fChain->SetBranchStatus("Muon_phi", true);
    nanoT->fChain->SetBranchStatus("Muon_mass", true);
    nanoT->fChain->SetBranchStatus("Muon_mediumId", true);
    nanoT->fChain->SetBranchStatus("Muon_tightId", true);
    nanoT->fChain->SetBranchStatus("Muon_highPurity", true);
    nanoT->fChain->SetBranchStatus("Muon_nTrackerLayers", true);
    nanoT->fChain->SetBranchStatus("Muon_pfRelIso04_all", true);
    nanoT->fChain->SetBranchStatus("Muon_tkRelIso", true); 
    nanoT->fChain->SetBranchStatus("Muon_dxy", true); 
    nanoT->fChain->SetBranchStatus("Muon_dz", true); 
    nanoT->fChain->SetBranchStatus("nMuon",true);
  	if (globalFlags_.isMC){
      nanoT->fChain->SetBranchStatus("GenDressedLepton_*",true);
      nanoT->fChain->SetBranchStatus("nGenDressedLepton",true);
    }

	//----------------------------------
	// Set trigger list
	//----------------------------------
    if(globalFlags_.is2016Pre || globalFlags_.is2016Post){
        trigList_ = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"};
    }
    if(globalFlags_.is2017 || globalFlags_.is2018){
        trigList_ = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8"};
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
    std::cout << "Sample has "<<nentries << " entries" << '\n';

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
        //if(i>100000) break;
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
    fout->Write();
    return EXIT_SUCCESS; 
}
