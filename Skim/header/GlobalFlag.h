#ifndef GlobalFlag_H
#define GlobalFlag_H
        
#include <iostream>
using namespace std;

class GlobalFlag{
    public: 
        GlobalFlag(string outName);
        ~GlobalFlag();
        
        bool isDebug;

        //Year 
        bool is2016Pre;
        bool is2016Post;
        bool is2017;
        bool is2018;


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
};
#endif
