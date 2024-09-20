#include <iostream>
#include <iomanip>
#include <fstream>
#include <TDirectory.h>

#include <filesystem>
#include <sys/stat.h>
#include <sys/types.h>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>

#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include "NanoTree.h"
#include "GlobalFlag.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]){
	// Check if no arguments are provided
  if (argc == 1) {
    std::cerr << "Error: No arguments provided. Use -h for help." << std::endl;
    return 1;
  }

  std::string jsonDir = "input/json/";
  std::vector<std::string> jsonFiles;

 // Read only FilesNano_*.json files in the directory
	for (const auto& entry : fs::directory_iterator(jsonDir)) {
  	if (entry.path().extension() == ".json" && 
    	entry.path().filename().string().rfind("FilesNano_", 0) == 0) {
    	jsonFiles.push_back(entry.path().string());
  	}
	}

  if (jsonFiles.empty()) {
    std::cerr << "No JSON files found in directory: " << jsonDir << std::endl;
    return 1;
  }

  nlohmann::json js;
  std::string outName;

  //--------------------------------
  // Parse command-line options
  //--------------------------------
  int opt;
  while ((opt = getopt(argc, argv, "o:h")) != -1) {
    switch (opt) {
      case 'o':
        outName = optarg;
        break;
      case 'h':
        // Loop through each JSON file and print available keys
        for (const auto& jsonFile : jsonFiles) {
          std::ifstream file(jsonFile);
          if (!file.is_open()) {
            std::cerr << "Could not open file: " << jsonFile << std::endl;
            continue;
          }
          
          try {
            js = nlohmann::json::parse(file);
          } catch (const std::exception& e) {
            std::cerr << "EXCEPTION: Error parsing file: " << jsonFile << std::endl;
            std::cerr << e.what() << std::endl;
            continue;
          }
          
          std::cout << "\nFor file: " << jsonFile << std::endl;
          for (auto& element : js.items()) {
            std::cout <<" ./runMain -o "<<element.key()<<"_Skim_1of100.root" << std::endl;
          }
        }
        return 0;
      default:
        std::cerr << "Use -h for help" << std::endl;
        return 1;
    }
  }

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Set GlobalFlag.cpp"<<endl;
  cout<<"--------------------------------------"<<endl;
  GlobalFlag *globF =  new GlobalFlag(outName);
  globF->printFlag();

  
  cout<<"\n--------------------------------------"<<endl;
  cout<<" Set and load NanoTree"<<endl;
  cout<<"--------------------------------------"<<endl;
  NanoTree *nanoT = new NanoTree(outName);
  nanoT->setInput(outName);
  nanoT->loadInput();
  nanoT->setInputJsonPath("input/json/");
  nanoT->loadInputJson();
  nanoT->loadJobFileNames();
  nanoT->loadTree();

	std::string outDir = "output";
  mkdir(outDir.c_str(), S_IRWXU);
	TFile* outFile = TFile::Open((outDir+"/"+outName).c_str() ,"RECREATE");
	TTree* newTree = nanoT->fChain->GetTree()->CloneTree(0);
	newTree->SetCacheSize(50*1024*1024);
	Long64_t nEntr = nanoT->GetEntries();

	int startEntry = 0;
	int endEntry = nEntr;
	int eventsPerJob = nEntr;
	std::cout << "Sample has "<<nEntr << " entries" << std::endl;
	cout << "Processing events "<<startEntry<< " to " << endEntry << endl;
	TH1F* hEvents_ = new TH1F("hEvents", "Cutflow", 5, -1.5, 3.5);
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
    //--------------------------------------- 
    //2016 Pre
    //--------------------------------------- 
    if(globF->is2016Pre){
      if(globF->isZeeJet){
        nanoT->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ->GetEntry(entry);
        passTrig = nanoT->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
      }
      if(globF->isZmmJet){
        nanoT->b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ->GetEntry(entry);
        passTrig = nanoT->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
      }
    }
    //--------------------------------------- 
    //2016 Post
    //--------------------------------------- 
    if(globF->is2016Post){
      if(globF->isZeeJet){
        nanoT->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ->GetEntry(entry);
        passTrig = nanoT->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
      }
      if(globF->isZmmJet){
        nanoT->b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ->GetEntry(entry);
        passTrig = nanoT->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
      }
    }
    //--------------------------------------- 
    //2017
    //--------------------------------------- 
    if(globF->is2017){
      if(globF->isZeeJet){
        nanoT->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL->GetEntry(entry);
        passTrig = nanoT->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
      }
      if(globF->isZmmJet){
        nanoT->b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8->GetEntry(entry);
        passTrig = nanoT->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
      }
    }
    //--------------------------------------- 
    //2018
    //--------------------------------------- 
    if(globF->is2018){
      if(globF->isZeeJet){
        nanoT->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL->GetEntry(entry);
        passTrig = nanoT->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
      }
      if(globF->isZmmJet){
        nanoT->b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8->GetEntry(entry);
        passTrig = nanoT->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
      }
    }
    
    if(passTrig){
      nanoT->fChain->GetTree()->GetEntry(entry);
      hEvents_->Fill(1);
    	newTree->Fill();
    }
	}
  std::cout<<"nEvents_Skim = "<<newTree->GetEntries()<<endl;
  cout << "Output file path =  "<< outDir+"/"+outName << endl;
  outFile->cd();
  newTree->Write();
	hEvents_->Write();
	outFile->Close();

	return 0;
}
