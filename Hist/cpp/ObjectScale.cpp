#include"ObjectScale.h"
#include<iostream>
   
//-------------------------------------
// Jet veto maps
//-------------------------------------
void ObjectScale::setJetVetoKey(TString oName){
     jetVetoKey = "jetvetomap";
     cout<<setw(25)<<"setJetVetoKey() = "<<jetVetoKey<<endl;
} 
   
void ObjectScale::setJetVetoName(TString oName){
     if(is22C || is22D){
         jetVetoName = "Winter22Run3_RunCD_V1";
     }
     if(is22E){
         jetVetoName = "Winter22Run3_RunE_V1";
     }
     if(is23A || is23B || is23C || is23Pre){
         jetVetoName = "Summer23Prompt23_RunC_V1";
     }
     if(is23D || is23E || is23F || is23G || is23Post){
         jetVetoName = "Summer23BPixPrompt23_RunD_V1";
     }
     if(is24){//FIXME
         jetVetoName = "Summer23BPixPrompt23_RunD_V1";
     }
     cout<<setw(25)<<"setJetVetoName() = "<<jetVetoName<<endl;
} 
   
void ObjectScale::setJetVetoJsonPath(TString oName){
     if(is22 || is22D){
         jetVetoJsonPath = "POG/JME/2022_Prompt/jetvetomaps.json.gz";
     }
     if(is23A || is23B || is23C || is23Pre){
         jetVetoJsonPath = "POG/JME/2023_Summer23/jetvetomaps.json.gz";
     }
     if(is23D || is23E || is23F || is23G || is23Post){
         jetVetoJsonPath = "POG/JME/2023_Summer23BPix/jetvetomaps.json.gz";
     }
     if(is24){//FIXME
         jetVetoJsonPath = "POG/JME/2023_Summer23BPix/jetvetomaps.json.gz";
     }
     cout<<setw(25)<<"setJetVetoJsonPath() = "<<jetVetoJsonPath<<endl;
} 

void ObjectScale::loadJetVetoRef(){
    cout<<"==> loadJetVetoRef()"<<endl;
    auto jvJson      = correction::CorrectionSet::from_file(jetVetoJsonPath);
    try{
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
      cout<<"\nEXCEPTION: in objS->checkJetVetoMap(): "<<e.what()<<endl;
      std::abort();
    }
  return isVeto;
}

//-------------------------------------
// Jet L1FastJet corrections
//-------------------------------------
void ObjectScale::setJetL1FastJetName(TString oName){
    if (isData){
        if(is22C){
            jetL1FastJetName = "Summer22_22Sep2023_RunCD_V2_DATA_L1FastJet_AK4PFPuppi";
        }
        if(is22D){
            jetL1FastJetName = "Summer22_22Sep2023_RunCD_V2_DATA_L1FastJet_AK4PFPuppi";
        }
        if(is22E){
            jetL1FastJetName = "Run22E-22Sep2023_DATA_L1FastJet_AK4PFPuppi";
        }
        if(is22F){
            jetL1FastJetName = "Run22F-Prompt_DATA_L1FastJet_AK4PFPuppi";
        }
        if(is22G){
            jetL1FastJetName = "Run22G-Prompt_DATA_L1FastJet_AK4PFPuppi";
        }
        if(is23A || is23B || is23Cv123){
            jetL1FastJetName = "Summer23Prompt23_RunCv123_V1_DATA_L1FastJet_AK4PFPuppi";
        }
        if(is23Cv4){
            jetL1FastJetName = "Summer23Prompt23_RunCv4_V1_DATA_L1FastJet_AK4PFPuppi";
        }
        if(is23D || is23E || is23F || is23G){
            jetL1FastJetName = "Summer23BPixPrompt23_RunD_V1_DATA_L1FastJet_AK4PFPuppi";
        }
        if(is24){//FIXME
            jetL1FastJetName = "Summer23BPixPrompt23_RunD_V1_DATA_L1FastJet_AK4PFPuppi";
        }
    }
    else{
        if(is22){ 
            jetL1FastJetName = "Summer22_22Sep2023_V2_MC_L1FastJet_AK4PFPuppi";
        }
        if(is23Pre){ 
            jetL1FastJetName = "Summer23Prompt23_V1_MC_L1FastJet_AK4PFPuppi";
        }
        if(is23Post){ 
            jetL1FastJetName = "Summer23BPixPrompt23_V1_MC_L1FastJet_AK4PFPuppi";
        }
        if(is24){//FIXME
            jetL1FastJetName = "Summer23BPixPrompt23_V1_MC_L1FastJet_AK4PFPuppi";
        }
    }
    cout<<setw(25)<<"setJetL1FastJetName() = ";
    cout<<jetL1FastJetName<<endl;
} 
   
