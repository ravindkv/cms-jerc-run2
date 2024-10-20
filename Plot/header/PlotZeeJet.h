#pragma once

#ifndef PlotZeeJet_h
#define PlotZeeJet_h
#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "Slide.h"
#include "GlobalFlag.h"

class PlotZeeJet : public GlobalFlag {
 public :
    PlotZeeJet(std::string oName): GlobalFlag(oName){}
    ~PlotZeeJet();
    
    int Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide);

 private :
    void addZeeJetSlides(Slide &channelSlide);

};
#endif

