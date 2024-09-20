#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(string outName){
    isDebug = false;

    //Years
    is2016Pre  = false;
    is2016Post = false;
    is2017 = false;
    is2018 = false;
    
    //Data or MC
    isData  = false;
    isMC    = false;
    
    //Channels
    isGamJet    = false;
    isZeeJet    = false;
    isZmmJet    = false;
    isMCTruth   = false;
    isFlavour   = false;
    isVetoMap   = false;
    isDiJet     = false;
    isIncJet    = false;
    isMultiJet  = false;
    isWqq       = false;
    
    //Sample (e.g. QCD)
    isQCD = false;
    isMG  = false;

    if (outName.find("2016Pre")!= string::npos) is2016Pre   = true;
    if (outName.find("2016Post")!= string::npos) is2016Post = true;
    if (outName.find("2017")!= string::npos) is2017 = true;
    if (outName.find("2018")!= string::npos) is2018 = true;

    if (outName.find("Data")!= string::npos) isData = true;
    if (outName.find("MC")!= string::npos)   isMC = true;

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

    if(outName.find("QCD")!= string::npos)    isQCD    = true;
    if(outName.find("MC")!= string::npos)     isMG     = true;
}

void GlobalFlag::printFlag(){
    if(isDebug   )cout<<"isDebug   = true"<<endl;
    if(is2016Pre   )cout<<"is2016Pre   = true"<<endl;
    if(is2016Post   )cout<<"is2016Post   = true"<<endl;
    if(is2017   )cout<<"is2017   = true"<<endl;
    if(is2018   )cout<<"is2018   = true"<<endl;
    if(isData    )cout<<"isData    = true"<<endl;
    if(isMC      )cout<<"isMC      = true"<<endl;
    if(isGamJet  )cout<<"isGamJet  = true"<<endl;
    if(isZeeJet  )cout<<"isZeeJet  = true"<<endl;
    if(isZmmJet  )cout<<"isZmmJet  = true"<<endl;
    if(isMCTruth )cout<<"isMCTruth = true"<<endl;
    if(isFlavour)cout<<"isFlavour= true"<<endl;
    if(isIncJet  )cout<<"isIncJet  = true"<<endl;
    if(isMultiJet)cout<<"isMultiJet= true"<<endl;
    if(isWqq     )cout<<"isWqq     = true"<<endl;

    if(isQCD     )cout<<"isQCD     = true"<<endl;
    if(isMG      )cout<<"isMG      = true"<<endl;
}

GlobalFlag::~GlobalFlag(){
}
