#pragma once

#ifndef PlotEras1D_H
#define PlotEras1D_H

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
class PlotEras1D {
public:
    PlotEras1D();
    ~PlotEras1D();
    
    // Setters remain the same...
    void setInput(const nlohmann::json &inputJson, const std::string & channel, const std::string & year);
    void setFigConfigEras1D(const FigConfigEras1D &params);

    // Loaders (use templates for histogram type)
    void loadHists(const std::string& sourceType, const std::vector<std::string>& bins);
    void drawHists(const std::vector<T*>& hists);

    void overlayDataWithMcInRatio(const std::string& outPdfName);

private:
    nlohmann::json inputJson_;
    std::string channel_;
    std::string year_;
    std::vector<std::string> dataEras_;
    std::vector<std::string> mcHtBins_;
    std::string histDir_;
    std::string histName_;
    
    std::vector<T*> dataHists_;  
    std::vector<T*> mcHists_;  

    std::shared_ptr<TdrStyle> tdrStyle_;
};
#include "../cpp/PlotEras1D.tpp"
#endif

