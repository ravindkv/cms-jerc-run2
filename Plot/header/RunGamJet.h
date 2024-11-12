#pragma once

#ifndef RunGamJet_h
#define RunGamJet_h
#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "Slide.h"
#include "GlobalFlag.h"

class RunGamJet : public GlobalFlag {
 public :
    RunGamJet(std::string oName): GlobalFlag(oName){}
    ~RunGamJet();
    
    int Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide);

 private :
    void addGamJetSlides(Slide &channelSlide);

};
#endif

