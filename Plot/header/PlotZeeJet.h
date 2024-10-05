#ifndef PlotZeeJet_h
#define PlotZeeJet_h
#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "MakeDPNote.h"
#include "GlobalFlag.h"

#include "LoadEras1D.h"
#include "LoadEra2D.h"
#include "LoadEraXY.h"
#include "LoadErasXY.h"

class PlotZeeJet : public GlobalFlag {
 public :
    PlotZeeJet(std::string oName): GlobalFlag(oName){}
    ~PlotZeeJet();
    
    int Run(nlohmann::json inputJson, TFile *outRoot, std::string & outLatex);

 private :
    void addZeeJetSlides(MakeDPNote &dpNote);

};
#endif

