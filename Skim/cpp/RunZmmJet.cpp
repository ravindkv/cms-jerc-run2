#include "RunZmmJet.h"
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
    nanoT->fChain->SetBranchStatus("Muon_charge", 1);
    nanoT->fChain->SetBranchStatus("Muon_pt", 1);
    nanoT->fChain->SetBranchStatus("Muon_eta", 1);
    nanoT->fChain->SetBranchStatus("Muon_phi", 1);
    nanoT->fChain->SetBranchStatus("Muon_mass", 1);
    nanoT->fChain->SetBranchStatus("Muon_mediumId", 1);
    nanoT->fChain->SetBranchStatus("Muon_tightId", 1);
    nanoT->fChain->SetBranchStatus("Muon_highPurity", 1);
    nanoT->fChain->SetBranchStatus("Muon_nTrackerLayers", 1);
    nanoT->fChain->SetBranchStatus("Muon_pfRelIso04_all", 1);
    nanoT->fChain->SetBranchStatus("Muon_tkRelIso", 1); 
    nanoT->fChain->SetBranchStatus("Muon_dxy", 1); 
    nanoT->fChain->SetBranchStatus("Muon_dz", 1); 
    nanoT->fChain->SetBranchStatus("nMuon",1);
  	if (globalFlags_.isMC){
      nanoT->fChain->SetBranchStatus("GenDressedLepton_*",1);
      nanoT->fChain->SetBranchStatus("nGenDressedLepton",1);
    }

	//----------------------------------
	// Set trigger list
	//----------------------------------
    std::vector<std::string> patterns;
    if(globalFlags_.is2016Pre || globalFlags_.is2016Post){
        patterns = { "HLT_Mu17*_Mu8*_DZ"};
    }
    if(globalFlags_.is2017 || globalFlags_.is2018){
        patterns = { "HLT_Mu17*_Mu8*_Mass8"};
    }
    trigList_  = Helper::GetMatchingBranchNames(nanoT->fChain->GetTree(), patterns);

    if (trigList_.empty()) {
        std::cerr << "No triggers found for channel: ZeeJet" << std::endl;
        exit(EXIT_FAILURE);
    }
	for (const auto& trigN : trigList_) {
		nanoT->fChain->SetBranchStatus(trigN.c_str(), 1);
	    nanoT->fChain->SetBranchAddress(trigN.c_str(), &trigVals_[trigN], &trigTBranches_[trigN]);
	} 
    TTree* newTree = nanoT->fChain->GetTree()->CloneTree(0);
    newTree->SetCacheSize(50*1024*1024);
    Long64_t nentries = nanoT->GetEntries();
    std::cout << "Sample has "<<nentries << " entries" << std::endl;

    //------------------------------------
    // Cutflow histograms
    //------------------------------------
    std::vector<std::string> cuts = { "NanoAOD", "Trigger"};
    
    // Create the histogram with the number of bins equal to the number of cuts
    auto h1EventInCutflow = std::make_unique<TH1D>("h1EventInCutflow", ";Cutflow;Events", cuts.size(), 0.5, cuts.size() + 0.5);
    
    // Map each cut name to a bin number (1-based bin number for ROOT)
    std::map<std::string, int> cutToBinMap;
    for (size_t i = 0; i < cuts.size(); ++i) {
    		cutToBinMap[cuts[i]] = i + 1;  // Bin numbers are 1-based in ROOT
    		h1EventInCutflow->GetXaxis()->SetBinLabel(i + 1, cuts[i].c_str());  // Set bin labels
    }
    
    //--------------------------------
    //Event loop
    //--------------------------------
    std::cout<<"---------------------------"<<std::endl;
    std::cout<<setw(10)<<"Progress"<<setw(10)<<"Time"<<std::endl;
    std::cout<<"---------------------------"<<std::endl;
    double totalTime = 0.0;
	auto startClock = std::chrono::high_resolution_clock::now();
	for(Long64_t i= 0; i < nentries; i++){
        //if(i>100000) break;
        Helper::printProgress(i, nentries, startClock, totalTime);
        
        Long64_t entry = nanoT->loadEntry(i);
        h1EventInCutflow->Fill(cutToBinMap["NanoAOD"]);

		for (const auto& trigN : trigList_) {
		    trigTBranches_[trigN]->GetEntry(entry);//Read only content of HLT branches from NanoTree
		    if (trigVals_[trigN]) {
            	nanoT->fChain->GetTree()->GetEntry(entry);// Then read content of ALL branches
                h1EventInCutflow->Fill(cutToBinMap["Trigger"]);
            	newTree->Fill();
		        break; // Event passes if any trigger is true
		    }
		}
    }
    Helper::printCutflow(h1EventInCutflow.get());
    std::cout<<"nEvents_Skim = "<<newTree->GetEntries()<<std::endl;
    std::cout << "Output file path = "<<fout->GetName()<<std::endl;
    fout->Write();
    return 0;
}
