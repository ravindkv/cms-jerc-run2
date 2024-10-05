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

    bool isPlotTH1D;
    bool isPlotTProfile;
    bool isPlotTH2D;
    bool isPlotTProfile2D;
    bool isPlotFlavXTaggedY;
    bool isPlotProjectXorY;
    
    void printFlag();
  private:
    string outName_;
};
#endif
