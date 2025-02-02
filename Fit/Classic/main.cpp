#include "GlobalFit.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <memory> // Include for unique_ptr

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

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
        entry.path().filename().string().rfind("FilesHist_", 0) == 0) {
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
            std::cout << "./runMain -o " << element.key() << "_Fit.root" << std::endl;
          }
        }
        return 0;
      default:
        std::cerr << "Use -h for help" << std::endl;
        return 1;
    }
  }

    using namespace ClassicFit;
    std::shared_ptr<GlobalFit> globFit = std::make_shared<GlobalFit>("Run3", "ratio", 0.0, 1.3);
    //globFit->loadInputJson();

    std::vector<TString> sampleList   = {"zjet"};
    std::vector<TString> hdmMethods   = {"mpf"};
    std::vector<TString> typesList    = {"Resp"};
    TString outputFileName = "output/outputRUN.root";
    globFit->setInput(sampleList, hdmMethods, typesList, outputFileName);

    globFit->run();

    return 0;
}

