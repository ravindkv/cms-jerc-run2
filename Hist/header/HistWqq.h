#ifndef HistWqq_h
#define HistWqq_h
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "TH1D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cmath>

#include "TRandom3.h"
#include "SkimTree.h"
#include "EventPick.h"
#include "ObjectPick.h"
#include "ObjectScale.h"
#include "GlobalFlag.h"
#include "correction.h"

class HistWqq: public GlobalFlag{
 public :
    HistWqq(TString oName): GlobalFlag(oName){}
    int Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout);
    ~HistWqq();

 private :
    ObjectPick* objectPick;   
    bool smearJets = false;

};

#endif

