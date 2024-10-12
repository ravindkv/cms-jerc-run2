#include"ObjectScale.h"
#include<iostream>
#include<stdexcept>  // For std::runtime_error

// Constructor implementation
ObjectScale::ObjectScale(GlobalFlag& globalFlags) : globalFlags_(globalFlags) {
}

// Set the SkimTree pointer
void ObjectScale::setTree(const std::shared_ptr<SkimTree>& skimTree) {
    skimTree_ = skimTree;
}

void ObjectScale::setInputs() {
  if (globalFlags_.getYear() == GlobalFlag::Year::Year2016Pre) {
    // Jet veto
    jetVetoJsonPath_ = "POG/JME/2016preVFP_UL/jetvetomaps.json.gz";
    jetVetoName_     = "Summer19UL16_V1";
    jetVetoKey_      = "jetvetomap_hot";
    // Jet L1, L2, L3
    jercJsonPath_          = "POG/JME/2016preVFP_UL/jet_jerc.json.gz";
    jetL1FastJetName_      = "Summer19UL16APV_V7_MC_L1FastJet_AK4PFchs";
    jetL2RelativeName_     = "Summer19UL16APV_V7_MC_L2Relative_AK4PFchs";
    jetL3AbsoluteName_     = "Summer19UL16APV_V7_MC_L3Absolute_AK4PFchs";
    jetL2L3ResidualName_   = "Summer19UL16APV_V7_MC_L2L3Residual_AK4PFchs";
    if (globalFlags_.isData()) {
      if (globalFlags_.getEra() == GlobalFlag::Era::Era2016PreBCD) {
        jetL1FastJetName_    = "Summer19UL16APV_RunBCD_V7_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL16APV_RunBCD_V7_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL16APV_RunBCD_V7_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL16APV_RunBCD_V7_DATA_L2L3Residual_AK4PFchs";
      } else if (globalFlags_.getEra() == GlobalFlag::Era::Era2016PreEF) {
        jetL1FastJetName_    = "Summer19UL16APV_RunEF_V7_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL16APV_RunEF_V7_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL16APV_RunEF_V7_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL16APV_RunEF_V7_DATA_L2L3Residual_AK4PFchs";
      } else {
        throw std::runtime_error("Error: inputs are not set for data in ObjectScale::setInputs() for 2016 pre.");
      }
    } // isData_
    // JER
    JERResoName_     = "Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs";
    JERSFName_       = "Summer20UL16APV_JRV3_MC_ScaleFactor_AK4PFchs";
    // Photon SS
    phoSsJsonPath_   = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName_       = "2022Re-recoBCD_SmearingJSON";
    // Electron SS
    eleSsJsonPath_   = "POG/EGM/electronSS.json";
    eleSsName_       = "2022Re-recoBCD_ScaleJSON";
    // Lumi golden JSON
    lumiJsonPath_    = "POG/LUM/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt";
    puJsonPath_      = "POG/LUM/2016preVFP_UL/puWeights.json";
    puName_          = "Collisions16_UltraLegacy_goldenJSON";
	} 

  else if (globalFlags_.getYear() == GlobalFlag::Year::Year2016Post) {
    // Jet veto
    jetVetoJsonPath_ = "POG/JME/2016postVFP_UL/jetvetomaps.json.gz";
    jetVetoName_     = "Summer19UL16_V1";
    jetVetoKey_      = "jetvetomap_hot";
    // Jet L1, L2, L3
    jercJsonPath_          = "POG/JME/2016postVFP_UL/jet_jerc.json.gz";
    jetL1FastJetName_      = "Summer19UL16_V7_MC_L1FastJet_AK4PFchs";
    jetL2RelativeName_     = "Summer19UL16_V7_MC_L2Relative_AK4PFchs";
    jetL3AbsoluteName_     = "Summer19UL16_V7_MC_L3Absolute_AK4PFchs";
    jetL2L3ResidualName_   = "Summer19UL16_V7_MC_L2L3Residual_AK4PFchs";
    if (globalFlags_.isData()) {
      if (globalFlags_.getEra() == GlobalFlag::Era::Era2016PostFGH) {
        jetL1FastJetName_    = "Summer19UL16_RunFGH_V7_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL16_RunFGH_V7_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL16_RunFGH_V7_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL16_RunFGH_V7_DATA_L2L3Residual_AK4PFchs";
      } else {
        throw std::runtime_error("Error: inputs are not set for data in ObjectScale::setInputs() for 2016 post.");
      }
    } // isData_
    // JER
    JERResoName_     = "Summer20UL16_JRV3_MC_PtResolution_AK4PFchs";
    JERSFName_       = "Summer20UL16_JRV3_MC_ScaleFactor_AK4PFchs";
    // Photon SS
    phoSsJsonPath_   = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName_       = "2022Re-recoBCD_SmearingJSON";
    // Electron SS
    eleSsJsonPath_   = "POG/EGM/electronSS.json";
    eleSsName_       = "2022Re-recoBCD_ScaleJSON";
    // Lumi golden JSON
    lumiJsonPath_    = "POG/LUM/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt";
    puJsonPath_      = "POG/LUM/2016postVFP_UL/puWeights.json";
    puName_          = "Collisions16_UltraLegacy_goldenJSON";
  }

  else if (globalFlags_.getYear() == GlobalFlag::Year::Year2017) {
    // Jet veto
    jetVetoJsonPath_ = "POG/JME/2017_UL/jetvetomaps.json.gz";
    jetVetoName_     = "Summer19UL17_V1";
    jetVetoKey_      = "jetvetomap_hot";
    // Jet L1, L2, L3
    jercJsonPath_          = "POG/JME/2017_UL/jet_jerc.json.gz";
    jetL1FastJetName_      = "Summer19UL17_V5_MC_L1FastJet_AK4PFchs";
    jetL2RelativeName_     = "Summer19UL17_V5_MC_L2Relative_AK4PFchs";
    jetL3AbsoluteName_     = "Summer19UL17_V5_MC_L3Absolute_AK4PFchs";
    jetL2L3ResidualName_   = "Summer19UL17_V5_MC_L2L3Residual_AK4PFchs";
    if (globalFlags_.isData()) {
      if (globalFlags_.getEra() == GlobalFlag::Era::Era2017B) {
        jetL1FastJetName_    = "Summer19UL17_RunB_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunB_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunB_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunB_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (globalFlags_.getEra() == GlobalFlag::Era::Era2017C) {
        jetL1FastJetName_    = "Summer19UL17_RunC_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunC_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunC_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunC_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (globalFlags_.getEra() == GlobalFlag::Era::Era2017D) {
        jetL1FastJetName_    = "Summer19UL17_RunD_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunD_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunD_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunD_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (globalFlags_.getEra() == GlobalFlag::Era::Era2017E) {
        jetL1FastJetName_    = "Summer19UL17_RunE_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunE_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunE_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunE_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (globalFlags_.getEra() == GlobalFlag::Era::Era2017F) {
        jetL1FastJetName_    = "Summer19UL17_RunF_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunF_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunF_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunF_V5_DATA_L2L3Residual_AK4PFchs";
      } else {
        throw std::runtime_error("Error: inputs are not set for data in ObjectScale::setInputs() for 2017.");
      }
    } // isData_
    // JER
    JERResoName_     = "Summer19UL17_JRV2_MC_PtResolution_AK4PFchs";
    JERSFName_       = "Summer19UL17_JRV2_MC_ScaleFactor_AK4PFchs";
    // Photon SS
    phoSsJsonPath_   = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName_       = "2022Re-recoBCD_SmearingJSON";
    // Electron SS
    eleSsJsonPath_   = "POG/EGM/electronSS.json";
    eleSsName_       = "2022Re-recoBCD_ScaleJSON";
    // Lumi golden JSON
    lumiJsonPath_    = "POG/LUM/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt";
    puJsonPath_      = "POG/LUM/2017_UL/puWeights.json";
    puName_          = "Collisions17_UltraLegacy_goldenJSON";
  } 

  else if (globalFlags_.getYear() == GlobalFlag::Year::Year2018) {
    // Jet veto
    jetVetoJsonPath_ = "POG/JME/2018_UL/jetvetomaps.json.gz";
    jetVetoName_     = "Summer19UL18_V1";
    jetVetoKey_      = "jetvetomap_hot";
    // Jet L1, L2, L3
    jercJsonPath_          = "POG/JME/2018_UL/jet_jerc.json.gz";
    jetL1FastJetName_      = "Summer19UL18_V5_MC_L1FastJet_AK4PFchs";
    jetL2RelativeName_     = "Summer19UL18_V5_MC_L2Relative_AK4PFchs";
    jetL3AbsoluteName_     = "Summer19UL18_V5_MC_L3Absolute_AK4PFchs";
    jetL2L3ResidualName_   = "Summer19UL18_V5_MC_L2L3Residual_AK4PFchs";
    if (globalFlags_.isData()) {
      if (globalFlags_.getEra() == GlobalFlag::Era::Era2018A) {
        jetL1FastJetName_    = "Summer19UL18_RunA_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL18_RunA_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL18_RunA_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL18_RunA_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (globalFlags_.getEra() == GlobalFlag::Era::Era2018B) {
        jetL1FastJetName_    = "Summer19UL18_RunB_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL18_RunB_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL18_RunB_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL18_RunB_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (globalFlags_.getEra() == GlobalFlag::Era::Era2018C) {
        jetL1FastJetName_    = "Summer19UL18_RunC_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL18_RunC_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL18_RunC_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL18_RunC_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (globalFlags_.getEra() == GlobalFlag::Era::Era2018D) {
        jetL1FastJetName_    = "Summer19UL18_RunD_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL18_RunD_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL18_RunD_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL18_RunD_V5_DATA_L2L3Residual_AK4PFchs";
      } else {
        throw std::runtime_error("Error: inputs are not set for data in ObjectScale::setInputs() for 2018.");
      }
    } // isData_
    // JER
    JERResoName_     = "Summer19UL18_JRV2_MC_PtResolution_AK4PFchs";
    JERSFName_       = "Summer19UL18_JRV2_MC_ScaleFactor_AK4PFchs";
    // Photon SS
    phoSsJsonPath_   = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName_       = "2022Re-recoBCD_SmearingJSON";
    // Electron SS
    eleSsJsonPath_   = "POG/EGM/electronSS.json";
    eleSsName_       = "2022Re-recoBCD_ScaleJSON";
    // Lumi golden JSON
    lumiJsonPath_    = "POG/LUM/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt";
    puJsonPath_      = "POG/LUM/2018_UL/puWeights.json";
    puName_          = "Collisions18_UltraLegacy_goldenJSON";
  }//is2018
  else {
    throw std::runtime_error("Error: Year is not specified correctly in ObjectScale::setInputs().");
  }

  std::cout<<"jetVetoJsonPath        = " << jetVetoJsonPath_      <<'\n';
  std::cout<<"jetVetoKey             = " << jetVetoKey_           <<'\n';
  std::cout<<"jetVetoName            = " << jetVetoName_          <<'\n'<<'\n';
  std::cout<<"jercJsonPath           = " << jercJsonPath_         <<'\n';
  std::cout<<"jetL1FastJetName       = " << jetL1FastJetName_     <<'\n';
  std::cout<<"jetL2RelativeName      = " << jetL2RelativeName_    <<'\n';
  std::cout<<"jetL2L3ResidualName    = " << jetL2L3ResidualName_  <<'\n';
  std::cout<<"JERResoName            = " << JERResoName_          <<'\n';
  std::cout<<"JERSFName              = " << JERSFName_            <<'\n'<<'\n';
  std::cout<<"phoSsJsonPath          = " << phoSsJsonPath_        <<'\n';
  std::cout<<"phoSsName              = " << phoSsName_            <<'\n'<<'\n';
  std::cout<<"eleSsJsonPath          = " << eleSsJsonPath_        <<'\n';
  std::cout<<"eleSsName              = " << eleSsName_            <<'\n'<<'\n';
  std::cout<<"lumiJsonPath           = " << lumiJsonPath_         <<'\n';
  std::cout<<"puJsonPath             = " << puJsonPath_           <<'\n';
  std::cout<<"puName                 = " << puName_               <<'\n';
}//setInputs 


