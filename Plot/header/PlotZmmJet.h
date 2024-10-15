#ifndef PlotZmmJet_h
#define PlotZmmJet_h
#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "MakeDPNote.h"
#include "GlobalFlag.h"

#include "LoadEras1D.h"
#include "LoadEra2D.h"
#include "LoadEraXY.h"
#include "LoadErasXY.h"

class PlotZmmJet : public GlobalFlag {
 public :
    PlotZmmJet(std::string oName): GlobalFlag(oName){}
    ~PlotZmmJet();
    
    int Run(nlohmann::json inputJson, TFile *outRoot, std::string & outLatex);

 private :
    void addZmmJetSlides(MakeDPNote &dpNote);

};
#endif

