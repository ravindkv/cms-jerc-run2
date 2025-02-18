#pragma once

#ifndef GlobalFlag_H
#define GlobalFlag_H
        
#include <iostream>

using namespace std;

class GlobalFlag{
  public: 
    GlobalFlag(std::string oName);
    ~GlobalFlag();
    
    bool isDebug;

    bool isPlotEras1D;
    bool isPlotEra1Ds;
    bool isPlotYear1Ds;
    bool isPlotEra2D;
    bool isPlotErasXY;
    bool isPlotEraXYs;
    bool isPlotTime1Ds;

    bool isPrintFigConfig;

    //Channels
    bool isGamJet;
    bool isZeeJet;
    bool isZmmJet;
    bool isMcTruth;
    bool isFlavour;
    bool isVetoMap;
    bool isDiJet;
    bool isIncJet;
    bool isMultiJet;
    bool isWqqe;
    bool isWqqm;
    
    void printFlag();
  private:
    string outName;
};
#endif