void ObjectScale::loadJetVetoRef() {
  std::cout << "==> loadJetVetoRef()" << '\n';
  try {
    loadedJetVetoRef_ = correction::CorrectionSet::from_file(jetVetoJsonPath_)->at(jetVetoName_);
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: ObjectScale::loadJetVetoRef()" << '\n';
    std::cerr << "Check " << jetVetoJsonPath_ << " or " << jetVetoName_ << '\n';
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to load Jet Veto Reference");
  }
}

auto ObjectScale::checkJetVetoMap() const -> bool {
  bool isVeto = false;
  const double maxEtaInMap = 5.191;
  const double maxPhiInMap = 3.1415926;

  try {
    for (int i = 0; i != skimTree_->nJet; ++i) {
      if (std::abs(skimTree_->Jet_eta[i]) > maxEtaInMap) continue;
      if (std::abs(skimTree_->Jet_phi[i]) > maxPhiInMap) continue;

      auto jvNumber = loadedJetVetoRef_->evaluate({jetVetoKey_, skimTree_->Jet_eta[i], skimTree_->Jet_phi[i]});
      if (globalFlags_.isDebug()) {
        std::cout << jetVetoKey_
            << ", jetEta= " << skimTree_->Jet_eta[i]
            << ", jetPhi= " << skimTree_->Jet_phi[i]
            << ", jetVetoNumber = " << jvNumber << '\n';
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

void ObjectScale::loadJetL1FastJetRef() {
  std::cout << "==> loadJetL1FastJetRef()" << '\n';
  try {
    loadedJetL1FastJetRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(jetL1FastJetName_);
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: ObjectScale::loadJetL1FastJetRef" << '\n';
    std::cerr << "Check " << jercJsonPath_ << " or " << jetL1FastJetName_ << '\n';
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to load Jet L1 Fast Jet Reference");
  }
}

auto ObjectScale::getL1FastJetCorrection(double jetArea, double jetEta, double jetPt, double rho) const -> double {
  double corrL1FastJet = 1.0;
  try {
    corrL1FastJet = loadedJetL1FastJetRef_->evaluate({jetArea, jetEta, jetPt, rho});
    if (globalFlags_.isDebug()) {
      std::cout << "jetArea = " << jetArea
            << ", jetEta= " << jetEta
            << ", jetPt= " << jetPt
            << ", rho = " << rho
            << ", corrL1FastJet = " << corrL1FastJet << '\n';
    }
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: in getL1FastJetCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Failed to get L1 Fast Jet Correction");
  }
  return corrL1FastJet;
}

void ObjectScale::loadJetL2RelativeRef() {
  std::cout << "==> loadJetL2RelativeRef()" << '\n';
  try {
    loadedJetL2RelativeRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(jetL2RelativeName_);
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: ObjectScale::loadJetL2RelativeRef" << '\n';
    std::cerr << "Check " << jercJsonPath_ << " or " << jetL2RelativeName_ << '\n';
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to load Jet L2 Relative Reference");
  }
}

auto ObjectScale::getL2RelativeCorrection(double jetEta, double jetPt) const -> double {
  double corrL2Relative = 1.0;
  try {
    corrL2Relative = loadedJetL2RelativeRef_->evaluate({jetEta, jetPt});
    if (globalFlags_.isDebug()) {
      std::cout << "jetEta= " << jetEta
            << ", jetPt= " << jetPt
            << ", corrL2Relative = " << corrL2Relative << '\n';
    }
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: in getL2RelativeCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Failed to get L2 Relative Correction");
  }
  return corrL2Relative;
}

void ObjectScale::loadJetL2L3ResidualRef() {
  std::cout << "==> loadJetL2L3ResidualRef()" << '\n';
  try {
    loadedJetL2L3ResidualRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(jetL2L3ResidualName_);
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: ObjectScale::loadJetL2L3ResidualRef" << '\n';
    std::cerr << "Check " << jercJsonPath_ << " or " << jetL2L3ResidualName_ << '\n';
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to load Jet L2 L3 Residual Reference");
  }
}

auto ObjectScale::getL2L3ResidualCorrection(double jetEta, double jetPt) const -> double {
  double corrL2L3Residual = 1.0;
  try {
    corrL2L3Residual = loadedJetL2L3ResidualRef_->evaluate({jetEta, jetPt});
    if (globalFlags_.isDebug()) {
      std::cout << ", jetEta= " << jetEta
            << ", jetPt= " << jetPt
            << ", corrL2L3Residual = " << corrL2L3Residual << '\n';
    }
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: in getL2L3ResidualCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Failed to get L2 L3 Residual Correction");
  }
  return corrL2L3Residual;
}


void ObjectScale::loadJERResoRef() {
  std::cout << "==> loadJERResoRef()" << '\n';
  try {
    loadedJERResoRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(JERResoName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ObjectScale::loadJERResoRef" << '\n';
    std::cout << "Check " << jercJsonPath_ << " or " << JERResoName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading JER Resolution Reference.");
  }
}

auto ObjectScale::getJERResolution(int index) const -> double {
  double JERReso = 1.0;
  try {
    JERReso = loadedJERResoRef_->evaluate({skimTree_->Jet_eta[index], skimTree_->Jet_pt[index], skimTree_->Rho});
    if (globalFlags_.isDebug()) std::cout << ", jetEta= " << skimTree_->Jet_eta[index]
              << ", jetPt= " << skimTree_->Jet_pt[index]
              << ", rho = " << skimTree_->Rho
              << ", JERReso = " << JERReso << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in getJERResolution(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating JER Resolution.");
  }
  return JERReso;
}

void ObjectScale::loadJERSFRef() {
  std::cout << "==> loadJERSFRef()" << '\n';
  try {
    loadedJERSFRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(JERSFName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ObjectScale::loadJERSFRef" << '\n';
    std::cout << "Check " << jercJsonPath_ << " or " << JERSFName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading JER Scale Factor Reference.");
  }
}

auto ObjectScale::getJERScaleFactor(int index, const std::string &syst) const -> double {
  double JERSF = 1.0;
  try {
    JERSF = loadedJERSFRef_->evaluate({skimTree_->Jet_eta[index], skimTree_->Jet_pt[index], syst});
    if (globalFlags_.isDebug()) std::cout << ", jeteta= " << skimTree_->Jet_eta[index]
              << ", syst  = " << syst
              << ", JERSF = " << JERSF << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in getJERScaleFactor(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating JER Scale Factor.");
  }
  return JERSF;
}

//-------------------------------------
// JER Correction
//-------------------------------------
auto ObjectScale::getJERCorrection(int index, const std::string &syst) const -> double {
  double resoJER = getJERResolution(index);
  double sfJER = getJERScaleFactor(index, syst);
  double corrJER = 1.0;
  double eta = skimTree_->Jet_eta[index];
  double pt = skimTree_->Jet_pt[index];
  double phi = skimTree_->Jet_phi[index];
  int genIdx = skimTree_->Jet_genJetIdx[index];
  generator->SetSeed(skimTree_->event + skimTree_->run + skimTree_->luminosityBlock);
  bool isMatch = false;
  if ((genIdx > -1) && (genIdx < skimTree_->nGenJet)) {
    double delR = DELTAR(phi, skimTree_->GenJet_phi[genIdx], eta, skimTree_->GenJet_eta[genIdx]);
    if (delR < 0.2 && std::abs(pt - skimTree_->GenJet_pt[genIdx]) < 3 * resoJER * pt) {
      isMatch = true;
    }
  }
  if (isMatch) { // scaling method
    corrJER = std::max(0.0, 1. + (sfJER - 1.) * (pt - skimTree_->GenJet_pt[genIdx]) / pt);
  } else { // stochastic smearing
    corrJER = std::max(0.0, 1 + generator->Gaus(0, resoJER) * sqrt(std::max(sfJER * sfJER - 1, 0.)));
    if (globalFlags_.isDebug()) {
      std::cout << "Resolution = " << resoJER << ", sfJER = " << sfJER << ", cJER_Twiki = " << corrJER << '\n';
    }
  }
  return corrJER;
}

//-------------------------------------
// Photon Scale and Smearing
//-------------------------------------
void ObjectScale::loadPhoSsRef() {
  std::cout << "==> loadPhoSsRef()" << '\n';
  try {
    loadedPhoSsRef_ = correction::CorrectionSet::from_file(phoSsJsonPath_)->at(phoSsName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ObjectScale::loadPhoSsRef()" << '\n';
    std::cout << "Check " << phoSsJsonPath_ << " or " << phoSsName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading Photon Scale and Smearing Reference.");
  }
}

auto ObjectScale::getPhoScaleCorrection(const std::string &nomOrSyst, int indexPho) const -> double {
  double phoScaleSF = 1.0;
  try {
    phoScaleSF = loadedPhoSsRef_->evaluate({
      nomOrSyst,
      skimTree_->Photon_seedGain[indexPho],
      static_cast<Float_t>(skimTree_->run),
      skimTree_->Photon_eta[indexPho],
      skimTree_->Photon_r9[indexPho],
      skimTree_->Photon_pt[indexPho]
    });
    if (globalFlags_.isDebug()) std::cout << "nomOrSyst = " << nomOrSyst
              << ", Photon_seedGain = " << skimTree_->Rho
              << ", run = " << skimTree_->run
              << ", Photon_eta= " << skimTree_->Photon_eta[indexPho]
              << ", Photon_r9 = " << skimTree_->Photon_r9[indexPho]
              << ", Photon_pt = " << skimTree_->Photon_pt[indexPho]
              << ", phoScaleSF= " << phoScaleSF << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in ObjectScale::getPhoScaleCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating Photon Scale Correction.");
  }
  return phoScaleSF;
}

auto ObjectScale::getPhoSmearCorrection(const std::string &nomOrSyst, int indexPho) const -> double {
  double phoSmearSF = 1.0;
  try {
    phoSmearSF = loadedPhoSsRef_->evaluate({
      nomOrSyst,
      skimTree_->Photon_eta[indexPho],
      skimTree_->Photon_r9[indexPho]
    });
    if (globalFlags_.isDebug()) std::cout << "nomOrSyst = " << nomOrSyst
              << ", Photon_eta= " << skimTree_->Photon_eta[indexPho]
              << ", Photon_r9 = " << skimTree_->Photon_r9[indexPho]
              << ", phoSmearSF= " << phoSmearSF << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in ObjectSmear::getPhoSmearCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating Photon Smear Correction.");
  }
  return phoSmearSF;
}

//-------------------------------------
// Electron Scale and Smearing
//-------------------------------------
void ObjectScale::loadEleSsRef() {
  std::cout << "==> loadEleSsRef()" << '\n';
  try {
    loadedEleSsRef_ = correction::CorrectionSet::from_file(eleSsJsonPath_)->at(eleSsName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ObjectScale::loadEleSsRef()" << '\n';
    std::cout << "Check " << eleSsJsonPath_ << " or " << eleSsName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading Electron Scale and Smearing Reference.");
  }
}

//-------------------------------------
// Golden lumi Json
//-------------------------------------
void ObjectScale::loadLumiJson() {
  std::cout << "==> loadLumiJson()" << '\n';
  std::ifstream file(lumiJsonPath_);
  file >> loadedLumiJson_;
}

auto ObjectScale::checkGoodLumi(const unsigned int &run, const unsigned int &lumi) const -> bool {
  try {
    auto it = loadedLumiJson_.find(std::to_string(run));
    if (it == loadedLumiJson_.end()) {
      return false;
    }
    for (const auto &lumiBlock : it.value()) {
      if (lumi >= lumiBlock[0] && lumi <= lumiBlock[1]) {
        return true;
      }
    }
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ObjectScale::checkGoodLumi()" << '\n';
    std::cout << "Run = " << run << ", Lumi = " << lumi << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error checking good luminosity.");
  }
  return false;
}


//-------------------------------------
// Pileup Json 
//-------------------------------------
void ObjectScale::loadPuRef() {
  std::cout << "==> loadPuRef()" << '\n';
  try {
    loadedPuRef_ = correction::CorrectionSet::from_file(puJsonPath_)->at(puName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ObjectScale::loadPuRef()" << '\n';
    std::cout << "Check " << puJsonPath_ << " or " << puName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading Pileup Reference.");
  }
}

auto ObjectScale::getPuCorrection(Float_t nTrueInt, const std::string &nomOrSyst) const -> double {
  double puSF = 1.0;
  try {
    puSF = loadedPuRef_->evaluate({nTrueInt, nomOrSyst.c_str()});
    if (globalFlags_.isDebug()) std::cout << "nomOrSyst = " << nomOrSyst 
              << ", nTrueInt = " << nTrueInt 
              << ", puSF= " << puSF << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in ObjectScale::getPuCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating Pileup Correction.");
  }
  return puSF;
}

void ObjectScale::setThresh() {
  double frac = 0.5;
  bThresh = 0.4184;
  cThresh = 0.137 + frac * (0.66 - 0.137);
}

auto ObjectScale::DELTAPHI(double phi1, double phi2) const -> double {
  double dphi = fabs(phi1 - phi2);
  return (dphi <= TMath::Pi() ? dphi : TMath::TwoPi() - dphi);
}

auto ObjectScale::DELTAR(double phi1, double phi2, double eta1, double eta2) const -> double {
  return sqrt(pow(DELTAPHI(phi1, phi2), 2) + pow(eta1 - eta2, 2));
}
 

