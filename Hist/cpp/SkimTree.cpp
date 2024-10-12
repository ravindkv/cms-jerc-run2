#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "SkimTree.h"

SkimTree::SkimTree(GlobalFlag& globalFlags)
    : globalFlags_(globalFlags),
	fCurrent_(-1), 
	outName_(""), 
	fChain_(std::make_unique<TChain>("Events")) {
    std::cout << "+ SkimTree initialized with outName = " << outName_ << std::endl;
}

SkimTree::~SkimTree() {
    // unique_ptr will automatically delete fChain_
}

void SkimTree::setInput(const std::string& outName) {
    outName_ = outName;
    std::cout << "+ setInput() = " << outName_ << std::endl;
}

void SkimTree::loadInput() {
    std::cout << "==> loadInput()" << std::endl;
    try {
        std::vector<std::string> v_outName = splitString(outName_, "_Hist_");
        if (v_outName.size() < 2) {
            throw std::runtime_error("Invalid outName format: Expected at least two parts separated by '_Hist_'");
        }
        loadedSampKey_ = v_outName.at(0);
        std::cout << "loadedSampKey_: " << loadedSampKey_ << std::endl;

        std::string nofN_root = v_outName.at(1);
        std::vector<std::string> v_nofN_root = splitString(nofN_root, ".root");
        if (v_nofN_root.empty()) {
            throw std::runtime_error("Invalid outName format: Missing '.root' extension");
        }

        std::string nofN = v_nofN_root.at(0);
        std::cout << "nofN: " << nofN << std::endl;

        std::vector<std::string> v_nofN = splitString(nofN, "of");
        if (v_nofN.size() != 2) {
            throw std::runtime_error("Invalid job numbering format in outName: Expected format 'NofM'");
        }

        loadedNthJob_ = std::stoi(v_nofN.at(0));
        loadedTotJob_ = std::stoi(v_nofN.at(1));
    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "Error in loadInput(): " << e.what() << "\n"
            << "Check the outName_: " << outName_ << "\n"
            << "outName format should be: DataOrMC_Year_Channel_Sample_Hist_NofM.root\n"
            << "Run ./runMain -h for more details";
        throw std::runtime_error(oss.str());
    }
}

void SkimTree::setInputJsonPath(const std::string& inDir) {
    std::string year;
    if (globalFlags_.getYear() == GlobalFlag::Year::Year2016Pre)
        year = "2016Pre";
    else if (globalFlags_.getYear() == GlobalFlag::Year::Year2016Post)
        year = "2016Post";
    else if (globalFlags_.getYear() == GlobalFlag::Year::Year2017)
        year = "2017";
    else if (globalFlags_.getYear() == GlobalFlag::Year::Year2018)
        year = "2018";
    else {
        throw std::runtime_error("Error: Provide correct year in SkimTree::setInputJsonPath()");
    }

    std::vector<std::string> tokens = splitString(loadedSampKey_, "_");
    if (tokens.size() < 3) {
        throw std::runtime_error("Invalid loadedSampKey_ format: Expected at least three parts separated by '_'");
    }
    std::string channel = tokens.at(2);
    inputJsonPath_ = inDir + "/FilesSkim_" + year + "_" + channel + ".json";
    std::cout << "+ setInputJsonPath() = " << inputJsonPath_ << std::endl;
}

void SkimTree::loadInputJson() {
    std::cout << "==> loadInputJson()" << std::endl;
    std::ifstream fileName(inputJsonPath_);
    if (!fileName.is_open()) {
        throw std::runtime_error("Unable to open input JSON file: " + inputJsonPath_);
    }

    nlohmann::json js;
    try {
        fileName >> js;
    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "Error parsing input JSON file: " << inputJsonPath_ << "\n"
            << e.what();
        throw std::runtime_error(oss.str());
    }

    try {
        js.at(loadedSampKey_).get_to(loadedAllFileNames_);
    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "Key not found in JSON: " << loadedSampKey_ << "\n"
            << e.what() << "\n"
            << "Available keys in the JSON file:";
        for (const auto& element : js.items()) {
            oss << "\n- " << element.key();
        }
        throw std::runtime_error(oss.str());
    }
}

