#include "RunChannel.h"
#include "FigConfig.h"
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
   
int RunChannel::Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide, ReadConfig &readConfig){
    channelSlide.startDocument("JME-21-001: L3Residual from Channel "+ channelStr);
    addChannelSlides(channelSlide);

    allChannelSlide.addCenteredTextSlide("Next we show results from "+channelStr+" channel");

    std::string channel = channelStr;
    std::vector<std::string> years;
    for (const auto& element : inputJson[channel].items()) {
        years.push_back(element.key());
    }

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
            allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from Channel channel for different Runs");
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
                allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from Channel channel for different years");
            }//year
        }//config
    }//isPlotEra1Ds


	//-------------------------------------
	// Plot1D: multiple hists from one year
	//-------------------------------------
    if(isPlotYear1Ds){
        auto outPlotDirYear1Ds = eosPlotDir+"/"+channel+"/"+"Year1Ds"; 
        std::filesystem::create_directories(outPlotDirYear1Ds);
        channelSlide.addCenteredTextSlide("Next we show 1D plots");
        for (const auto & config: figConfigVecYear1Ds){
            auto names = config.histNames;
            std::string name("");
            for (const auto& n: names){name = n + "_";}
            auto dir  = config.histDir;
            if(isPrintFigConfig) config.print();
            std::vector<std::string> plotsForSlide;
            for (auto &year: years){
                auto dataEras = Helper::getEras(inputJson, channel, year, "Data");
                auto mcBins = Helper::getEras(inputJson, channel, year, "MC");
                auto year1Ds = std::make_unique<PlotYear1Ds<TProfile>>();
                year1Ds->setInput(inputJson, channel, year);
                year1Ds->setFigConfigYear1Ds(config);
                year1Ds->loadHists("Data", dataEras);
                year1Ds->loadHists("MC", mcBins);
                year1Ds->setOverlayMC(true);
                string outName = Helper::dirToName(dir);
                auto outPdfName = outPlotDirYear1Ds+"/"+year+"_"+outName+"_"+name+".pdf";
                year1Ds->overlayDataWithMcInRatio(outPdfName);
                plotsForSlide.push_back(outPdfName);
            }//year
            channelSlide.addPlotSlide(plotsForSlide, dir+"/Energy Fractions for different year");
            allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from Channel channel for different years");
        }//config
    }//isPlotYear1Ds

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
            allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from Channel channel for different years");
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
                allChannelSlide.addPlotSlide(plotsForSlide, dir+"/"+name+" from Channel channel for "+ year);
            }//year
        }//config
    }//isPlotXY

    // After all plots are generated
    channelSlide.addCenteredTextSlide("Thank you!");
    channelSlide.endDocument();
    return 0;
}


void RunChannel::addChannelSlides(Slide & channelSlide){

  //channelSlide.makeSlideFromJsonFile("../Hist/config/PickObjectZeeJet.json");
  //channelSlide.makeSlideFromJsonFile("../Hist/config/VarBin.json");
  channelSlide.makeSlideFromJsonFile("../Skim/input/SamplesNano_ZeeJet.json");
} 


