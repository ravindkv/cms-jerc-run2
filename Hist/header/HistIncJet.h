#ifndef HistIncJet_h
#define HistIncJet_h

#include "GlobalFlag.h"  // Necessary header for inheritance

// Forward declarations. Full inclusion in the .cpp code
class SkimTree;
class EventPick;
class ObjectPick;
class ObjectScale;
class TFile;

class HistIncJet: public GlobalFlag{
public :
  explicit HistIncJet(TString oName): GlobalFlag(oName){}
  // UTILITIES
  struct TrigRange
  { 
    double ptThresh;
    double ptMin;
    double ptMax;
    double absEtaMin;
    double absEtaMax;
  };
  int Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout);
  
  ~HistIncJet() = default;
};

#endif
