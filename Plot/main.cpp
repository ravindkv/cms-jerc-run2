#include "PlotZeeJet.h"
//#include "PlotZmmJet.h"
//#include "PlotGamJet.h"
//#include "PlotMCTruth.h"
//#include "PlotFlavour.h"
//#include "PlotVetoMap.h"
//#include "PlotIncJet.h"
//#include "PlotDiJet.h"
//#include "PlotMultiJet.h"
//#include "PlotWqq.h"
#include "GlobalFlag.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>

#include "MakeDPNote.h"

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
  string chLatex = outPath;
  size_t pos = chLatex.find(".root");
  chLatex.replace(pos, 5, ".tex");
  MakeDPNote chNote(chLatex);

  bool isAll = true;
  string allLatex = "all.tex";
  MakeDPNote allNote(allLatex);
  allNote.startDocument("JME-21-001: All channel");

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Set GlobalFlag.cpp"<<endl;
  cout<<"--------------------------------------"<<endl;
  GlobalFlag *globF =  new GlobalFlag(outName);
  globF->printFlag();

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Create plots and slides"<<endl;
  cout<<"--------------------------------------"<<endl;

  if(globF->isZeeJet or isAll){
    cout<<"==> Running ZeeJet"<<endl;
    PlotZeeJet *zeeJet = new PlotZeeJet("ZeeJet");
    zeeJet->Run(js, outRoot, chNote, allNote);
  }
  /*
  if(globF->isZmmJet){
    cout<<"==> Running ZmmJet"<<endl;
    PlotZmmJet *zmmJet = new PlotZmmJet(outName);
    zmmJet->Run(js, outRoot, chLatex);
  }

  if(globF->isGamJet){
    cout<<"==> Running GamJet"<<endl;
    PlotGamJet *gamJet = new PlotGamJet(oName);
    gamJet->Run(js, outRoot, chLatex);  
  }
  if(globF->isMCTruth){
    cout<<"==> Running MCTruth"<<endl;
    PlotMCTruth *mcTruth = new PlotMCTruth(oName);
    mcTruth->Run(js, outRoot, chLatex);  
  }
  if(globF->isFlavour){
    cout<<"==> Running Flavour"<<endl;
    PlotFlavour *mcFlavour = new PlotFlavour(oName);
    mcFlavour->Run(js, outRoot, chLatex);  
  }
  if(globF->isVetoMap){
    cout<<"==> Running VetoMap"<<endl;
    PlotVetoMap *vetoMap = new PlotVetoMap(oName);
    vetoMap->Run(js, outRoot, chLatex);  
  }
  if(globF->isIncJet){
    cout<<"==> Running IncJet"<<endl;
    PlotIncJet *incJet = new PlotIncJet(oName);
    incJet->Run(js, outRoot, chLatex);  
  }
  if(globF->isDiJet){
    cout<<"==> Running DiJet"<<endl;
    PlotDiJet *diJet = new PlotDiJet(oName);
    diJet->Run(js, outRoot, chLatex);  
  }
  if(globF->isMultiJet){
    cout<<"==> Running MultiJet"<<endl;
    PlotMultiJet *multiJet = new PlotMultiJet(oName);
    multiJet->Run(js, outRoot, chLatex);  
  }
  if(globF->isWqq){
    cout<<"==> Running Wqq"<<endl;
    PlotWqq *diJet = new PlotWqq(oName);
    diJet->Run(js, outRoot, chLatex);  
  }
  */
  allNote.addCenteredTextSlide("Thank you!");
  allNote.endDocument();
  std::cout<<chLatex<<std::endl;
  std::cout<<allLatex<<std::endl;


  return 0;
}
