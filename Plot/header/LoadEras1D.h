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
#include <TDRStyle.h>
#include <TPad.h>
#include <TROOT.h>
#include <THStack.h>
#include <TProfile.h>
#include <nlohmann/json.hpp>


template <typename T>
class LoadEras1D {
public:
    LoadEras1D();
    ~LoadEras1D();
    
    // Setters remain the same...
    void setInputJson(const nlohmann::json &inputJson);
    void setChannel(const std::string & channel);
    void setYear(const std::string & year);
    void setMCHTBins(const std::vector<std::string>& htBins);
    void setDataEras(const std::vector<std::string>& dataEras);
    void setHistDir(const std::string &histDir);
    void setHistName(const std::string &histName);

    // Loaders (use templates for histogram type)
    void loadDataHists(const bool & normTo1);
    void loadMCHists(const bool & normTo1);
    void drawHists(TDRStyle &tdrS, const std::vector<T*>& hists);

    void overlayDataWithMCInRatio(TFile* outRootFile, const std::string& outputFileName);
    void calculateHistRatio(T* dataHist, T* mcHist, TGraphErrors* ratioGraph);

private:
    nlohmann::json inputJson_;
    std::string channel_;
    std::string year_;
    std::vector<std::string> dataEras_;
    std::vector<std::string> mcHTBins_;
    std::string histDir_;
    std::string histName_;
    
    std::vector<T*> dataHists_;  
    std::vector<T*> mcHists_;  

    T* combineHists(const std::vector<T*>& hists);
};
#include "../tpp/LoadEras1D.tpp"
#endif

