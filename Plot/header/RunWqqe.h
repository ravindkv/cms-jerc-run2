#pragma once

#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "Slide.h"
#include "GlobalFlag.h"

class RunWqqe : public GlobalFlag {
 public :
    RunWqqe(std::string oName): GlobalFlag(oName){}
    ~RunWqqe();
    
    int Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide);

 private :
    void addWqqeSlides(Slide &channelSlide);

};

