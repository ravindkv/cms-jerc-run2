#include "PlotZeeJet.h"
#include "MakeDPNote.h"
#include "LoadSepErasInOnePlotTProfile.h"

using json = nlohmann::json;
using namespace std;
   
int PlotZeeJet::Run(nlohmann::json inputJson, TFile *outRoot, string & outLatex){
		MakeDPNote dpNote(outLatex);
    dpNote.startDocument("My Plots");

    string channel = "ZeeJet";
    string profileDir_  = "passJet1EtaJet2Pt";
    map<string, string> profileNameAndDir;
    profileNameAndDir["pJetEtaInRefPt"] = profileDir_;
    profileNameAndDir["pDbRespInRefPt"] = profileDir_;
    profileNameAndDir["pMpfRespInRefPt"] = profileDir_;
    profileNameAndDir["pJetChhefInRefPt"] = profileDir_;
    profileNameAndDir["pJetNhefInRefPt"] = profileDir_;
    profileNameAndDir["pJetNeemefInRefPt"] = profileDir_;
    profileNameAndDir["pJetChemefInRefPt"] = profileDir_;
    profileNameAndDir["pJetMuefInRefPt"] = profileDir_;

    for (const auto & nameAndDir: profileNameAndDir){
      string profileName = nameAndDir.first;
      string profileDir  = nameAndDir.second;
      // Create a map of plot paths and their corresponding descriptions
      vector<string> plotsForSlide;
      
      //Eras in one plot. Make plots for all year
      std::vector<std::string> years;
      for (const auto& element : inputJson[channel].items()) {
        years.push_back(element.key());
      }
      
      for (auto &year: years){
        vector<string> dataEras;
        for (const auto& element : inputJson[channel][year]["Data"].items()) {
          dataEras.push_back(element.key());
        }
        vector<string> mcBins;
        for (const auto& element : inputJson[channel][year]["MC"].items()) {
          mcBins.push_back(element.key());
        }
        LoadSepErasInOnePlotTProfile* sepErasPlotter = new LoadSepErasInOnePlotTProfile();
        sepErasPlotter->setInputJson(inputJson);
        sepErasPlotter->setChannel(channel);
        sepErasPlotter->setYear(year);
        sepErasPlotter->setMCHTBins(mcBins);
        sepErasPlotter->setDataEras(dataEras);
        sepErasPlotter->setTProfileDir(profileDir);
        sepErasPlotter->setTProfileName(profileName);
      
        sepErasPlotter->loadDataTProfiles();
        sepErasPlotter->loadMCTProfiles();
      
        string plotOutPath = channel+"_"+year+"_"+profileDir+"_"+profileName+".pdf";
        string plotDescription = profileName+" for "+year;
        plotsForSlide.push_back(plotOutPath);
        sepErasPlotter->overlayDataWithMCInRatio(outRoot, plotOutPath);
      }
      // Add slide with multiple plots and descriptions
      dpNote.addSlide(plotsForSlide, profileName+" for different years");
    }

    // After all plots are generated
    dpNote.endDocument();
    cout<<outRoot->GetName()<<endl;
    cout<<outLatex<<endl;
    return 0;
}
   
