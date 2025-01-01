#include "RunZeeJet.h"
#include "RunZmmJet.h"
#include "RunGamJet.h"
#include "RunMultiJet.h"

//#include "RunMCTruth.h"
//#include "RunFlavour.h"
//#include "RunVetoMap.h"
//#include "RunIncJet.h"
//#include "RunDiJet.h"
//#include "RunWqq.h"
#include "GlobalFlag.h"
#include "Slide.h"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>


using namespace std;

int main(int argc, char* argv[]){
	// Check if no arguments are provided
  if (argc == 1) {
    std::cerr << "Error: No arguments provided. Use -h for help." << std::endl;
    return 1;
  }

  std::string jsonFile = "input/root/json/MergedHistFiles.json";
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
          std::cout <<"./runMain -o "<<element.key()+"_Plot.tex" << std::endl;
        }
        file.close();  // Close the file after usage
        return 0;
      default:
        std::cerr << "Use -h for help" << std::endl;
        return 1;
    }
  }

  //string eosPlotDir="/eos/user/r/rverma/www/public/cms-jerc-run2/Plot";
  string eosPlotDir="/eos/cms/store/group/phys_jetmet/rverma/cms-jerc-run2/Plot";
  string localTexDir = "./output";
  std::filesystem::create_directories(localTexDir);

  std::string chLatex = localTexDir+"/"+outName;
  Slide channelSlide(chLatex);

  bool isAllChannel = false;
  std::string allLatex = localTexDir+"/AllChannel_Plot.tex";
  Slide allChannelSlide(allLatex);
  allChannelSlide.startDocument("JME-21-001: All channel");

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Set GlobalFlag.cpp"<<endl;
  cout<<"--------------------------------------"<<endl;
  GlobalFlag *globF =  new GlobalFlag(outName);
  globF->printFlag();

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Create plots and slides"<<endl;
  cout<<"--------------------------------------"<<endl;

  if(globF->isZeeJet or isAllChannel){
    cout<<"==> Running ZeeJet"<<endl;
    RunZeeJet *zeeJet = new RunZeeJet(outName);
    zeeJet->Run(js, eosPlotDir, channelSlide, allChannelSlide);
  }

  if(globF->isZmmJet or isAllChannel){
    cout<<"==> Running ZmmJet"<<endl;
    RunZmmJet *zmmJet = new RunZmmJet(outName);
    zmmJet->Run(js, eosPlotDir, channelSlide, allChannelSlide);
  }

  if(globF->isGamJet or isAllChannel){
    cout<<"==> Running GamJet"<<endl;
    RunGamJet *gamJet = new RunGamJet(outName);
    gamJet->Run(js, eosPlotDir, channelSlide, allChannelSlide);
  }

  if(globF->isMultiJet or isAllChannel){
    cout<<"==> Running MultiJet"<<endl;
    RunMultiJet *multiJet = new RunMultiJet(outName);
    multiJet->Run(js, eosPlotDir, channelSlide, allChannelSlide);
  }
  /*
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
  if(globF->isWqq){
    cout<<"==> Running Wqq"<<endl;
    PlotWqq *diJet = new PlotWqq(oName);
    diJet->Run(js, outRoot, chLatex);  
  }
  */
  allChannelSlide.addCenteredTextSlide("Thank you!");
  allChannelSlide.endDocument();
  std::cout<<chLatex<<std::endl;
  std::cout<<allLatex<<std::endl;


  return 0;
}
