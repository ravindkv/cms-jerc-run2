#include"ScaleObject.h"
#include"Helper.h"
#include<iostream>
#include<stdexcept>  // For std::runtime_error

// Constructor implementation
ScaleObject::ScaleObject(GlobalFlag& globalFlags) : 
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    era_(globalFlags_.getEra()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()),
    isData_(globalFlags_.isData()),
    isMC_(globalFlags_.isMC()){
}

void ScaleObject::setInputs() {
  if (year_ == GlobalFlag::Year::Year2016Pre) {
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
    if (isData_) {
      if (era_ == GlobalFlag::Era::Era2016PreBCD) {
        jetL1FastJetName_    = "Summer19UL16APV_RunBCD_V7_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL16APV_RunBCD_V7_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL16APV_RunBCD_V7_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL16APV_RunBCD_V7_DATA_L2L3Residual_AK4PFchs";
      } else if (era_ == GlobalFlag::Era::Era2016PreEF) {
        jetL1FastJetName_    = "Summer19UL16APV_RunEF_V7_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL16APV_RunEF_V7_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL16APV_RunEF_V7_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL16APV_RunEF_V7_DATA_L2L3Residual_AK4PFchs";
      } else {
        throw std::runtime_error("Error: inputs are not set for data in ScaleObject::setInputs() for 2016 pre.");
      }
    } // isData_
    // Jer
    JerResoName_     = "Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs";
    JerSfName_       = "Summer20UL16APV_JRV3_MC_ScaleFactor_AK4PFchs";
    // Photon SS
    phoSsJsonPath_   = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName_       = "2022Re-recoBCD_SmearingJSON";
    //Muon Rochester
    muRochJsonPath_  = "POG/MUO/roccor/RoccoR2016aUL.txt";
    // Electron SS
    eleSsJsonPath_   = "POG/EGM/electronSS.json";
    eleSsName_       = "2022Re-recoBCD_ScaleJSON";
    // Lumi golden JSON
    lumiJsonPath_    = "POG/LUM/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt";
    puJsonPath_      = "POG/LUM/2016preVFP_UL/puWeights.json";
    puName_          = "Collisions16_UltraLegacy_goldenJSON";
	} 

  else if (year_ == GlobalFlag::Year::Year2016Post) {
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
    if (isData_) {
      if (era_ == GlobalFlag::Era::Era2016PostFGH) {
        jetL1FastJetName_    = "Summer19UL16_RunFGH_V7_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL16_RunFGH_V7_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL16_RunFGH_V7_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL16_RunFGH_V7_DATA_L2L3Residual_AK4PFchs";
      } else {
        throw std::runtime_error("Error: inputs are not set for data in ScaleObject::setInputs() for 2016 post.");
      }
    } // isData_
    // Jer
    JerResoName_     = "Summer20UL16_JRV3_MC_PtResolution_AK4PFchs";
    JerSfName_       = "Summer20UL16_JRV3_MC_ScaleFactor_AK4PFchs";
    // Photon SS
    phoSsJsonPath_   = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName_       = "2022Re-recoBCD_SmearingJSON";
    //Muon Rochester
    muRochJsonPath_  = "POG/MUO/roccor/RoccoR2016bUL.txt";
    // Electron SS
    eleSsJsonPath_   = "POG/EGM/electronSS.json";
    eleSsName_       = "2022Re-recoBCD_ScaleJSON";
    // Lumi golden JSON
    lumiJsonPath_    = "POG/LUM/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt";
    puJsonPath_      = "POG/LUM/2016postVFP_UL/puWeights.json";
    puName_          = "Collisions16_UltraLegacy_goldenJSON";
  }

  else if (year_ == GlobalFlag::Year::Year2017) {
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
    if (isData_) {
      if (era_ == GlobalFlag::Era::Era2017B) {
        jetL1FastJetName_    = "Summer19UL17_RunB_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunB_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunB_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunB_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (era_ == GlobalFlag::Era::Era2017C) {
        jetL1FastJetName_    = "Summer19UL17_RunC_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunC_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunC_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunC_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (era_ == GlobalFlag::Era::Era2017D) {
        jetL1FastJetName_    = "Summer19UL17_RunD_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunD_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunD_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunD_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (era_ == GlobalFlag::Era::Era2017E) {
        jetL1FastJetName_    = "Summer19UL17_RunE_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunE_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunE_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunE_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (era_ == GlobalFlag::Era::Era2017F) {
        jetL1FastJetName_    = "Summer19UL17_RunF_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL17_RunF_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL17_RunF_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL17_RunF_V5_DATA_L2L3Residual_AK4PFchs";
      } else {
        throw std::runtime_error("Error: inputs are not set for data in ScaleObject::setInputs() for 2017.");
      }
    } // isData_
    // Jer
    JerResoName_     = "Summer19UL17_JRV2_MC_PtResolution_AK4PFchs";
    JerSfName_       = "Summer19UL17_JRV2_MC_ScaleFactor_AK4PFchs";
    // Photon SS
    phoSsJsonPath_   = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName_       = "2022Re-recoBCD_SmearingJSON";
    //Muon Rochester
    muRochJsonPath_  = "POG/MUO/roccor/RoccoR2017UL.txt";
    // Electron SS
    eleSsJsonPath_   = "POG/EGM/electronSS.json";
    eleSsName_       = "2022Re-recoBCD_ScaleJSON";
    // Lumi golden JSON
    lumiJsonPath_    = "POG/LUM/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt";
    puJsonPath_      = "POG/LUM/2017_UL/puWeights.json";
    puName_          = "Collisions17_UltraLegacy_goldenJSON";
  } 

  else if (year_ == GlobalFlag::Year::Year2018) {
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
    if (isData_) {
      if (era_ == GlobalFlag::Era::Era2018A) {
        jetL1FastJetName_    = "Summer19UL18_RunA_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL18_RunA_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL18_RunA_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL18_RunA_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (era_ == GlobalFlag::Era::Era2018B) {
        jetL1FastJetName_    = "Summer19UL18_RunB_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL18_RunB_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL18_RunB_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL18_RunB_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (era_ == GlobalFlag::Era::Era2018C) {
        jetL1FastJetName_    = "Summer19UL18_RunC_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL18_RunC_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL18_RunC_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL18_RunC_V5_DATA_L2L3Residual_AK4PFchs";
      } else if (era_ == GlobalFlag::Era::Era2018D) {
        jetL1FastJetName_    = "Summer19UL18_RunD_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName_   = "Summer19UL18_RunD_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName_   = "Summer19UL18_RunD_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName_ = "Summer19UL18_RunD_V5_DATA_L2L3Residual_AK4PFchs";
      } else {
        throw std::runtime_error("Error: inputs are not set for data in ScaleObject::setInputs() for 2018.");
      }
    } // isData_
    // Jer
    JerResoName_     = "Summer19UL18_JRV2_MC_PtResolution_AK4PFchs";
    JerSfName_       = "Summer19UL18_JRV2_MC_ScaleFactor_AK4PFchs";
    // Photon SS
    phoSsJsonPath_   = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName_       = "2022Re-recoBCD_SmearingJSON";
    //Muon Rochester
    muRochJsonPath_  = "POG/MUO/roccor/RoccoR2018UL.txt";
    // Electron SS
    eleSsJsonPath_   = "POG/EGM/electronSS.json";
    eleSsName_       = "2022Re-recoBCD_ScaleJSON";
    // Lumi golden JSON
    lumiJsonPath_    = "POG/LUM/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt";
    puJsonPath_      = "POG/LUM/2018_UL/puWeights.json";
    puName_          = "Collisions18_UltraLegacy_goldenJSON";
  }//is2018
  else {
    throw std::runtime_error("Error: Year is not specified correctly in ScaleObject::setInputs().");
  }

  std::cout<<"jetVetoJsonPath        = " << jetVetoJsonPath_      <<'\n';
  std::cout<<"jetVetoKey             = " << jetVetoKey_           <<'\n';
  std::cout<<"jetVetoName            = " << jetVetoName_          <<'\n'<<'\n';
  std::cout<<"jercJsonPath           = " << jercJsonPath_         <<'\n';
  std::cout<<"jetL1FastJetName       = " << jetL1FastJetName_     <<'\n';
  std::cout<<"jetL2RelativeName      = " << jetL2RelativeName_    <<'\n';
  std::cout<<"jetL2L3ResidualName    = " << jetL2L3ResidualName_  <<'\n';
  std::cout<<"JerResoName            = " << JerResoName_          <<'\n';
  std::cout<<"JerSfName              = " << JerSfName_            <<'\n'<<'\n';
  std::cout<<"phoSsJsonPath          = " << phoSsJsonPath_        <<'\n';
  std::cout<<"phoSsName              = " << phoSsName_            <<'\n'<<'\n';
  std::cout<<"muRochJsonPath         = " << muRochJsonPath_       <<'\n'<<'\n';
  std::cout<<"eleSsJsonPath          = " << eleSsJsonPath_        <<'\n';
  std::cout<<"eleSsName              = " << eleSsName_            <<'\n'<<'\n';
  std::cout<<"lumiJsonPath           = " << lumiJsonPath_         <<'\n';
  std::cout<<"puJsonPath             = " << puJsonPath_           <<'\n';
  std::cout<<"puName                 = " << puName_               <<'\n';
}//setInputs 


