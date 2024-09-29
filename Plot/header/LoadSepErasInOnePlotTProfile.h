#ifndef LoadSepErasInOnePlotTProfile_H
#define LoadSepErasInOnePlotTProfile_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <TH1F.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TPad.h>
#include <TROOT.h>
#include <THStack.h>
#include <TProfile.h>
#include <nlohmann/json.hpp>

class LoadSepErasInOnePlotTProfile {
public:
  LoadSepErasInOnePlotTProfile();
 ~LoadSepErasInOnePlotTProfile();
  
  // setters
  void setInputJson(const nlohmann::json &inputJson);
  void setChannel(const std::string & channel);
  void setYear(const std::string & year);
  void setMCHTBins(const std::vector<std::string>& htBins);
  void setDataEras(const std::vector<std::string>& dataEras);
  void setTProfileDir(const std::string &profileDir);
  void setTProfileName(const std::string &profileName);
  
  // loaders
  void loadDataTProfiles();
  void loadMCTProfiles();

  void overlayDataWithMCInRatio(TFile* outRootFile, const std::string& outputFileName);


private:
  nlohmann::json inputJson_;
  std::string channel_;
  std::string year_;
  std::vector<std::string> dataEras_;
  std::vector<std::string> mcHTBins_;
  std::string profileDir_;
  std::string profileName_;
  
  std::vector<TProfile*> dataProfiles_;  
  std::vector<TProfile*> mcProfiles_;  
  TProfile* combineTProfiles(const std::vector<TProfile*>& profiles);

};

#endif

