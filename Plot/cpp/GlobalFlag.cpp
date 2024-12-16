#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(std::string oName){
  outName = oName;
  isDebug = false;
  isPlotEras1D = true;
  isPlotEra2D = false;
  isPlotErasXY = true;
  isPlotEraXY =  false;

  isPrintFigConfig = false;

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
  if(isPlotEras1D )cout<<"isPlotEras1D   = true"<<endl;
  if(isPlotEra2D  )cout<<"isPlotEra2D   = true"<<endl;
  if(isPlotErasXY )cout<<"isPlotErasXY   = true"<<endl;
  if(isPlotEraXY  )cout<<"isPlotEraXY   = true"<<endl;

  if(isPrintFigConfig  )cout<<"isPrintFigConfig   = true"<<endl;

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