void ScaleObject::loadJetVetoRef() {
  std::cout << "==> loadJetVetoRef()" << '\n';
  try {
    loadedJetVetoRef_ = correction::CorrectionSet::from_file(jetVetoJsonPath_)->at(jetVetoName_);
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: ScaleObject::loadJetVetoRef()" << '\n';
    std::cerr << "Check " << jetVetoJsonPath_ << " or " << jetVetoName_ << '\n';
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to load Jet Veto Reference");
  }
}

auto ScaleObject::checkJetVetoMap(const SkimTree& skimT) const -> bool {
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

void ScaleObject::loadJetL1FastJetRef() {
  std::cout << "==> loadJetL1FastJetRef()" << '\n';
  try {
    loadedJetL1FastJetRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(jetL1FastJetName_);
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: ScaleObject::loadJetL1FastJetRef" << '\n';
    std::cerr << "Check " << jercJsonPath_ << " or " << jetL1FastJetName_ << '\n';
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to load Jet L1 Fast Jet Reference");
  }
}

auto ScaleObject::getL1FastJetCorrection(double jetArea, double jetEta, double jetPt, double rho) const -> double {
  double corrL1FastJet = 1.0;
  try {
    corrL1FastJet = loadedJetL1FastJetRef_->evaluate({jetArea, jetEta, jetPt, rho});
    if (isDebug_) {std::cout 
            << "jetArea = " << jetArea
            << ", jetEta= " << jetEta
            << ", jetPt= " << jetPt
            << ", rho = " << rho
            << ", corrL1FastJet = " << corrL1FastJet 
            << '\n';
    }
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: in getL1FastJetCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Failed to get L1 Fast Jet Correction");
  }
  return corrL1FastJet;
}

void ScaleObject::loadJetL2RelativeRef() {
  std::cout << "==> loadJetL2RelativeRef()" << '\n';
  try {
    loadedJetL2RelativeRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(jetL2RelativeName_);
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: ScaleObject::loadJetL2RelativeRef" << '\n';
    std::cerr << "Check " << jercJsonPath_ << " or " << jetL2RelativeName_ << '\n';
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to load Jet L2 Relative Reference");
  }
}

auto ScaleObject::getL2RelativeCorrection(double jetEta, double jetPt) const -> double {
  double corrL2Relative = 1.0;
  try {
    corrL2Relative = loadedJetL2RelativeRef_->evaluate({jetEta, jetPt});
    if (isDebug_) {std::cout 
            << "jetEta= " << jetEta
            << ", jetPt= " << jetPt
            << ", corrL2Relative = " 
            << corrL2Relative << '\n';
    }
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: in getL2RelativeCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Failed to get L2 Relative Correction");
  }
  return corrL2Relative;
}

void ScaleObject::loadJetL2L3ResidualRef() {
  std::cout << "==> loadJetL2L3ResidualRef()" << '\n';
  try {
    loadedJetL2L3ResidualRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(jetL2L3ResidualName_);
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: ScaleObject::loadJetL2L3ResidualRef" << '\n';
    std::cerr << "Check " << jercJsonPath_ << " or " << jetL2L3ResidualName_ << '\n';
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to load Jet L2 L3 Residual Reference");
  }
}

auto ScaleObject::getL2L3ResidualCorrection(double jetEta, double jetPt) const -> double {
  double corrL2L3Residual = 1.0;
  try {
    corrL2L3Residual = loadedJetL2L3ResidualRef_->evaluate({jetEta, jetPt});
    if (isDebug_) {std::cout 
            << ", jetEta= " << jetEta
            << ", jetPt= " << jetPt
            << ", corrL2L3Residual = " << corrL2L3Residual 
            << '\n';
    }
  } catch (const std::exception &e) {
    std::cerr << "\nEXCEPTION: in getL2L3ResidualCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Failed to get L2 L3 Residual Correction");
  }
  return corrL2L3Residual;
}


