#include "RunGamJet.h"
#include "FigConfig.h"
#include "ReadConfig.h"
#include "Helper.h"
#include "PlotEras1D.h"
#include "PlotEra2D.h"
#include "PlotEraXY.h"
#include "PlotErasXY.h"

#include <filesystem>
using json = nlohmann::json;
   
int RunGamJet::Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide){
    channelSlide.startDocument("JME-21-001: L3Residual from GamJet channel");
    addGamJetSlides(channelSlide);

    allChannelSlide.addCenteredTextSlide("Next we show results from GamJet channel");

    std::string channel = "GamJet";
    std::vector<std::string> years;
    for (const auto& element : inputJson[channel].items()) {
        years.push_back(element.key());
    }

    ReadConfig readConfig("input/config/json/FigConfigGamJet.json");
    const auto& figConfigVecEras1D = readConfig.getFigConfigVecEras1D();
    const auto& figConfigVecEra2D  = readConfig.getFigConfigVecEra2D();
    const auto& figConfigVecErasXY = readConfig.getFigConfigVecErasXY();
    const auto& figConfigVecEraXY  = readConfig.getFigConfigVecEraXY();

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
            allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from GamJet channel for different years");
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
                    auto outPdfName = outPlotDirEra2D+"/"+dataEraOrMcBin+"_"+dir+"_"+name+".pdf";
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
                auto outPdfName = outPlotDirErasXY+"/"+year+"_"+dir+"_"+name+".pdf";
                erasXY->overlayDataWithMcInRatio(outPdfName);
                plotsForSlide.push_back(outPdfName);
            }
            channelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" for different years");
        }//year
    }//config

	//--------------------------
	// PlotXY: one era in one plot
	//--------------------------
    if(isPlotEraXY){
        auto outPlotDirEraXY = eosPlotDir+"/"+channel+"/"+"EraXY"; 
        std::filesystem::create_directories(outPlotDirEraXY);
        channelSlide.addCenteredTextSlide("Next we show Projection of X in MANY Y bins");
        for (const auto & config: figConfigVecEraXY){
            auto name = config.histName;
            auto dir  = config.histDir;
            if(isPrintFigConfig) config.print();
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "MC");
                std::vector<std::string> plotsForSlide;
                for (const auto& dataEra: dataEras){
                    auto eraXY = std::make_unique<PlotEraXY<TProfile2D>>();
                    eraXY->setInput(inputJson, channel, year);
                    eraXY->setFigConfigEraXY(config);
                    eraXY->loadHists("Data", dataEra, std::vector<std::string>{});
                    eraXY->loadHists("MC", "", mcBins);
                    auto outPdfName = outPlotDirEraXY+"/"+dataEra+"_"+dir+"_"+name+".pdf";
                    eraXY->overlayDataWithMcInRatio(outPdfName);
                    plotsForSlide.push_back(outPdfName);
                }//eras
                channelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" for "+ year);
                allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from GamJet channel for "+ year);
            }//year
        }//config
    }//isPlotXY

    // After all plots are generated
    channelSlide.addCenteredTextSlide("Thank you!");
    channelSlide.endDocument();
    return 0;
}


