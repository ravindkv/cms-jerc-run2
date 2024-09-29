#include"ObjectScale.h"
#include<iostream>

void ObjectScale::setInputs(){
  if (is2016Pre){
    // Jet veto
    jetVetoJsonPath = "POG/JME/2016preVFP_UL/jetvetomaps.json.gz";
    jetVetoName     = "Summer19UL16_V1";
    jetVetoKey      = "jetvetomap_hot";
    // Jet L1, L2, L3
    jercJsonPath          = "POG/JME/2016preVFP_UL/jet_jerc.json.gz";
    jetL1FastJetName      = "Summer19UL16APV_V7_MC_L1FastJet_AK4PFchs";
    jetL2RelativeName     = "Summer19UL16APV_V7_MC_L2Relative_AK4PFchs";
    jetL3AbsoluteName     = "Summer19UL16APV_V7_MC_L3Absolute_AK4PFchs";
    jetL2L3ResidualName   = "Summer19UL16APV_V7_MC_L2L3Residual_AK4PFchs";
    if (isData){
      if(is2016PreBCD){
        jetL1FastJetName    = "Summer19UL16APV_RunBCD_V7_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL16APV_RunBCD_V7_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL16APV_RunBCD_V7_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL16APV_RunBCD_V7_DATA_L2L3Residual_AK4PFchs";
      }
      else if(is2016PreEF){
        jetL1FastJetName    = "Summer19UL16APV_RunEF_V7_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL16APV_RunEF_V7_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL16APV_RunEF_V7_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL16APV_RunEF_V7_DATA_L2L3Residual_AK4PFchs";
      }
      else{
        cout<<setw(25)<<"Error: inputs are not set for data in ObjectScale::setInputs()"<<endl;
        std::abort();
      }
    }//isData
    //JER 
    JERResoName     = "Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs";
    JERSFName       = "Summer20UL16APV_JRV3_MC_ScaleFactor_AK4PFchs";
    //Photon SS 
    phoSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName     = "2022Re-recoBCD_SmearingJSON";
    //Electron SS 
    eleSsJsonPath = "POG/EGM/electronSS.json";
    eleSsName     = "2022Re-recoBCD_ScaleJSON";
    //Lumi golden JSON
    lumiJsonPath = "POG/LUM/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt";
  }

  else if (is2016Post){
    // Jet veto
    jetVetoJsonPath = "POG/JME/2016postVFP_UL/jetvetomaps.json.gz";
    jetVetoName     = "Summer19UL16_V1";
    jetVetoKey      = "jetvetomap_hot";
    // Jet L1, L2, L3
    jercJsonPath          = "POG/JME/2016postVFP_UL/jet_jerc.json.gz";
    jetL1FastJetName      = "Summer19UL16_V7_MC_L1FastJet_AK4PFchs";
    jetL2RelativeName     = "Summer19UL16_V7_MC_L2Relative_AK4PFchs";
    jetL3AbsoluteName     = "Summer19UL16_V7_MC_L3Absolute_AK4PFchs";
    jetL2L3ResidualName   = "Summer19UL16_V7_MC_L2L3Residual_AK4PFchs";
    if (isData){
      if(is2016PostFGH){
        jetL1FastJetName    = "Summer19UL16_RunFGH_V7_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL16_RunFGH_V7_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL16_RunFGH_V7_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL16_RunFGH_V7_DATA_L2L3Residual_AK4PFchs";
      }
      else{
        cout<<setw(25)<<"Error: inputs are not set for data in ObjectScale::setInputs()"<<endl;
        std::abort();
      }
    }//isData
    //JER 
    JERResoName     = "Summer20UL16_JRV3_MC_PtResolution_AK4PFchs";
    JERSFName       = "Summer20UL16_JRV3_MC_ScaleFactor_AK4PFchs";
    //Photon SS 
    phoSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName     = "2022Re-recoBCD_SmearingJSON";
    //Electron SS 
    eleSsJsonPath = "POG/EGM/electronSS.json";
    eleSsName     = "2022Re-recoBCD_ScaleJSON";
    //Lumi golden JSON
    lumiJsonPath = "POG/LUM/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt";
  }

  else if (is2017){
    // Jet veto
    jetVetoJsonPath = "POG/JME/2017_UL/jetvetomaps.json.gz";
    jetVetoName     = "Summer19UL17_V1";
    jetVetoKey      = "jetvetomap_hot";
    // Jet L1, L2, L3
    jercJsonPath          = "POG/JME/2017_UL/jet_jerc.json.gz";
    jetL1FastJetName      = "Summer19UL17_V5_MC_L1FastJet_AK4PFchs";
    jetL2RelativeName     = "Summer19UL17_V5_MC_L2Relative_AK4PFchs";
    jetL3AbsoluteName     = "Summer19UL17_V5_MC_L3Absolute_AK4PFchs";
    jetL2L3ResidualName   = "Summer19UL17_V5_MC_L2L3Residual_AK4PFchs";
    if (isData){
      if(is2017B){
        jetL1FastJetName    = "Summer19UL17_RunB_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL17_RunB_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL17_RunB_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL17_RunB_V5_DATA_L2L3Residual_AK4PFchs";
      }
      else if(is2017C){
        jetL1FastJetName    = "Summer19UL17_RunC_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL17_RunC_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL17_RunC_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL17_RunC_V5_DATA_L2L3Residual_AK4PFchs";
      }
      else if(is2017D){
        jetL1FastJetName    = "Summer19UL17_RunD_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL17_RunD_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL17_RunD_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL17_RunD_V5_DATA_L2L3Residual_AK4PFchs";
      }
      else if(is2017E){
        jetL1FastJetName    = "Summer19UL17_RunE_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL17_RunE_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL17_RunE_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL17_RunE_V5_DATA_L2L3Residual_AK4PFchs";
      }
      else if(is2017F){
        jetL1FastJetName    = "Summer19UL17_RunF_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL17_RunF_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL17_RunF_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL17_RunF_V5_DATA_L2L3Residual_AK4PFchs";
      }
      else{
        cout<<setw(25)<<"Error: inputs are not set for data in ObjectScale::setInputs()"<<endl;
        std::abort();
      }
    }//isData
    //JER 
    JERResoName     = "Summer19UL17_JRV2_MC_PtResolution_AK4PFchs";
    JERSFName       = "Summer19UL17_JRV2_MC_ScaleFactor_AK4PFchs";
    //Photon SS 
    phoSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName     = "2022Re-recoBCD_SmearingJSON";
    //Electron SS 
    eleSsJsonPath = "POG/EGM/electronSS.json";
    eleSsName     = "2022Re-recoBCD_ScaleJSON";
    //Lumi golden JSON
    lumiJsonPath = "POG/LUM/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt";
  }

  else if (is2018){
    // Jet veto
    jetVetoJsonPath = "POG/JME/2018_UL/jetvetomaps.json.gz";
    jetVetoName     = "Summer19UL18_V1";
    jetVetoKey      = "jetvetomap_hot";
    // Jet L1, L2, L3
    jercJsonPath          = "POG/JME/2018_UL/jet_jerc.json.gz";
    jetL1FastJetName      = "Summer19UL18_V5_MC_L1FastJet_AK4PFchs";
    jetL2RelativeName     = "Summer19UL18_V5_MC_L2Relative_AK4PFchs";
    jetL3AbsoluteName     = "Summer19UL18_V5_MC_L3Absolute_AK4PFchs";
    jetL2L3ResidualName   = "Summer19UL18_V5_MC_L2L3Residual_AK4PFchs";
    if (isData){
      if(is2018A){
        jetL1FastJetName    = "Summer19UL18_RunA_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL18_RunA_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL18_RunA_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL18_RunA_V5_DATA_L2L3Residual_AK4PFchs";
      }
      else if(is2018B){
        jetL1FastJetName    = "Summer19UL18_RunB_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL18_RunB_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL18_RunB_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL18_RunB_V5_DATA_L2L3Residual_AK4PFchs";
      }
      else if(is2018C){
        jetL1FastJetName    = "Summer19UL18_RunC_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL18_RunC_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL18_RunC_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL18_RunC_V5_DATA_L2L3Residual_AK4PFchs";
      }
      else if(is2018D){
        jetL1FastJetName    = "Summer19UL18_RunD_V5_DATA_L1FastJet_AK4PFchs";
        jetL2RelativeName   = "Summer19UL18_RunD_V5_DATA_L2Relative_AK4PFchs";
        jetL3AbsoluteName   = "Summer19UL18_RunD_V5_DATA_L3Absolute_AK4PFchs";
        jetL2L3ResidualName = "Summer19UL18_RunD_V5_DATA_L2L3Residual_AK4PFchs";
      }
      else{
        cout<<setw(25)<<"Error: inputs are not set for data in ObjectScale::setInputs()"<<endl;
        std::abort();
      }
    }//isData
    //JER 
    JERResoName     = "Summer19UL18_JRV2_MC_PtResolution_AK4PFchs";
    JERSFName       = "Summer19UL18_JRV2_MC_ScaleFactor_AK4PFchs";
    //Photon SS 
    phoSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    phoSsName     = "2022Re-recoBCD_SmearingJSON";
    //Electron SS 
    eleSsJsonPath = "POG/EGM/electronSS.json";
    eleSsName     = "2022Re-recoBCD_ScaleJSON";
    //Lumi golden JSON
    lumiJsonPath = "POG/LUM/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt";
  }//is2018

  else{
    cout<<setw(25)<<"Error: inputs are not set in ObjectScale::setInputs()"<<endl;
    std::abort();
  }
  cout<<"jetVetoJsonPath        = " << jetVetoJsonPath          <<endl;
  cout<<"jetVetoKey             = " << jetVetoKey               <<endl;
  cout<<"jetVetoName            = " << jetVetoName              <<endl<<endl;
  cout<<"jercJsonPath           = " << jercJsonPath             <<endl;
  cout<<"jetL1FastJetName       = " << jetL1FastJetName         <<endl;
  cout<<"jetL2RelativeName      = " << jetL2RelativeName        <<endl;
  cout<<"jetL2L3ResidualName    = " << jetL2L3ResidualName      <<endl;
  cout<<"JERResoName            = " << JERResoName              <<endl;
  cout<<"JERSFName              = " << JERSFName                <<endl<<endl;
  cout<<"phoSsJsonPath          = " << phoSsJsonPath            <<endl;
  cout<<"phoSsName              = " << phoSsName                <<endl<<endl;
  cout<<"eleSsJsonPath          = " << eleSsJsonPath            <<endl;
  cout<<"eleSsName              = " << eleSsName                <<endl<<endl;
  cout<<"lumiJsonPath           = " << lumiJsonPath             <<endl;
}//setInputs 

