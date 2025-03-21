#include "RunZeeJet.h"
#include "FigConfig.h"
#include "ReadConfig.h"
#include "Helper.h"
#include "PlotTime1Ds.h"
#include "PlotEras1D.h"
#include "PlotEra1Ds.h"
#include "PlotYear1Ds.h"
#include "PlotEra2D.h"
#include "PlotEraXYs.h"
#include "PlotErasXY.h"

#include <filesystem>
using json = nlohmann::json;
   
int RunZeeJet::Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide){
    channelSlide.startDocument("JME-21-001: L3Residual from ZeeJet channel");
    //addZeeJetSlides(channelSlide);

    allChannelSlide.addCenteredTextSlide("Next we show results from ZeeJet channel");

    std::string channel = "ZeeJet";
    std::vector<std::string> years;
    for (const auto& element : inputJson[channel].items()) {
        years.push_back(element.key());
    }

    ReadConfig readConfig("input/config/json/FigConfigZeeJet.json");
    const auto& figConfigVecTime1Ds = readConfig.getFigConfigVecTime1Ds();
    const auto& figConfigVecEras1D = readConfig.getFigConfigVecEras1D();
    const auto& figConfigVecEra1Ds = readConfig.getFigConfigVecEra1Ds();
    const auto& figConfigVecYear1Ds = readConfig.getFigConfigVecYear1Ds();
    const auto& figConfigVecEra2D  = readConfig.getFigConfigVecEra2D();
    const auto& figConfigVecErasXY = readConfig.getFigConfigVecErasXY();
    const auto& figConfigVecEraXYs  = readConfig.getFigConfigVecEraXYs();

	//-------------------------------------
	// Plot1D: As function of Run (Time) 
	//-------------------------------------
    if(isPlotTime1Ds){
        auto outPlotDirTime1Ds = eosPlotDir+"/"+channel+"/"+"Time1Ds"; 
        std::filesystem::create_directories(outPlotDirTime1Ds);
        channelSlide.addCenteredTextSlide("Next we show 1D plots");
        for (const auto & config: figConfigVecTime1Ds){
            auto names = config.histNames;
            std::string name("");
            for (const auto& n: names){name = n + "_";}
            auto dir  = config.histDir;
            if(isPrintFigConfig) config.print();
            std::vector<std::string> plotsForSlide;
            for (auto &year: years){
                auto time1D = std::make_unique<PlotTime1Ds<TProfile>>();
                time1D->setInput(inputJson, channel, year);
                time1D->setFigConfigTime1Ds(config);
                time1D->loadHists("Data");
                string outName = Helper::dirToName(dir);
                auto outPdfName = outPlotDirTime1Ds+"/"+year+"_"+outName+"_"+name+".pdf";
                time1D->overlayData(outPdfName);
                plotsForSlide.push_back(outPdfName);
            }
            channelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" for different Runs");
            allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from GamJet channel for different Runs");
        }//config
    }//isPlot1D

	//-------------------------------------
	// Plot1D: multiple hists from one era
	//-------------------------------------
    if(isPlotEra1Ds){
        auto outPlotDirEra1Ds = eosPlotDir+"/"+channel+"/"+"Era1Ds"; 
        std::filesystem::create_directories(outPlotDirEra1Ds);
        channelSlide.addCenteredTextSlide("Next we show 1D plots");
        for (const auto & config: figConfigVecEra1Ds){
            auto names = config.histNames;
            std::string name("");
            for (const auto& n: names){name = n + "_";}
            auto dir  = config.histDir;
            if(isPrintFigConfig) config.print();
            for (auto &year: years){
                std::vector<std::string> plotsForSlide;
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "MC");
                for (auto &era: dataEras){
                    auto era1Ds = std::make_unique<PlotEra1Ds<TProfile>>();
                    era1Ds->setInput(inputJson, channel, year);
                    era1Ds->setFigConfigEra1Ds(config);
                    era1Ds->loadHist("Data", era);
                    era1Ds->loadHists("MC", mcBins);
                    era1Ds->setOverlayMC(true);
                    string outName = Helper::dirToName(dir);
                    auto outPdfName = outPlotDirEra1Ds+"/"+year+"_"+era+"_"+outName+"_"+name+".pdf";
                    era1Ds->overlayDataWithMcInRatio(outPdfName);
                    plotsForSlide.push_back(outPdfName);
                }//era
                channelSlide.addPlotSlide(plotsForSlide, dir+"/Energy Fractions for "+ year);
                allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from GamJet channel for different years");
            }//year
        }//config
    }//isPlotEra1Ds

	//-------------------------------------
	// Plot1D: multiple eras in one plot
	//-------------------------------------
    if(isPlotEras1D){
        auto outPlotDirEras1D = eosPlotDir+"/"+channel+"/"+"Eras1D"; 
        std::filesystem::create_directories(outPlotDirEras1D);
        channelSlide.addCenteredTextSlide("Next we show 1D plots");
        for (const auto & config: figConfigVecEras1D){
            auto name = config.histName;
            auto dir  = config.histDir;
            if(isPrintFigConfig) config.print();
            std::vector<std::string> plotsForSlide;
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "MC");
                auto eras1D = std::make_unique<PlotEras1D<TProfile>>();
                eras1D->setInput(inputJson, channel, year);
                eras1D->setFigConfigEras1D(config);
                eras1D->loadHists("Data", dataEras);
                eras1D->loadHists("MC", mcBins);
                string outName = Helper::dirToName(dir);
                auto outPdfName = outPlotDirEras1D+"/"+year+"_"+outName+"_"+name+".pdf";
                eras1D->overlayDataWithMcInRatio(outPdfName);
                plotsForSlide.push_back(outPdfName);
            }//year
            channelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" for different years");
            allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from ZeeJet channel for different years");
        }//config
    }//isPlot1D


	//-------------------------------------
	// Plot2D: one era in one plot
	//-------------------------------------
    if(isPlotEra2D){
        channelSlide.addCenteredTextSlide("Next we show 2D plots");
        auto outPlotDirEra2D = eosPlotDir+"/"+channel+"/"+"Era2D"; 
        std::filesystem::create_directories(outPlotDirEra2D);
        for (const auto & config: figConfigVecEra2D){
            auto name = config.histName;
            auto dir  = config.histDir;
            if(isPrintFigConfig) config.print();
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "MC");
                auto dataErasOrMcBins = dataEras;
                dataErasOrMcBins.insert(dataErasOrMcBins.end(), mcBins.begin(), mcBins.end());
                std::vector<std::string> plotsForSlide;
                for (auto dataEraOrMcBin: dataErasOrMcBins){
                    auto era2D = std::make_unique<PlotEra2D<TProfile2D>>();
                    era2D->setInput(inputJson, channel, year);
                    era2D->setDataEraOrMcBin(dataEraOrMcBin);
                    era2D->setFigConfigEra2D(config);
                    string outName = Helper::dirToName(dir);
                    auto outPdfName = outPlotDirEra2D+"/"+dataEraOrMcBin+"_"+outName+"_"+name+".pdf";
                    era2D->drawHist2D(outPdfName);
                    plotsForSlide.push_back(outPdfName);
                }//eras
                channelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" for "+ year);
            }//year
        }//config
    }//isPlot2D

	//-------------------------------------
	// PlotXY: multiple eras in one plot
	//-------------------------------------
    if(isPlotErasXY){
        channelSlide.addCenteredTextSlide("Next we show Projection of X in ONE Y bin");
        auto outPlotDirErasXY = eosPlotDir+"/"+channel+"/"+"ErasXY"; 
        std::filesystem::create_directories(outPlotDirErasXY);
        for (const auto & config: figConfigVecErasXY){
            auto name = config.histName;
            auto dir  = config.histDir;
            if(isPrintFigConfig) config.print();
            std::vector<std::string> plotsForSlide;
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "MC");
                auto erasXY = std::make_unique<PlotErasXY<TProfile2D>>();
                erasXY->setInput(inputJson, channel, year);
                erasXY->setFigConfigErasXY(config);
                erasXY->loadHists(dataEras, "Data");
                erasXY->loadHists(mcBins, "MC");
                string outName = Helper::dirToName(dir);
                auto outPdfName = outPlotDirErasXY+"/"+year+"_"+outName+"_"+name+".pdf";
                erasXY->overlayDataWithMcInRatio(outPdfName);
                plotsForSlide.push_back(outPdfName);
            }
            channelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" for different years");
        }//year
    }//config

	//--------------------------
	// PlotXY: one era in one plot
	//--------------------------
    if(isPlotEraXYs){
        auto outPlotDirEraXYs = eosPlotDir+"/"+channel+"/"+"EraXYs"; 
        std::filesystem::create_directories(outPlotDirEraXYs);
        channelSlide.addCenteredTextSlide("Next we show Projection of X in MANY Y bins");
        for (const auto & config: figConfigVecEraXYs){
            auto name = config.histName;
            auto dir  = config.histDir;
            if(isPrintFigConfig) config.print();
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "MC");
                std::vector<std::string> plotsForSlide;
                for (const auto& dataEra: dataEras){
                    auto eraXY = std::make_unique<PlotEraXYs<TProfile2D>>();
                    eraXY->setInput(inputJson, channel, year);
                    eraXY->setFigConfigEraXYs(config);
                    eraXY->loadHists("Data", dataEra, std::vector<std::string>{});
                    eraXY->loadHists("MC", "", mcBins);
                    string outName = Helper::dirToName(dir);
                    auto outPdfName = outPlotDirEraXYs+"/"+dataEra+"_"+outName+"_"+name+".pdf";
                    eraXY->overlayDataWithMcInRatio(outPdfName);
                    plotsForSlide.push_back(outPdfName);
                }//eras
                channelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" for "+ year);
                allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from ZeeJet channel for "+ year);
            }//year
        }//config
    }//isPlotXY

    // After all plots are generated
    channelSlide.addCenteredTextSlide("Thank you!");
    channelSlide.endDocument();
    return 0;
}


