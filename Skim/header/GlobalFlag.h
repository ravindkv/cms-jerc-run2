#ifndef GlobalFlag_H
#define GlobalFlag_H
        
#include <iostream>
using namespace std;

class GlobalFlag{
    public: 
        GlobalFlag(const string& outName);
        ~GlobalFlag() = default;
        
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
        bool isZeeJet;
        bool isZmmJet;
        bool isGamJet;
        bool isGamJetFake;
        bool isMultiJet;
        bool isWqq;

        //Sample (e.g. QCD)
        bool isQCD;
        bool isMG;
        void printFlag();
};
#endif
