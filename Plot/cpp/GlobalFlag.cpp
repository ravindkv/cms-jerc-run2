#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(std::string oName){
  outName = oName;
  isDebug = false;
  justLatex = false;
  isPlotTProfile = true;
  isPlotTH1D = true;
  isPlotTProfile2D = true;
  isPlotTH2D = true;
  isPlotProjectXorY = false;
  isPlotFlavXTaggedY = false;

  if(outName.find("GamJet")!= string::npos)    isGamJet    = true;
  if(outName.find("ZeeJet")!= string::npos)    isZeeJet   = true;
  if(outName.find("ZmmJet")!= string::npos)    isZmmJet    = true;
  if(outName.find("MCTruth")!= string::npos)   isMCTruth   = true;
  if(outName.find("Flavour")!= string::npos)   isFlavour   = true;
  if(outName.find("VetoMap")!= string::npos)   isVetoMap   = true;
  if(outName.find("DiJet")!= string::npos)     isDiJet     = true;
  if(outName.find("IncJet")!= string::npos)    isIncJet    = true;
  if(outName.find("MultiJet")!= string::npos)  isMultiJet  = true;
  if(outName.find("Wqq")!= string::npos)       isWqq       = true;
}

void GlobalFlag::printFlag(){
  if(isDebug   )cout<<"isDebug   = true"<<endl;
  if(isPlotTH1D   )cout<<"isPlotTH1D   = true"<<endl;
  if(isPlotTProfile   )cout<<"isPlotTProfile   = true"<<endl;
  if(isPlotTH2D   )cout<<"isPlotTH2D   = true"<<endl;
  if(isPlotTProfile2D   )cout<<"isPlotTProfile2D   = true"<<endl;
  if(isPlotFlavXTaggedY   )cout<<"isPlotFlavXTaggedY   = true"<<endl;
  if(isPlotProjectXorY   )cout<<"isPlotProjectXorY   = true"<<endl;

  if(isGamJet  )cout<<"isGamJet  = true"<<endl;
  if(isZeeJet  )cout<<"isZeeJet  = true"<<endl;
  if(isZmmJet  )cout<<"isZmmJet  = true"<<endl;
  if(isMCTruth )cout<<"isMCTruth = true"<<endl;
  if(isFlavour)cout<<"isFlavour= true"<<endl;
  if(isIncJet  )cout<<"isIncJet  = true"<<endl;
  if(isMultiJet)cout<<"isMultiJet= true"<<endl;
  if(isWqq     )cout<<"isWqq     = true"<<endl;
}

GlobalFlag::~GlobalFlag(){
}
