#include "RunGamJet.h"
#include "HistCutflow.h"
#include "Helper.h"
   
// Constructor implementation
RunGamJet::RunGamJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}

auto RunGamJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile *fout) -> int{
    fout->cd();

	//----------------------------------
	// Add channel specific branches 
	//----------------------------------
    nanoT->fChain->SetBranchStatus("nPhoton",true);
   	nanoT->fChain->SetBranchStatus("Photon_e*",true);
   	nanoT->fChain->SetBranchStatus("Photon_hoe",true);
   	nanoT->fChain->SetBranchStatus("Photon_phi",true);
   	nanoT->fChain->SetBranchStatus("Photon_pt",true);
   	nanoT->fChain->SetBranchStatus("Photon_r9",true);
   	nanoT->fChain->SetBranchStatus("Photon_cutBased",true);
   	nanoT->fChain->SetBranchStatus("Photon_mvaID*",true);
   	nanoT->fChain->SetBranchStatus("Photon_pf*",true);
   	nanoT->fChain->SetBranchStatus("Photon_pixel*",true);
   	nanoT->fChain->SetBranchStatus("Photon_jetIdx",true);
   	nanoT->fChain->SetBranchStatus("Photon_genPartIdx",true);
   	nanoT->fChain->SetBranchStatus("Photon_seedGain",true);
   	nanoT->fChain->SetBranchStatus("Photon_mass",true);
  	if(globalFlags_.isMC){
      nanoT->fChain->SetBranchStatus("GenIsolatedPhoton_*",true);
      nanoT->fChain->SetBranchStatus("nGenIsolatedPhoton",true);
    }

	//----------------------------------
	// Set trigger list
	//----------------------------------
    std::vector<std::string> patterns;
    if(globalFlags_.is2016Pre || globalFlags_.is2016Post){
        patterns = { 
        	"HLT_Photon175",
			"HLT_Photon165_R9Id90_HE10_IsoM",
			"HLT_Photon120_R9Id90_HE10_IsoM",
			"HLT_Photon90_R9Id90_HE10_IsoM",
			"HLT_Photon75_R9Id90_HE10_IsoM",
			"HLT_Photon50_R9Id90_HE10_IsoM",
			"HLT_Photon36_R9Id90_HE10_IsoM",
			"HLT_Photon30_R9Id90_HE10_IsoM",
			"HLT_Photon22_R9Id90_HE10_IsoM"
        };
    }
    if(globalFlags_.is2017){
        patterns = { 
			"HLT_Photon200",
            "HLT_Photon165_R9Id90_HE10_IsoM",
            "HLT_Photon120_R9Id90_HE10_IsoM",
            "HLT_Photon90_R9Id90_HE10_IsoM",
            "HLT_Photon75_R9Id90_HE10_IsoM",
            "HLT_Photon50_R9Id90_HE10_IsoM",
            "HLT_Photon30_HoverELoose",
            "HLT_Photon20_HoverELoose"
        }; 
    }
    if(globalFlags_.is2018){
        patterns = { 
			"HLT_Photon200",
            "HLT_Photon110EB_TightID_TightIso",
            "HLT_Photon100EB_TightID_TightIso",
            "HLT_Photon90_R9Id90_HE10_IsoM",
            "HLT_Photon75_R9Id90_HE10_IsoM",
            "HLT_Photon50_R9Id90_HE10_IsoM",
            "HLT_Photon30_HoverELoose",
            "HLT_Photon20_HoverELoose"
        }; 
    }
    trigList_  = patterns; 

    if (trigList_.empty()) {
        std::cerr << "No triggers found for channel: ZeeJet" << '\n';
        exit(EXIT_FAILURE);
    }


    if (trigList_.empty()) {
        std::cerr << "No triggers found for channel: GamJet" << '\n';
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
