#pragma once

#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "Slide.h"
#include "GlobalFlag.h"

class RunWqqm : public GlobalFlag {
 public :
    RunWqqm(std::string oName): GlobalFlag(oName){}
    ~RunWqqm();
    
    int Run(nlohmann::json inputJson, std::string eosPlotDir, Slide &channelSlide, Slide &allChannelSlide);

 private :
    void addWqqmSlides(Slide &channelSlide);

};

