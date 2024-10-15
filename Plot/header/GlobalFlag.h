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

    bool justLatex;
    bool isPlotTH1D;
    bool isPlotTProfile;
    bool isPlotTH2D;
    bool isPlotTProfile2D;
    bool isPlotFlavXTaggedY;
    bool isPlotProjectXorY;

        //Channels
    bool isGamJet;
    bool isZeeJet;
    bool isZmmJet;
    bool isMCTruth;
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
