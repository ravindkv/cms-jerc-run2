#pragma once

#ifndef RunZeeJet_h
#define RunZeeJet_h
#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "Slide.h"
#include "GlobalFlag.h"

class RunZeeJet : public GlobalFlag {
 public :
    RunZeeJet(std::string oName): GlobalFlag(oName){}
    ~RunZeeJet();
    
    int Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide);

 private :
    void addZeeJetSlides(Slide &channelSlide);

};
#endif

