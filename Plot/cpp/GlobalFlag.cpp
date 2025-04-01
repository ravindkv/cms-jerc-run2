#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(std::string oName){
  outName = oName;
  isDebug = false;
  isPlotTime1Ds = false;
  isPlotEras1D  = false;
  isPlotEra1Ds  = false;
  isPlotYear1Ds = true;
  isPlotEra2D   = false;
  isPlotErasXY  = false;
  isPlotEraXYs  = false;

  isPrintFigConfig = false;

  // --- Channels ---
  const std::string channelZeeJet      = "ZeeJet";
  const std::string channelZmmJet      = "ZmmJet";
  const std::string channelGamJetFake  = "GamJetFake";
  const std::string channelGamJet      = "GamJet";
  const std::string channelMultiJet    = "MultiJet";
  const std::string channelDiJet       = "DiJet";
  const std::string channelWqqe        = "Wqqe";
  const std::string channelWqqm        = "Wqqm";
  
  int channelCount = 0;
  if (outName.find(channelZeeJet) != std::string::npos) {
      isZeeJet = true;
      channelStr = channelZeeJet;
      ++channelCount;
  }
  if (outName.find(channelZmmJet) != std::string::npos) {
      isZmmJet = true;
      channelStr = channelZmmJet;
      ++channelCount;
  }
  if (outName.find(channelGamJetFake) != std::string::npos) {
      isGamJetFake = true;
      channelStr = channelGamJetFake;
      ++channelCount;
  }
  // Note: using an independent if here because we want to allow only one of these two.
  if (outName.find(channelGamJet) != std::string::npos) {
      // If GamJetFake was found, this one should not be set.
      if (channelCount > 0) {
          throw std::runtime_error("Error: Multiple channel flags found in " + outName);
      }
      isGamJet = true;
      channelStr = channelGamJet;
      ++channelCount;
  }
  if (outName.find(channelMultiJet) != std::string::npos) {
      isMultiJet = true;
      channelStr = channelMultiJet;
      ++channelCount;
  }
  if (outName.find(channelDiJet) != std::string::npos) {
      isDiJet = true;
      channelStr = channelDiJet;
      ++channelCount;
  }
  if (outName.find(channelWqqe) != std::string::npos) {
      isWqqe = true;
      channelStr = channelWqqe;
      ++channelCount;
  }
  if (outName.find(channelWqqm) != std::string::npos) {
      isWqqm = true;
      channelStr = channelWqqm;
      ++channelCount;
  }
  if (channelCount != 1) {
      throw std::runtime_error("Error: Exactly one channel flag must be set in " + outName);
  }
}

void GlobalFlag::printFlag(){
  if(isDebug   )cout<<"isDebug    = true"<<endl;
  if(isPlotEras1D )cout<<"isPlotEras1D   = true"<<endl;
  if(isPlotEra1Ds )cout<<"isPlotEra1Ds   = true"<<endl;
  if(isPlotYear1Ds )cout<<"isPlotYear1Ds   = true"<<endl;
  if(isPlotEra2D  )cout<<"isPlotEra2D   = true"<<endl;
  if(isPlotErasXY )cout<<"isPlotErasXY   = true"<<endl;
  if(isPlotEraXYs  )cout<<"isPlotEraXYs   = true"<<endl;
  if(isPlotTime1Ds )cout<<"isPlotTime1Ds   = true"<<endl;

  if(isPrintFigConfig  )cout<<"isPrintFigConfig   = true"<<endl;

  if(isGamJet  )cout<<"isGamJet  = true"<<endl;
  if(isZeeJet  )cout<<"isZeeJet  = true"<<endl;
  if(isZmmJet  )cout<<"isZmmJet  = true"<<endl;
  if(isWqqe  )cout<<"isWqqe  = true"<<endl;
  if(isWqqm  )cout<<"isWqqm  = true"<<endl;
  if(isMcTruth )cout<<"isMcTruth = true"<<endl;
  if(isFlavour)cout<<"isFlavour= true"<<endl;
  if(isIncJet  )cout<<"isIncJet  = true"<<endl;
  if(isMultiJet)cout<<"isMultiJet= true"<<endl;
}

GlobalFlag::~GlobalFlag(){
}
