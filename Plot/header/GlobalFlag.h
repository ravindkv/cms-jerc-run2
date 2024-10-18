#ifndef GlobalFlag_H
#define GlobalFlag_H
        
#include <iostream>
#include <TString.h>

using namespace std;

class GlobalFlag{
  public: 
    GlobalFlag(std::string oName);
    ~GlobalFlag();
    
    bool isDebug;

    bool isPlot1D;
    bool isPlot2D;
    bool isPlotXY;

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
    bool isWqq;
    
    void printFlag();
  private:
    string outName;
};
#endif
