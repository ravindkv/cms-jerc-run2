#ifndef LoadEras1D_H
#define LoadEras1D_H

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
#include "TDRStyle.h"

template <typename T>
class LoadEras1D {
public:
    LoadEras1D();
    ~LoadEras1D();
    
    // Setters remain the same...
    void setInputJson(const nlohmann::json &inputJson);
    void setChannel(const std::string & channel);
    void setYear(const std::string & year);
    void setMcHtBins(const std::vector<std::string>& htBins);
    void setDataEras(const std::vector<std::string>& dataEras);
    void setHistDir(const std::string &histDir);
    void setHistName(const std::string &histName);
    void setFigConfig(const FigConfig &params);

    // Loaders (use templates for histogram type)
    void loadDataHists();
    void loadMcHists();
    void drawHists(const std::vector<T*>& hists);

    void overlayDataWithMcInRatio(TFile* outRootFile, const std::string& outPdfName);
    void calculateHistRatio(T* dataHist, T* mcHist, TGraphErrors* ratioGraph);

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

    T* combineHists(const std::vector<T*>& hists);

    std::shared_ptr<TDRStyle> tdrStyle_;
};
#include "../cpp/LoadEras1D.tpp"
#endif

