#include "TDRStyle.h"
#include "FigStyle.h"
#include "LoadSepErasInOnePlotTProfile.h"

LoadSepErasInOnePlotTProfile::LoadSepErasInOnePlotTProfile(): 
  channel_(""), 
  year_(""), 
  profileDir_(""), 
  profileName_("")
 {
  mcHTBins_.clear();
  dataEras_.clear();
  dataProfiles_.clear();
  mcProfiles_.clear();
}

// Clean up each cloned profile
LoadSepErasInOnePlotTProfile::~LoadSepErasInOnePlotTProfile() {
  for (auto profile : dataProfiles_) {
      delete profile; 
  }
  for (auto profile : mcProfiles_) {
      delete profile; 
  }
}

void LoadSepErasInOnePlotTProfile::setInputJson(const nlohmann::json &inputJson) {
  inputJson_ = inputJson;
}

// set Channels
void LoadSepErasInOnePlotTProfile::setChannel(const std::string & channel) {
  channel_ = channel;
}

// set Years
void LoadSepErasInOnePlotTProfile::setYear(const std::string & year) {
  year_ = year;
}

// set MCHTBins
void LoadSepErasInOnePlotTProfile::setMCHTBins(const std::vector<std::string>& htBins) {
  mcHTBins_ = htBins;
}

// set DataEras
void LoadSepErasInOnePlotTProfile::setDataEras(const std::vector<std::string>& dataEras) {
  dataEras_ = dataEras;
}

// set TProfileDir
void LoadSepErasInOnePlotTProfile::setTProfileDir(const std::string & profileDir) {
  profileDir_ = profileDir;
}

// set TProfileName
void LoadSepErasInOnePlotTProfile::setTProfileName(const std::string & profileName) {
  profileName_ = profileName;
}

// load Data TProfiles
void LoadSepErasInOnePlotTProfile::loadDataTProfiles() {
  for (const auto& era : dataEras_) {
    std::string fileName = inputJson_[channel_][year_]["Data"][era];
    std::string path = profileDir_ + "/" + profileName_;

    TFile file(fileName.c_str());
    if (file.IsZombie()) {
      std::cerr << "Error: Could not open file " << fileName << std::endl;
      return;
    }
    TProfile* profile = (TProfile*)file.Get(path.c_str());
    if (profile) {
			gROOT->cd();  // Change the directory to ROOT's global directory
      TProfile* clonedProfile = (TProfile*)profile->Clone(era.c_str());  // Clone the profile
      //std::cout<<era<<": "<< path<<": "<<clonedProfile->GetRMS()<<std::endl;
      dataProfiles_.push_back(clonedProfile);
    } else {
      std::cerr << "Error: Could not retrieve TProfile "<<path <<"from " << fileName << std::endl;
    }

    file.Close();  // Now it's safe to close the file since the TProfile has been cloned
  }
}

// load MC TProfiles
void LoadSepErasInOnePlotTProfile::loadMCTProfiles() {
  for (const auto& htBin : mcHTBins_) {
    std::string fileName = inputJson_[channel_][year_]["MC"][htBin];
    std::string path = profileDir_ + "/" + profileName_;

    TFile file(fileName.c_str());
    if (file.IsZombie()) {
      std::cerr << "Error: Could not open file " << fileName << std::endl;
      return;
    }
    TProfile* profile = (TProfile*)file.Get(path.c_str());
    if (profile) {
			gROOT->cd();  // Change the directory to ROOT's global directory
      TProfile* clonedProfile = (TProfile*)profile->Clone(htBin.c_str());  // Clone the profile
      //std::cout<<htBin<<": "<< path<<": "<<clonedProfile->GetRMS()<<std::endl;
      mcProfiles_.push_back(clonedProfile);
    } else {
      std::cerr << "Error: Could not retrieve TProfile "<<path <<"from " << fileName << std::endl;
    }

    file.Close();  // Now it's safe to close the file since the TProfile has been cloned
  }
}

// overlay Data with MC and Plot Ratio
void LoadSepErasInOnePlotTProfile::overlayDataWithMCInRatio(TFile* outRootFile, const std::string &outputFile) {
	outRootFile->cd();
  TDRStyle tdrStyle;
  tdrStyle.setTDRStyle();
  TCanvas canvas("c", "Data and MC Ratio", 600, 600);
  // Create a pad for the overlay
  TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.3, 1.0, 1.0);
  double legYmin = 0.85 - 0.05*dataProfiles_.size();
  TLegend *leg = new TLegend(0.55,legYmin,0.95,0.90); 
  //tdrStyle.CMS_lumi(pad1);
  pad1->SetBottomMargin(0.02);
  pad1->Draw();
  pad1->cd();
  FigStyle figStyle;
  for (int i=0; i<dataProfiles_.size(); i++){
    auto profile =  dataProfiles_.at(i);
		if(profile!= nullptr){
			//figStyle.setHistStyle(yourHistogram, "Histogram Title", profileName_, "Y-Axis Label");
			figStyle.setStyle(profile, "Profile Title", profileName_, "Response");
			figStyle.setColor(profile, i); 
    	if (i==0) {
    	  profile->Draw("HIST"); // Draw first profile
    	} else {
    	  profile->Draw("HIST SAME"); // overlay subsequent profiles
    	}
      leg->AddEntry(profile, profile->GetName(), "L");
		} 
		else {
    	std::cerr << "Error: dataProfiles_ is either empty or contains nullptr." << std::endl;
    	return;
		}
  }
  leg->Draw();
  // Create a pad for the ratio
  canvas.cd();
  TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.3);
  pad2->SetTopMargin(0.02);
  pad2->SetBottomMargin(0.4);
  pad2->Draw();
  pad2->cd();

  // load merged MC profile
  TProfile *mergedMCTProfile = combineTProfiles(mcProfiles_);
  if (!mergedMCTProfile) {
    std::cerr << "Error: Could not create merged MC profile." << std::endl;
    return;
  }

  for (int i=0; i<dataProfiles_.size(); i++){
    auto profile =  dataProfiles_.at(i);
    TProfile *ratioTProfile = (TProfile*)profile->Clone();
    ratioTProfile->Divide(mergedMCTProfile); // Data / Merged MC
    ratioTProfile->SetLineColor(kRed); // Example color for ratio
		figStyle.setStyleRatio(ratioTProfile, "Profile Title", profileName_, "Data/MC");
		figStyle.setColor(ratioTProfile, i); 
    if (i ==0) {
      ratioTProfile->Draw("HIST"); // Draw first profile
    } else {
      ratioTProfile->Draw("HIST SAME"); // overlay subsequent profiles
    }
  }
	// Save the canvas
	outRootFile->Write();
	canvas.SaveAs(outputFile.c_str());
}

TProfile* LoadSepErasInOnePlotTProfile::combineTProfiles(const std::vector<TProfile*>& profiles) {
    TProfile *combinedTProfile = nullptr;
    for (const auto &entry : profiles) {
        if (entry->GetEntries() == 0) {
            std::cerr << "Warning: A TProfile in the combine function has no entries." << std::endl;
            continue; // Skip empty profiles
        }
        if (!combinedTProfile) {
            combinedTProfile = (TProfile*)entry->Clone();
        } else {
            combinedTProfile->Add(entry);
        }
    }
    return combinedTProfile;
}

