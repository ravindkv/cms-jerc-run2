#include "RunGamJetFake.h"
#include "HistCutflow.h"
#include "Helper.h"
   
// Constructor implementation
RunGamJetFake::RunGamJetFake(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}

auto RunGamJetFake::Run(std::shared_ptr<NanoTree>& nanoT, TFile *fout) -> int{
    fout->cd();

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
        h1EventInCutflow->fill("Trigger");
        nanoT->fChain->GetTree()->GetEntry(entry);
        newTree->Fill();
    }
    Helper::printCutflow(h1EventInCutflow->getHistogram());
    std::cout<<"nEvents_Skim = "<<newTree->GetEntries()<<'\n';
    std::cout << "Output file path = "<<fout->GetName()<<'\n';
    fout->Write();
    return EXIT_SUCCESS; 
}
