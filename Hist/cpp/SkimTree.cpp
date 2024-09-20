#include<iostream>
#include "SkimTree.h"

void SkimTree::setInput(string oName){
  iName  = oName;
  cout<<"+ setInput() = "<<iName<<endl;
} 
void SkimTree::loadInput(){
  cout<<"==> loadInput()"<<endl;
  try{
    std::vector<std::string> v_iName    = splitString(iName, "_Hist_");
    loadedSampKey = v_iName.at(0); 
    std::cout << "loadedSampKey: " << loadedSampKey << std::endl;
    std::string nofN_root   = v_iName.at(1); 
    std::vector<std::string> v_nofN_root  = splitString(nofN_root, ".root"); 
    std::string nofN    = v_nofN_root.at(0); 
    std::cout << "nofN: " << nofN << std::endl;
    std::vector<std::string> v_nofN     = splitString(nofN, "of"); 
    loadedNthJob = std::stoi(v_nofN.at(0));
    loadedTotJob = std::stoi(v_nofN.at(1));
  }catch(const std::exception &e){
    cout<<"\nEXCEPTION: Check the iName_: "<<iName<<endl;
    cout<<"iName format should be: DataOrMC_Year_Channel_Sample_Hist_nofN.rooot"<<endl;
    cout<<"Run ./runMakeHist -h for more details"<<endl;
    cout<<e.what()<<endl;
    std::abort();
  }
} 
void SkimTree::setInputJsonPath(string inDir){
  string year = "2022";
  TString oN = iName;
  if(is23) year = "2023";
  if(is24) year = "2024";
  string channel = splitString(loadedSampKey, "_").at(2);
  inputJsonPath = inDir+"/FilesSkim_"+year+"_"+channel+".json";
  cout<<"+ setInputJsonPath() = "<<inputJsonPath<<endl;
}

