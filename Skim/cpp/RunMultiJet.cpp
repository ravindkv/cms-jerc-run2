#include "RunMultiJet.h"
#include "HistCutflow.h"
#include "Helper.h"
   
// Constructor implementation
RunMultiJet::RunMultiJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}


auto RunMultiJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile *fout) -> int{
    fout->cd();

	//----------------------------------
	// Set trigger list
	//----------------------------------
    std::vector<std::string> patterns;
    patterns = { "HLT_PFJet*", "HLT_DiPFJet*"};
    trigList_  = Helper::GetMatchingBranchNames(nanoT->fChain->GetTree(), patterns);

    if (trigList_.empty()) {
        std::cerr << "No triggers found for channel: MultiJet" << '\n';
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
    //newTree->Write();
    fout->Write();
    return EXIT_SUCCESS;
}
