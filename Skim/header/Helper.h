
#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "Rtypes.h"
#include "TH1D.h"
#include "TTree.h"
#include "TBranch.h"
#include "TObjArray.h"

class Helper {
public:
    static void initProgress(); 
    static void printBanner(const std::string &title);
    static void printProgress(Long64_t jentry, Long64_t nentries, 
                       std::chrono::time_point<std::chrono::high_resolution_clock>& startClock, 
                       double& totTime);
    
    static void printCutflow(const TH1D* hist);


    //All magic numbers
    static const int tTreeCatchSize = 52428800;//50*1024*1024
    static constexpr unsigned long cutFlowBinMargin = 0.5;
    static const int coutTabWidth5 = 5; 
    static const int coutTabWidth10 = 10; 
    static const int coutTabWidth15 = 15; 
    static const int coutTabWidth20 = 20; 

    static const int hundred = 100; 
    static const int sixty = 60; 
};

