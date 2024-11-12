#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "SkimTree.h"
#include "Helper.h"

SkimTree::SkimTree(GlobalFlag& globalFlags): 
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    era_(globalFlags_.getEra()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()),
    isData_(globalFlags_.isData()),
    isMC_(globalFlags_.isMC()),
	fCurrent_(-1), 
	outName_(""), 
	fChain_(std::make_unique<TChain>("Events")) {
    std::cout << "+ SkimTree initialized with outName = " << outName_ << '\n';
}

SkimTree::~SkimTree() {
    // unique_ptr will automatically delete fChain_
}

void SkimTree::setInput(const std::string& outName) {
    outName_ = outName;
    std::cout << "+ setInput() = " << outName_ << '\n';
}

void SkimTree::loadInput() {
    std::cout << "==> loadInput()" << '\n';
    try {
        std::vector<std::string> v_outName = Helper::splitString(outName_, "_Hist_");
        if (v_outName.size() < 2) {
            throw std::runtime_error("Invalid outName format: Expected at least two parts separated by '_Hist_'");
        }
        loadedSampKey_ = v_outName.at(0);
        std::cout << "loadedSampKey_: " << loadedSampKey_ << '\n';

        std::string nofN_root = v_outName.at(1);
        std::vector<std::string> v_nofN_root = Helper::splitString(nofN_root, ".root");
        if (v_nofN_root.empty()) {
            throw std::runtime_error("Invalid outName format: Missing '.root' extension");
        }

        std::string nofN = v_nofN_root.at(0);
        std::cout << "nofN: " << nofN << '\n';

        std::vector<std::string> v_nofN = Helper::splitString(nofN, "of");
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
    if (year_ == GlobalFlag::Year::Year2016Pre)
        year = "2016Pre";
    else if (year_ == GlobalFlag::Year::Year2016Post)
        year = "2016Post";
    else if (year_ == GlobalFlag::Year::Year2017)
        year = "2017";
    else if (year_ == GlobalFlag::Year::Year2018)
        year = "2018";
    else {
        throw std::runtime_error("Error: Provide correct year in SkimTree::setInputJsonPath()");
    }

    std::vector<std::string> tokens = Helper::splitString(loadedSampKey_, "_");
    if (tokens.size() < 3) {
        throw std::runtime_error("Invalid loadedSampKey_ format: Expected at least three parts separated by '_'");
    }
    std::string channel = tokens.at(1);
    inputJsonPath_ = inDir + "/FilesSkim_" + channel + "_" + year + ".json";
    std::cout << "+ setInputJsonPath() = " << inputJsonPath_ << '\n';
}

void SkimTree::loadInputJson() {
    std::cout << "==> loadInputJson()" << '\n';
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
    std::cout << "==> loadJobFileNames()" << '\n';
    int nFiles = static_cast<int>(loadedAllFileNames_.size());
    std::cout << "Total files = " << nFiles << '\n';

    if (loadedTotJob_ > nFiles) {
        std::cout << "Since loadedTotJob_ > nFiles, setting loadedTotJob_ to nFiles: " << nFiles << '\n';
        loadedTotJob_ = nFiles;
    }

    if (loadedNthJob_ > loadedTotJob_) {
        throw std::runtime_error("Error: loadedNthJob_ > loadedTotJob_ in loadJobFileNames()");
    }

    if (loadedNthJob_ > 0 && loadedTotJob_ > 0) {
        std::cout << "Jobs: " << loadedNthJob_ << " of " << loadedTotJob_ << '\n';
        std::cout << static_cast<double>(nFiles) / loadedTotJob_ << " files per job on average" << '\n';
    } else {
        throw std::runtime_error("Error: Make sure loadedNthJob_ > 0 and loadedTotJob_ > 0 in loadJobFileNames()");
    }

    std::vector<std::vector<std::string>> smallVectors = Helper::splitVector(loadedAllFileNames_, loadedTotJob_);
    if (loadedNthJob_ - 1 >= static_cast<int>(smallVectors.size())) {
        throw std::runtime_error("Error: loadedNthJob_ is out of range after splitting file names in loadJobFileNames()");
    }
    loadedJobFileNames_ = smallVectors[loadedNthJob_ - 1];
}

void SkimTree::loadTree() {
    std::cout << "==> loadTree()" << '\n';
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
        std::cout << dir + fName << "  " << fChain_->GetEntries() << '\n';
    }

    fChain_->SetBranchStatus("*", true);
    fChain_->SetBranchAddress("run", &run);
    fChain_->SetBranchAddress("luminosityBlock", &luminosityBlock);
    fChain_->SetBranchAddress("event", &event);

	//--------------------------------------- 
	//Jet for all channels 
	//--------------------------------------- 
	fChain_->SetBranchAddress("nJet", &nJet);
	fChain_->SetBranchAddress("Jet_area", &Jet_area);

	fChain_->SetBranchAddress("Jet_btagDeepFlavB", &Jet_btagDeepFlavB);
	fChain_->SetBranchAddress("Jet_btagDeepFlavCvL", &Jet_btagDeepFlavCvL);
	fChain_->SetBranchAddress("Jet_btagDeepFlavCvB", &Jet_btagDeepFlavCvB);
	fChain_->SetBranchAddress("Jet_btagDeepFlavG", &Jet_btagDeepFlavG);
	fChain_->SetBranchAddress("Jet_btagDeepFlavQG", &Jet_btagDeepFlavQG);
	fChain_->SetBranchAddress("Jet_btagDeepFlavUDS", &Jet_btagDeepFlavUDS);

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
	
	if(channel_ == GlobalFlag::Channel::VetoMap || 
        channel_ == GlobalFlag::Channel::DiJet || 
        channel_ == GlobalFlag::Channel::IncJet || 
        channel_ == GlobalFlag::Channel::MultiJet || 
        channel_ == GlobalFlag::Channel::Wqq){
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
	if(channel_ == GlobalFlag::Channel::GamJet){
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

	    fChain_->SetBranchAddress("HLT_Photon20", &HLT_Photon20);
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
        if (year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post) {
            fChain_->SetBranchAddress("HLT_Photon36",                      &HLT_Photon36);
            fChain_->SetBranchAddress("HLT_Photon30",                      &HLT_Photon30);
            fChain_->SetBranchAddress("HLT_Photon22",                      &HLT_Photon22);
            fChain_->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM",    &HLT_Photon165_R9Id90_HE10_IsoM);
            fChain_->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM",    &HLT_Photon120_R9Id90_HE10_IsoM);
            fChain_->SetBranchAddress("HLT_Photon36_R9Id90_HE10_IsoM",     &HLT_Photon36_R9Id90_HE10_IsoM);
            fChain_->SetBranchAddress("HLT_Photon30_R9Id90_HE10_IsoM",     &HLT_Photon30_R9Id90_HE10_IsoM);
            fChain_->SetBranchAddress("HLT_Photon22_R9Id90_HE10_IsoM",     &HLT_Photon22_R9Id90_HE10_IsoM); 
        }
        else if (year_ == GlobalFlag::Year::Year2017){
            fChain_->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM",    &HLT_Photon165_R9Id90_HE10_IsoM);
            fChain_->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM",    &HLT_Photon120_R9Id90_HE10_IsoM);
            fChain_->SetBranchAddress("HLT_Photon60_HoverELoose",          &HLT_Photon60_HoverELoose);
            fChain_->SetBranchAddress("HLT_Photon50_HoverELoose",          &HLT_Photon50_HoverELoose);
            fChain_->SetBranchAddress("HLT_Photon40_HoverELoose",          &HLT_Photon40_HoverELoose);
            fChain_->SetBranchAddress("HLT_Photon30_HoverELoose",          &HLT_Photon30_HoverELoose);
            fChain_->SetBranchAddress("HLT_Photon20_HoverELoose",          &HLT_Photon20_HoverELoose); 
        }
        else if (year_ == GlobalFlag::Year::Year2018){
            fChain_->SetBranchAddress("HLT_Photon120EB_TightID_TightIso",  &HLT_Photon120EB_TightID_TightIso);
            fChain_->SetBranchAddress("HLT_Photon110EB_TightID_TightIso",  &HLT_Photon110EB_TightID_TightIso);
            fChain_->SetBranchAddress("HLT_Photon100EB_TightID_TightIso",  &HLT_Photon100EB_TightID_TightIso);
            fChain_->SetBranchAddress("HLT_Photon30_HoverELoose",          &HLT_Photon30_HoverELoose);
            fChain_->SetBranchAddress("HLT_Photon20_HoverELoose",          &HLT_Photon20_HoverELoose); 
        }
	}//GamJet
	
	//--------------------------------------- 
	// Electron (for DiEleJet)
	//--------------------------------------- 
	if(channel_ == GlobalFlag::Channel::ZeeJet){
		//status
		fChain_->SetBranchStatus("nElectron",true);
		fChain_->SetBranchStatus("Electron_*",true);
		//address
		fChain_->SetBranchAddress("nElectron", &nElectron);
		fChain_->SetBranchAddress("Electron_charge", &Electron_charge);	
		fChain_->SetBranchAddress("Electron_pt", &Electron_pt);
		fChain_->SetBranchAddress("Electron_deltaEtaSC", &Electron_deltaEtaSC);
		fChain_->SetBranchAddress("Electron_eta", &Electron_eta);
		fChain_->SetBranchAddress("Electron_phi", &Electron_phi);
		fChain_->SetBranchAddress("Electron_mass", &Electron_mass);
		fChain_->SetBranchAddress("Electron_eCorr", &Electron_eCorr);
		fChain_->SetBranchAddress("Electron_cutBased", &Electron_cutBased);
		//address trigger
		if(year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post)
			fChain_->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
		if(year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018)
			fChain_->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL);
	}
	
	//--------------------------------------- 
	// Muon (for DiMuJet)
	//--------------------------------------- 
	if (channel_ == GlobalFlag::Channel::ZmmJet){
	  	//status
	  	fChain_->SetBranchStatus("nMuon",true);
	  	fChain_->SetBranchStatus("Muon_*",true);
	  	//address
	  	fChain_->SetBranchAddress("nMuon", &nMuon);
	  	fChain_->SetBranchAddress("Muon_nTrackerLayers", &Muon_nTrackerLayers);
	  	fChain_->SetBranchAddress("Muon_charge", &Muon_charge);
	  	fChain_->SetBranchAddress("Muon_pt", &Muon_pt);
	  	fChain_->SetBranchAddress("Muon_eta", &Muon_eta);
	  	fChain_->SetBranchAddress("Muon_phi", &Muon_phi);
	  	fChain_->SetBranchAddress("Muon_mass", &Muon_mass);
	  	fChain_->SetBranchAddress("Muon_mediumId", &Muon_mediumId);
	  	fChain_->SetBranchAddress("Muon_tightId", &Muon_tightId);
	  	fChain_->SetBranchAddress("Muon_highPurity", &Muon_highPurity);
	  	fChain_->SetBranchAddress("Muon_pfRelIso04_all", &Muon_pfRelIso04_all);
	  	fChain_->SetBranchAddress("Muon_tkRelIso", &Muon_tkRelIso);
	  	fChain_->SetBranchAddress("Muon_dxy", &Muon_dxy);
	  	fChain_->SetBranchAddress("Muon_dz", &Muon_dz);
	  	//address trigger
	  	if(year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post)
	    fChain_->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ);
	  	if(year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018)
	    fChain_->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8);
	}
	
	fChain_->SetBranchAddress("ChsMET_phi", &ChsMET_phi);
	fChain_->SetBranchAddress("ChsMET_pt",  &ChsMET_pt);
	fChain_->SetBranchAddress("fixedGridRhoFastjetAll", &Rho);
	fChain_->SetBranchAddress("PV_z", &PV_z);
	//fChain_->SetBranchAddress("GenVtx_z", &GenVtx_z);
	fChain_->SetBranchAddress("PV_npvs", &PV_npvs);
	fChain_->SetBranchAddress("PV_npvsGood", &PV_npvsGood);
	
	fChain_->SetBranchStatus("Flag_goodVertices",true);
	fChain_->SetBranchAddress("Flag_goodVertices",&Flag_goodVertices);
	fChain_->SetBranchStatus("Flag_globalSuperTightHalo2016Filter",true);
	fChain_->SetBranchAddress("Flag_globalSuperTightHalo2016Filter", &Flag_globalSuperTightHalo2016Filter);
	fChain_->SetBranchStatus("Flag_HBHENoiseFilter",true);
	fChain_->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter);
	fChain_->SetBranchStatus("Flag_HBHENoiseIsoFilter",true);
	fChain_->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter);
	fChain_->SetBranchStatus("Flag_EcalDeadCellTriggerPrimitiveFilter",true);
	fChain_->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter);
	fChain_->SetBranchStatus("Flag_BadPFMuonFilter",true);
	fChain_->SetBranchAddress("Flag_BadPFMuonFilter",&Flag_BadPFMuonFilter);
	fChain_->SetBranchStatus("Flag_eeBadScFilter",true);
	fChain_->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter);
	if(year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018){
	    fChain_->SetBranchStatus("Flag_ecalBadCalibFilter",true);
	    fChain_->SetBranchAddress("Flag_ecalBadCalibFilter",&Flag_ecalBadCalibFilter);
	}
	  
	
	if (isMC_){ 
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
	  	if (channel_ == GlobalFlag::Channel::GamJet){
	   		fChain_->SetBranchAddress("nGenIsolatedPhoton", &nGenIsolatedPhoton);
	   		fChain_->SetBranchAddress("GenIsolatedPhoton_eta", &GenIsolatedPhoton_eta);
	   		fChain_->SetBranchAddress("GenIsolatedPhoton_mass", &GenIsolatedPhoton_mass);
	   		fChain_->SetBranchAddress("GenIsolatedPhoton_phi", &GenIsolatedPhoton_phi);
	   		fChain_->SetBranchAddress("GenIsolatedPhoton_pt", &GenIsolatedPhoton_pt);
	  	}
	  	if (channel_ == GlobalFlag::Channel::ZeeJet || channel_ == GlobalFlag::Channel::ZmmJet){
	   		fChain_->SetBranchAddress("nGenDressedLepton", &nGenDressedLepton);
	   		fChain_->SetBranchAddress("GenDressedLepton_eta", &GenDressedLepton_eta);
	   		fChain_->SetBranchAddress("GenDressedLepton_mass", &GenDressedLepton_mass);
	   		fChain_->SetBranchAddress("GenDressedLepton_phi", &GenDressedLepton_phi);
	   		fChain_->SetBranchAddress("GenDressedLepton_pt", &GenDressedLepton_pt);
	   		fChain_->SetBranchAddress("GenDressedLepton_pdgId", &GenDressedLepton_pdgId);
	  	}
	} // isMC_
}

auto SkimTree::getEntries() const -> Long64_t {
    return fChain_ ? fChain_->GetEntries() : 0;
}

auto SkimTree::getChain() const -> TChain* {
    return fChain_.get();  // Return raw pointer to fChain_
}

auto SkimTree::getEntry(Long64_t entry) -> Int_t {
    return fChain_ ? fChain_->GetEntry(entry) : 0;
}

auto SkimTree::loadEntry(Long64_t entry) -> Long64_t {
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

