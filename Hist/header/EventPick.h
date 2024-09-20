#ifndef EVENTPICK_H
#define EVENTPICK_H

#include<vector>
#include<string>

#include <TFile.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <iostream>
#include <iomanip>

#include"SkimTree.h"
#include"GlobalFlag.h"

class EventPick: public GlobalFlag{
public:
	EventPick(TString oName): GlobalFlag(oName){};

    bool passFilter(SkimTree *tree);
    bool passHLT(SkimTree *tree);
    vector<string> getTrigNames();
    map<string, const Bool_t *> getTrigValues();

    void printBins(TH1D *hist);
    void printInfo(TObject *obj);
    void scanDirectory(TDirectory *dir, const std::string &path);
    void scanTFile(TFile *file);
    
	~EventPick();
	
};
#endif
