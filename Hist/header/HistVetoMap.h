#ifndef HistVetoMap_h
#define HistVetoMap_h

#include "GlobalFlag.h"  // Necessary header for inheritance

// Forward declarations. Full inclusion in the .cpp code
class SkimTree;
class EventPick;
class ObjectPick;
class ObjectScale;
class TFile;

class HistVetoMap: public GlobalFlag{
public :
  explicit HistVetoMap(TString oName): GlobalFlag(oName){}
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
  
  ~HistVetoMap() = default;
};

#endif