void SkimTree::loadJobFileNames() {
    std::cout << "==> loadJobFileNames()" << std::endl;
    int nFiles = static_cast<int>(loadedAllFileNames_.size());
    std::cout << "Total files = " << nFiles << std::endl;

    if (loadedTotJob_ > nFiles) {
        std::cout << "Since loadedTotJob_ > nFiles, setting loadedTotJob_ to nFiles: " << nFiles << std::endl;
        loadedTotJob_ = nFiles;
    }

    if (loadedNthJob_ > loadedTotJob_) {
        throw std::runtime_error("Error: loadedNthJob_ > loadedTotJob_ in loadJobFileNames()");
    }

    if (loadedNthJob_ > 0 && loadedTotJob_ > 0) {
        std::cout << "Jobs: " << loadedNthJob_ << " of " << loadedTotJob_ << std::endl;
        std::cout << static_cast<double>(nFiles) / loadedTotJob_ << " files per job on average" << std::endl;
    } else {
        throw std::runtime_error("Error: Make sure loadedNthJob_ > 0 and loadedTotJob_ > 0 in loadJobFileNames()");
    }

    std::vector<std::vector<std::string>> smallVectors = splitVector(loadedAllFileNames_, loadedTotJob_);
    if (loadedNthJob_ - 1 >= static_cast<int>(smallVectors.size())) {
        throw std::runtime_error("Error: loadedNthJob_ is out of range after splitting file names in loadJobFileNames()");
    }
    loadedJobFileNames_ = smallVectors[loadedNthJob_ - 1];
}

