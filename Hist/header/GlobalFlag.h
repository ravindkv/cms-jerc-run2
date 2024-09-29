#ifndef GlobalFlag_H
#define GlobalFlag_H
        
#include <iostream>
#include <TString.h>

using namespace std;

class GlobalFlag{
  public: 
    GlobalFlag(TString oName);
    ~GlobalFlag();
    
    bool isDebug;
    
    //Year 
    bool is2016Pre;
    bool is2016PreBCD;
    bool is2016PreEF;

    bool is2016Post;
    bool is2016PostFGH;

    bool is2017;
    bool is2017B;
    bool is2017C;
    bool is2017D;
    bool is2017E;
    bool is2017F;

    bool is2018;
    bool is2018A;
    bool is2018B;
    bool is2018C;
    bool is2018D;
    
    //Data or MC
    bool isData;
    bool isMC;
    
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
    
    //Sample (e.g. QCD)
    bool isQCD;
    bool isMG;
    void printFlag();
  private:
    string outName;
};
#endif
