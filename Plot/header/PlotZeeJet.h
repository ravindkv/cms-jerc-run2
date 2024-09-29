#ifndef PlotZeeJet_h
#define PlotZeeJet_h
#include <nlohmann/json.hpp>
#include <TFile.h>

class PlotZeeJet {
 public :
    PlotZeeJet(){}
    int Run(nlohmann::json inputJson, TFile *outRoot, std::string & outLatex);
    ~PlotZeeJet();

 private :
    bool smearJets = false;

};

#endif

