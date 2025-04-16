#include "RunZeeJet.h"
#include "RunZmmJet.h"
#include "RunGamJet.h"
#include "RunGamJetFake.h"
#include "RunMultiJet.h"
#include "RunWqqe.h"
#include "RunWqqm.h"

#include "SkimFile.h"
#include "SkimTree.h"
#include "RunsTree.h"
#include "PickEvent.h"
#include "PickObject.h"
#include "ScaleEvent.h"
#include "ScaleObject.h"
#include "GlobalFlag.h"
#include "Helper.h"

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
            std::cout << "./runMain -o " << element.key() << "_Hist_1of100.root" << std::endl;
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
    globalFlag.setNDebug(10000);
    globalFlag.printFlags();  

    Helper::printBanner("Set and load SkimFile.cpp");
    const std::string& inJsonDir = "input/json/";
    std::shared_ptr<SkimFile> skimF = std::make_shared<SkimFile>(globalFlag, outName, inJsonDir);

    Helper::printBanner("Set and load RunsTree.cpp");
    std::shared_ptr<RunsTree> runsT = std::make_shared<RunsTree>(globalFlag);
    Double_t normGenEventSumw = 1.0; 
    if (globalFlag.isMC()) {
        std::string cacheFilePath = "config/RunsTree.json";
        normGenEventSumw = runsT->getCachedNormGenEventSumw(skimF->getSampleKey(), 
                                                     cacheFilePath, 
                                                     skimF->getAllFileNames());
    }

    Helper::printBanner("Set and load SkimTree.cpp");
    std::shared_ptr<SkimTree> skimT = std::make_shared<SkimTree>(globalFlag);
    skimT->loadTree(skimF->getJobFileNames());

    Helper::printBanner("Set and load ScaleEvent.cpp");
    // Pass GlobalFlag reference to ScaleEvent
    std::shared_ptr<ScaleEvent> scaleEvent = std::make_shared<ScaleEvent>(globalFlag);
    try {
        scaleEvent->loadJetVetoRef();
        if (globalFlag.isData()) {
            scaleEvent->loadGoldenLumiJson();
            scaleEvent->loadHltLumiJson();
            scaleEvent->setLumiPerEra(skimF->getXsecOrLumiNano());
        }else{
            scaleEvent->loadPuRef();
            scaleEvent->setNormGenEventSumw(normGenEventSumw);
            scaleEvent->setLumiWeightInput(globalFlag.getLumiPerYear(), skimF->getXsecOrLumiNano(), skimF->getEventsNano());
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    Helper::printBanner("Set and load ScaleObject.cpp");
    // Pass GlobalFlag reference to ScaleObject
    std::shared_ptr<ScaleObject> scaleObj = std::make_shared<ScaleObject>(globalFlag);
    try {
        scaleObj->loadJetL1FastJetRef();
        scaleObj->loadJetL2RelativeRef();
        if (globalFlag.isData()) {
            scaleObj->loadJetL2L3ResidualRef();
        }else{
            scaleObj->loadJerResoRef();
            scaleObj->loadJerSfRef();
        }
        if (globalFlag.getChannel() == GlobalFlag::Channel::GamJet) {  
            // scaleObj->loadPhoSsRef();
            // scaleObj->loadEleSsRef();
        }
        if (globalFlag.getChannel() == GlobalFlag::Channel::ZmmJet) { 
            scaleObj->loadMuRochRef();
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    Helper::printBanner("Set and load PickEvent.cpp");
    auto pickEvent = std::make_unique<PickEvent>(globalFlag);

    Helper::printBanner("Set and load PickObject.cpp");
    auto pickObject = std::make_unique<PickObject>(globalFlag);

    std::string outDir = "output";
    mkdir(outDir.c_str(), S_IRWXU);
    auto fout = std::make_unique<TFile>((outDir + "/" + outName).c_str(), "RECREATE");

    Helper::printBanner("Loop over events and fill Histos");

    
    if (globalFlag.getChannel() == GlobalFlag::Channel::ZeeJet) {
        std::cout << "==> Running ZeeJet" << std::endl;
        auto zeeJet = std::make_unique<RunZeeJet>(globalFlag);
        zeeJet->Run(skimT, pickEvent.get(), scaleEvent.get(), scaleObj.get(), fout.get());
    }
    if (globalFlag.getChannel() == GlobalFlag::Channel::ZmmJet) {
        std::cout << "==> Running ZmmJet" << std::endl;
        auto zmmJet = std::make_unique<RunZmmJet>(globalFlag);
        zmmJet->Run(skimT, pickEvent.get(), scaleEvent.get(), scaleObj.get(), fout.get());
    }

    if (globalFlag.getChannel() == GlobalFlag::Channel::GamJet) {
        std::cout << "==> Running GamJet" << std::endl;
        auto gamJet = std::make_unique<RunGamJet>(globalFlag);
        gamJet->Run(skimT, pickEvent.get(), scaleEvent.get(), scaleObj.get(), fout.get());
    }
    if (globalFlag.getChannel() == GlobalFlag::Channel::GamJetFake) {
        std::cout << "==> Running GamJetFake" << std::endl;
        auto gamJetFake = std::make_unique<RunGamJetFake>(globalFlag);
        gamJetFake->Run(skimT, pickEvent.get(), pickObject.get(), scaleEvent.get(), scaleObj.get(), fout.get());
    }

    if (globalFlag.getChannel() == GlobalFlag::Channel::MultiJet) {
        std::cout << "==> Running MultiJet" << std::endl;
        auto multiJet = std::make_unique<RunMultiJet>(globalFlag);
        multiJet->Run(skimT, pickEvent.get(), pickObject.get(), scaleEvent.get(), scaleObj.get(), fout.get());
    }
    if (globalFlag.getChannel() == GlobalFlag::Channel::Wqqe) {
        std::cout << "==> Running Wqqe" << std::endl;
        auto wqqe = std::make_unique<RunWqqe>(globalFlag);
        wqqe->Run(skimT, pickEvent.get(), pickObject.get(), scaleEvent.get(), scaleObj.get(), fout.get());
    }
    if (globalFlag.getChannel() == GlobalFlag::Channel::Wqqm) {
        std::cout << "==> Running Wqqm" << std::endl;
        auto wqqm = std::make_unique<RunWqqm>(globalFlag);
        wqqm->Run(skimT, pickEvent.get(), pickObject.get(), scaleEvent.get(), scaleObj.get(), fout.get());
    }
/*

  if (globalFlag->isMCTruth) {
    std::cout << "==> Running MCTruth" << std::endl;
    auto mcTruth = std::make_unique<RunMCTruth>(outName);
    mcTruth->Run(skimT.get(), pickEvent.get(), pickObject.get(), scaleEvent.get(), scaleObj.get(), fout.get());
  }
  if (globalFlag->isFlavour) {
    std::cout << "==> Running Flavour" << std::endl;
    auto mcFlavour = std::make_unique<RunFlavour>(outName);
    mcFlavour->Run(skimT.get(), pickEvent.get(), pickObject.get(), scaleEvent.get(), scaleObj.get(), fout.get());
  }
  if (globalFlag->isVetoMap) {
    std::cout << "==> Running VetoMap" << std::endl;
    auto vetoMap = std::make_unique<RunVetoMap>(outName);
    vetoMap->Run(skimT.get(), pickEvent.get(), pickObject.get(), scaleEvent.get(), scaleObj.get(), fout.get());
  }
  if (globalFlag->isIncJet) {
    std::cout << "==> Running IncJet" << std::endl;
    auto incJet = std::make_unique<RunIncJet>(outName);
    incJet->Run(skimT.get(), pickEvent.get(), pickObject.get(), scaleEvent.get(), scaleObj.get(), fout.get());
  }
  if (globalFlag->isDiJet) {
    std::cout << "==> Running DiJet" << std::endl;
    auto diJet = std::make_unique<RunDiJet>(outName);
    diJet->Run(skimT.get(), pickEvent.get(), pickObject.get(), scaleEvent.get(), scaleObj.get(), fout.get());
  }
  */

  return 0;
}

