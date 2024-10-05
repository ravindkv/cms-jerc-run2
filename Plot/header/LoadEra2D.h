#ifndef LoadEra2D_H
#define LoadEra2D_H

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
#include <TDRStyle.h>
#include <TPad.h>
#include <TROOT.h>
#include <THStack.h>
#include <TProfile.h>
#include <nlohmann/json.hpp>


template <typename T>
class LoadEra2D {
public:
    LoadEra2D();
    ~LoadEra2D();
    
    // Setters remain the same...
    void setInputJson(const nlohmann::json &inputJson);
    void setChannel(const std::string & channel);
    void setYear(const std::string & year);
    void setDataEraOrMCBin(const std::string & dataEraOrMCBin);
    void setHistDir(const std::string &histDir);
    void setHistName(const std::string &histName);
    void drawHist2D(TFile* outRootFile, const std::string& outputFileName);

private:
    nlohmann::json inputJson_;
    std::string channel_;
    std::string year_;
    std::string dataEraOrMCBin_;
    std::string histDir_;
    std::string histName_;
};
#include "../tpp/LoadEra2D.tpp"
#endif

