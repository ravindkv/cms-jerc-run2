#pragma once

#ifndef PlotTime1Ds_H
#define PlotTime1Ds_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <TH1F.h>
#include <TH1D.h>
#include <TProfile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TPad.h>
#include <TROOT.h>
#include <TProfile.h>
#include <nlohmann/json.hpp>

#include "FigConfig.h"
#include "TdrStyle.h"

template <typename T>
class PlotTime1Ds {
public:
    PlotTime1Ds();
    ~PlotTime1Ds();
    
    // Setters
    void setInput(const nlohmann::json &inputJson, const std::string & channel, const std::string& year);
    void setFigConfigTime1Ds(const FigConfigTime1Ds &params);

    // Loaders
    void loadHists(const std::string& sourceType);
    
    // Drawing
    void drawHists();
    
    // Overlay Data
    void overlayData(const std::string& outPdfName);


    
private:
    nlohmann::json inputJson_;
    std::string channel_;
    std::string year_;
    std::vector<std::string> dataEras_;
    std::string histDir_;
    std::vector<std::string> trigDirs_;
    std::vector<std::string> histNames_;
    double yMin_;
    double yMax_;
    
    // Map: histogram name -> year -> vector of histograms per era
    std::map<std::string, std::map<std::string, std::vector<T*>>> dataHists_;  

    // Template function to generate a sorted vector of EraName and RunNumber
    std::vector<std::pair<std::string, int>> getEraRunMap(const std::vector<T*>& histsToDraw);

    std::shared_ptr<TdrStyle> tdrStyle_;
};

#include "../tpp/PlotTime1Ds.tpp"
#endif

