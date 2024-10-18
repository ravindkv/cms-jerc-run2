#include "SkimZeeJet.h"
   
// Constructor implementation
SkimZeeJet::SkimZeeJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}


auto SkimZeeJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile *fout) -> int{

    TTree* newTree = nanoT->fChain->GetTree()->CloneTree(0);
    newTree->SetCacheSize(50*1024*1024);
    Long64_t nEntr = nanoT->GetEntries();
    
    int startEntry = 0;
    int endEntry = nEntr;
    int eventsPerJob = nEntr;
    std::cout << "Sample has "<<nEntr << " entries" << std::endl;
    cout << "Processing events "<<startEntry<< " to " << endEntry << endl;
    TH1D* hEvents_ = new TH1D("hEvents", "Cutflow", 5, -1.5, 3.5);
    hEvents_->GetXaxis()->SetBinLabel(1, "NanoAOD");
    hEvents_->GetXaxis()->SetBinLabel(2, "Trig");
    
    //--------------------------------
    //Event loop
    //--------------------------------
    std::cout<<"---------------------------"<<std::endl;
    std::cout<<setw(10)<<"Progress"<<setw(10)<<"Time"<<std::endl;
    std::cout<<"---------------------------"<<std::endl;
    double totalTime = 0.0;
	auto startClock = std::chrono::high_resolution_clock::now();
    bool passTrig = false;
	for(Long64_t i= startEntry; i < endEntry; i++){
        //if(i>100000) break;
        if(endEntry > 100  && i%(eventsPerJob/100) == 0){// print after every 1% of events
            totalTime+= std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-startClock).count();
            int sec = (int)(totalTime)%60;
            int min = (int)(totalTime)/60;
            std::cout<<setw(10)<<100*i/endEntry<<" %"<<setw(10)<<min<<"m "<<sec<<"s"<<std::endl;
            startClock = std::chrono::high_resolution_clock::now();
        }
        Long64_t entry = nanoT->loadEntry(i);
        hEvents_->Fill(0);

        if(globalFlags_.is2016Pre){
            nanoT->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ->GetEntry(entry);
            passTrig = nanoT->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
        }

        if(globalFlags_.is2016Post){
            nanoT->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ->GetEntry(entry);
            passTrig = nanoT->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
        }

        if(globalFlags_.is2017){
            nanoT->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL->GetEntry(entry);
            passTrig = nanoT->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
        }
        if(globalFlags_.is2018){
            nanoT->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL->GetEntry(entry);
            passTrig = nanoT->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
        }
        
        if(passTrig){
            nanoT->fChain->GetTree()->GetEntry(entry);
            hEvents_->Fill(1);
            newTree->Fill();
        }
    }
    std::cout<<"nEvents_Skim = "<<newTree->GetEntries()<<std::endl;
    std::cout << "Output file path = "<<fout->GetName()<<std::endl;
    newTree->Write();
	hEvents_->Write();
    return 0;
}