void RunZeeJet::addZeeJetSlides(Slide & channelSlide){
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
  channelSlide.addTextSlide(samples16, "List of samples for 2016Pre and 2016Post"); 


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
  channelSlide.addTextSlide(samples17_18, "List of samples for 2017 and 2018"); 


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
  channelSlide.addTextSlide(triggers, "List of triggers for Run-2"); 

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
  channelSlide.addTextSlide(filters, "List of MET filters for Run-2"); 


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
  channelSlide.addTextSlide(jetVetoInfo, "The jet veto map"); 

  //----------------------------
  // JetVetoMap 
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> jecJer = {
    {"2016Pre", {
        "JEC: Summer19UL16APV_V7_MC, Summer19UL16APV_RunEra_V7_DATA",
        "JER: Summer20UL16APV_JRV3_MC"
    }},
    {"2016Post", {
        "JEC: Summer19UL16_V7_MC, Summer19UL16_RunEra_V7_DATA",
        "JER: Summer20UL16_JRV3_MC"
    }},
    {"2017", {
        "JEC: Summer19UL17_V5_MC, Summer19UL17_RunEra_V5_DATA",
        "JER: Summer19UL17_JRV2_MC"
    }},
    {"2018", {
        "JEC: Summer19UL18_V5_MC, Summer19UL18_RunEra_V5_DATA",
        "JER: Summer19UL18_JRV2_MC"
    }}
	};
  channelSlide.addTextSlide(jecJer, "JEC and JER"); 

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
  channelSlide.addTextSlide(objSel, "Object selection"); 

  //----------------------------
  // Event selection
  //----------------------------
    std::vector<std::string> cuts = {
    		"", "", "", "", "",
    		"", "", "", "",
    		"passJet1EtaJet2Pt", "passResponse"
    };
	std::vector<std::pair<std::string, std::vector<std::string>>> evtSel = {
    {"passSkim", {
        "We produce skim files from NanoAOD with HLT selections",
    }},
    {"passHLT", {
        "Then apply channel specific HLT",
    }},
    {"passGoodLumi", {
        "Events with good lumi sections",
    }},
    {"passMetFilter", {
        "Events passing the MET filters"
    }},
    {"passAtleast1Ref", {
        "nZ > =1",
    }},
    {"passAtleast1Jet", {
        "nJet > =1",
    }},
    {"passJetVetoMap", {
        "If any jet falls in the veto region",
    }},
    {"passDPhiRefJet1", {
        "|DeltaPhi(Z, leading jet) - pi| < 0.44",
    }},
    {"passRefBarrel", {
        "|eta| of Z < 1.3 ",
    }},
    {"passAlpha", {
        "subleading jet pT / Reference Z pT < 1.0",
    }},
    {"passResponse", {
        "|1 - DB| < 0.7, |1 - MPF| < 0.7|",
    }},
	};
  channelSlide.addTextSlide(evtSel, "Event selection: Cutflow"); 


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
  channelSlide.addTextSlide(respAndFrac, "Response and energy fractions in Z pT bins"); 
} 
