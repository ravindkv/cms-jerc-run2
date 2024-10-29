#pragma once

#ifndef PlotEra2D_H
#define PlotEra2D_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <TH1F.h>
#include <TProfile.h>
#include <TH2D.h>
#include <TProfile2D.h>
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
class PlotEra2D {
public:
    PlotEra2D();
    ~PlotEra2D();
    
    // Setters remain the same...
    void setInput(const nlohmann::json &inputJson, const std::string & channel, const std::string & year);
    void setDataEraOrMcBin(const std::string & dataEraOrMcBin);
    void setFigConfigEra2D(const FigConfigEra2D &params);
    void drawHist2D(const std::string& outPdfName);

private:
    nlohmann::json inputJson_;
    std::string channel_;
    std::string year_;
    std::string dataEraOrMcBin_;
    std::string histDir_;
    std::string histName_;
    std::shared_ptr<TdrStyle> tdrStyle_;
};
#include "../cpp/PlotEra2D.tpp"
#endif

