#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(std::string oName){
  outName_ = oName;
  isDebug = false;
  isPlotTProfile = false;
  isPlotTH1D = false;
  isPlotTProfile2D = false;
  isPlotTH2D = false;
  isPlotProjectXorY = true;
  isPlotFlavXTaggedY = false;
}

void GlobalFlag::printFlag(){
  if(isDebug   )cout<<"isDebug   = true"<<endl;
  if(isPlotTH1D   )cout<<"isPlotTH1D   = true"<<endl;
  if(isPlotTProfile   )cout<<"isPlotTProfile   = true"<<endl;
  if(isPlotTH2D   )cout<<"isPlotTH2D   = true"<<endl;
  if(isPlotTProfile2D   )cout<<"isPlotTProfile2D   = true"<<endl;
  if(isPlotFlavXTaggedY   )cout<<"isPlotFlavXTaggedY   = true"<<endl;
  if(isPlotProjectXorY   )cout<<"isPlotProjectXorY   = true"<<endl;
}

GlobalFlag::~GlobalFlag(){
}
