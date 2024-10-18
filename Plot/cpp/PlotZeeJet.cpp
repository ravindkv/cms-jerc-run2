#include "PlotZeeJet.h"
#include "FigConfig.h"
#include "ReadConfig.h"
#include "Helper.h"
#include "LoadEras1D.h"
#include "LoadEra2D.h"
#include "LoadEraXY.h"
#include "LoadErasXY.h"

using json = nlohmann::json;
   
int PlotZeeJet::Run(nlohmann::json inputJson, TFile *outRoot, MakeDPNote &chNote, MakeDPNote &allNote){
    chNote.startDocument("JME-21-001: L3Residual from ZeeJet channel");
    addZeeJetSlides(chNote);

    allNote.addCenteredTextSlide("Next we show results from ZeeJet channel");

    std::string channel = "ZeeJet";
    std::vector<std::string> years;
    for (const auto& element : inputJson[channel].items()) {
        years.push_back(element.key());
    }

    ReadConfig readConfig("config/FigZeeJet.json");
    const auto& figConfigMap = readConfig.getFigConfigMap();

    bool added1DSlide = false;
    bool added2DSlide = false;
    bool addedXYSlide = false;

    for (const auto & config: figConfigMap){
        std::string name = config.first;
        FigConfig figConfig = config.second;
        figConfig.print();
        std::string dir = figConfig.dirName;
		//-------------------------------------
		// Plot1D: multiple eras in one plot
		//-------------------------------------
		bool isH1 = name.rfind("h1", 0) == 0;
		bool isP1 = name.rfind("p1", 0) == 0;
        if(isPlot1D && (isH1 || isP1)){
            if (!added1DSlide) {
                chNote.addCenteredTextSlide("Next we show 1D plots");
                added1DSlide = true;
            }
            std::vector<std::string> plotsForSlide;
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "Mc");
                std::string plotOutPath = channel+"_"+year+"_"+dir+"_"+name+".pdf";
                plotsForSlide.push_back(plotOutPath);
                auto eras1D = std::make_unique<LoadEras1D<TProfile>>();
                eras1D->setInputJson(inputJson);
                eras1D->setChannel(channel);
                eras1D->setYear(year);
                eras1D->setMcHtBins(mcBins);
                eras1D->setDataEras(dataEras);
                eras1D->setHistDir(dir);
                eras1D->setHistName(name);
                eras1D->setFigConfig(figConfig);
                eras1D->loadDataHists();
                eras1D->loadMcHists();
                eras1D->overlayDataWithMcInRatio(outRoot, plotOutPath);
            }//year
            chNote.addPlotSlide(plotsForSlide, name+" for different years");
            allNote.addPlotSlide(plotsForSlide, name+" from ZeeJet channel for different years");
        }//isPlot1D


		//-------------------------------------
		// Plot2D: one era in one plot
		//-------------------------------------
		bool isH2 = name.rfind("h2", 0) == 0;
		bool isP2 = name.rfind("p2", 0) == 0;
        int varBinSize =  figConfig.varBins.size();
        if(isPlot2D && (isH2 || isP2) && varBinSize>0){
            if (!added2DSlide) {
                chNote.addCenteredTextSlide("Next we show 2D plots");
                added2DSlide = true;
            }
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "Mc");
                auto dataErasOrMcBins = dataEras;
                dataErasOrMcBins.insert(dataErasOrMcBins.end(), mcBins.begin(), mcBins.end());
                std::vector<std::string> plotsForSlide;
                for (auto dataEraOrMcBin: dataErasOrMcBins){
                    std::string plotOutPath = dataEraOrMcBin+"_"+dir+"_"+name+".pdf";
                    plotsForSlide.push_back(plotOutPath);
                    auto era2D = std::make_unique<LoadEra2D<TProfile2D>>();
                    era2D->setInputJson(inputJson);
                    era2D->setChannel(channel);
                    era2D->setYear(year);
                    era2D->setDataEraOrMcBin(dataEraOrMcBin);
                    era2D->setHistDir(dir);
                    era2D->setHistName(name);
                    era2D->setFigConfig(figConfig);
                    era2D->drawHist2D(outRoot, plotOutPath);
                }//eras
                chNote.addPlotSlide(plotsForSlide, name+" for "+ year);
            }//year
        }//isPlot2D

		//-------------------------------------
		// PlotXY: 
		//-------------------------------------
        if(isPlotXY && (isP2)){
	        //--------------------------
	        // multiple eras in one plot
	        //--------------------------
            std::vector<std::string> plotsForSlide;
            if (!addedXYSlide) {
                chNote.addCenteredTextSlide("Next we show XY plots");
                addedXYSlide = true;
            }
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "Mc");
                auto erasXY = std::make_unique<LoadErasXY<TProfile2D>>();
                erasXY->setInputJson(inputJson);
                erasXY->setChannel(channel);
                erasXY->setYear(year);
                erasXY->setMcHtBins(mcBins);
                erasXY->setDataEras(dataEras);
                erasXY->setFigConfig(figConfig);
                erasXY->setHistDir(dir);
                erasXY->setHistName(name);
                erasXY->loadDataHists();
                erasXY->loadMcHists();
                std::string plotOutPath = channel+"_"+year+"_"+dir+"_"+name+".pdf";
                erasXY->overlayDataWithMcInRatio(outRoot, plotOutPath);
                plotsForSlide.push_back(plotOutPath);
            }
            chNote.addPlotSlide(plotsForSlide, name+" for different years");

	        //--------------------------
	        // one era in one plot
	        //--------------------------
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "Mc");
                std::vector<std::string> plotsForSlide;
                for (const auto& dataEra: dataEras){
                    std::string plotOutPath = dataEra+"_"+dir+"_"+name+".pdf";
                    plotsForSlide.push_back(plotOutPath);
                    auto eraXY = std::make_unique<LoadEraXY<TProfile2D>>();
                    eraXY->setInputJson(inputJson);
                    eraXY->setChannel(channel);
                    eraXY->setYear(year);
                    eraXY->setMcHtBins(mcBins);
                    eraXY->setDataEra(dataEra);
                    eraXY->setFigConfig(figConfig);
                    eraXY->setHistDir(dir);
                    eraXY->setHistName(name);
                    eraXY->loadDataHists();
                    //eraXY->loadMcHists();
                    eraXY->overlayDataWithMcInRatio(outRoot, plotOutPath);
                }//eras
                chNote.addPlotSlide(plotsForSlide, name+" for "+ year);
                allNote.addPlotSlide(plotsForSlide, name+" from ZeeJet channel for "+ year);
            }//year
        }//isPlotXY
    }//figConfigs

    // After all plots are generated
    chNote.addCenteredTextSlide("Thank you!");
    chNote.endDocument();
    cout<<outRoot->GetName()<<endl;
    return 0;
}


