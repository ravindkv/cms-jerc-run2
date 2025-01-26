#pragma once

#ifndef PlotEraXYs_H
#define PlotEraXYs_H

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
#include <THStack.h>
#include <TProfile.h>
#include <nlohmann/json.hpp>

#include "FigConfig.h"
#include "TdrStyle.h"

template <typename T>
class PlotEraXYs {
public:
    PlotEraXYs();
    ~PlotEraXYs();
    
    // Setters remain the same...
    void setInput(const nlohmann::json &inputJson, const std::string & channel, const std::string & year);
    void setFigConfigEraXYs(const FigConfigEraXYs &params);

    // Loaders (use templates for histogram type)
    void loadHists(const std::string& sourceType, const std::string& dataEra, const std::vector<std::string>& mcHtBins);
    void drawHists(const std::vector<TProfile*>& hists);

    void overlayDataWithMcInRatio(const std::string& outPdfName);

private:
    nlohmann::json inputJson_;
    std::string channel_;
    std::string year_;
    std::vector<std::string> mcHtBins_;
    std::string dataEra_;
    std::vector<double> varBins_;
    std::string varName_;
    bool varIsOnXaxis_;
    std::string histDir_;
    std::vector<std::string> trigDirs_;
    std::string histName_;
    
    std::vector<TProfile*> dataHists_;  
    std::vector<TProfile*> mcHists_;  

    std::shared_ptr<TdrStyle> tdrStyle_;
    TProfile* projectAndClone(T* hist, const std::string& bin, int i);
};
#include "../tpp/PlotEraXYs.tpp"
#endif