void SkimTree::loadTree() {
    std::cout << "==> loadTree()" << std::endl;
    if (!fChain_) {
        fChain_ = std::make_unique<TChain>("Events");
    }
    fChain_->SetCacheSize(100 * 1024 * 1024);

    if (loadedJobFileNames_.empty()) {
        throw std::runtime_error("Error: No files to load in loadTree()");
    }

    std::string dir = ""; // Adjust as needed
    for (const auto& fName : loadedJobFileNames_) {
        if (fChain_->Add((dir + fName).c_str()) == 0) {
            throw std::runtime_error("Error adding file to TChain: " + dir + fName);
        }
        std::cout << dir + fName << "  " << fChain_->GetEntries() << std::endl;
    }

    fChain_->SetBranchStatus("*", 1);
    fChain_->SetBranchAddress("run", &run);
    fChain_->SetBranchAddress("luminosityBlock", &luminosityBlock);
    fChain_->SetBranchAddress("event", &event);

	//--------------------------------------- 
	//Jet for all channels 
	//--------------------------------------- 
	fChain_->SetBranchAddress("nJet", &nJet);
	fChain_->SetBranchAddress("Jet_area", &Jet_area);
	fChain_->SetBranchAddress("Jet_btagDeepB", &Jet_btagDeepB);
	fChain_->SetBranchAddress("Jet_btagDeepC", &Jet_btagDeepC);
	fChain_->SetBranchAddress("Jet_btagDeepFlavQG"  , &Jet_qgl);//tmp
	fChain_->SetBranchAddress("Jet_chEmEF"  , &Jet_chEmEF);
	fChain_->SetBranchAddress("Jet_chHEF"   , &Jet_chHEF);
	fChain_->SetBranchAddress("Jet_eta"     , &Jet_eta);
	fChain_->SetBranchAddress("Jet_mass"    , &Jet_mass);
	fChain_->SetBranchAddress("Jet_muEF"    , &Jet_muEF);
	fChain_->SetBranchAddress("Jet_neEmEF"  , &Jet_neEmEF);
	fChain_->SetBranchAddress("Jet_neHEF"   , &Jet_neHEF);
	fChain_->SetBranchAddress("Jet_phi"     , &Jet_phi);
	fChain_->SetBranchAddress("Jet_pt"    , &Jet_pt);
	fChain_->SetBranchAddress("Jet_rawFactor", &Jet_rawFactor);
	fChain_->SetBranchAddress("Jet_jetId", &Jet_jetId);
	
	if(globalFlags_.getChannel() == GlobalFlag::Channel::VetoMap || 
        globalFlags_.getChannel() == GlobalFlag::Channel::DiJet || 
        globalFlags_.getChannel() == GlobalFlag::Channel::IncJet || 
        globalFlags_.getChannel() == GlobalFlag::Channel::MultiJet || 
        globalFlags_.getChannel() == GlobalFlag::Channel::Wqq){
	    fChain_->SetBranchAddress("HLT_PFJet40"            , & HLT_PFJet40            );
	    fChain_->SetBranchAddress("HLT_PFJet60"            , & HLT_PFJet60            );
	    fChain_->SetBranchAddress("HLT_PFJet80"            , & HLT_PFJet80            );
	    fChain_->SetBranchAddress("HLT_PFJet140"           , & HLT_PFJet140           );
	    fChain_->SetBranchAddress("HLT_PFJet200"           , & HLT_PFJet200           );
	    fChain_->SetBranchAddress("HLT_PFJet260"           , & HLT_PFJet260           );
	    fChain_->SetBranchAddress("HLT_PFJet320"           , & HLT_PFJet320           );
	    fChain_->SetBranchAddress("HLT_PFJet400"           , & HLT_PFJet400           );
	    fChain_->SetBranchAddress("HLT_PFJet450"           , & HLT_PFJet450           );
	    fChain_->SetBranchAddress("HLT_PFJet500"           , & HLT_PFJet500           );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve40"       , & HLT_DiPFJetAve40       );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve60"       , & HLT_DiPFJetAve60       );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve80"       , & HLT_DiPFJetAve80       );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve140"      , & HLT_DiPFJetAve140      );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve200"      , & HLT_DiPFJetAve200      );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve260"      , & HLT_DiPFJetAve260      );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve320"      , & HLT_DiPFJetAve320      );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve400"      , & HLT_DiPFJetAve400      );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve500"      , & HLT_DiPFJetAve500      );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve60_HFJEC" , & HLT_DiPFJetAve60_HFJEC );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve80_HFJEC" , & HLT_DiPFJetAve80_HFJEC );
	    fChain_->SetBranchAddress("HLT_DiPFJetAve100_HFJEC", & HLT_DiPFJetAve100_HFJEC);
	    fChain_->SetBranchAddress("HLT_DiPFJetAve160_HFJEC", & HLT_DiPFJetAve160_HFJEC);
	    fChain_->SetBranchAddress("HLT_DiPFJetAve220_HFJEC", & HLT_DiPFJetAve220_HFJEC);
	    fChain_->SetBranchAddress("HLT_DiPFJetAve300_HFJEC", & HLT_DiPFJetAve300_HFJEC);
	}
	
	//--------------------------------------- 
	// Photon (for GamJet)
	//--------------------------------------- 
	if(globalFlags_.getChannel() == GlobalFlag::Channel::GamJet){
	  	fChain_->SetBranchAddress("nPhoton", &nPhoton);
	  	fChain_->SetBranchAddress("Photon_eCorr", &Photon_eCorr);
	  	fChain_->SetBranchAddress("Photon_energyErr", &Photon_energyErr);
	  	fChain_->SetBranchAddress("Photon_eta", &Photon_eta);
	  	fChain_->SetBranchAddress("Photon_hoe", &Photon_hoe);
	  	fChain_->SetBranchAddress("Photon_mass", &Photon_mass);
	  	fChain_->SetBranchAddress("Photon_phi", &Photon_phi);
	  	fChain_->SetBranchAddress("Photon_pt", &Photon_pt);
	  	fChain_->SetBranchAddress("Photon_r9", &Photon_r9);
	  	fChain_->SetBranchAddress("Photon_cutBased", &Photon_cutBased);
	  	fChain_->SetBranchAddress("Photon_jetIdx", &Photon_jetIdx);
	  	fChain_->SetBranchAddress("Photon_seedGain", &Photon_seedGain);
	  	///if (is22 || is23) {
	    	fChain_->SetBranchAddress("HLT_Photon300_NoHE", &HLT_Photon300_NoHE);
	    	fChain_->SetBranchAddress("HLT_Photon33", &HLT_Photon33);
	    	fChain_->SetBranchAddress("HLT_Photon50", &HLT_Photon50);
	    	fChain_->SetBranchAddress("HLT_Photon75", &HLT_Photon75);
	    	fChain_->SetBranchAddress("HLT_Photon90", &HLT_Photon90);
	    	fChain_->SetBranchAddress("HLT_Photon120", &HLT_Photon120);
	    	fChain_->SetBranchAddress("HLT_Photon150", &HLT_Photon150);
	    	fChain_->SetBranchAddress("HLT_Photon175", &HLT_Photon175);
	    	fChain_->SetBranchAddress("HLT_Photon200", &HLT_Photon200);
	    	
	    	fChain_->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM", &HLT_Photon50_R9Id90_HE10_IsoM);
	    	fChain_->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM", &HLT_Photon75_R9Id90_HE10_IsoM);
	    	fChain_->SetBranchAddress("HLT_Photon90_R9Id90_HE10_IsoM", &HLT_Photon90_R9Id90_HE10_IsoM);
	    	fChain_->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM", &HLT_Photon120_R9Id90_HE10_IsoM);
	    	fChain_->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM", &HLT_Photon165_R9Id90_HE10_IsoM);
	    	
	    	fChain_->SetBranchAddress("HLT_Photon20_HoverELoose", &HLT_Photon20_HoverELoose);
	    	fChain_->SetBranchAddress("HLT_Photon30_HoverELoose", &HLT_Photon30_HoverELoose);
	    	fChain_->SetBranchAddress("HLT_Photon30EB_TightID_TightIso", &HLT_Photon30EB_TightID_TightIso);
	    	fChain_->SetBranchAddress("HLT_Photon100EBHE10", &HLT_Photon100EBHE10);
	    	fChain_->SetBranchAddress("HLT_Photon110EB_TightID_TightIso", &HLT_Photon110EB_TightID_TightIso);
	  	//} // is22 || is23
	}
	
	//--------------------------------------- 
	// Electron (for DiEleJet)
	//--------------------------------------- 
	if(globalFlags_.getChannel() == GlobalFlag::Channel::ZeeJet){
		//status
		fChain_->SetBranchStatus("nElectron",1);
		fChain_->SetBranchStatus("Electron_*",1);
		//address
		fChain_->SetBranchAddress("nElectron", &nElectron);
		fChain_->SetBranchAddress("Electron_charge", &Electron_charge);	
		fChain_->SetBranchAddress("Electron_pt", &Electron_pt);
		fChain_->SetBranchAddress("Electron_deltaEtaSC", &Electron_deltaEtaSC);
		fChain_->SetBranchAddress("Electron_eta", &Electron_eta);
		fChain_->SetBranchAddress("Electron_phi", &Electron_phi);
		fChain_->SetBranchAddress("Electron_mass", &Electron_mass);
		fChain_->SetBranchAddress("Electron_cutBased", &Electron_cutBased);
		//address trigger
		if(globalFlags_.getYear() == GlobalFlag::Year::Year2016Pre || globalFlags_.getYear() == GlobalFlag::Year::Year2016Post)
			fChain_->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
		if(globalFlags_.getYear() == GlobalFlag::Year::Year2017 || globalFlags_.getYear() == GlobalFlag::Year::Year2018)
			fChain_->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL);
	}
	
	//--------------------------------------- 
	// Muon (for DiMuJet)
	//--------------------------------------- 
	if (globalFlags_.getChannel() == GlobalFlag::Channel::ZmmJet){
	  	//status
	  	fChain_->SetBranchStatus("nMuon",1);
	  	fChain_->SetBranchStatus("Muon_*",1);
	  	//address
	  	fChain_->SetBranchAddress("nMuon", &nMuon);
	  	fChain_->SetBranchAddress("Muoncharge", &Muon_charge);
	  	fChain_->SetBranchAddress("Muonpt", &Muon_pt);
	  	fChain_->SetBranchAddress("Muoneta", &Muon_eta);
	  	fChain_->SetBranchAddress("Muonphi", &Muon_phi);
	  	fChain_->SetBranchAddress("Muonmass", &Muon_mass);
	  	fChain_->SetBranchAddress("MuonmediumId", &Muon_mediumId);
	  	fChain_->SetBranchAddress("MuontightId", &Muon_tightId);
	  	fChain_->SetBranchAddress("MuonhighPurity", &Muon_highPurity);
	  	fChain_->SetBranchAddress("MuonpfRelIso04all", &Muon_pfRelIso04_all);
	  	fChain_->SetBranchAddress("MuontkRelIso", &Muon_tkRelIso);
	  	fChain_->SetBranchAddress("Muondxy", &Muon_dxy);
	  	fChain_->SetBranchAddress("Muondz", &Muon_dz);
	  	//address trigger
	  	if(globalFlags_.getYear() == GlobalFlag::Year::Year2016Pre || globalFlags_.getYear() == GlobalFlag::Year::Year2016Post)
	    fChain_->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ);
	  	if(globalFlags_.getYear() == GlobalFlag::Year::Year2017 || globalFlags_.getYear() == GlobalFlag::Year::Year2018)
	    fChain_->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8);
	}
	
	fChain_->SetBranchAddress("ChsMET_phi", &ChsMET_phi);
	fChain_->SetBranchAddress("ChsMET_pt",  &ChsMET_pt);
	fChain_->SetBranchAddress("fixedGridRhoFastjetAll", &Rho);
	fChain_->SetBranchAddress("PV_z", &PV_z);
	//fChain_->SetBranchAddress("GenVtx_z", &GenVtx_z);
	fChain_->SetBranchAddress("PV_npvs", &PV_npvs);
	fChain_->SetBranchAddress("PV_npvsGood", &PV_npvsGood);
	
	fChain_->SetBranchStatus("Flag_goodVertices",1);
	fChain_->SetBranchAddress("Flag_goodVertices",&Flag_goodVertices);
	fChain_->SetBranchStatus("Flag_globalSuperTightHalo2016Filter",1);
	fChain_->SetBranchAddress("Flag_globalSuperTightHalo2016Filter", &Flag_globalSuperTightHalo2016Filter);
	fChain_->SetBranchStatus("Flag_HBHENoiseFilter",1);
	fChain_->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter);
	fChain_->SetBranchStatus("Flag_HBHENoiseIsoFilter",1);
	fChain_->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter);
	fChain_->SetBranchStatus("Flag_EcalDeadCellTriggerPrimitiveFilter",1);
	fChain_->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter);
	fChain_->SetBranchStatus("Flag_BadPFMuonFilter",1);
	fChain_->SetBranchAddress("Flag_BadPFMuonFilter",&Flag_BadPFMuonFilter);
	fChain_->SetBranchStatus("Flag_eeBadScFilter",1);
	fChain_->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter);
	if(globalFlags_.getYear() == GlobalFlag::Year::Year2017 || globalFlags_.getYear() == GlobalFlag::Year::Year2018){
	    fChain_->SetBranchStatus("Flag_ecalBadCalibFilter",1);
	    fChain_->SetBranchAddress("Flag_ecalBadCalibFilter",&Flag_ecalBadCalibFilter);
	}
	  
	
	if (globalFlags_.isMC()){ 
		fChain_->SetBranchAddress("genWeight", &genWeight);
		fChain_->SetBranchAddress("nPSWeight", &nPSWeight);
		//fChain_->SetBranchAddress("PSWeight", &PSWeight);//seg fault
		fChain_->SetBranchAddress("Pileup_nTrueInt", &Pileup_nTrueInt);
		
		fChain_->SetBranchAddress("nGenJet", &nGenJet);
		fChain_->SetBranchAddress("GenJet_eta", &GenJet_eta);
		fChain_->SetBranchAddress("GenJet_mass", &GenJet_mass);
		fChain_->SetBranchAddress("GenJet_phi", &GenJet_phi);
		fChain_->SetBranchAddress("GenJet_pt", &GenJet_pt);
		fChain_->SetBranchAddress("GenJet_partonFlavour", &GenJet_partonFlavour);
		fChain_->SetBranchAddress("LHE_HT", &LHE_HT);
		fChain_->SetBranchAddress("Jet_genJetIdx", &Jet_genJetIdx);
	  	if (globalFlags_.getChannel() == GlobalFlag::Channel::GamJet){
	   		fChain_->SetBranchAddress("nGenIsolatedPhoton", &nGenIsolatedPhoton);
	   		fChain_->SetBranchAddress("GenIsolatedPhoton_eta", &GenIsolatedPhoton_eta);
	   		fChain_->SetBranchAddress("GenIsolatedPhoton_mass", &GenIsolatedPhoton_mass);
	   		fChain_->SetBranchAddress("GenIsolatedPhoton_phi", &GenIsolatedPhoton_phi);
	   		fChain_->SetBranchAddress("GenIsolatedPhoton_pt", &GenIsolatedPhoton_pt);
	   		fChain_->SetBranchAddress("GenIsolatedPhoton_pdgId", &GenIsolatedPhoton_pdgId);
	  	}
	  	if (globalFlags_.getChannel() == GlobalFlag::Channel::ZeeJet || globalFlags_.getChannel() == GlobalFlag::Channel::ZmmJet){
	   		fChain_->SetBranchAddress("nGenDressedLepton", &nGenDressedLepton);
	   		fChain_->SetBranchAddress("GenDressedLepton_eta", &GenDressedLepton_eta);
	   		fChain_->SetBranchAddress("GenDressedLepton_mass", &GenDressedLepton_mass);
	   		fChain_->SetBranchAddress("GenDressedLepton_phi", &GenDressedLepton_phi);
	   		fChain_->SetBranchAddress("GenDressedLepton_pt", &GenDressedLepton_pt);
	   		fChain_->SetBranchAddress("GenDressedLepton_pdgId", &GenDressedLepton_pdgId);
	  	}
	} // globalFlags_.isMC()
}

