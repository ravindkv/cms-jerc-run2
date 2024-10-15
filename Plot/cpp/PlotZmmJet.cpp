#include "PlotZmmJet.h"

using json = nlohmann::json;
using namespace std;
   
int PlotZmmJet::Run(nlohmann::json inputJson, TFile *outRoot, string & outLatex){
    MakeDPNote dpNote(outLatex);
    dpNote.startDocument("JME-21-001: L3Residual from ZmmJet channel");
		
    addZmmJetSlides(dpNote);

    string channel = "ZmmJet";
    //Eras in one plot. Make plots for all year
    std::vector<std::string> years;
    for (const auto& element : inputJson[channel].items()) {
      years.push_back(element.key());
    }

		//----------------------------
    if(isPlotTProfile){
      dpNote.addCenteredTextSlide("Next we show TProfiles for responses and energy fractions");

      std::vector<std::pair<std::string, std::string>> pNamesAndDir;
      std::string dir_ = "passJet1EtaJet2Pt";
      //pNamesAndDir.push_back(std::make_pair("p1JetEtaInRefPt", dir_));
      pNamesAndDir.push_back(std::make_pair("p1DbRespInRefPt", dir_));
      pNamesAndDir.push_back(std::make_pair("p1MpfRespInRefPt", dir_));
      pNamesAndDir.push_back(std::make_pair("p1JetChhefInRefPt", dir_));
      pNamesAndDir.push_back(std::make_pair("p1JetNehefInRefPt", dir_));
      pNamesAndDir.push_back(std::make_pair("p1JetNeemefInRefPt", dir_));
      pNamesAndDir.push_back(std::make_pair("p1JetChemefInRefPt", dir_));
      pNamesAndDir.push_back(std::make_pair("p1JetMuefInRefPt", dir_));

      for (const auto & nameAndDir: pNamesAndDir){
        string name = nameAndDir.first;
        string dir  = nameAndDir.second;
        // Create a map of plot paths and their corresponding descriptions
        vector<string> plotsForSlide;
        for (auto &year: years){
          vector<string> dataEras;
          for (const auto& element : inputJson[channel][year]["Data"].items()) {
            dataEras.push_back(element.key());
          }
          vector<string> mcBins;
          for (const auto& element : inputJson[channel][year]["MC"].items()) {
            mcBins.push_back(element.key());
          }
          string plotOutPath = channel+"_"+year+"_"+dir+"_"+name+".pdf";
          plotsForSlide.push_back(plotOutPath);
          if(!justLatex){
            auto eras1D = std::make_unique<LoadEras1D<TProfile>>();
            eras1D->setInputJson(inputJson);
            eras1D->setChannel(channel);
            eras1D->setYear(year);
            eras1D->setMCHTBins(mcBins);
            eras1D->setDataEras(dataEras);
            eras1D->setHistDir(dir);
            eras1D->setHistName(name);
            eras1D->loadDataHists(false);
            eras1D->loadMCHists(false);
            eras1D->overlayDataWithMCInRatio(outRoot, plotOutPath);
          }
        }//year
        // Add slide with multiple plots and descriptions
        dpNote.addPlotSlide(plotsForSlide, name+" for different years");
      }//nameAndDir
    }//isPlotTProfile

		//----------------------------
    if(isPlotTH1D){
      dpNote.addCenteredTextSlide("Next we show TH1D for events");

      std::vector<std::pair<std::string, std::string>> hNamesAndDir;
      std::string dir_ = "passAtleast1Ref";
      hNamesAndDir.push_back(std::make_pair("h1EventInRefPt", dir_));
      hNamesAndDir.push_back(std::make_pair("h1EventInDphiJetRef", "passRefBarrel"));
      hNamesAndDir.push_back(std::make_pair("h1EventInDrJetRef", "passRefBarrel"));

      for (const auto & nameAndDir: hNamesAndDir){
        string name = nameAndDir.first;
        string dir  = nameAndDir.second;
        // Create a map of plot paths and their corresponding descriptions
        vector<string> plotsForSlide;
        for (auto &year: years){
          vector<string> dataEras;
          for (const auto& element : inputJson[channel][year]["Data"].items()) {
            dataEras.push_back(element.key());
          }
          vector<string> mcBins;
          for (const auto& element : inputJson[channel][year]["MC"].items()) {
            mcBins.push_back(element.key());
          }
          string plotOutPath = channel+"_"+year+"_"+dir+"_"+name+".pdf";
          plotsForSlide.push_back(plotOutPath);
          if(!justLatex){
            auto eras1D = std::make_unique<LoadEras1D<TH1D>>();
            eras1D->setInputJson(inputJson);
            eras1D->setChannel(channel);
            eras1D->setYear(year);
            eras1D->setMCHTBins(mcBins);
            eras1D->setDataEras(dataEras);
            eras1D->setHistDir(dir);
            eras1D->setHistName(name);
            eras1D->loadDataHists(true);
            eras1D->loadMCHists(true);
            eras1D->overlayDataWithMCInRatio(outRoot, plotOutPath);
          }
        }//year
        // Add slide with multiple plots and descriptions
        dpNote.addPlotSlide(plotsForSlide, name+" for different years");
      }//nameAndDir
    }//isPlotTH1D

    //----------------------------
    if(isPlotProjectXorY){
      std::vector<std::pair<std::string, std::string>> p2NamesAndDir;
      std::string dir_ = "passJet1EtaJet2Pt";
      p2NamesAndDir.push_back(std::make_pair("p2DbRespInJet1EtaRefPt", dir_));
      dpNote.addCenteredTextSlide("Next we show Projection of X in Y bins");
      for (const auto & nameAndDir: p2NamesAndDir){
        string name = nameAndDir.first;
        string dir  = nameAndDir.second;
        // Create a map of plot paths and their corresponding descriptions
        for (auto &year: years){
          vector<string> dataEras;
          for (const auto& element : inputJson[channel][year]["Data"].items()) {
            dataEras.push_back(element.key());
          }
          vector<string> mcBins;
          for (const auto& element : inputJson[channel][year]["MC"].items()) {
            mcBins.push_back(element.key());
          }
          vector<string> plotsForSlide;
          for (const auto& dataEra: dataEras){
            string plotOutPath = dataEra+"_"+dir+"_"+name+".pdf";
            plotsForSlide.push_back(plotOutPath);
            if(!justLatex){
              auto eraXY = std::make_unique<LoadEraXY<TProfile2D>>();
              eraXY->setInputJson(inputJson);
              eraXY->setChannel(channel);
              eraXY->setYear(year);
              eraXY->setMCHTBins(mcBins);
              eraXY->setDataEra(dataEra);
              eraXY->setVarBins({0.0, 1.3, 2.5, 3.0, 3.5});
              eraXY->setVarName("#eta");
              eraXY->setVarIsOnXaxis(true);
              eraXY->setHistDir(dir);
              eraXY->setHistName(name);
              eraXY->loadDataHists();
              //eraXY->loadMCHists();
              eraXY->overlayDataWithMCInRatio(outRoot, plotOutPath);
            }
          }//eras
          // Add slide with multiple plots and descriptions
          dpNote.addPlotSlide(plotsForSlide, name+" for "+ year);
        }//year
      }//nameAndDir

      dpNote.addCenteredTextSlide("Next we show Projection of X in ONE Y bin");
      for (const auto & nameAndDir: p2NamesAndDir){
        string name = nameAndDir.first;
        string dir  = nameAndDir.second;
        // Create a map of plot paths and their corresponding descriptions
        vector<string> plotsForSlide;
        for (auto &year: years){
          vector<string> dataEras;
          for (const auto& element : inputJson[channel][year]["Data"].items()) {
            dataEras.push_back(element.key());
          }
          vector<string> mcBins;
          for (const auto& element : inputJson[channel][year]["MC"].items()) {
            mcBins.push_back(element.key());
          }
          string plotOutPath = channel+"_"+year+"_"+dir+"_"+name+".pdf";
          plotsForSlide.push_back(plotOutPath);
          if(!justLatex){
            auto erasXY = std::make_unique<LoadErasXY<TProfile2D>>();
            erasXY->setInputJson(inputJson);
            erasXY->setChannel(channel);
            erasXY->setYear(year);
            erasXY->setMCHTBins(mcBins);
            erasXY->setDataEras(dataEras);
            erasXY->setVarMin(0.0);
            erasXY->setVarMax(1.3);
            erasXY->setVarName("#eta");
            erasXY->setVarIsOnXaxis(true);
            erasXY->setHistDir(dir);
            erasXY->setHistName(name);
            erasXY->loadDataHists();
            erasXY->loadMCHists();
            erasXY->overlayDataWithMCInRatio(outRoot, plotOutPath);
          }
        }//year
        dpNote.addPlotSlide(plotsForSlide, name+" for different years");
      }//nameAndDir
    }//isPlotProjectXorY


    //----------------------------
    if(isPlotTProfile2D){
      dpNote.addCenteredTextSlide("Next we show TProfile2D for responses");

      std::vector<std::pair<std::string, std::string>> h2NamesAndDir;
      std::string dir_ = "passJet1EtaJet2Pt";
      h2NamesAndDir.push_back(std::make_pair("p2DbRespInJet1EtaRefPt", dir_));
      h2NamesAndDir.push_back(std::make_pair("p2MpfRespInJet1EtaRefPt", dir_));

      for (const auto & nameAndDir: h2NamesAndDir){
        string name = nameAndDir.first;
        string dir  = nameAndDir.second;
        // Create a map of plot paths and their corresponding descriptions
        for (auto &year: years){
          vector<string> dataErasOrMCBins;
          for (const auto& element : inputJson[channel][year]["Data"].items()) {
            dataErasOrMCBins.push_back(element.key());
          }
          for (const auto& element : inputJson[channel][year]["MC"].items()) {
            dataErasOrMCBins.push_back(element.key());
          }
          vector<string> plotsForSlide;
          for (auto dataEraOrMCBin: dataErasOrMCBins){
            string plotOutPath = dataEraOrMCBin+"_"+dir+"_"+name+".pdf";
            plotsForSlide.push_back(plotOutPath);
            if(!justLatex){
              auto era2D = std::make_unique<LoadEra2D<TProfile2D>>();
              era2D->setInputJson(inputJson);
              era2D->setChannel(channel);
              era2D->setYear(year);
              era2D->setDataEraOrMCBin(dataEraOrMCBin);
              era2D->setHistDir(dir);
              era2D->setHistName(name);
              era2D->drawHist2D(outRoot, plotOutPath);
            }
          }//eras
          dpNote.addPlotSlide(plotsForSlide, name+" for "+ year);
        }//year
      }//nameAndDir
    }//isPlotTProfile2D


		//----------------------------
    if(isPlotTH2D){
      dpNote.addCenteredTextSlide("Next we show TH2D for events");

      std::vector<std::pair<std::string, std::string>> h2NamesAndDir;
      std::string dir_ = "passAtleast1Ref";
      h2NamesAndDir.push_back(std::make_pair("h2EventInRefEtaRefPhi", dir_));

      for (const auto & nameAndDir: h2NamesAndDir){
        string name = nameAndDir.first;
        string dir  = nameAndDir.second;
        // Create a map of plot paths and their corresponding descriptions
        for (auto &year: years){
          vector<string> dataErasOrMCBins;
          for (const auto& element : inputJson[channel][year]["Data"].items()) {
            dataErasOrMCBins.push_back(element.key());
          }
          for (const auto& element : inputJson[channel][year]["MC"].items()) {
            dataErasOrMCBins.push_back(element.key());
          }
          
          vector<string> plotsForSlide;
          for (auto dataEraOrMCBin: dataErasOrMCBins){
            string plotOutPath = dataEraOrMCBin+"_"+dir+"_"+name+".pdf";
            plotsForSlide.push_back(plotOutPath);
            if(!justLatex){
              auto era2D = std::make_unique<LoadEra2D<TH2D>>();
              era2D->setInputJson(inputJson);
              era2D->setChannel(channel);
              era2D->setYear(year);
              era2D->setDataEraOrMCBin(dataEraOrMCBin);
              era2D->setHistDir(dir);
              era2D->setHistName(name);
              era2D->drawHist2D(outRoot, plotOutPath);
            }
          }//eras
          dpNote.addPlotSlide(plotsForSlide, name+" for "+ year);
        }//year
      }//nameAndDir
    }//isPlotTH2D


    // After all plots are generated
    dpNote.addCenteredTextSlide("Thank you!");
    dpNote.endDocument();
    cout<<outRoot->GetName()<<endl;
    cout<<outLatex<<endl;
    return 0;
}


