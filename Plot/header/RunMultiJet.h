#pragma once

#ifndef RunMultiJet_h
#define RunMultiJet_h
#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "Slide.h"
#include "GlobalFlag.h"

class RunMultiJet : public GlobalFlag {
 public :
    RunMultiJet(std::string oName): GlobalFlag(oName){}
    ~RunMultiJet();
    
    int Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide);

 private :
    void addMultiJetSlides(Slide &channelSlide);

};
#endif

