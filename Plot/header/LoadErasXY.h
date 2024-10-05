#ifndef LoadErasXY_H
#define LoadErasXY_H

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
#include <TDRStyle.h>
#include <TPad.h>
#include <TROOT.h>
#include <THStack.h>
#include <TProfile.h>
#include <nlohmann/json.hpp>


template <typename T>
class LoadErasXY {
public:
    LoadErasXY();
    ~LoadErasXY();
    
    // Setters remain the same...
    void setInputJson(const nlohmann::json &inputJson);
    void setChannel(const std::string & channel);
    void setYear(const std::string & year);
    void setMCHTBins(const std::vector<std::string>& htBins);
    void setDataEras(const std::vector<std::string>& dataEras);
    void setVarMin(const double & varMin);
    void setVarMax(const double & varMax);
    void setVarName(const std::string &varName);
    void setVarIsOnXaxis(const bool &varIsOnXaxis);
    void setHistDir(const std::string &histDir);
    void setHistName(const std::string &histName);

    // Loaders (use templates for histogram type)
    void loadDataHists();
    void loadMCHists();
    void drawHists(TDRStyle &tdrS, const std::vector<TH1D*>& hists);

    void overlayDataWithMCInRatio(TFile* outRootFile, const std::string& outputFileName);
    void calculateHistRatio(TH1D* dataHist, TH1D* mcHist, TGraphErrors* ratioGraph);

private:
    nlohmann::json inputJson_;
    std::string channel_;
    std::string year_;
    std::vector<std::string> mcHTBins_;
    std::vector<std::string> dataEras_;
    double varMin_;
    double varMax_;
    std::string varName_;
    bool varIsOnXaxis_;
    std::string histDir_;
    std::string histName_;
    
    std::vector<TH1D*> dataHists_;  
    std::vector<TH1D*> mcHists_;  
    TH1D* combineHists(const std::vector<TH1D*>& hists);
};
#include "../tpp/LoadErasXY.tpp"
#endif
