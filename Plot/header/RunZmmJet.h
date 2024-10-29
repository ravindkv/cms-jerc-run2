#pragma once

#ifndef RunZmmJet_h
#define RunZmmJet_h
#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "Slide.h"
#include "GlobalFlag.h"

class RunZmmJet : public GlobalFlag {
 public :
    RunZmmJet(std::string oName): GlobalFlag(oName){}
    ~RunZmmJet();
    
    int Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide);

 private :
    void addZmmJetSlides(Slide &channelSlide);

};
#endif