Long64_t SkimTree::getEntries() const {
    return fChain_ ? fChain_->GetEntries() : 0;
}

TChain* SkimTree::getChain() const {
    return fChain_.get();  // Return raw pointer to fChain_
}

Int_t SkimTree::getEntry(Long64_t entry) {
    return fChain_ ? fChain_->GetEntry(entry) : 0;
}

Long64_t SkimTree::loadEntry(Long64_t entry) {
    // Set the environment to read one entry
    if (!fChain_) {
        throw std::runtime_error("Error: fChain_ is not initialized in loadEntry()");
    }
    Long64_t centry = fChain_->LoadTree(entry);
    if (centry < 0) {
        throw std::runtime_error("Error loading entry in loadEntry()");
    }
    if (fChain_->GetTreeNumber() != fCurrent_) {
        fCurrent_ = fChain_->GetTreeNumber();
    }
    // Uncomment for debugging
    // std::cout << entry << ", " << centry << ", " << fCurrent_ << std::endl;
    return centry;
}

std::vector<std::vector<std::string>> SkimTree::splitVector(const std::vector<std::string>& strings, int n) const {
    if (n <= 0) {
        throw std::invalid_argument("n must be greater than 0 in splitVector");
    }
    size_t totalSize = strings.size();
    size_t baseSize = totalSize / n;
    size_t remainder = totalSize % n;
    std::vector<std::vector<std::string>> smallVectors;
    size_t index = 0;

    for (int i = 0; i < n; ++i) {
        size_t currentSize = baseSize + (i < remainder ? 1 : 0);
        smallVectors.emplace_back(strings.begin() + index, strings.begin() + index + currentSize);
        index += currentSize;
    }
    return smallVectors;
}

std::vector<std::string> SkimTree::splitString(const std::string& s, const std::string& delimiter) const {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;

    while ((end = s.find(delimiter, start)) != std::string::npos) {
        tokens.emplace_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.emplace_back(s.substr(start)); // Last token
    return tokens;
}

