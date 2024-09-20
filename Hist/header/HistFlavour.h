#ifndef HistFlavour_h
#define HistFlavour_h

#include "GlobalFlag.h"  // Necessary header for inheritance

// Forward declarations. Full inclusion in the .cpp code
class SkimTree;
class EventPick;
class ObjectPick;
class ObjectScale;
class TFile;

class HistFlavour: public GlobalFlag{
public :
  explicit HistFlavour(TString oName): GlobalFlag(oName){}
  int Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout);
  
  ~HistFlavour() = default;
};

#endif

