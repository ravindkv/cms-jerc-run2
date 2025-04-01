#include "GlobalFlag.h"
#include "Slide.h"
#include "RunChannel.h"

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
          std::cout <<"./runMain -o "<<element.key()+"_Plot.tex" << std::endl;
        }
        file.close();  // Close the file after usage
        return 0;
      default:
        std::cerr << "Use -h for help" << std::endl;
        return 1;
    }
  }

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
  GlobalFlag *globalFlag =  new GlobalFlag(outName);
  globalFlag->printFlag();

  cout<<"\n--------------------------------------"<<endl;
  cout<<" Create plots and slides"<<endl;
  cout<<"--------------------------------------"<<endl;
  std::string channelConfigPath = "config/json/ReadConfig"+globalFlag->channelStr+".json";
  ReadConfig readConfig(channelConfigPath);

  std::cout << "==> Running for: " << channelConfigPath << std::endl;
  auto runCh = std::make_unique<RunChannel>(outName);
  runCh->Run(js, eosPlotDir, channelSlide, allChannelSlide, readConfig);

  allChannelSlide.addCenteredTextSlide("Thank you!");
  allChannelSlide.endDocument();
  std::cout<<chLatex<<std::endl;
  std::cout<<allLatex<<std::endl;


  return 0;
}