void ScaleObject::loadJerResoRef() {
  std::cout << "==> loadJerResoRef()" << '\n';
  try {
    loadedJerResoRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(JerResoName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ScaleObject::loadJerResoRef" << '\n';
    std::cout << "Check " << jercJsonPath_ << " or " << JerResoName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading Jer Resolution Reference.");
  }
}

auto ScaleObject::getJerResolution(const SkimTree& skimT, int index) const -> double {
  double JerReso = 1.0;
  try {
    JerReso = loadedJerResoRef_->evaluate({skimT.Jet_eta[index], skimT.Jet_pt[index], skimT.Rho});
    if (isDebug_) std::cout 
                << ", jetEta= " << skimT.Jet_eta[index]
                << ", jetPt= " << skimT.Jet_pt[index]
                << ", rho = " << skimT.Rho
                << ", JerReso = " << JerReso 
                << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in getJerResolution(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating Jer Resolution.");
  }
  return JerReso;
}

void ScaleObject::loadJerSfRef() {
  std::cout << "==> loadJerSfRef()" << '\n';
  try {
    loadedJerSfRef_ = correction::CorrectionSet::from_file(jercJsonPath_)->at(JerSfName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ScaleObject::loadJerSfRef" << '\n';
    std::cout << "Check " << jercJsonPath_ << " or " << JerSfName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading Jer Scale Factor Reference.");
  }
}

auto ScaleObject::getJerScaleFactor(const SkimTree& skimT, int index, const std::string &syst) const -> double {
  double JerSf = 1.0;
  try {
    JerSf = loadedJerSfRef_->evaluate({skimT.Jet_eta[index], syst});
    if (isDebug_) std::cout 
                << ", jeteta= " << skimT.Jet_eta[index]
                << ", syst  = " << syst
                << ", JerSf = " << JerSf 
                << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in getJerScaleFactor(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating Jer Scale Factor.");
  }
  return JerSf;
}

//-------------------------------------
// Jer Correction
//-------------------------------------
auto ScaleObject::getJerCorrection(const SkimTree& skimT, int index, const std::string &syst) const -> double {
  double resoJer = getJerResolution(skimT, index);
  double sfJer = getJerScaleFactor(skimT, index, syst);
  double corrJer = 1.0;
  double eta = skimT.Jet_eta[index];
  double pt = skimT.Jet_pt[index];
  double phi = skimT.Jet_phi[index];
  int genIdx = skimT.Jet_genJetIdx[index];
  randomNumGen->SetSeed(skimT.event + skimT.run + skimT.luminosityBlock);
  bool isMatch = false;
  if ((genIdx > -1) && (genIdx < skimT.nGenJet)) {
    double delR = Helper::DELTAR(phi, skimT.GenJet_phi[genIdx], eta, skimT.GenJet_eta[genIdx]);
    if (delR < 0.2 && std::abs(pt - skimT.GenJet_pt[genIdx]) < 3 * resoJer * pt) {
      isMatch = true;
    }
  }
  if (isMatch) { // scaling method
    corrJer = std::max(0.0, 1. + (sfJer - 1.) * (pt - skimT.GenJet_pt[genIdx]) / pt);
  } else { // stochastic smearing
    corrJer = std::max(0.0, 1 + randomNumGen->Gaus(0, resoJer) * sqrt(std::max(sfJer * sfJer - 1, 0.)));
    if (isDebug_) {std::cout 
                << "Resolution = " 
                << resoJer << ", sfJer = " 
                << sfJer << ", cJer_Twiki = " 
                << corrJer 
                << '\n';
    }
  }
  return corrJer;
}

//-------------------------------------
// Photon Scale and Smearing
//-------------------------------------
void ScaleObject::loadPhoSsRef() {
  std::cout << "==> loadPhoSsRef()" << '\n';
  try {
    loadedPhoSsRef_ = correction::CorrectionSet::from_file(phoSsJsonPath_)->at(phoSsName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ScaleObject::loadPhoSsRef()" << '\n';
    std::cout << "Check " << phoSsJsonPath_ << " or " << phoSsName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading Photon Scale and Smearing Reference.");
  }
}

auto ScaleObject::getPhoScaleCorrection(const SkimTree& skimT, const std::string &nomOrSyst, int indexPho) const -> double {
  double phoScaleSf = 1.0;
  try {
    phoScaleSf = loadedPhoSsRef_->evaluate({
      nomOrSyst,
      skimT.Photon_seedGain[indexPho],
      static_cast<Float_t>(skimT.run),
      skimT.Photon_eta[indexPho],
      skimT.Photon_r9[indexPho],
      skimT.Photon_pt[indexPho]
    });
    if (isDebug_) std::cout 
                << "nomOrSyst = " << nomOrSyst
                << ", Photon_seedGain = " << skimT.Rho
                << ", run = " << skimT.run
                << ", Photon_eta= " << skimT.Photon_eta[indexPho]
                << ", Photon_r9 = " << skimT.Photon_r9[indexPho]
                << ", Photon_pt = " << skimT.Photon_pt[indexPho]
                << ", phoScaleSf= " << phoScaleSf  
                << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in ScaleObject::getPhoScaleCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating Photon Scale Correction.");
  }
  return phoScaleSf;
}

auto ScaleObject::getPhoSmearCorrection(const SkimTree& skimT, const std::string &nomOrSyst, int indexPho) const -> double {
  double phoSmearSf = 1.0;
  try {
    phoSmearSf = loadedPhoSsRef_->evaluate({
      nomOrSyst,
      skimT.Photon_eta[indexPho],
      skimT.Photon_r9[indexPho]
    });
    if (isDebug_) std::cout 
                << "nomOrSyst = " << nomOrSyst
                << ", Photon_eta= " << skimT.Photon_eta[indexPho]
                << ", Photon_r9 = " << skimT.Photon_r9[indexPho]
                << ", phoSmearSf= " << phoSmearSf 
                << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in ObjectSmear::getPhoSmearCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating Photon Smear Correction.");
  }
  return phoSmearSf;
}

//-------------------------------------
// Muon Roch Correction
//-------------------------------------
void ScaleObject::loadMuRochRef() {
  std::cout << "==> loadMuRochRef()" << '\n';
  try {
    loadedRochRef_.init(muRochJsonPath_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ScaleObject::loadMuRochRef()" << '\n';
    std::cout << "Check " << muRochJsonPath_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading muon rochester corr file.");
  }
}
//
auto ScaleObject::getMuRochCorrection(const SkimTree& skimT, int index, const std::string &syst) const -> double {
    double corrMuRoch = 1.0;
    double Q   = skimT.Muon_charge[index];
    double pt  = skimT.Muon_pt[index];
    double eta = skimT.Muon_eta[index];
    double phi = skimT.Muon_phi[index];
    double s = 0.0;
    double m = 0.0;
    double genPt = -9999.;
    double u = -9999.;
    int nl = -9999;
    bool isMatched = false;
    
    if(isData_){
        corrMuRoch = loadedRochRef_.kScaleDT(Q, pt, eta, phi, s, m); 
    }
    if(isMC_){
        for (int i = 0; i < skimT.nGenDressedLepton; ++i) {
            if (std::abs(skimT.GenDressedLepton_pdgId[i]) == 13) {
                double delR = Helper::DELTAR(phi, skimT.GenDressedLepton_phi[i], eta, skimT.GenDressedLepton_eta[i]);
                if(delR < 0.2){
                    genPt = skimT.GenDressedLepton_pt[i];
                    isMatched = true;
                    break;
                }
            }//PDG
        }//for nGen
        if(isMatched){
            corrMuRoch = loadedRochRef_.kSpreadMC(Q, pt, eta, phi, genPt, s, m);
        }
        else{
            randomNumGen->SetSeed(skimT.event + skimT.run + skimT.luminosityBlock);
            u = randomNumGen->Uniform(0.0, 1.0);
            nl = skimT.Muon_nTrackerLayers[index];
            corrMuRoch = loadedRochRef_.kSmearMC(Q, pt, eta, phi, nl, u, s, m);
        }
    }//isMC
    if (isDebug_) std::cout 
                << ", Q = " << Q
                << ", pt = " << pt
                << ", eta = " << eta
                << ", phi = " << phi
                << ", genPt = " << genPt
                << ", nl = " << nl
                << ", u = " << u
                << ", s = " << s
                << ", m = " << m
                << ", isMatched = " << isMatched
                << ", corrMuRoch = " << corrMuRoch
                << '\n';
   return corrMuRoch; 
}

//-------------------------------------
// Electron Scale and Smearing
//-------------------------------------
void ScaleObject::loadEleSsRef() {
  std::cout << "==> loadEleSsRef()" << '\n';
  try {
    loadedEleSsRef_ = correction::CorrectionSet::from_file(eleSsJsonPath_)->at(eleSsName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ScaleObject::loadEleSsRef()" << '\n';
    std::cout << "Check " << eleSsJsonPath_ << " or " << eleSsName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading Electron Scale and Smearing Reference.");
  }
}
auto ScaleObject::getEleSsCorrection(const SkimTree& skimT, int index, const std::string &syst) const -> double {
    double corrEleSs = 1.0;
    double pt   = skimT.Electron_pt[index];
    double eta  = skimT.Electron_eta[index];
    double mass = skimT.Electron_mass[index];

    // Calculate energy from pt, eta, and mass
    double energy = std::sqrt(std::pow(pt * std::cosh(eta), 2) + std::pow(mass, 2));
    double corrE = skimT.Electron_eCorr[index] * energy;

    // Convert corrected energy back to new pT
    double newPt = std::sqrt((std::pow(corrE, 2) - std::pow(mass, 2)) / std::pow(std::cosh(eta), 2));
    corrEleSs = newPt/pt;

    if (isDebug_) std::cout 
                << ", pt = " << pt
                << ", eta = " << eta
                << ", energy = " << energy
                << ", eCorr = " << skimT.Electron_eCorr[index]
                << ", newPt = " << newPt
                << ", corrEleSs = " << corrEleSs
                << '\n';
   return corrEleSs; 
}

//-------------------------------------
// Golden lumi Json
//-------------------------------------
void ScaleObject::loadLumiJson() {
  std::cout << "==> loadLumiJson()" << '\n';
  std::ifstream file(lumiJsonPath_);
  file >> loadedLumiJson_;
}

auto ScaleObject::checkGoodLumi(const unsigned int &run, const unsigned int &lumi) const -> bool {
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
    std::cout << "\nEXCEPTION: ScaleObject::checkGoodLumi()" << '\n';
    std::cout << "Run = " << run << ", Lumi = " << lumi << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error checking good luminosity.");
  }
  return false;
}


//-------------------------------------
// Pileup Json 
//-------------------------------------
void ScaleObject::loadPuRef() {
  std::cout << "==> loadPuRef()" << '\n';
  try {
    loadedPuRef_ = correction::CorrectionSet::from_file(puJsonPath_)->at(puName_);
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: ScaleObject::loadPuRef()" << '\n';
    std::cout << "Check " << puJsonPath_ << " or " << puName_ << '\n';
    std::cout << e.what() << '\n';
    throw std::runtime_error("Error loading Pileup Reference.");
  }
}

auto ScaleObject::getPuCorrection(Float_t nTrueInt, const std::string &nomOrSyst) const -> double {
  double puSf = 1.0;
  try {
    puSf = loadedPuRef_->evaluate({nTrueInt, nomOrSyst.c_str()});
    if (isDebug_) std::cout << "nomOrSyst = " << nomOrSyst 
              << ", nTrueInt = " << nTrueInt 
              << ", puSf= " << puSf << '\n';
  } catch (const std::exception &e) {
    std::cout << "\nEXCEPTION: in ScaleObject::getPuCorrection(): " << e.what() << '\n';
    throw std::runtime_error("Error calculating Pileup Correction.");
  }
  return puSf;
}

