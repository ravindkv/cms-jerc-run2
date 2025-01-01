#include"ScaleEvent.h"
#include"TrigDetail.h"
#include<iostream>
#include<regex>
#include<stdexcept>  // For std::runtime_error

// Constructor implementation
ScaleEvent::ScaleEvent(GlobalFlag& globalFlags) : 
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    era_(globalFlags_.getEra()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()),
    isData_(globalFlags_.isData()),
    isMC_(globalFlags_.isMC()){
}

void ScaleEvent::setInputs() {
  if (year_ == GlobalFlag::Year::Year2016Pre) {
    // Jet veto
    jetVetoJsonPath_ = "POG/JME/2016preVFP_UL/jetvetomaps.json.gz";
    jetVetoName_     = "Summer19UL16_V1";
    jetVetoKey_      = "jetvetomap_hot";
    // Lumi golden JSON
    goldenLumiJsonPath_    = "POG/LUM/Golden/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt";
    hltLumiJsonPath_    = "POG/LUM/HLT/LumiForTrig_2016Pre_HLT_Photon.json";
    puJsonPath_      = "POG/PU/2016preVFP_UL/puWeights.json";
    puName_          = "Collisions16_UltraLegacy_goldenJSON";
	} 

  else if (year_ == GlobalFlag::Year::Year2016Post) {
    // Jet veto
    jetVetoJsonPath_ = "POG/JME/2016postVFP_UL/jetvetomaps.json.gz";
    jetVetoName_     = "Summer19UL16_V1";
    jetVetoKey_      = "jetvetomap_hot";
    // Lumi golden JSON
    goldenLumiJsonPath_    = "POG/LUM/Golden/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt";
    hltLumiJsonPath_    = "POG/LUM/HLT/LumiForTrig_2016Post_HLT_Photon.json";
    puJsonPath_      = "POG/PU/2016postVFP_UL/puWeights.json";
    puName_          = "Collisions16_UltraLegacy_goldenJSON";
  }

  else if (year_ == GlobalFlag::Year::Year2017) {
    // Jet veto
    jetVetoJsonPath_ = "POG/JME/2017_UL/jetvetomaps.json.gz";
    jetVetoName_     = "Summer19UL17_V1";
    jetVetoKey_      = "jetvetomap_hot";
    // Lumi golden JSON
    goldenLumiJsonPath_    = "POG/LUM/Golden/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt";
    hltLumiJsonPath_    = "POG/LUM/HLT/LumiForTrig_2017_HLT_Photon.json";
    puJsonPath_      = "POG/PU/2017_UL/puWeights.json";
    puName_          = "Collisions17_UltraLegacy_goldenJSON";
  } 

  else if (year_ == GlobalFlag::Year::Year2018) {
    // Jet veto
    jetVetoJsonPath_ = "POG/JME/2018_UL/jetvetomaps.json.gz";
    jetVetoName_     = "Summer19UL18_V1";
    jetVetoKey_      = "jetvetomap_hot";
    // Lumi golden JSON
    goldenLumiJsonPath_    = "POG/LUM/Golden/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt";
    hltLumiJsonPath_    = "POG/LUM/HLT/LumiForTrig_2018_HLT_Photon.json";
    puJsonPath_      = "POG/PU/2018_UL/puWeights.json";
    puName_          = "Collisions18_UltraLegacy_goldenJSON";
  }//is2018
  else {
    throw std::runtime_error("Error: Year is not specified correctly in ScaleEvent::setInputs().");
  }

  std::cout<<"jetVetoJsonPath        = " << jetVetoJsonPath_      <<'\n';
  std::cout<<"jetVetoKey             = " << jetVetoKey_           <<'\n';
  std::cout<<"jetVetoName            = " << jetVetoName_          <<'\n'<<'\n';
  std::cout<<"goldenLumiJsonPath           = " << goldenLumiJsonPath_         <<'\n';
  std::cout<<"hltLumiJsonPath           = " << hltLumiJsonPath_         <<'\n';
  std::cout<<"puJsonPath             = " << puJsonPath_           <<'\n';
  std::cout<<"puName                 = " << puName_               <<'\n';
}//setInputs 


