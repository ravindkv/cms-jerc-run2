#ifndef HistMCTruth_h
#define HistMCTruth_h

#include "GlobalFlag.h"  // Necessary header for inheritance

// Forward declarations. Full inclusion in the .cpp code
class SkimTree;
class EventPick;
class ObjectPick;
class ObjectScale;
class TFile;

class HistMCTruth: public GlobalFlag{
public :
  explicit HistMCTruth(TString oName): GlobalFlag(oName){}
  int Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout);
  
  ~HistMCTruth() = default;
};

#endif