void SkimTree:: loadInputJson(){
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

void SkimTree::loadJobFileNames(){
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

void SkimTree::loadTree(){
  cout<<"==> loadTree()"<<endl;
  TString oN = iName;
  fChain->SetCacheSize(100*1024*1024);
  int nFiles = loadedJobFileNames.size();
  //string dir = "root://cms-xrd-global.cern.ch/";
  //string dir = "root://cmsxrootd.fnal.gov/";
  string dir = "";
  for(int fileI=0; fileI<nFiles; fileI++){
    string fName = loadedJobFileNames[fileI];
    fChain->Add( (dir + fName).c_str());
    cout << dir+fName << "  " << fChain->GetEntries() << endl;
  }
	fChain->SetBranchStatus("*",1);
	fChain->SetBranchAddress("run", &run);
	fChain->SetBranchAddress("luminosityBlock", &luminosityBlock);
	fChain->SetBranchAddress("event", &event);

  //--------------------------------------- 
  //Jet for all channels 
  //--------------------------------------- 
	fChain->SetBranchAddress("nJet", &nJet);
	fChain->SetBranchAddress("Jet_area", &Jet_area);
	fChain->SetBranchAddress("Jet_btagDeepFlavB"   , &Jet_btagDeepFlavB);
	fChain->SetBranchAddress("Jet_btagDeepFlavCvB" , &Jet_btagDeepFlavCvB);
	fChain->SetBranchAddress("Jet_btagDeepFlavCvL" , &Jet_btagDeepFlavCvL);
	fChain->SetBranchAddress("Jet_btagDeepFlavQG"  , &Jet_btagDeepFlavQG);
	fChain->SetBranchAddress("Jet_chEmEF"  , &Jet_chEmEF);
	fChain->SetBranchAddress("Jet_chHEF"   , &Jet_chHEF);
	fChain->SetBranchAddress("Jet_eta"     , &Jet_eta);
	fChain->SetBranchAddress("Jet_mass"    , &Jet_mass);
	fChain->SetBranchAddress("Jet_muEF"    , &Jet_muEF);
	fChain->SetBranchAddress("Jet_neEmEF"  , &Jet_neEmEF);
	fChain->SetBranchAddress("Jet_neHEF"   , &Jet_neHEF);
	fChain->SetBranchAddress("Jet_phi"     , &Jet_phi);
	fChain->SetBranchAddress("Jet_pt"    , &Jet_pt);
	fChain->SetBranchAddress("Jet_rawFactor", &Jet_rawFactor);
	fChain->SetBranchAddress("Jet_jetId", &Jet_jetId);

  if(isVetoMap || isDiJet || isIncJet || isMultiJet || isWqq){
      fChain->SetBranchAddress("HLT_PFJet40"            , & HLT_PFJet40            );
      fChain->SetBranchAddress("HLT_PFJet60"            , & HLT_PFJet60            );
      fChain->SetBranchAddress("HLT_PFJet80"            , & HLT_PFJet80            );
      fChain->SetBranchAddress("HLT_PFJet140"           , & HLT_PFJet140           );
      fChain->SetBranchAddress("HLT_PFJet200"           , & HLT_PFJet200           );
      fChain->SetBranchAddress("HLT_PFJet260"           , & HLT_PFJet260           );
      fChain->SetBranchAddress("HLT_PFJet320"           , & HLT_PFJet320           );
      fChain->SetBranchAddress("HLT_PFJet400"           , & HLT_PFJet400           );
      fChain->SetBranchAddress("HLT_PFJet450"           , & HLT_PFJet450           );
      fChain->SetBranchAddress("HLT_PFJet500"           , & HLT_PFJet500           );
      fChain->SetBranchAddress("HLT_DiPFJetAve40"       , & HLT_DiPFJetAve40       );
      fChain->SetBranchAddress("HLT_DiPFJetAve60"       , & HLT_DiPFJetAve60       );
      fChain->SetBranchAddress("HLT_DiPFJetAve80"       , & HLT_DiPFJetAve80       );
      fChain->SetBranchAddress("HLT_DiPFJetAve140"      , & HLT_DiPFJetAve140      );
      fChain->SetBranchAddress("HLT_DiPFJetAve200"      , & HLT_DiPFJetAve200      );
      fChain->SetBranchAddress("HLT_DiPFJetAve260"      , & HLT_DiPFJetAve260      );
      fChain->SetBranchAddress("HLT_DiPFJetAve320"      , & HLT_DiPFJetAve320      );
      fChain->SetBranchAddress("HLT_DiPFJetAve400"      , & HLT_DiPFJetAve400      );
      fChain->SetBranchAddress("HLT_DiPFJetAve500"      , & HLT_DiPFJetAve500      );
      fChain->SetBranchAddress("HLT_DiPFJetAve60_HFJEC" , & HLT_DiPFJetAve60_HFJEC );
      fChain->SetBranchAddress("HLT_DiPFJetAve80_HFJEC" , & HLT_DiPFJetAve80_HFJEC );
      fChain->SetBranchAddress("HLT_DiPFJetAve100_HFJEC", & HLT_DiPFJetAve100_HFJEC);
      fChain->SetBranchAddress("HLT_DiPFJetAve160_HFJEC", & HLT_DiPFJetAve160_HFJEC);
      fChain->SetBranchAddress("HLT_DiPFJetAve220_HFJEC", & HLT_DiPFJetAve220_HFJEC);
      fChain->SetBranchAddress("HLT_DiPFJetAve300_HFJEC", & HLT_DiPFJetAve300_HFJEC);
  }

  //--------------------------------------- 
  // Photon (for GamJet)
  //--------------------------------------- 
	if(isGamJet){
		fChain->SetBranchAddress("nPhoton", &nPhoton);
		fChain->SetBranchAddress("Photon_eCorr", &Photon_eCorr);
		fChain->SetBranchAddress("Photon_energyErr", &Photon_energyErr);
		fChain->SetBranchAddress("Photon_eta", &Photon_eta);
		fChain->SetBranchAddress("Photon_hoe", &Photon_hoe);
		fChain->SetBranchAddress("Photon_mass", &Photon_mass);
		fChain->SetBranchAddress("Photon_phi", &Photon_phi);
		fChain->SetBranchAddress("Photon_pt", &Photon_pt);
		fChain->SetBranchAddress("Photon_r9", &Photon_r9);
		fChain->SetBranchAddress("Photon_cutBased", &Photon_cutBased);
		fChain->SetBranchAddress("Photon_jetIdx", &Photon_jetIdx);
		fChain->SetBranchAddress("Photon_seedGain", &Photon_seedGain);
		if (is22 || is23) {
	    fChain->SetBranchAddress("HLT_Photon300_NoHE", &HLT_Photon300_NoHE);
	    fChain->SetBranchAddress("HLT_Photon33", &HLT_Photon33);
	    fChain->SetBranchAddress("HLT_Photon50", &HLT_Photon50);
	    fChain->SetBranchAddress("HLT_Photon75", &HLT_Photon75);
	    fChain->SetBranchAddress("HLT_Photon90", &HLT_Photon90);
	    fChain->SetBranchAddress("HLT_Photon120", &HLT_Photon120);
	    fChain->SetBranchAddress("HLT_Photon150", &HLT_Photon150);
	    fChain->SetBranchAddress("HLT_Photon175", &HLT_Photon175);
	    fChain->SetBranchAddress("HLT_Photon200", &HLT_Photon200);
	    
	    fChain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM", &HLT_Photon50_R9Id90_HE10_IsoM);
	    fChain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM", &HLT_Photon75_R9Id90_HE10_IsoM);
	    fChain->SetBranchAddress("HLT_Photon90_R9Id90_HE10_IsoM", &HLT_Photon90_R9Id90_HE10_IsoM);
	    fChain->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM", &HLT_Photon120_R9Id90_HE10_IsoM);
	    fChain->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM", &HLT_Photon165_R9Id90_HE10_IsoM);
	    
	    fChain->SetBranchAddress("HLT_Photon20_HoverELoose", &HLT_Photon20_HoverELoose);
	    fChain->SetBranchAddress("HLT_Photon30_HoverELoose", &HLT_Photon30_HoverELoose);
	    fChain->SetBranchAddress("HLT_Photon30EB_TightID_TightIso", &HLT_Photon30EB_TightID_TightIso);
	    fChain->SetBranchAddress("HLT_Photon100EBHE10", &HLT_Photon100EBHE10);
	    fChain->SetBranchAddress("HLT_Photon110EB_TightID_TightIso", &HLT_Photon110EB_TightID_TightIso);
		} // is22 || is23
	}

  //--------------------------------------- 
  // Electron (for DiEleJet)
  //--------------------------------------- 
	if(isZeeJet){
		//status
		fChain->SetBranchStatus("nElectron",1);
		fChain->SetBranchStatus("Electron_*",1);
		//address
		fChain->SetBranchAddress("nElectron", &nEle_);
		fChain->SetBranchAddress("Electron_charge", &eleCharge_);	
		fChain->SetBranchAddress("Electron_pt", &elePt_);
		fChain->SetBranchAddress("Electron_deltaEtaSC", &eleDeltaEtaSC_);
		fChain->SetBranchAddress("Electron_eta", &eleEta_);
		fChain->SetBranchAddress("Electron_phi", &elePhi_);
		fChain->SetBranchAddress("Electron_mass", &eleMass_);
		fChain->SetBranchAddress("Electron_cutBased", &eleID_);
		//address trigger
    fChain->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
  }

  //--------------------------------------- 
  // Muon (for DiMuJet)
  //--------------------------------------- 
  if (isZmmJet){
		//status
		fChain->SetBranchStatus("nMuon",1);
		fChain->SetBranchStatus("Muon_*",1);
		//address
		fChain->SetBranchAddress("nMuon", &nMuon_);
		fChain->SetBranchAddress("Muon_charge", &muCharge_);
		fChain->SetBranchAddress("Muon_pt", &muPt_);
		fChain->SetBranchAddress("Muon_eta", &muEta_);
		fChain->SetBranchAddress("Muon_phi", &muPhi_);
		fChain->SetBranchAddress("Muon_mass", &muMass_);
		fChain->SetBranchAddress("Muon_mediumId", &muMediumId_);
		fChain->SetBranchAddress("Muon_tightId", &muTightId_);
		fChain->SetBranchAddress("Muon_highPurity", &muHighPurity_);
		fChain->SetBranchAddress("Muon_pfRelIso04_all", &muPFRelIso_);
		fChain->SetBranchAddress("Muon_tkRelIso", &muTkRelIso_);
		fChain->SetBranchAddress("Muon_dxy", &muDxy_);
		fChain->SetBranchAddress("Muon_dz", &muDz_);
		//address trigger
    fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8);
  }

	fChain->SetBranchAddress("RawPuppiMET_phi", &RawPuppiMET_phi);
	fChain->SetBranchAddress("RawPuppiMET_pt", &RawPuppiMET_pt);
	fChain->SetBranchAddress("Rho_fixedGridRhoFastjetAll", &Rho);
	fChain->SetBranchAddress("PV_z", &PV_z);
	fChain->SetBranchAddress("GenVtx_z", &GenVtx_z);
	fChain->SetBranchAddress("PV_npvs", &PV_npvs);
	fChain->SetBranchAddress("PV_npvsGood", &PV_npvsGood);
	
	fChain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter);
	fChain->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter);
	fChain->SetBranchAddress("Flag_CSCTightHaloFilter", &Flag_CSCTightHaloFilter);
	fChain->SetBranchAddress("Flag_CSCTightHaloTrkMuUnvetoFilter", &Flag_CSCTightHaloTrkMuUnvetoFilter);
	fChain->SetBranchAddress("Flag_CSCTightHalo2015Filter", &Flag_CSCTightHalo2015Filter);
	fChain->SetBranchAddress("Flag_globalTightHalo2016Filter", &Flag_globalTightHalo2016Filter);
	fChain->SetBranchAddress("Flag_globalSuperTightHalo2016Filter", &Flag_globalSuperTightHalo2016Filter);
	fChain->SetBranchAddress("Flag_HcalStripHaloFilter", &Flag_HcalStripHaloFilter);
	fChain->SetBranchAddress("Flag_hcalLaserEventFilter", &Flag_hcalLaserEventFilter);
	fChain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter);
	fChain->SetBranchAddress("Flag_EcalDeadCellBoundaryEnergyFilter", &Flag_EcalDeadCellBoundaryEnergyFilter);
	fChain->SetBranchAddress("Flag_ecalBadCalibFilter", &Flag_ecalBadCalibFilter);
	fChain->SetBranchAddress("Flag_goodVertices", &Flag_goodVertices);
	fChain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter);
	fChain->SetBranchAddress("Flag_ecalLaserCorrFilter", &Flag_ecalLaserCorrFilter);
	fChain->SetBranchAddress("Flag_trkPOGFilters", &Flag_trkPOGFilters);
	fChain->SetBranchAddress("Flag_chargedHadronTrackResolutionFilter", &Flag_chargedHadronTrackResolutionFilter);
	fChain->SetBranchAddress("Flag_muonBadTrackFilter", &Flag_muonBadTrackFilter);
	fChain->SetBranchAddress("Flag_BadChargedCandidateFilter", &Flag_BadChargedCandidateFilter);
	fChain->SetBranchAddress("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter);
	fChain->SetBranchAddress("Flag_BadPFMuonDzFilter", &Flag_BadPFMuonDzFilter);
	fChain->SetBranchAddress("Flag_hfNoisyHitsFilter", &Flag_hfNoisyHitsFilter);
	fChain->SetBranchAddress("Flag_BadChargedCandidateSummer16Filter", &Flag_BadChargedCandidateSummer16Filter);
	fChain->SetBranchAddress("Flag_BadPFMuonSummer16Filter", &Flag_BadPFMuonSummer16Filter);
	fChain->SetBranchAddress("Flag_trkPOG_manystripclus53X", &Flag_trkPOG_manystripclus53X);
	fChain->SetBranchAddress("Flag_trkPOG_toomanystripclus53X", &Flag_trkPOG_toomanystripclus53X);
	fChain->SetBranchAddress("Flag_trkPOG_logErrorTooManyClusters", &Flag_trkPOG_logErrorTooManyClusters);
	fChain->SetBranchAddress("Flag_METFilters", &Flag_METFilters);

  if (isMC){ 
    fChain->SetBranchAddress("genWeight", &genWeight);
    fChain->SetBranchAddress("nPSWeight", &nPSWeight);
    //fChain->SetBranchAddress("PSWeight", &PSWeight);//seg fault
    fChain->SetBranchAddress("Pileup_nTrueInt", &Pileup_nTrueInt);

    fChain->SetBranchAddress("nGenJet", &nGenJet);
    fChain->SetBranchAddress("GenJet_eta", &GenJet_eta);
    fChain->SetBranchAddress("GenJet_mass", &GenJet_mass);
    fChain->SetBranchAddress("GenJet_phi", &GenJet_phi);
    fChain->SetBranchAddress("GenJet_pt", &GenJet_pt);
    fChain->SetBranchAddress("GenJet_partonFlavour", &GenJet_partonFlavour);
    fChain->SetBranchAddress("LHE_HT", &LHE_HT);
    fChain->SetBranchAddress("Jet_genJetIdx", &Jet_genJetIdx);
    if (isGamJet){
      fChain->SetBranchAddress("nGenIsolatedPhoton", &nGenIsolatedPhoton);
      fChain->SetBranchAddress("GenIsolatedPhoton_eta", &GenIsolatedPhoton_eta);
      fChain->SetBranchAddress("GenIsolatedPhoton_mass", &GenIsolatedPhoton_mass);
      fChain->SetBranchAddress("GenIsolatedPhoton_phi", &GenIsolatedPhoton_phi);
      fChain->SetBranchAddress("GenIsolatedPhoton_pt", &GenIsolatedPhoton_pt);
      fChain->SetBranchAddress("GenIsolatedPhoton_pdgId", &GenIsolatedPhoton_pdgId);
    }
    if (isZeeJet || isZmmJet){
      fChain->SetBranchAddress("nGenDressedLepton", &nGenDressedLepton);
      fChain->SetBranchAddress("GenDressedLepton_eta", &GenDressedLepton_eta);
      fChain->SetBranchAddress("GenDressedLepton_mass", &GenDressedLepton_mass);
      fChain->SetBranchAddress("GenDressedLepton_phi", &GenDressedLepton_phi);
      fChain->SetBranchAddress("GenDressedLepton_pt", &GenDressedLepton_pt);
      fChain->SetBranchAddress("GenDressedLepton_pdgId", &GenDressedLepton_pdgId);
    }
  } // isMC
}

std::vector<std::vector<std::string>> SkimTree::splitVector(const std::vector<std::string>& strings, int n) {
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
std::vector<std::string> SkimTree::splitString(const std::string& s, const std::string& delimiter) {
  std::vector<std::string> tokens;
  size_t start = 0, end = 0;
  
  while ((end = s.find(delimiter, start)) != std::string::npos) {
    tokens.push_back(s.substr(start, end - start));
    start = end + delimiter.length();
  }
  tokens.push_back(s.substr(start)); // Last token
  
  return tokens;
}

//SkimTree::SkimTree(oName){
//  fCurrent  = -1;
//}

SkimTree::~SkimTree(){
  delete fChain;
  // will be some memory leak due to created vectors
}

Long64_t SkimTree::GetEntries(){
  return fChain->GetEntries();
}

Int_t SkimTree::GetEntry(Long64_t entry){
  fChain->GetEntry(entry);
  return fChain->GetEntries();
}
Long64_t SkimTree::loadEntry(Long64_t entry)                  
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