void PlotZeeJet::addZeeJetSlides(MakeDPNote & chNote){
  //----------------------------
  // Samples
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> samples16 = {
  {"2016Pre", {
    "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16NanoAODAPVv9-20UL16APVJMENano_106X_mcRun2_asymptotic_preVFP_v11-v1/NANOAODSIM",
    "/DoubleEG/Run2016B-ver1_HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
    "/DoubleEG/Run2016B-ver2_HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
    "/DoubleEG/Run2016C/D/E/F-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/v2/NANOAOD",
  }},
  {"2016Post", {
    "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16NanoAODv9-20UL16JMENano_106X_mcRun2_asymptotic_v17-v1/NANOAODSIM",
    "/DoubleEG/Run2016F-UL2016_MiniAODv2_JMENanoAODv9-v2/NANOAOD",
    "/DoubleEG/Run2016G/H-UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
    }}
  };
  chNote.addTextSlide(samples16, "List of samples for 2016Pre and 2016Post"); 


  //----------------------------
  // Samples
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> samples17_18 = {
  {"2017",{
    "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17NanoAODv9-20UL17JMENano_106X_mc2017_realistic_v9-v1/NANOAODSIM",
    "/DoubleEG/Run2017B/C/D/E/F-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
  }},
  {"2018", {
      "/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18NanoAODv9-20UL18JMENano_106X_upgrade2018_realistic_v16_L1v1-v1/NANOAODSIM",
      "/EGamma/Run2018A/B/C/D-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
	}}
  };
  chNote.addTextSlide(samples17_18, "List of samples for 2017 and 2018"); 


  //----------------------------
  // Triggers
  //----------------------------
  std::vector<std::pair<std::string, std::vector<std::string>>> triggers = {
  {"2016UL", {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",
  }},
  {"2017UL", {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"
  }},
  {"2018UL",{
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"
  }}
  };
  chNote.addTextSlide(triggers, "List of triggers for Run-2"); 

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
  chNote.addTextSlide(filters, "List of MET filters for Run-2"); 


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
  chNote.addTextSlide(jetVetoInfo, "The jet veto map"); 

  //----------------------------
  // Object selection
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> objSel = {
    {"Electron", {
        "Tight cut based ID",
        "pT > = 25 GeV, |eta| =< 2.4, outside the gap",
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
  chNote.addTextSlide(objSel, "Object selection"); 

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
  chNote.addTextSlide(evtSel, "Event selection"); 


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
  chNote.addTextSlide(respAndFrac, "Response and energy fractions in Z pT bins"); 
} 
