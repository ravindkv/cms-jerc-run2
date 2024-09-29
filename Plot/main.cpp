//#include "PlotGamJet.h"
//#include "PlotZmmJet.h"
#include "PlotZeeJet.h"
//#include "PlotMCTruth.h"
//#include "PlotFlavour.h"
//#include "PlotVetoMap.h"
//#include "PlotIncJet.h"
//#include "PlotDiJet.h"
//#include "PlotMultiJet.h"
//#include "PlotWqq.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]){
	// Check if no arguments are provided
  if (argc == 1) {
    std::cerr << "Error: No arguments provided. Use -h for help." << std::endl;
    return 1;
  }

	std::string jsonFile = "input/json/MergedHistFiles.json";
  nlohmann::json js;
  std::string outName;

  //--------------------------------
  // Parse command-line options
  //--------------------------------
  int opt;
  std::ifstream file;  // Declare the ifstream here, outside the switch block
  file.open(jsonFile);  // Open the file here inside the case
  if (!file.is_open()) {
    std::cerr << "Could not open file: " << jsonFile << std::endl;
    std::abort();
  }
  try {
    js = nlohmann::json::parse(file);
  } catch (const std::exception& e) {
    std::cerr << "EXCEPTION: Error parsing file: " << jsonFile << std::endl;
    std::cerr << e.what() << std::endl;
    std::abort();
  }

  while ((opt = getopt(argc, argv, "o:h")) != -1) {
    switch (opt) {
      case 'o':
        outName = optarg;
        break;
      case 'h':
        std::cout << "\nFor file: " << jsonFile << std::endl;
        for (auto& element : js.items()) {
          std::cout <<" ./runMain -o "<<element.key()+"_Plot.root" << std::endl;
        }
        file.close();  // Close the file after usage
        return 0;
      default:
        std::cerr << "Use -h for help" << std::endl;
        return 1;
    }
  }

  TString oName = outName;

  string outDir = "output";
  mkdir(outDir.c_str(), S_IRWXU);
  string outPath = outDir+"/"+outName;
  TFile *outRoot  = new TFile(outPath.c_str(), "RECREATE");
  string outLatex = outPath;
  size_t pos = outLatex.find(".root");
  outLatex.replace(pos, 5, ".tex");

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Loop over events and fill Plotos"<<endl;
  cout<<"--------------------------------------"<<endl;
  //if(globF->isZeeJet){
    cout<<"==> Running ZeeJet"<<endl;
    PlotZeeJet *zeeJet = new PlotZeeJet();
    zeeJet->Run(js, outRoot, outLatex);
  //}
  /*
  if(globF->isGamJet){
    cout<<"==> Running GamJet"<<endl;
    PlotGamJet *gamJet = new PlotGamJet(oName);
    gamJet->Run(js, outRoot, outLatex);  
  }
  if(globF->isZeeJet){
    cout<<"==> Running ZeeJet"<<endl;
    PlotZeeJet *zeeJet = new PlotZeeJet(oName);
    zeeJet->Run(js, outRoot, outLatex);  
  }
  if(globF->isZmmJet){
    cout<<"==> Running ZmmJet"<<endl;
    PlotZmmJet *zmmJet = new PlotZmmJet(oName);
    zmmJet->Run(js, outRoot, outLatex);  
  }
  if(globF->isMCTruth){
    cout<<"==> Running MCTruth"<<endl;
    PlotMCTruth *mcTruth = new PlotMCTruth(oName);
    mcTruth->Run(js, outRoot, outLatex);  
  }
  if(globF->isFlavour){
    cout<<"==> Running Flavour"<<endl;
    PlotFlavour *mcFlavour = new PlotFlavour(oName);
    mcFlavour->Run(js, outRoot, outLatex);  
  }
  if(globF->isVetoMap){
    cout<<"==> Running VetoMap"<<endl;
    PlotVetoMap *vetoMap = new PlotVetoMap(oName);
    vetoMap->Run(js, outRoot, outLatex);  
  }
  if(globF->isIncJet){
    cout<<"==> Running IncJet"<<endl;
    PlotIncJet *incJet = new PlotIncJet(oName);
    incJet->Run(js, outRoot, outLatex);  
  }
  if(globF->isDiJet){
    cout<<"==> Running DiJet"<<endl;
    PlotDiJet *diJet = new PlotDiJet(oName);
    diJet->Run(js, outRoot, outLatex);  
  }
  if(globF->isMultiJet){
    cout<<"==> Running MultiJet"<<endl;
    PlotMultiJet *multiJet = new PlotMultiJet(oName);
    multiJet->Run(js, outRoot, outLatex);  
  }
  if(globF->isWqq){
    cout<<"==> Running Wqq"<<endl;
    PlotWqq *diJet = new PlotWqq(oName);
    diJet->Run(js, outRoot, outLatex);  
  }
  */

  return 0;
}