void ObjectScale::setJetL1FastJetJsonPath(TString oName){
    if(is22 || is22D){
        jetL1FastJetJsonPath = "POG/JME/2022_Summer22/jet_jerc.json.gz";
    }
    if(is23A || is23B || is23C || is23Pre){
        jetL1FastJetJsonPath = "POG/JME/2023_Summer23/jet_jerc.json.gz";
    }
    if(is23D || is23E || is23F || is23G || is23Post){
        jetL1FastJetJsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    if(is24){//FIXME
        jetL1FastJetJsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    cout<<setw(25)<<"setJetL1FastJetJsonPath() = "<<jetL1FastJetJsonPath<<endl;
} 
   
void ObjectScale::loadJetL1FastJetRef(){
     cout<<"==> loadJetL1FastJetRef()"<<endl;
     auto jercJson  = correction::CorrectionSet::from_file(jetL1FastJetJsonPath);
    try{
        loadedJetL1FastJetRef = jercJson->at(jetL1FastJetName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetL1FastJetRef"<<endl;
        cout<<"Check "<<jetL1FastJetJsonPath<<" or jetL1FastJetName"<<endl;
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

//-------------------------------------
// Jet L2Relative corrections
//-------------------------------------
void ObjectScale::setJetL2RelativeName(TString oName){
    if (isData){
        if(is22C){
            jetL2RelativeName = "Summer22_22Sep2023_RunCD_V2_DATA_L2Relative_AK4PFPuppi";
        }
        if(is22D){
            jetL2RelativeName = "Summer22_22Sep2023_RunCD_V2_DATA_L2Relative_AK4PFPuppi";
        }
        if(is22E){
            jetL2RelativeName = "Run22E-22Sep2023_DATA_L2Relative_AK4PFPuppi";
        }
        if(is22F){
            jetL2RelativeName = "Run22F-Prompt_DATA_L2Relative_AK4PFPuppi";
        }
        if(is22G){
            jetL2RelativeName = "Run22G-Prompt_DATA_L2Relative_AK4PFPuppi";
        }
        if(is23A || is23B || is23Cv123){
            jetL2RelativeName = "Summer23Prompt23_RunCv123_V1_DATA_L2Relative_AK4PFPuppi";
        }
        if(is23Cv4){
            jetL2RelativeName = "Summer23Prompt23_RunCv4_V1_DATA_L2Relative_AK4PFPuppi";
        }
        if(is23D || is23E || is23F || is23G){
            jetL2RelativeName = "Summer23BPixPrompt23_RunD_V1_DATA_L2Relative_AK4PFPuppi";
        }
        if(is24){//FIXME
            jetL2RelativeName = "Summer23BPixPrompt23_RunD_V1_DATA_L2Relative_AK4PFPuppi";
        }
    }
    else{
        if(is22){ 
            jetL2RelativeName = "Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi";
        }
        if(is23Pre){ 
            jetL2RelativeName = "Summer23Prompt23_V1_MC_L2Relative_AK4PFPuppi";
        }
        if(is23Post){ 
            jetL2RelativeName = "Summer23BPixPrompt23_V1_MC_L2Relative_AK4PFPuppi";
        }
        if(is24){//FIXME
            jetL2RelativeName = "Summer23BPixPrompt23_V1_MC_L2Relative_AK4PFPuppi";
        }
    }
    cout<<setw(25)<<"setJetL2RelativeName() = ";
    cout<<jetL2RelativeName<<endl;
} 
   
void ObjectScale::setJetL2RelativeJsonPath(TString oName){
    if(is22 || is22D){
        jetL2RelativeJsonPath = "POG/JME/2022_Summer22/jet_jerc.json.gz";
    }
    if(is23A || is23B || is23C || is23Pre){
        jetL2RelativeJsonPath = "POG/JME/2023_Summer23/jet_jerc.json.gz";
    }
    if(is23D || is23E || is23F || is23G || is23Post){
        jetL2RelativeJsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    if(is24){//FIXME
        jetL2RelativeJsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    cout<<setw(25)<<"setJetL2RelativeJsonPath() = "<<jetL2RelativeJsonPath<<endl;
} 
   
void ObjectScale::loadJetL2RelativeRef(){
     cout<<"==> loadJetL2RelativeRef()"<<endl;
     auto jercJson  = correction::CorrectionSet::from_file(jetL2RelativeJsonPath);
    try{
        loadedJetL2RelativeRef = jercJson->at(jetL2RelativeName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetL2RelativeRef"<<endl;
        cout<<"Check "<<jetL2RelativeJsonPath<<" or jetL2RelativeName"<<endl;
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


//-------------------------------------
// Jet L2L3Residual corrections
//-------------------------------------
void ObjectScale::setJetL2L3ResidualName(TString oName){
    if (isData){
        if(is22C){
            jetL2L3ResidualName = "Summer22_22Sep2023_RunCD_V2_DATA_L2L3Residual_AK4PFPuppi";
        }
        if(is22D){
            jetL2L3ResidualName = "Summer22_22Sep2023_RunCD_V2_DATA_L2L3Residual_AK4PFPuppi";
        }
        if(is22E){
            jetL2L3ResidualName = "Run22E-22Sep2023_DATA_L2L3Residual_AK4PFPuppi";
        }
        if(is22F){
            jetL2L3ResidualName = "Run22F-Prompt_DATA_L2L3Residual_AK4PFPuppi";
        }
        if(is22G){
            jetL2L3ResidualName = "Run22G-Prompt_DATA_L2L3Residual_AK4PFPuppi";
        }
        if(is23A || is23B || is23Cv123){
            jetL2L3ResidualName = "Summer23Prompt23_RunCv123_V1_DATA_L2L3Residual_AK4PFPuppi";
        }
        if(is23Cv4){
            jetL2L3ResidualName = "Summer23Prompt23_RunCv4_V1_DATA_L2L3Residual_AK4PFPuppi";
        }
        if(is23D || is23E || is23F || is23G){
            jetL2L3ResidualName = "Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi";
        }
        if(is24){//FIXME
            jetL2L3ResidualName = "Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi";
        }
    }
    else{
        if(is22){ 
            jetL2L3ResidualName = "Summer22_22Sep2023_V2_MC_L2L3Residual_AK4PFPuppi";
        }
        if(is23Pre){ 
            jetL2L3ResidualName = "Summer23Prompt23_V1_MC_L2L3Residual_AK4PFPuppi";
        }
        if(is23Post){ 
            jetL2L3ResidualName = "Summer23BPixPrompt23_V1_MC_L2L3Residual_AK4PFPuppi";
        }
        if(is24){//FIXME
            jetL2L3ResidualName = "Summer23BPixPrompt23_V1_MC_L2L3Residual_AK4PFPuppi";
        }
    }
    cout<<setw(25)<<"setJetL2L3ResidualName() = ";
    cout<<jetL2L3ResidualName<<endl;
} 
   
void ObjectScale::setJetL2L3ResidualJsonPath(TString oName){
    if(is22 || is22D){
        jetL2L3ResidualJsonPath = "POG/JME/2022_Summer22/jet_jerc.json.gz";
    }
    if(is23A || is23B || is23C || is23Pre){
        jetL2L3ResidualJsonPath = "POG/JME/2023_Summer23/jet_jerc.json.gz";
    }
    if(is23D || is23E || is23F || is23G || is23Post){
        jetL2L3ResidualJsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    if(is24){//FIXME
        jetL2L3ResidualJsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    cout<<setw(25)<<"setJetL2L3ResidualJsonPath() = "<<jetL2L3ResidualJsonPath<<endl;
} 
   
void ObjectScale::loadJetL2L3ResidualRef(){
     cout<<"==> loadJetL2L3ResidualRef()"<<endl;
     auto jercJson  = correction::CorrectionSet::from_file(jetL2L3ResidualJsonPath);
    try{
        loadedJetL2L3ResidualRef = jercJson->at(jetL2L3ResidualName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetL2L3ResidualRef"<<endl;
        cout<<"Check "<<jetL2L3ResidualJsonPath<<" or jetL2L3ResidualName"<<endl;
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



//-------------------------------------
// Jet L2L3 corrections
//-------------------------------------
void ObjectScale::setJetL2L3Names(TString oName){
    if (isData){
        if(is22C){
            jetL2L3Names.push_back("Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer22_22Sep2023_RunCD_V2_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is22D){
            jetL2L3Names.push_back("Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer22_22Sep2023_RunCD_V2_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is22E){
            jetL2L3Names.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
            jetL2L3Names.push_back("Run22E-22Sep2023_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is22F){
            jetL2L3Names.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
            jetL2L3Names.push_back("Run22F-Prompt_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is22G){
            jetL2L3Names.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
            jetL2L3Names.push_back("Run22G-Prompt_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is23A || is23B || is23Cv123){
            jetL2L3Names.push_back("Summer23Prompt23_RunCv123_V1_DATA_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23Prompt23_RunCv123_V1_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is23Cv4){
            jetL2L3Names.push_back("Summer23Prompt23_RunCv4_V1_DATA_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23Prompt23_RunCv4_V1_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is23D || is23E || is23F || is23G){
            jetL2L3Names.push_back("Summer23BPixPrompt23_RunD_V1_DATA_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is24){//FIXME
            jetL2L3Names.push_back("Summer23BPixPrompt23_RunD_V1_DATA_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
        }
    }
    else{
        if(is22){ 
            jetL2L3Names.push_back("Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi");
        }
        if(is23Pre){ 
            jetL2L3Names.push_back("Summer23Prompt23_V1_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23Prompt23_V1_MC_L2L3Residual_AK4PFPuppi");
        }
        if(is23Post){ 
            jetL2L3Names.push_back("Summer23BPixPrompt23_V1_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23BPixPrompt23_V1_MC_L2L3Residual_AK4PFPuppi");
        }
        if(is24){//FIXME
            jetL2L3Names.push_back("Summer23BPixPrompt23_V1_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23BPixPrompt23_V1_MC_L2L3Residual_AK4PFPuppi");
        }
    }
    cout<<setw(25)<<"setJetL2L3Names() = ";
    for(string jN: jetL2L3Names){ cout<<jN<<", ";}
    cout<<endl;
} 
   
void ObjectScale::setJetL2L3JsonPath(TString oName){
    if(is22 || is22D){
        jetL2L3JsonPath = "POG/JME/2022_Summer22/jet_jerc.json.gz";
    }
    if(is23A || is23B || is23C || is23Pre){
        jetL2L3JsonPath = "POG/JME/2023_Summer23/jet_jerc.json.gz";
    }
    if(is23D || is23E || is23F || is23G || is23Post){
        jetL2L3JsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    if(is24){//FIXME
        jetL2L3JsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    cout<<setw(25)<<"setJetL2L3JsonPath() = "<<jetL2L3JsonPath<<endl;
} 
   
void ObjectScale::loadJetL2L3Refs(){
     cout<<"==> loadJetL2L3Refs()"<<endl;
     auto jercJson  = correction::CorrectionSet::from_file(jetL2L3JsonPath);
    try{
        for(auto name: jetL2L3Names){ 
            loadedJetL2L3Refs.push_back(jercJson->at(name));
        }
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetL2L3Refs"<<endl;
        cout<<"Check "<<jetL2L3JsonPath<<" or jetL2L3Names"<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 


//-------------------------------------
// JER Resolution
//-------------------------------------
void ObjectScale::setJERResoName(TString oName){
    if(is24){//FIXME
        JERResoName = "Summer23BPixPrompt23_RunD_JRV1_MC_PtResolution_AK4PFPuppi";
    }
    cout<<setw(25)<<"setJERResoName() = ";
    cout<<JERResoName<<endl;
} 
   
void ObjectScale::setJERResoJsonPath(TString oName){
    if(is24){//FIXME
        JERResoJsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    cout<<setw(25)<<"setJERJsonPath() = "<<JERResoJsonPath<<endl;
} 
   
void ObjectScale::loadJERResoRef(){
     cout<<"==> loadJERResoRef()"<<endl;
     auto jercJson  = correction::CorrectionSet::from_file(JERResoJsonPath);
    try{
        loadedJERResoRef = jercJson->at(JERResoName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJERResoRef"<<endl;
        cout<<"Check "<<JERResoJsonPath<<" or JERResoName"<<endl;
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

//-------------------------------------
// JER SF 
//-------------------------------------
void ObjectScale::setJERSFName(TString oName){
    if(is24){//FIXME
        JERSFName = "Summer23BPixPrompt23_RunD_JRV1_MC_ScaleFactor_AK4PFPuppi";
    }
    cout<<setw(25)<<"setJERSFName() = ";
    cout<<JERSFName<<endl;
} 
   
void ObjectScale::setJERSFJsonPath(TString oName){
    if(is24){//FIXME
        JERSFJsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    cout<<setw(25)<<"setJERJsonPath() = "<<JERSFJsonPath<<endl;
} 
   
void ObjectScale::loadJERSFRef(){
     cout<<"==> loadJERSFRef()"<<endl;
     auto jercJson  = correction::CorrectionSet::from_file(JERSFJsonPath);
    try{
        loadedJERSFRef = jercJson->at(JERSFName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJERSFRef"<<endl;
        cout<<"Check "<<JERSFJsonPath<<" or JERSFName"<<endl;
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
void ObjectScale::setPhoSsName(TString oName){
    if(is22B || is22C || is22D || isMC){
        //phoSsName = "2022Re-recoBCD_ScaleJSON";
        phoSsName = "2022Re-recoBCD_SmearingJSON";
    }
    if(is22E || is22F || is22G || (isMC && isPostEE)){
        //phoSsName = "2022Re-recoE+PromptFG_ScaleJSON";
        phoSsName = "2022Re-recoE+PromptFG_SmearingJSON";
    }
     cout<<setw(25)<<"setPhoSsName() = "<<phoSsName<<endl;
} 
   
void ObjectScale::setPhoSsJsonPath(TString oName){
    if(is22B || is22C || is22D || isMC){
        phoSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    }
    if(is22E || is22F || is22G || (isMC && isPostEE)){
        phoSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoE+PromptFG/photonSS.json";
    }
     cout<<setw(25)<<"setPhoSsJsonPath() = "<<phoSsJsonPath<<endl;
} 

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

//-------------------------------------
// Electron Scale and Smearing 
//-------------------------------------
void ObjectScale::setEleSsName(TString oName){
    if(is22B || is22C || is22D){
        eleSsName = "2022Re-recoBCD_ScaleJSON";
    }
    if(is22E || is22F || is22G){
        eleSsName = "2022Re-recoE+PromptFG_ScaleJSON";
    }
     cout<<setw(25)<<"setEleSsName() = "<<eleSsName<<endl;
} 
   
void ObjectScale::setEleSsJsonPath(TString oName){
    if(is22B || is22C || is22D || isMC){
        eleSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoBCD/electronSS.json";
    }
    if(is22E || is22F || is22G || (isMC && isPostEE)){
        eleSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoE+PromptFG/electronSS.json";
    }
     cout<<setw(25)<<"setEleSsJsonPath() = "<<eleSsJsonPath<<endl;
} 

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

void ObjectScale::PrintInfo(string info, bool printcout)
   {
     //*ferr << info << endl << flush;
     if (printcout) cout << info << endl << flush;
}
//-------------------------------------
// Golden lumi Json 
//-------------------------------------
void ObjectScale::setLumiJsonPath(TString oName){
    if (is22)
      lumiJsonPath = "POG/files/Cert_Collisions2022_355100_362760_Golden.json";
    if (is23)
      lumiJsonPath = "POG/files/Cert_Collisions2023_366442_370790_Golden.json";
    if (is24)
      lumiJsonPath = "POG/files/Cert_Collisions2024_378981_381594_Golden.json";
    cout<<setw(25)<<"setLumiJsonPath() = "<<lumiJsonPath<<endl;
} 

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

void ObjectScale::setThresh(TString oName){
    double frac = 0.5;
    if(is22 || is23 || is24){
      bThresh = 0.4184;
      cThresh = 0.137+frac*(0.66-0.137);
    } 
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
