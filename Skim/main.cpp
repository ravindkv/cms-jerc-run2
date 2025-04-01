
#include "GlobalFlag.h"
#include "NanoFile.h"
#include "NanoTree.h"
#include "ReadConfig.h"
#include "RunChannel.h"
#include "Helper.h"

#include <iostream>
#include <filesystem>
#include <sys/stat.h>
#include <nlohmann/json.hpp>

#include <TFile.h>


namespace fs = std::filesystem;

int main(int argc, char* argv[]){
	// Check if no arguments are provided
    if (argc == 1) {
      std::cerr << "Error: No arguments provided. Use -h for help." << std::endl;
      return 1;
    }
    
    std::string jsonDir = "input/json/";
    std::vector<std::string> jsonFiles;
    
    //Read only FilesNano_*.json files in the directory
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
              std::cout <<"./runMain -o "<<element.key()<<"_Skim_1of100.root" << std::endl;
            }
          }
          return 0;
        default:
          std::cerr << "Use -h for help" << std::endl;
          return 1;
      }
    }
    
    Helper::printBanner("Set GlobalFlag.cpp");
    GlobalFlag globalFlag(outName);
    globalFlag.setDebug(false);
    globalFlag.printFlag();
    string channelStr = globalFlag.channelStr;
    string yearStr = globalFlag.yearStr;

    Helper::printBanner("Set NanoFile.cpp");
    std::string inputSamplePath = "input/json/FilesNano_"+channelStr+"_"+yearStr+".json";
    std::shared_ptr<NanoFile> nanoF = std::make_shared<NanoFile>(globalFlag, outName, inputSamplePath);

    Helper::printBanner("Set and load NanoTree.cpp");
    std::shared_ptr<NanoTree> nanoT = std::make_shared<NanoTree>(globalFlag);
    nanoT->loadTree(nanoF->getJobFileNames());

    std::string outDir = "output";
    mkdir(outDir.c_str(), S_IRWXU);
    auto fout = std::make_unique<TFile>((outDir + "/" + outName).c_str(), "RECREATE");

    Helper::printBanner("Set and load ReadConfig.cpp");
    std::string commonConfigPath  = "config/ReadConfigCommon.json";
    std::string channelConfigPath = "config/ReadConfig"+channelStr+".json";
    ReadConfig readConfig(commonConfigPath, channelConfigPath);

    Helper::printBanner("Finally RunChannel.cpp");
    std::cout << "==> Running for: " << channelConfigPath << std::endl;
    auto runCh = std::make_unique<RunChannel>(globalFlag);
    runCh->Run(nanoT, readConfig, fout.get());

	return 0;
}
