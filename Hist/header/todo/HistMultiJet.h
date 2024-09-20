#ifndef HistMultiJet_h
#define HistMultiJet_h
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

//----------------------------------------
// HistMultiJet class that runs these
//----------------------------------------
class HistMultiJet: public GlobalFlag{
public :
  HistMultiJet(TString oName): GlobalFlag(oName){}
  // UTILITIES
  struct range
  {
    double ptmin;
    double ptmax;
    double absetamin;
    double absetamax;
  };
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;
  int Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout);
  
  ~HistMultiJet();
private :
  bool smearJets = false;

};

#endif

