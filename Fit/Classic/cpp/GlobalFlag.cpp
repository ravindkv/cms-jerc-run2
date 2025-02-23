#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(std::string oName){
  outName = oName;
  isDebug = true;
  isPrintConfig = true;

  if(outName.find("L3Res")!= string::npos)    isL3Res    = true;
}

void GlobalFlag::printFlag(){
  cout<<"outName = "<<outName<<endl;
  if(isDebug   )cout<<"isDebug    = true"<<endl;

  if(isPrintConfig  )cout<<"isPrintConfig   = true"<<endl;

  if(isL3Res  )cout<<"isL3Res  = true"<<endl;
}

GlobalFlag::~GlobalFlag(){
}
