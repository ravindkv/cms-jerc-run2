#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "SkimTree.h"

SkimTree::SkimTree(GlobalFlag& globalFlags): 
    globalFlags_(globalFlags),
    trigDetail_(globalFlags),
    year_(globalFlags_.getYear()),
    era_(globalFlags_.getEra()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()),
    isData_(globalFlags_.isData()),
    isMC_(globalFlags_.isMC()),
	fCurrent_(-1), 
	fChain_(std::make_unique<TChain>("Events")) {
}

SkimTree::~SkimTree() {
    // unique_ptr will automatically delete fChain_
}

// Helper function: Validate and open a file
TFile* SkimTree::validateAndOpenFile(const std::string& fullPath) {
    TFile* file = TFile::Open(fullPath.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Failed to open or corrupted file " << fullPath << '\n';
        if (file) file->Close();
        return nullptr;
    }
    // Check file size (using 3000 bytes as a threshold)
    if (file->GetSize() < 3000) {
        std::cerr << "Warning: file " << fullPath << " has less than 3000 bytes, skipping.\n";
        file->Close();
        return nullptr;
    }
    TTree* tree = dynamic_cast<TTree*>(file->Get("Events"));
    if (!tree) {
        std::cerr << "Error: 'Events' not found in " << fullPath << '\n';
        file->Close();
        return nullptr;
    }

    if (tree->GetEntries() == 0) {
        std::cerr << "Warning: 'Events' TTree in file " << fullPath << " has 0 entries. Skipping file.\n";
        file->Close();
        return nullptr;
    }
    return file;
}

// Helper function: Add file to TChains
bool SkimTree::addFileToChains(const std::string& fullPath) {
    int added = fChain_->Add(fullPath.c_str());
    if (added == 0) {
        std::cerr << "Warning: TChain::Add failed for " << fullPath << '\n';
        return false;
    }
    return true;
}

void SkimTree::loadTree(std::vector<std::string> skimFileList) {
    std::cout << "==> loadTree()" << '\n';
    if (!fChain_) {
        fChain_ = std::make_unique<TChain>("Events");
    }
    fChain_->SetCacheSize(100 * 1024 * 1024);

    int totalFiles = 0;
    int addedFiles = 0;
    int failedFiles = 0;

    if (skimFileList.empty()) {
        throw std::runtime_error("Error: No files to load in loadTree()");
    }

    std::string dir = ""; // Adjust as needed
    for (const auto& fName : skimFileList) {
        totalFiles++;
        std::string fullPath = fName;
        std::cout << fullPath << '\n';
        // Validate and open the file
        TFile* file = validateAndOpenFile(fullPath);
        if (!file) {
            failedFiles++;
            continue;
        }
        // Add file to the TChains
        if (!addFileToChains(fullPath)) {
            failedFiles++;
            file->Close();
            continue;
        }
        std::cout << "Total Entries: " << fChain_->GetEntries()<< '\n';
        addedFiles++;
        file->Close();
    }

    // Final summary
    std::cout << "==> Finished loading files.\n";
    std::cout << "Total files processed: " << totalFiles << '\n';
    std::cout << "Successfully added files: " << addedFiles << '\n';
    std::cout << "Failed to add files: " << failedFiles << '\n';

    if (fChain_->GetNtrees() == 0) {
        std::cerr << "Error: No valid ROOT files were added to the TChain. Exiting.\n";
        return;
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
	//fChain_->SetBranchAddress("Jet_btagDeepFlavUDS", &Jet_btagDeepFlavUDS);

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
	
	//--------------------------------------- 
	// HLT 
	//--------------------------------------- 
	initializeTriggers();

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
	}//GamJet
	
	//--------------------------------------- 
	// Electron (for DiEleJet)
	//--------------------------------------- 
	if(channel_ == GlobalFlag::Channel::ZeeJet || channel_ == GlobalFlag::Channel::Wqqe){
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
	    fChain_->SetBranchAddress("Jet_electronIdx1", &Jet_electronIdx1);
	    fChain_->SetBranchAddress("Jet_electronIdx2", &Jet_electronIdx2);
	
	}
	
	//--------------------------------------- 
	// Muon (for DiMuJet)
	//--------------------------------------- 
	if (channel_ == GlobalFlag::Channel::ZmmJet || channel_ == GlobalFlag::Channel::Wqqm){
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
	    fChain_->SetBranchAddress("Jet_muonIdx1", &Jet_muonIdx1);
	    fChain_->SetBranchAddress("Jet_muonIdx2", &Jet_muonIdx2);
	}
	
	fChain_->SetBranchAddress("ChsMET_phi", &ChsMET_phi);
	fChain_->SetBranchAddress("ChsMET_pt",  &ChsMET_pt);
	fChain_->SetBranchAddress("fixedGridRhoFastjetAll", &Rho);
	fChain_->SetBranchAddress("PV_z", &PV_z);
	//fChain_->SetBranchAddress("GenVtx_z", &GenVtx_z);
	fChain_->SetBranchAddress("PV_npvs", &PV_npvs);
	fChain_->SetBranchAddress("PV_npvsGood", &PV_npvsGood);
	
	
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


//--------------------------------------- 
// HLT 
//--------------------------------------- 
void SkimTree::initializeTriggers() {
    const auto& triggerNames = trigDetail_.getTrigNames();

    size_t numTriggers = triggerNames.size();
    std::cout<<"numTriggers = "<<numTriggers<<std::endl;
    // Reserve space to prevent reallocations
    trigNames_.reserve(numTriggers);
    trigValues_.reserve(numTriggers);
    trigNameToIndex_.reserve(numTriggers);

    for (size_t index = 0; index < numTriggers; ++index) {
        const auto& trigName = triggerNames[index];
        trigNames_.emplace_back(trigName);
        trigValues_.emplace_back(0); // Initialize to 0 (false)
        trigNameToIndex_.emplace(trigName, index);

        // Set branch status and address
        fChain_->SetBranchStatus(trigName.c_str(), true);
        fChain_->SetBranchAddress(trigName.c_str(), &trigValues_[index]);
    }
    // After population
    if (isDebug_) {
        std::cout << "Initialized " << trigNames_.size() << " trigger(s)" << std::endl;
    }
    
    if (trigNames_.size() != trigValues_.size()) {
        throw std::runtime_error("Mismatch between trigNames_ and trigValues sizes during initialization.");
    }
}


Bool_t SkimTree::getTrigValue(const std::string& trigName) const {
    auto it = trigNameToIndex_.find(trigName);
    if (it != trigNameToIndex_.end()) {
        return trigValues_[it->second] != 0; // Convert to bool
    }
    else{
        if (isDebug_) {
            std::cerr << "Trigger name not found: " << trigName << std::endl;
        }
        return false; // Default to false if trigger not found
    }
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

