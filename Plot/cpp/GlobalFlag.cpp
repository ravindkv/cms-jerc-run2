#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(std::string oName){
  outName = oName;
  isDebug = false;
  isPlot1D = false;
  isPlot2D = false;
  isPlotXY = true;

  if(outName.find("GamJet")!= string::npos)    isGamJet    = true;
  if(outName.find("ZeeJet")!= string::npos)    isZeeJet   = true;
  if(outName.find("ZmmJet")!= string::npos)    isZmmJet    = true;
  if(outName.find("MCTruth")!= string::npos)   isMcTruth   = true;
  if(outName.find("Flavour")!= string::npos)   isFlavour   = true;
  if(outName.find("VetoMap")!= string::npos)   isVetoMap   = true;
  if(outName.find("DiJet")!= string::npos)     isDiJet     = true;
  if(outName.find("IncJet")!= string::npos)    isIncJet    = true;
  if(outName.find("MultiJet")!= string::npos)  isMultiJet  = true;
  if(outName.find("Wqq")!= string::npos)       isWqq       = true;
}

void GlobalFlag::printFlag(){
  if(isDebug   )cout<<"isDebug    = true"<<endl;
  if(isPlot1D  )cout<<"isPlot1D   = true"<<endl;
  if(isPlot2D  )cout<<"isPlot2D   = true"<<endl;
  if(isPlotXY  )cout<<"isPlotXY   = true"<<endl;

  if(isGamJet  )cout<<"isGamJet  = true"<<endl;
  if(isZeeJet  )cout<<"isZeeJet  = true"<<endl;
  if(isZmmJet  )cout<<"isZmmJet  = true"<<endl;
  if(isMcTruth )cout<<"isMcTruth = true"<<endl;
  if(isFlavour)cout<<"isFlavour= true"<<endl;
  if(isIncJet  )cout<<"isIncJet  = true"<<endl;
  if(isMultiJet)cout<<"isMultiJet= true"<<endl;
  if(isWqq     )cout<<"isWqq     = true"<<endl;
}

GlobalFlag::~GlobalFlag(){
}
