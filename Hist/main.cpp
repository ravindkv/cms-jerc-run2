#include "HistGamJet.h"
#include "HistZmmJet.h"
#include "HistZeeJet.h"
#include "HistMCTruth.h"
#include "HistFlavour.h"
#include "HistVetoMap.h"
#include "HistIncJet.h"
//#include "HistDiJet.h"
//#include "HistMultiJet.h"
#include "HistWqq.h"
#include "SkimTree.h"
#include "EventPick.h"
#include "ObjectScale.h"
#include "GlobalFlag.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
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
    	entry.path().filename().string().rfind("FilesSkim_", 0) == 0) {
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
            std::cout <<" ./runMain -o "<<element.key()<<"_Hist_1of100.root" << std::endl;
          }
        }
        return 0;
      default:
        std::cerr << "Use -h for help" << std::endl;
        return 1;
    }
  }
  TString oName = outName;

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Set GlobalFlag.cpp"<<endl;
  cout<<"--------------------------------------"<<endl;
  GlobalFlag *globF =  new GlobalFlag(oName);
  globF->printFlag();

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Set and load SkimTree.cpp"<<endl;
  cout<<"--------------------------------------"<<endl;
  SkimTree *skimT = new SkimTree(oName);
  skimT->setInput(outName);
  skimT->loadInput();
  cout<<endl;
  skimT->setInputJsonPath("input/json/");
  skimT->loadInputJson();
  cout<<endl;
  skimT->loadJobFileNames();
  cout<<endl;
  skimT->loadTree();
  cout<<endl;

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Set and load EventPick.cpp"<<endl;
  cout<<"--------------------------------------"<<endl;
  EventPick *eventP = new EventPick(oName);

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Set and load ObjectPick.cpp"<<endl;
  cout<<"--------------------------------------"<<endl;
  ObjectPick *objP = new ObjectPick(oName);
  objP->setTree(skimT);

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Set and load ObjectScale.cpp"<<endl;
  cout<<"--------------------------------------"<<endl;
  ObjectScale *objS = new ObjectScale(oName);
  objS->setInputs();
  objS->loadJetVetoRef(); 
  objS->loadJetL1FastJetRef();    
  objS->loadJetL2RelativeRef();   
  objS->loadJetL2L3ResidualRef(); 
  objS->loadJERResoRef(); 
  objS->loadJERSFRef();   
  if(globF->isGamJet){//Scale and Smearing
    objS->loadPhoSsRef(); 
    objS->loadEleSsRef(); 
  }
  //Lumi
  if (globF->isData){
    objS->loadLumiJson(); 
  }

  string outDir = "output";
  mkdir(outDir.c_str(), S_IRWXU);
  TFile *fout = new TFile(outDir+"/"+oName, "RECREATE");

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Loop over events and fill Histos"<<endl;
  cout<<"--------------------------------------"<<endl;
  if(globF->isGamJet){
    cout<<"==> Running GamJet"<<endl;
    HistGamJet *gamJet = new HistGamJet(oName);
    gamJet->Run(skimT, eventP, objP, objS, fout);  
  }
  if(globF->isZeeJet){
    cout<<"==> Running ZeeJet"<<endl;
    HistZeeJet *zeeJet = new HistZeeJet(oName);
    zeeJet->Run(skimT, eventP, objP, objS, fout);  
  }
  if(globF->isZmmJet){
    cout<<"==> Running ZmmJet"<<endl;
    HistZmmJet *zmmJet = new HistZmmJet(oName);
    zmmJet->Run(skimT, eventP, objP, objS, fout);  
  }
  if(globF->isMCTruth){
    cout<<"==> Running MCTruth"<<endl;
    HistMCTruth *mcTruth = new HistMCTruth(oName);
    mcTruth->Run(skimT, eventP, objP, objS, fout);  
  }
  if(globF->isFlavour){
    cout<<"==> Running Flavour"<<endl;
    HistFlavour *mcFlavour = new HistFlavour(oName);
    mcFlavour->Run(skimT, eventP, objP, objS, fout);  
  }
  if(globF->isVetoMap){
    cout<<"==> Running VetoMap"<<endl;
    HistVetoMap *vetoMap = new HistVetoMap(oName);
    vetoMap->Run(skimT, eventP, objP, objS, fout);  
  }
  if(globF->isIncJet){
    cout<<"==> Running IncJet"<<endl;
    HistIncJet *incJet = new HistIncJet(oName);
    incJet->Run(skimT, eventP, objP, objS, fout);  
  }
  /*
  if(globF->isDiJet){
    cout<<"==> Running DiJet"<<endl;
    HistDiJet *diJet = new HistDiJet(oName);
    diJet->Run(skimT, eventP, objP, objS, fout);  
  }
  if(globF->isMultiJet){
    cout<<"==> Running MultiJet"<<endl;
    HistMultiJet *multiJet = new HistMultiJet(oName);
    multiJet->Run(skimT, eventP, objP, objS, fout);  
  }
  */
  if(globF->isWqq){
    cout<<"==> Running Wqq"<<endl;
    HistWqq *diJet = new HistWqq(oName);
    diJet->Run(skimT, eventP, objP, objS, fout);  
  }

  return 0;
}
