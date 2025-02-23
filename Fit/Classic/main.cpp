#include "RunL3Res.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <memory> // Include for unique_ptr
#include <unistd.h>
#include "TString.h"

#include "GlobalFlag.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    // Check if no arguments are provided
    if (argc == 1) {
      std::cerr << "Error: No arguments provided. Use -h for help." << std::endl;
      return 1;
    }
    
    std::string jsonDir = "input/json/";
    std::vector<std::string> jsonFiles;
    
    // Read only FilesFit_*.json files in the directory
    for (const auto& entry : fs::directory_iterator(jsonDir)) {
      if (entry.path().extension() == ".json" &&
          entry.path().filename().string().rfind("FilesFit_L3ResOutput", 0) == 0) {
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
  
    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set GlobalFlag.cpp"<<endl;
    cout<<"--------------------------------------"<<endl;
    GlobalFlag globFlags(outName);
    globFlags.printFlag();

    //------------------------------------------
    // Input filenames
    //------------------------------------------
    std::string inName = outName; 
    std::string toReplace = "L3ResOutput";
    std::string replacement = "L3ResInput";
    size_t pos = inName.find(toReplace);
    if (pos != std::string::npos) {
        inName.replace(pos, toReplace.length(), replacement);
    }
    // Remove the "_Fit.root" extension
    pos = inName.rfind("_Fit.root");
    if (pos != std::string::npos) {
        inName.erase(pos, std::string("_Fit.root").length());
    }

    std::string inputJsonPath = "input/json/FilesFit_L3ResInput.json";
    std::ifstream fileName(inputJsonPath);
    if (!fileName.is_open()) {
        throw std::runtime_error("Unable to open input JSON file: " + inputJsonPath);
    }
    nlohmann::json jsIn;
    fileName >> jsIn;
    std::string inFileName;
    try {
        jsIn.at(inName).get_to(inFileName);
    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "Key not found in JSON: " << inName << "\n" << e.what() << "\n"
            << "Available keys in the JSON file:";
        for (const auto& element : jsIn.items()) {
            oss << "\n- " << element.key();
        }
        throw std::runtime_error(oss.str());
    }
    const std::map<TString, TString> inFileNames = {
        {"jes", inFileName},
    };

    //------------------------------------------
    // Output filename
    //------------------------------------------
    std::string outDir = "output";
    if (!fs::exists(outDir)) fs::create_directories(outDir);
    TString outputFileName = outDir+"/"+outName;

    //------------------------------------------
    // FitConfig
    //------------------------------------------
    std::vector<TString> sampleList   = {"Wqqm"};
    std::vector<TString> hdmMethods   = {"Mpf"};
    std::vector<TString> typesList    = {"Resp"};

    //------------------------------------------
    // Run RunL3Res 
    //------------------------------------------
    if(globFlags.isL3Res){
        std::shared_ptr<RunL3Res> l3Res = std::make_shared<RunL3Res>(globFlags);
        l3Res->Run();
    }

    return 0;
}

