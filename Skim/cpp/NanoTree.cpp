#include "NanoTree.h"

void NanoTree::setInput(string oName){
  outName  = oName;
  cout<<"+ setInput() = "<<outName<<endl;
} 
void NanoTree::loadInput(){
    cout<<"==> loadInput()"<<endl;
    try{
        std::vector<std::string> v_iName      = splitString(outName, "_Skim_");
        loadedSampKey = v_iName.at(0); 
        std::cout << "loadedSampKey: " << loadedSampKey << std::endl;
        std::string nofN_root   = v_iName.at(1); 
        std::vector<std::string> v_nofN_root    = splitString(nofN_root, ".root"); 
        std::string nofN        = v_nofN_root.at(0); 
        std::cout << "nofN: " << nofN << std::endl;
        std::vector<std::string> v_nofN         = splitString(nofN, "of"); 
        loadedNthJob = std::stoi(v_nofN.at(0));
        loadedTotJob = std::stoi(v_nofN.at(1));
    }catch(const std::exception &e){
        cout<<"\nEXCEPTION: Check the outName: "<<outName<<endl;
        cout<<"outName format should be: DataOrMC_Year_Channel_Sample_Skim_nofN.rooot"<<endl;
        cout<<"Run ./runMain -h for more details"<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 
void NanoTree::setInputJsonPath(string inDir){
    string year = "2016Pre";
    if(is2016Post) year = "2016Post";
    if(is2017) year = "2017";
    if(is2018) year = "2018";
    string channel = splitString(loadedSampKey, "_").at(2);
    inputJsonPath = inDir+"/FilesNano_"+year+"_"+channel+".json";
    cout<<"+ setInputJsonPath() = "<<inputJsonPath<<endl;
}

void NanoTree:: loadInputJson(){
    cout<<"==> loadInputJson()"<<endl;
    ifstream fileName_(inputJsonPath.c_str());
    nlohmann::json js;
    try{
        js = nlohmann::json::parse(fileName_);
    } catch (const exception& e) {
        cout<<"\nEXCEPTION: Check the input json inputJsonPath: "<<inputJsonPath<<endl;
        cout<<e.what()<<endl;
        abort();
    }
    try{
        js.at(loadedSampKey).get_to(loadedAllFileNames);
    }catch (const exception & e){
        cout<<"\nEXCEPTION: Check the loadedSampKey: "<<loadedSampKey<<endl;
        cout<<e.what()<<endl;
        cout<<"Choose loadedSampKey from the following:"<<endl;
        for (auto& element : js.items()) {
            cout << element.key() << endl;
        }
        abort();
    }//
}

void NanoTree::loadJobFileNames(){
    cout<<"==> loadJobFileNames()"<<endl;
    int nFiles  = loadedAllFileNames.size();
    cout<<"Total files = "<<nFiles<<endl;
    if (loadedTotJob > nFiles){
        cout<<"Since loadedTotJob > nFiles, setting it to the nFiles, loadedTotJob = "<<nFiles<<endl;
        loadedTotJob = nFiles;
    }
    if (loadedNthJob > loadedTotJob){
        cout<<"Error: Make sure loadedNthJob < loadedTotJob"<<endl;
        std::abort();
    }
	if (loadedNthJob>0 && loadedTotJob>0){
	    cout <<"jobs: " <<loadedNthJob << " of " << loadedTotJob << endl;
		cout << (1.*nFiles)/loadedTotJob << " files per job on average" << endl;
	}
    else{
        cout<<"\n Error: Make sure loadedNthJob > 0 and loadedTotJob > 0\n ";
        std::abort();
    }
    std::vector<std::vector<std::string>> smallVectors = splitVector(loadedAllFileNames, loadedTotJob);
    loadedJobFileNames = smallVectors[loadedNthJob-1];
}

void NanoTree::loadTree(){
  cout<<"==> loadTree()"<<endl;
  std::cout << "Start NanoTree" << std::endl;
  fChain->SetCacheSize(100*1024*1024);
  bool isCopy = false;
  int nFiles = loadedJobFileNames.size();
  string dir = "root://cms-xrd-global.cern.ch/";
  //string dir = "root://cmsxrootd.fnal.gov/";
  //string dir = "./"; 
  for(int fileI=0; fileI<nFiles; fileI++){
      string fName = loadedJobFileNames[fileI];
      if(isCopy){
          string singleFile = fName.substr(fName.find_last_of("/")+1,fName.size());
          string xrdcp_command = "xrdcp " + dir + fName + " " + singleFile ;
          cout << xrdcp_command.c_str() << endl;
          system(xrdcp_command.c_str());
          fChain->Add( singleFile.c_str());
          cout << singleFile << "  " << fChain->GetEntries() << endl;
      }
      else{
          std::filesystem::path filePath = "/eos/cms/" + fName;
          if (std::filesystem::exists(filePath)) {
  			dir = "/eos/cms/"; 
  	}
          fChain->Add( (dir + fName).c_str());
          cout << dir+fName << "  " << fChain->GetEntries() << endl;
      }
  }
  std::cout << "Begin" << std::endl;
  fChain->SetBranchStatus("*",0);
  // event
  fChain->SetBranchStatus("run",1);
  fChain->SetBranchStatus("event",1);
  fChain->SetBranchStatus("luminosityBlock",1);
  
  //--------------------------------------- 
  //Jet for all channels 
  //--------------------------------------- 
  fChain->SetBranchStatus("Jet_btagDeep*",1);
  fChain->SetBranchStatus("Jet_chEmEF",1);
  fChain->SetBranchStatus("Jet_chHEF" ,1);
  fChain->SetBranchStatus("Jet_eta"   ,1);
  fChain->SetBranchStatus("Jet_mass"  ,1);
  fChain->SetBranchStatus("Jet_muEF"  ,1);
  fChain->SetBranchStatus("Jet_neEmEF",1);
  fChain->SetBranchStatus("Jet_neHEF" ,1);
  fChain->SetBranchStatus("Jet_phi"   ,1);
  fChain->SetBranchStatus("Jet_pt"    ,1);
  fChain->SetBranchStatus("Jet_rawFactor",1);
  fChain->SetBranchStatus("Jet_jetId",1);
  fChain->SetBranchStatus("Jet_area",1);
  fChain->SetBranchStatus("nJet",1);
  
  //common branches
  fChain->SetBranchStatus("PV_z");
  //fChain->SetBranchStatus("GenVtx_z");
  fChain->SetBranchStatus("PV_npvs",1);
  fChain->SetBranchStatus("PV_npvsGood",1);
  fChain->SetBranchStatus("MET_pt",1);
  fChain->SetBranchStatus("MET_phi",1);
  fChain->SetBranchStatus("ChsMET_pt",1);
  fChain->SetBranchStatus("ChsMET_phi",1);
  fChain->SetBranchStatus("RawPuppiMET_pt",1);
  fChain->SetBranchStatus("RawPuppiMET_phi",1);
  fChain->SetBranchStatus("fixedGridRhoFastjetAll");
  
  if(is2016Pre || is2016Post){
    fChain->SetBranchStatus("Flag_goodVertices", 1);
    fChain->SetBranchStatus("Flag_globalSuperTightHalo2016Filter", 1);
    fChain->SetBranchStatus("Flag_HBHENoiseFilter", 1);
    fChain->SetBranchStatus("Flag_HBHENoiseIsoFilter", 1);
    fChain->SetBranchStatus("Flag_EcalDeadCellTriggerPrimitiveFilter", 1);
    fChain->SetBranchStatus("Flag_BadPFMuonFilter", 1);
    fChain->SetBranchStatus("Flag_BadPFMuonDzFilter", 1);
    fChain->SetBranchStatus("Flag_eeBadScFilter", 1);
  }
  if(is2017 || is2018){
    fChain->SetBranchStatus("Flag_goodVertices", 1);
    fChain->SetBranchStatus("Flag_globalSuperTightHalo2016Filter", 1);
    fChain->SetBranchStatus("Flag_HBHENoiseFilter", 1);
    fChain->SetBranchStatus("Flag_HBHENoiseIsoFilter", 1);
    fChain->SetBranchStatus("Flag_EcalDeadCellTriggerPrimitiveFilter", 1);
    fChain->SetBranchStatus("Flag_BadPFMuonFilter", 1);
    fChain->SetBranchStatus("Flag_BadPFMuonDzFilter", 1);
    fChain->SetBranchStatus("Flag_eeBadScFilter", 1);
    fChain->SetBranchStatus("Flag_ecalBadCalibFilter", 1);
  }
  
  if(isMC){
    fChain->SetBranchStatus("Jet_genJetIdx",1);
    fChain->SetBranchStatus("genWeight");
    fChain->SetBranchStatus("nPSWeight");
    fChain->SetBranchStatus("PSWeight");
    fChain->SetBranchStatus("LHE_HT");
    fChain->SetBranchStatus("Pileup_nTrueInt");
    fChain->SetBranchStatus("GenJet_*",1);
    fChain->SetBranchStatus("nGenJet",1);
  }//MC
  
  //--------------------------------------- 
  // Channels 
  //--------------------------------------- 
  if(isZeeJet){
    fChain->SetBranchStatus("nElectron", 1);
    fChain->SetBranchStatus("Electron_charge", 1);
    fChain->SetBranchStatus("Electron_pt", 1);
    fChain->SetBranchStatus("Electron_deltaEtaSC", 1);
    fChain->SetBranchStatus("Electron_eta", 1);
    fChain->SetBranchStatus("Electron_phi", 1);
    fChain->SetBranchStatus("Electron_mass", 1);
    fChain->SetBranchStatus("Electron_cutBased", 1);
    fChain->SetBranchStatus("Electron_seedGain", 1);
    fChain->SetBranchStatus("Electron_pfRelIso03_all", 1);
    fChain->SetBranchStatus("Electron_sieie", 1);
    fChain->SetBranchStatus("Electron_photonIdx", 1);
    fChain->SetBranchStatus("Electron_mvaFall17V2Iso_WP*", 1);
    fChain->SetBranchStatus("nElectron",1);
  	if(isMC){
      fChain->SetBranchStatus("GenDressedLepton_*",1);
      fChain->SetBranchStatus("nGenDressedLepton",1);
    }
    fChain->SetBranchStatus("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL*", 1);
    fChain->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL, & b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL);
    fChain->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ, & b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
    }
  
  if(isZmmJet){
    fChain->SetBranchStatus("Muon_charge", 1);
    fChain->SetBranchStatus("Muon_pt", 1);
    fChain->SetBranchStatus("Muon_eta", 1);
    fChain->SetBranchStatus("Muon_phi", 1);
    fChain->SetBranchStatus("Muon_mass", 1);
    fChain->SetBranchStatus("Muon_mediumId", 1);
    fChain->SetBranchStatus("Muon_tightId", 1);
    fChain->SetBranchStatus("Muon_highPurity", 1);
    fChain->SetBranchStatus("Muon_pfRelIso04_all", 1);
    fChain->SetBranchStatus("Muon_tkRelIso", 1); 
    fChain->SetBranchStatus("Muon_dxy", 1); 
    fChain->SetBranchStatus("Muon_dz", 1); 
    fChain->SetBranchStatus("nMuon",1);
  	if (isMC){
      fChain->SetBranchStatus("GenDressedLepton_*",1);
      fChain->SetBranchStatus("nGenDressedLepton",1);
    }
    fChain->SetBranchStatus("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ*", 1);
    fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", & HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ);
    fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", & HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8);
  }
}//loadTree

NanoTree::~NanoTree(){
    delete fChain;
}

Long64_t NanoTree::GetEntries(){
    return fChain->GetEntries();
}

Int_t NanoTree::GetEntry(Long64_t entry){
    return fChain->GetEntry(entry);
}

Long64_t NanoTree::loadEntry(Long64_t entry)                                  
{                                                                              
// Set the environment to read one entry                                                  
   if (!fChain) return -5;                                                     
   Long64_t centry = fChain->LoadTree(entry);                                  
   if (centry < 0) return centry;                                              
   if (fChain->GetTreeNumber() != fCurrent) {                                  
      fCurrent = fChain->GetTreeNumber();                                      
   }                                                                           
   //cout<<entry<<", "<<centry<<", "<<fCurrent<<endl;
   return centry;                                                              
}

std::vector<std::vector<std::string>> NanoTree::splitVector(const std::vector<std::string>& strings, int n) {
    int size = strings.size() / n;  // Size of each small vector
    int remainder = strings.size() % n;  // Remaining elements
    std::vector<std::vector<std::string>> smallVectors;
    int index = 0;
    for (int i = 0; i < n; ++i) {
        if (i < remainder) {
            smallVectors.push_back(std::vector<std::string>(
                        strings.begin() + index, strings.begin() + index + size + 1));
            index += size + 1;
        } else {
            smallVectors.push_back(std::vector<std::string>(
                        strings.begin() + index, strings.begin() + index + size));
            index += size;
        }
    }
    return smallVectors;
}

std::vector<std::string> NanoTree::splitString(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start)); // Last token
    
    return tokens;
}