void ScaleEvent::loadJetVetoRef() {
  std::cout << "==> loadJetVetoRef()" << '\n';
  try {
    loadedJetVetoRef_ = correction::CorrectionSet::from_file(jetVetoJsonPath_)->at(jetVetoName_);
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: ScaleEvent::loadJetVetoRef()" << '\n';
    std::cerr << "Check " << jetVetoJsonPath_ << " or " << jetVetoName_ << '\n';
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to load Jet Veto Reference");
  }
}

auto ScaleEvent::checkJetVetoMap(const SkimTree& skimT) const -> bool {
  bool isVeto = false;
  const double maxEtaInMap = 5.191;
  const double maxPhiInMap = 3.1415926;

  try {
    for (int i = 0; i != skimT.nJet; ++i) {
      if (std::abs(skimT.Jet_eta[i]) > maxEtaInMap) continue;
      if (std::abs(skimT.Jet_phi[i]) > maxPhiInMap) continue;

      auto jvNumber = loadedJetVetoRef_->evaluate({jetVetoKey_, skimT.Jet_eta[i], skimT.Jet_phi[i]});
      if (isDebug_) {
        std::cout << 
            jetVetoKey_
            << ", jetEta= " << skimT.Jet_eta[i]
            << ", jetPhi= " << skimT.Jet_phi[i]
            << ", jetVetoNumber = " << jvNumber 
            << '\n';
      }
      if (jvNumber > 0) {
        isVeto = true;
        break; // Exit the loop as soon as we find a veto.
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: in checkJetVetoMap(): " << e.what() << '\n';
    throw std::runtime_error("Failed to check Jet Veto Map");
  }
  return isVeto;
}

//-------------------------------------
// Golden lumi Json
//-------------------------------------
void ScaleEvent::loadGoldenLumiJson() {
  std::cout << "==> loadGoldenLumiJson()" << '\n';
  std::ifstream file(goldenLumiJsonPath_);
  file >> loadedGoldenLumiJson_;
}

auto ScaleEvent::checkGoodLumi(const unsigned int &run, const unsigned int &lumi) const -> bool {
  try {
    auto it = loadedGoldenLumiJson_.find(std::to_string(run));
    if (it == loadedGoldenLumiJson_.end()) {
      return false;
    }
    for (const auto &lumiBlock : it.value()) {
      if (lumi >= lumiBlock[0] && lumi <= lumiBlock[1]) {
        return true;
      }
    }
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ScaleEvent::checkGoodLumi()" << '\n';
    std::cout << "Run = " << run << ", Lumi = " << lumi << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error checking good luminosity.");
  }
  return false;
}


//-------------------------------------
// HLT Lumi Json
//-------------------------------------
void ScaleEvent::loadHltLumiJson() {
  std::cout << "==> loadHltLumiJson()" << '\n';
  std::ifstream file(hltLumiJsonPath_);
  file >> loadedHltLumiJson_;
}

double ScaleEvent::getHltLumiPerRun(const std::string& hltPathBase, const std::string& runNumber) const {
    double recordedLumi = 0.0;
    // Construct a regex to match HLT paths that start with hltPathBase_v followed by digits
    // Example: ^HLT_Photon20_v\d+$
    std::string regexPattern = "^" + hltPathBase + "_v\\d+$";
    std::regex hltRegex(regexPattern);
    // Iterate over all HLT paths in the JSON
    for (const auto& [currentHltPath, runs] : loadedHltLumiJson_.items()) {
        // Use regex to match the HLT path
        if (std::regex_match(currentHltPath, hltRegex)) {
            // Check if the specified run exists within this HLT path
            if (runs.contains(runNumber)) {
                const nlohmann::json& records = runs.at(runNumber);
                // Iterate over all records for this run and retrieve 'recorded' luminosity
                for (const auto& record : records) {
                    if (record.contains("recorded") && record["recorded"].is_number()) {
                        recordedLumi = record["recorded"].get<double>();
                        return recordedLumi; // Return upon finding the first valid record
                    } else {
                        std::cerr << "Warning: 'recorded' field missing or not a number in record: " << record << std::endl;
                    }
                }
            }
        }
    }

    // If not found, return 0.0 and optionally log a message
    std::cerr << "Warning: Recorded luminosity not found for HLT path base '" << hltPathBase 
              << "' and run '" << runNumber << "'." << std::endl;
    return 0.0;
}

double ScaleEvent::cacheHltLumiPerRun(const std::shared_ptr<SkimTree>& skimT, const double& pt) const {
    if (isDebug_) printRunTriggerLumiCache(); 
    // 1) Extract the run number as an integer
    int currentRun = skimT->run;

    // We'll need to return the luminosity for *the first triggered path*,
    // but now we maintain distinct values for each (run, trigger).
    double matchedLumi = 0.0;

    // 2) Retrieve the trigDetails map from TrigDetail
    TrigDetail trigDetail(globalFlags_);
    if (channel_ == GlobalFlag::Channel::GamJet) {
        const auto& trigs = trigDetail.getTrigMapRangePt();

        // Loop over the triggers
        for (const auto& trig : trigs) {
            const std::string& trigName  = trig.first;     // e.g., "HLT_Photon50_R9Id90_HE10_IsoM"
            const TrigRangePt& trigRange = trig.second;    // {ptMin, ptMax}
            bool passesHlt = false;

            // Check if the event fired this trigger & if pt is in the correct range
            if (skimT->getTrigValue(trigName) && 
                pt >= trigRange.ptMin && pt < trigRange.ptMax) {
                passesHlt = true;
            }
            if (passesHlt) {
                // 2a) We check if we already have a cached luminosity for (run, triggerName).
                auto runIter = run2Trig2Lumi_.find(currentRun);
                if (runIter == run2Trig2Lumi_.end()) {
                    // No entry for this run yet, so create one
                    run2Trig2Lumi_[currentRun] = {};
                    runIter = run2Trig2Lumi_.find(currentRun);
                }

                auto& trig2lumiMap = runIter->second; // This is the map triggerName -> lumi
                if (trig2lumiMap.find(trigName) == trig2lumiMap.end()) {
                    // Not in cache, so we do the JSON lookup
                    double newLumi = getHltLumiPerRun(trigName, std::to_string(currentRun));
                    trig2lumiMap[trigName] = newLumi;
                }

                // 2b) Retrieve the cached luminosity for (run, triggerName)
                matchedLumi = trig2lumiMap[trigName];

                // If we only care about the *first* triggered path, break.
                // Otherwise, we could continue checking other triggers.
                break;
            }
        } // end for (auto& trig : trigs)
    } // end if GamJet

    return matchedLumi;
}

void ScaleEvent::printRunTriggerLumiCache() const {
    std::cout << "\n==> ScaleEvent::printRunTriggerLumiCache(): Current Cache Contents\n";
    // Iterate over the outer map (run -> trigger -> luminosity)
    for (const auto& [run, trig2lumi] : run2Trig2Lumi_) {
        std::cout << "Run: " << run << "\n";
        for (const auto& [trigger, lumi] : trig2lumi) {
            std::cout << "  Trigger: " << trigger 
                      << ", Luminosity: " << lumi << " ub^-1\n";
        }
    }
}


//-------------------------------------
// Pileup Json 
//-------------------------------------
void ScaleEvent::loadPuRef() {
  std::cout << "==> loadPuRef()" << '\n';
  try {
    loadedPuRef_ = correction::CorrectionSet::from_file(puJsonPath_)->at(puName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ScaleEvent::loadPuRef()" << '\n';
    std::cout << "Check " << puJsonPath_ << " or " << puName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading Pileup Reference.");
  }
}

auto ScaleEvent::getPuCorrection(Float_t nTrueInt, const std::string &nomOrSyst) const -> double {
  double puSf = 1.0;
  try {
    puSf = loadedPuRef_->evaluate({nTrueInt, nomOrSyst.c_str()});
    if (isDebug_) std::cout << "nomOrSyst = " << nomOrSyst 
              << ", nTrueInt = " << nTrueInt 
              << ", puSf= " << puSf << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in ScaleEvent::getPuCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating Pileup Correction.");
  }
  return puSf;
}