void RunGamJet::addGamJetSlides(Slide & channelSlide){
  //----------------------------
  // Samples
  //----------------------------
	std::vector<std::pair<std::string, std::vector<std::string>>> samples2016Pre = {
  {"2016Pre", {
            "/GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODAPVv9-20UL16APVJMENano_4cores5k_106X_mcRun2_asymptotic_preVFP_v11-v1/NANOAODSIM",
            "/GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODAPVv9-20UL16APVJMENano_106X_mcRun2_asymptotic_preVFP_v11-v1/NANOAODSIM",
            "/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODAPVv9-20UL16APVJMENano_106X_mcRun2_asymptotic_preVFP_v11-v1/NANOAODSIM",
            "/GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODAPVv9-20UL16APVJMENano_106X_mcRun2_asymptotic_preVFP_v11-v1/NANOAODSIM",
            "/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODAPVv9-20UL16APVJMENano_106X_mcRun2_asymptotic_preVFP_v11-v1/NANOAODSIM"
            "/DoubleEG/Run2016B-ver1_HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2016B-ver2_HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2016C-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2016D-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2016E-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2016F-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v2/NANOAOD"
  }}
  };
  channelSlide.addTextSlide(samples2016Pre, "List of samples for 2016Pre"); 


	std::vector<std::pair<std::string, std::vector<std::string>>> samples2016Post = {
  {"2016Post", {
            "/GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODv9-20UL16JMENano_4cores5k_106X_mcRun2_asymptotic_v17-v1/NANOAODSIM",
            "/GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODv9-20UL16JMENano_106X_mcRun2_asymptotic_v17-v1/NANOAODSIM",
            "/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODv9-20UL16JMENano_106X_mcRun2_asymptotic_v17-v1/NANOAODSIM",
            "/GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODv9-20UL16JMENano_106X_mcRun2_asymptotic_v17-v1/NANOAODSIM",
            "/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16NanoAODv9-20UL16JMENano_106X_mcRun2_asymptotic_v17-v1/NANOAODSIM"
            "/DoubleEG/Run2016F-UL2016_MiniAODv2_JMENanoAODv9-v2/NANOAOD",
            "/DoubleEG/Run2016G-UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2016H-UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD"
  }}
  };
  channelSlide.addTextSlide(samples2016Post, "List of samples for 2016Post"); 


	std::vector<std::pair<std::string, std::vector<std::string>>> samples2017 = {
  {"2017", {
            "/GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17NanoAODv9-4cores5k_106X_mc2017_realistic_v9-v1/NANOAODSIM",
            "/GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1/NANOAODSIM",
            "/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1/NANOAODSIM",
            "/GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1/NANOAODSIM",
            "/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL17NanoAODv9-106X_mc2017_realistic_v9-v1/NANOAODSIM"
            "/DoubleEG/Run2017B-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2017C-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2017D-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2017E-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/DoubleEG/Run2017F-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD"
  }}
  };
  channelSlide.addTextSlide(samples2017, "List of samples for 2017"); 


	std::vector<std::pair<std::string, std::vector<std::string>>> samples2018 = {
  {"2018", {
            "/GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18NanoAODv9-4cores5k_106X_upgrade2018_realistic_v16_L1v1-v1/NANOAODSIM",
            "/GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1/NANOAODSIM",
            "/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1/NANOAODSIM",
            "/GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1/NANOAODSIM",
            "/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL18NanoAODv9-106X_upgrade2018_realistic_v16_L1v1-v1/NANOAODSIM"
            "/EGamma/Run2018A-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/EGamma/Run2018B-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/EGamma/Run2018C-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD",
            "/EGamma/Run2018D-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD"
  }}
  };
  channelSlide.addTextSlide(samples2018, "List of samples for 2018"); 


  //----------------------------
  // Triggers
  //----------------------------
  std::vector<std::pair<std::string, std::vector<std::string>>> triggers = {
  {"Common for all years", {
    "HLT_Photon200 OR HLT_Photon175 OR HLT_Photon150 OR HLT_Photon120 OR HLT_Photon90 OR HLT_Photon75 OR HLT_Photon50", 
    "HLT_Photon33 OR HLT_Photon20 OR HLT_Photon90_R9Id90_HE10_IsoM OR HLT_Photon75_R9Id90_HE10_IsoM OR HLT_Photon50_R9Id90_HE10_IsoM"
  }},
  {"2016UL", {
    "HLT_Photon36 OR HLT_Photon30 OR HLT_Photon22 OR HLT_Photon165_R9Id90_HE10_IsoM OR HLT_Photon120_R9Id90_HE10_IsoM", 
    "HLT_Photon36_R9Id90_HE10_IsoM OR HLT_Photon30_R9Id90_HE10_IsoM OR HLT_Photon22_R9Id90_HE10_IsoM",  
  }},
  {"2017UL", {
    "HLT_Photon165_R9Id90_HE10_IsoM OR HLT_Photon120_R9Id90_HE10_IsoM OR HLT_Photon60_HoverELoose", 
    "HLT_Photon50_HoverELoose OR HLT_Photon40_HoverELoose OR HLT_Photon30_HoverELoose OR HLT_Photon20_HoverELoose",  
  }},
  {"2018UL", {
    "HLT_Photon120EB_TightID_TightIso OR HLT_Photon110EB_TightID_TightIso OR HLT_Photon100EB_TightID_TightIso OR HLT_Photon30_HoverELoose OR HLT_Photon20_HoverELoose",  
  }},
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
  // JEC and JER
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
    {"Photon", {
        "Tight cut based ID",
        "pT > = 20 GeV, |eta| =< 1.4442",
    }},
    {"Jets", {
        "Tight lepton veto ID",
        "pT > 12 GeV, deltaR (Ref, jet) >= 0.2",
    }}
	};
  channelSlide.addTextSlide(objSel, "Object selection"); 

  //----------------------------
  // Event selection
  //----------------------------
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
        "nPhoton > =1",
    }},
    {"passAtleast1Jet", {
        "nJet > =1",
    }},
    {"passJetVetoMap", {
        "If any jet falls in the veto region",
    }},
    {"passDPhiRefJet1", {
        "|DeltaPhi(Photon, leading jet) - pi| < 0.44",
    }},
    {"passRefBarrel", {
        "|eta| of Photon < 1.3 ",
    }},
    {"passJet1EtaJet2Pt", {
        "leading jet eta < 1.3 ", "sub-leading jet pT < 30 or < pT of Photon "
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
  channelSlide.addTextSlide(respAndFrac, "Response and energy fractions in Photon pT bins"); 
} 