void ObjectScale::loadJetVetoRef(){
    cout<<"==> loadJetVetoRef()"<<endl;
    try{
        auto jvJson      = correction::CorrectionSet::from_file(jetVetoJsonPath);
        loadedJetVetoRef = jvJson->at(jetVetoName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetVetoRef()"<<endl;
        cout<<"Check "<<jetVetoJsonPath<<" or "<<jetVetoName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 
//https://cms-nanoaod-integration.web.cern.ch/commonJSONSFs/summaries/JME_2023_Summer23BPix_jetvetomaps.html
bool ObjectScale::checkJetVetoMap(SkimTree *tree){
    bool isVeto = false;
    double maxEtaInMap = 5.191;
    double maxPhiInMap = 3.1415926; 
    try{ 
      for (int i = 0; i != tree->nJet; ++i){
        if(abs(tree->Jet_eta[i]) > maxEtaInMap) continue;
        if(abs(tree->Jet_phi[i]) > maxPhiInMap) continue;
        auto jvNumber= loadedJetVetoRef->evaluate({jetVetoKey, tree->Jet_eta[i], tree->Jet_phi[i]});
        if(jvNumber>0) isVeto = true; 
      }
    } catch (const std::exception& e) {
      cout<<"\nEXCEPTION: in checkJetVetoMap(): "<<e.what()<<endl;
      std::abort();
    }
  return isVeto;
}

void ObjectScale::loadJetL1FastJetRef(){
     cout<<"==> loadJetL1FastJetRef()"<<endl;
    try{
        auto jercJson  = correction::CorrectionSet::from_file(jercJsonPath);
        loadedJetL1FastJetRef = jercJson->at(jetL1FastJetName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetL1FastJetRef"<<endl;
        cout<<"Check "<<jercJsonPath<<" or "<< jetL1FastJetName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 

double ObjectScale::getL1FastJetCorrection(double jetArea, double jetEta, double jetPt, double rho){
  double corrL1FastJet = 1.0;
  try{ 
    corrL1FastJet = loadedJetL1FastJetRef->evaluate({jetArea, jetEta, jetPt, rho}); 
    if(isDebug) cout<<"jetArea = "<<jetArea
                    <<", jetEta= "<<jetEta
                    <<", jetPt= "<<jetPt
                    <<", rho = "<<rho
                    <<", corrL1FastJet = "<<corrL1FastJet<<endl;
  } catch (const std::exception& e) {
    cout<<"\nEXCEPTION: in getL1FastJetCorrection(): "<<e.what()<<endl;
    std::abort();
  }
  return corrL1FastJet;
}

void ObjectScale::loadJetL2RelativeRef(){
     cout<<"==> loadJetL2RelativeRef()"<<endl;
    try{
        auto jercJson  = correction::CorrectionSet::from_file(jercJsonPath);
        loadedJetL2RelativeRef = jercJson->at(jetL2RelativeName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetL2RelativeRef"<<endl;
        cout<<"Check "<<jercJsonPath<<" or "<<jetL2RelativeName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 

double ObjectScale::getL2RelativeCorrection(double jetEta,double jetPhi, double jetPt){
  double corrL2Relative = 1.0;
  try{ 
    corrL2Relative = loadedJetL2RelativeRef->evaluate({jetEta, jetPhi, jetPt}); 
    if(isDebug)cout <<", jetEta= "<<jetEta
                    <<", jetPhi= "<<jetPhi
                    <<", jetPt= "<<jetPt
                    <<", corrL2Relative = "<<corrL2Relative<<endl;
  } catch (const std::exception& e) {
    cout<<"\nEXCEPTION: in getL2RelativeCorrection(): "<<e.what()<<endl;
    std::abort();
  }
  return corrL2Relative;
}

void ObjectScale::loadJetL2L3ResidualRef(){
     cout<<"==> loadJetL2L3ResidualRef()"<<endl;
    try{
        auto jercJson  = correction::CorrectionSet::from_file(jercJsonPath);
        loadedJetL2L3ResidualRef = jercJson->at(jetL2L3ResidualName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetL2L3ResidualRef"<<endl;
        cout<<"Check "<<jercJsonPath<<" or "<< jetL2L3ResidualName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 

double ObjectScale::getL2L3ResidualCorrection(double jetEta, double jetPt){
  double corrL2L3Residual = 1.0;
  try{ 
    corrL2L3Residual = loadedJetL2L3ResidualRef->evaluate({jetEta, jetPt}); 
    if(isDebug)cout <<", jetEta= "<<jetEta
                    <<", jetPt= "<<jetPt
                    <<", corrL2L3Residual = "<<corrL2L3Residual<<endl;
  } catch (const std::exception& e) {
    cout<<"\nEXCEPTION: in getL2L3ResidualCorrection(): "<<e.what()<<endl;
    std::abort();
  }
  return corrL2L3Residual;
}

   
void ObjectScale::loadJERResoRef(){
     cout<<"==> loadJERResoRef()"<<endl;
    try{
        auto jercJson  = correction::CorrectionSet::from_file(jercJsonPath);
        loadedJERResoRef = jercJson->at(JERResoName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJERResoRef"<<endl;
        cout<<"Check "<<jercJsonPath<<" or "<< JERResoName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 
double ObjectScale::getJERResolution(SkimTree *tree, int index){
  double JERReso = 1.0;
  try{ 
    JERReso = loadedJERResoRef->evaluate({tree->Jet_eta[index], tree->Jet_pt[index], tree->Rho}); 
    if(isDebug)cout <<", jetEta= "<<tree->Jet_eta[index]
                    <<", jetPt= "<<tree->Jet_pt[index]
                    <<", rho = "<<tree->Rho
                    <<", JERReso = "<<JERReso<<endl;
  } catch (const std::exception& e) {
    cout<<"\nEXCEPTION: in getJERResolution(): "<<e.what()<<endl;
    std::abort();
  }
  return JERReso;
}

void ObjectScale::loadJERSFRef(){
     cout<<"==> loadJERSFRef()"<<endl;
    try{
        auto jercJson  = correction::CorrectionSet::from_file(jercJsonPath);
        loadedJERSFRef = jercJson->at(JERSFName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJERSFRef"<<endl;
        cout<<"Check "<<jercJsonPath<<" or"<< JERSFName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 
double ObjectScale::getJERScaleFactor(SkimTree *tree, int index, string syst){
  double JERSF = 1.0;
  try{ 
    JERSF = loadedJERSFRef->evaluate({tree->Jet_eta[index], tree->Jet_pt[index], syst}); 
    if(isDebug)cout <<", jeteta= "<<tree->Jet_eta[index]
                    <<", syst  = "<<syst
                    <<", JERSF = "<<JERSF<<endl;
  } catch (const std::exception& e) {
    cout<<"\nEXCEPTION: in getJERScaleFactor(): "<<e.what()<<endl;
    std::abort();
  }
  return JERSF;
}

//-------------------------------------
// JER Correction
//-------------------------------------
double ObjectScale::getJERCorrection(SkimTree *tree, int index, string syst){
  double resoJER = getJERResolution(tree, index);
  double sfJER   = getJERScaleFactor(tree, index, syst);
	double corrJER = 1.0;
	double eta = tree->Jet_eta[index];
	double pt  = tree->Jet_pt[index];
	double phi = tree->Jet_phi[index];
	int genIdx = tree->Jet_genJetIdx[index];
	generator->SetSeed(tree->event + tree->run + tree->luminosityBlock);
  bool isMatch = false;
  if ((genIdx>-1) && (genIdx < tree->nGenJet)){
  	double delR = DELTAR(phi, tree->GenJet_phi[genIdx], eta, tree->GenJet_eta[genIdx]); 
  	if(delR<0.2 && abs(pt - tree->GenJet_pt[genIdx])<3*resoJER*pt){
  	    isMatch = true;
  	}
  }
	if(isMatch){ //scaling method
        corrJER = max(0.0, 1. + (sfJER - 1.) * (pt - tree->GenJet_pt[genIdx])/pt);
    }else{ // stochastic smearing
	    corrJER = max(0.0, 1 + generator->Gaus(0, resoJER) * sqrt( max(sfJER*sfJER - 1, 0.) ));
      if(isDebug){
      cout<<"Resolution = "<<resoJER<< ", sfJER = "<<sfJER <<", cJER_Twiki = "<<corrJER<<endl;
      }
    }
  return corrJER;
}

//-------------------------------------
// Photon Scale and Smearing 
//-------------------------------------
void ObjectScale::loadPhoSsRef(){
  cout<<"==> loadPhoSsRef()"<<endl;
  auto ssJson      = correction::CorrectionSet::from_file(phoSsJsonPath);
  try{
    loadedPhoSsRef = ssJson->at(phoSsName);
  }catch (const std::exception & e){
    cout<<"\nEXCEPTION: ObjectScale::loadPhoSsRef()"<<endl;
    cout<<"Check "<<phoSsJsonPath<<" or "<<phoSsName<<endl;
    cout<<e.what()<<endl;
    std::abort();
  }
} 
double ObjectScale::getPhoScaleCorrection(string nomOrSyst, SkimTree *tree, int indexPho){
  double phoScaleSF = 1.0;
  try{ 
    phoScaleSF  = loadedPhoSsRef->evaluate({nomOrSyst, 
                tree->Photon_seedGain[indexPho], 
                static_cast<Float_t>(tree->run), 
                tree->Photon_eta[indexPho], 
                tree->Photon_r9[indexPho],
                tree->Photon_pt[indexPho]});
    if(isDebug)cout <<"nomOrSyst = "<<nomOrSyst 
                    <<", Photon_seedGain = "<<tree->Rho
                    <<", run = "<<tree->run
                    <<", Photon_eta= "<<tree->Photon_eta[indexPho]
                    <<", Photon_r9 = "<<tree->Photon_r9[indexPho]
                    <<", Photon_pt = "<<tree->Photon_pt[indexPho]
                    <<", phoScaleSF= "<<phoScaleSF<<endl;
  } catch (const std::exception& e) {
    cout<<"\nEXCEPTION: in ObjectScale::getPhoScaleCorrection(): "<<e.what()<<endl;
    std::abort();
  }
  return phoScaleSF;
}

double ObjectScale::getPhoSmearCorrection(string nomOrSyst, SkimTree *tree, int indexPho){
  double phoSmearSF = 1.0;
  try{ 
    phoSmearSF  = loadedPhoSsRef->evaluate({nomOrSyst, 
                tree->Photon_eta[indexPho], 
                tree->Photon_r9[indexPho]});
    if(isDebug)cout <<"nomOrSyst = "<<nomOrSyst 
                    <<", Photon_eta= "<<tree->Photon_eta[indexPho]
                    <<", Photon_r9 = "<<tree->Photon_r9[indexPho]
                    <<", phoSmearSF= "<<phoSmearSF<<endl;
  } catch (const std::exception& e) {
    cout<<"\nEXCEPTION: in ObjectSmear::getPhoSmearCorrection(): "<<e.what()<<endl;
    std::abort();
  }
  return phoSmearSF;
}

//-------------------------------------
// Electron Scale and Smearing 
//-------------------------------------
void ObjectScale::loadEleSsRef(){
  cout<<"==> loadEleSsRef()"<<endl;
  auto ssJson      = correction::CorrectionSet::from_file(eleSsJsonPath);
  try{
    loadedEleSsRef = ssJson->at(eleSsName);
  }catch (const std::exception & e){
    cout<<"\nEXCEPTION: ObjectScale::loadEleSsRef()"<<endl;
    cout<<"Check "<<eleSsJsonPath<<" or "<<eleSsName<<endl;
    cout<<e.what()<<endl;
    std::abort();
  }
} 

void ObjectScale::PrintInfo(string info, bool printcout){
    //*ferr << info << endl << flush;
    if (printcout) cout << info << endl << flush;
}
//-------------------------------------
// Golden lumi Json 
//-------------------------------------
void ObjectScale::loadLumiJson(){
  cout<<"==> loadLumiJson()"<<endl;
  std::ifstream file(lumiJsonPath);
  file >> loadedLumiJson;
}

bool ObjectScale::checkGoodLumi(unsigned int &run, unsigned int &lumi){
  try{
    auto it = loadedLumiJson.find(std::to_string(run));
    if (it != loadedLumiJson.end()) {
      for (const auto& range : it.value()) {
        if (lumi >= range[0] && lumi <= range[1]) {
            return true;
        }
      }
    }
  } catch (const std::exception& e) {
      cout<<e.what()<<" in the golden JSON"<<endl;
  }
  return false;
}

void ObjectScale::setThresh(){
  double frac = 0.5;
  bThresh = 0.4184;
  cThresh = 0.137+frac*(0.66-0.137);
}

double ObjectScale::DELTAPHI(double phi1, double phi2)
{
  double dphi = fabs(phi1 - phi2);
  return (dphi <= TMath::Pi() ? dphi : TMath::TwoPi() - dphi);
}
double ObjectScale::DELTAR(double phi1, double phi2, double eta1, double eta2)
{
  return sqrt(pow(DELTAPHI(phi1, phi2), 2) + pow(eta1 - eta2, 2));
}
