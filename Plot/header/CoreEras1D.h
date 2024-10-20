#pragma once

#ifndef CoreEras1D_H
#define CoreEras1D_H

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
class CoreEras1D {
public:
    CoreEras1D();
    ~CoreEras1D();
    
    // Setters remain the same...
    void setInputJson(const nlohmann::json &inputJson);
    void setChannel(const std::string & channel);
    void setYear(const std::string & year);
    void setMcHtBins(const std::vector<std::string>& htBins);
    void setDataEras(const std::vector<std::string>& dataEras);

    void setFigConfigEras1D(const FigConfigEras1D &params);

    // Loaders (use templates for histogram type)
    void loadDataHists();
    void loadMcHists();
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
#include "../cpp/CoreEras1D.tpp"
#endif

