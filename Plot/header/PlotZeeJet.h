#ifndef PlotZeeJet_h
#define PlotZeeJet_h
#include <nlohmann/json.hpp>
#include <TFile.h>
#include <iostream>
#include "MakeDPNote.h"
#include "GlobalFlag.h"

class PlotZeeJet : public GlobalFlag {
 public :
    PlotZeeJet(std::string oName): GlobalFlag(oName){}
    ~PlotZeeJet();
    
    int Run(nlohmann::json inputJson, TFile *outRoot, MakeDPNote &chNote, MakeDPNote &allNote);

 private :
    void addZeeJetSlides(MakeDPNote &chNote);

};
#endif

