#pragma once

#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "Slide.h"
#include "Note.h"
#include "GlobalFlag.h"
#include "ReadConfig.h"

class RunChannel : public GlobalFlag {
 public :
    RunChannel(std::string oName): GlobalFlag(oName){}
    virtual ~RunChannel() = default;
    
    int Run(nlohmann::json inputJson, std::string eosPlotDir, Slide& channelSlide, Note& channelNote , ReadConfig &readConfig);

 private :
    void addChannelSlides(Slide &channelSlide);

};