void PlotZmmJet::addZmmJetSlides(MakeDPNote & dpNote){
  //----------------------------
  // Samples
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> samples16 = {
  {"2016Pre", {
    "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16NanoAODAPVv9-20UL16APVJMENano_106X_mcRun2_asymptotic_preVFP_v11-v1/NANOAODSIM",
    "/DoubleMuon/Run2016B-ver1/ver2_HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
    "/DoubleMuon/Run2016C/D/E/F-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/v2/NANOAOD",
  }},
  {"2016Post", {
    "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16NanoAODv9-20UL16JMENano_106X_mcRun2_asymptotic_v17-v1/NANOAODSIM",
    "/DoubleMuon/Run2016F-UL2016_MiniAODv2_JMENanoAODv9-v2/NANOAOD",
    "/DoubleMuon/Run2016G/H-UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
    }}
  };
  dpNote.addTextSlide(samples16, "List of samples for 2016Pre and 2016Post"); 


  //----------------------------
  // Samples
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> samples17_18 = {
  {"2017",{
    "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17NanoAODv9-20UL17JMENano_106X_mc2017_realistic_v9-v1/NANOAODSIM",
    "/DoubleMuon/Run2017B/C/D/E/F-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
  }},
  {"2018", {
      "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18NanoAODv9-20UL18JMENano_106X_upgrade2018_realistic_v16_L1v1-v1/NANOAODSIM",
      "/DoubleMuon/Run2018A/B/C/D-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD"
	}}
  };
  dpNote.addTextSlide(samples17_18, "List of samples for 2017 and 2018"); 


  //----------------------------
  // Triggers
  //----------------------------
  std::vector<std::pair<std::string, std::vector<std::string>>> triggers = {
  {"2016UL", {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"
  }},
  {"2017UL", {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8",
  }},
  {"2018UL",{
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8",
  }}
  };
  dpNote.addTextSlide(triggers, "List of triggers for Run-2"); 

  //----------------------------
  // Filters 
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> filters = {
  {"2016UL", {
        "Flag_goodVertices",
        "Flag_globalSuperTightHalo2016Filter",
        "Flag_HBHENoiseFilter",
        "Flag_HBHENoiseIsoFilter",
        "Flag_EcalDeadCellTriggerPrimitiveFilter",
        "Flag_BadPFMuonFilter",
        "Flag_BadPFMuonDzFilter",
        "Flag_eeBadScFilter"
    }},
  {"2017-18UL", {
        "2016UL + Flag_ecalBadCalibFilter"
  }}
  };
  dpNote.addTextSlide(filters, "List of MET filters for Run-2"); 


  //----------------------------
  // JetVetoMap 
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> jetVetoInfo = {
    {"2016Pre", {
        "jetVetoJsonPath: POG/JME/2016preVFP_UL/jetvetomaps.json.gz",
        "jetVetoName: Summer19UL16_V1",
        "jetVetoKey: jetvetomap_hot"
    }},
    {"2016Post", {
        "jetVetoJsonPath: POG/JME/2016postVFP_UL/jetvetomaps.json.gz",
        "jetVetoName: Summer19UL16_V1",
        "jetVetoKey: jetvetomap_hot"
    }},
    {"2017", {
        "jetVetoJsonPath: POG/JME/2017_UL/jetvetomaps.json.gz",
        "jetVetoName: Summer19UL17_V1",
        "jetVetoKey: jetvetomap_hot"
    }},
    {"2018", {
        "jetVetoJsonPath: POG/JME/2018_UL/jetvetomaps.json.gz",
        "jetVetoName: Summer19UL18_V1",
        "jetVetoKey: jetvetomap_hot"
    }}
	};
  dpNote.addTextSlide(jetVetoInfo, "The jet veto map"); 

  //----------------------------
  // Object selection
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> objSel = {
    {"Muon", {
        "Tight cut based ID",
        "pT > = 20 GeV, |eta| =< 2.3, dxy < 0.2, dz < 0.5",
    }},
    {"Jets", {
        "Tight lepton veto ID",
        "pT > 12 GeV, deltaR (Ref, jet) >= 0.2",
    }},
    {"Ref Z boson", {
        "Opposite charged lepton",
        "| mass - 91.1876 | < 20, pT > 15, |eta| = < 1.3",
    }}
	};
  dpNote.addTextSlide(objSel, "Object selection"); 

  //----------------------------
  // Event selection
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> evtSel = {
    {"HLT", {
        "Events passing the HLT",
    }},
    {"Golden Lumi", {
        "Events with good lumi sections",
    }},
    {"MET filters", {
        "Events passing the MET filters"
    }},
    {"Object", {
        "nZ >= 1, nJet > =1",
        "|DeltaPhi(Z, leading jet) - pi| < 0.44",
        "leading jet eta < 1.3 ",
        "sub-leading jet pT < 30 or < pT of Z "
    }}
	};
  dpNote.addTextSlide(evtSel, "Event selection"); 


  //----------------------------
  // Response and Energy fractions 
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> respAndFrac = {
    {"Response and Correction Types", {
        "Db = Direct Balance", 
        "MPF = MET Projection Fraction", 
        "Resp = Response"
    }},
    {"Energy Fractions", {
        "Chhef = Charged Hadron Energy Fraction", 
        "Nehef = Neutral Hadron Energy Fraction", 
        "Neemef = Neutral Electron Magnetic Energy Fraction", 
        "Chemef = Charged Electron Magnetic Energy Fraction", 
        "Muef = Muon Energy Fraction"
    }}
	};
  dpNote.addTextSlide(respAndFrac, "Response and energy fractions in Z pT bins"); 
} 
