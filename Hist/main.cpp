#include "RunZeeJet.h"
#include "RunZmmJet.h"
#include "RunGamJet.h"
//#include "RunMultiJet.h"

//#include "HistMCTruth.h"
//#include "HistFlavour.h"
//#include "HistVetoMap.h"
//#include "HistIncJet.h"
//#include "HistDiJet.h"
//#include "HistWqq.h"

#include "SkimTree.h"
#include "EventPick.h"
#include "ScaleObject.h"
#include "GlobalFlag.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>

#include "Logger.h"
#include <memory> // Include for unique_ptr

using namespace std;
namespace fs = std::filesystem;


int main(int argc, char* argv[]) {

    /*
    // Initialize the logger in main.cpp
    Logger::getInstance().init("logfile.txt");

    // Set the console log level to ERROR
    Logger::setConsoleLogLevel(ERROR);

    // Example logging from main.cpp
    Logger::getInstance().LogInfo("Program started.");
    Logger::getInstance().LogError("An error occurred.");
    */


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
            std::cout << " ./runMain -o " << element.key() << "_Hist_1of100.root" << std::endl;
          }
        }
        return 0;
      default:
        std::cerr << "Use -h for help" << std::endl;
        return 1;
    }
  }

	std::cout << "\n--------------------------------------" << std::endl;
    std::cout << " Set GlobalFlag.cpp" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    // Initialize GlobalFlag instance
    GlobalFlag globalFlag(outName);
    globalFlag.setDebug(false);
    globalFlag.setNDebug(1000);
    globalFlag.printFlags();  

    std::cout << "\n--------------------------------------" << std::endl;
    std::cout << " Set and load SkimTree.cpp" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    std::shared_ptr<SkimTree> skimT = std::make_shared<SkimTree>(globalFlag);
    skimT->setInput(outName);
    skimT->loadInput();
    skimT->setInputJsonPath("input/json/");
    skimT->loadInputJson();
    skimT->loadJobFileNames();
    skimT->loadTree();

    std::cout << "\n--------------------------------------" << std::endl;
    std::cout << " Set and load ScaleObject.cpp" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    // Pass GlobalFlag reference to ScaleObject
    std::shared_ptr<ScaleObject> scaleObj = std::make_shared<ScaleObject>(globalFlag);
    
    try {
        scaleObj->setInputs();
        scaleObj->loadJetVetoRef();
        scaleObj->loadJetL1FastJetRef();
        scaleObj->loadJetL2RelativeRef();
        scaleObj->loadJetL2L3ResidualRef();
        scaleObj->loadJerResoRef();
        scaleObj->loadJerSfRef();
        scaleObj->loadPuRef();

        // Use the GlobalFlag instance for conditional checks
        if (globalFlag.getChannel() == GlobalFlag::Channel::GamJet) {  // Scale and Smearing
            //scaleObj->loadPhoSsRef();
            //scaleObj->loadEleSsRef();
        }
        if (globalFlag.getChannel() == GlobalFlag::Channel::ZmmJet) { 
            scaleObj->loadMuRochRef();
        }
        if (globalFlag.isData()) {
            scaleObj->loadLumiJson();
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return EXIT_FAILURE;  // Exit with failure code
    }

    std::cout << "\n--------------------------------------" << std::endl;
    std::cout << " Set and load EventPick.cpp" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    // Pass GlobalFlag reference to EventPick
    auto eventP = std::make_unique<EventPick>(globalFlag);

    std::cout << "\n--------------------------------------" << std::endl;
    std::cout << " Set and load ObjectPick.cpp" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    // Pass GlobalFlag reference to ObjectPick
    auto objP = std::make_unique<ObjectPick>(globalFlag);

    // Output directory setup
    std::string outDir = "output";
    mkdir(outDir.c_str(), S_IRWXU);
    auto fout = std::make_unique<TFile>((outDir + "/" + outName).c_str(), "RECREATE");

    std::cout << "\n--------------------------------------" << std::endl;
    std::cout << " Loop over events and fill Histos" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    if (globalFlag.getChannel() == GlobalFlag::Channel::ZeeJet) {
        std::cout << "==> Running ZeeJet" << std::endl;
        auto zeeJet = std::make_unique<RunZeeJet>(globalFlag);
        zeeJet->Run(skimT, eventP.get(), objP.get(), scaleObj.get(), fout.get());
    }
    if (globalFlag.getChannel() == GlobalFlag::Channel::ZmmJet) {
        std::cout << "==> Running ZmmJet" << std::endl;
        auto zmmJet = std::make_unique<RunZmmJet>(globalFlag);
        zmmJet->Run(skimT, eventP.get(), objP.get(), scaleObj.get(), fout.get());
    }

    if (globalFlag.getChannel() == GlobalFlag::Channel::GamJet) {
        std::cout << "==> Running GamJet" << std::endl;
        auto gamJet = std::make_unique<RunGamJet>(globalFlag);
        gamJet->Run(skimT, eventP.get(), objP.get(), scaleObj.get(), fout.get());
    }

/*

    if (globalFlag.getChannel() == GlobalFlag::Channel::MultiJet) {
        std::cout << "==> Running MultiJet" << std::endl;
        auto multiJet = std::make_unique<RunMultiJet>(globalFlag);
        multiJet->Run(skimT, eventP.get(), objP.get(), scaleObj.get(), fout.get());
    }
  if (globalFlag->isMCTruth) {
    std::cout << "==> Running MCTruth" << std::endl;
    auto mcTruth = std::make_unique<RunMCTruth>(outName);
    mcTruth->Run(skimT.get(), eventP.get(), objP.get(), scaleObj.get(), fout.get());
  }
  if (globalFlag->isFlavour) {
    std::cout << "==> Running Flavour" << std::endl;
    auto mcFlavour = std::make_unique<RunFlavour>(outName);
    mcFlavour->Run(skimT.get(), eventP.get(), objP.get(), scaleObj.get(), fout.get());
  }
  if (globalFlag->isVetoMap) {
    std::cout << "==> Running VetoMap" << std::endl;
    auto vetoMap = std::make_unique<RunVetoMap>(outName);
    vetoMap->Run(skimT.get(), eventP.get(), objP.get(), scaleObj.get(), fout.get());
  }
  if (globalFlag->isIncJet) {
    std::cout << "==> Running IncJet" << std::endl;
    auto incJet = std::make_unique<RunIncJet>(outName);
    incJet->Run(skimT.get(), eventP.get(), objP.get(), scaleObj.get(), fout.get());
  }
  if (globalFlag->isDiJet) {
    std::cout << "==> Running DiJet" << std::endl;
    auto diJet = std::make_unique<RunDiJet>(outName);
    diJet->Run(skimT.get(), eventP.get(), objP.get(), scaleObj.get(), fout.get());
  }
  if (globalFlag->isWqq) {
    std::cout << "==> Running Wqq" << std::endl;
    auto wqq = std::make_unique<RunWqq>(outName);
    wqq->Run(skimT.get(), eventP.get(), objP.get(), scaleObj.get(), fout.get());
  }
  */

  return 0;
}

