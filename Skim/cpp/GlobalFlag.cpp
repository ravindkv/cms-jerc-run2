#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(const string& outName) : 
    isDebug(false), 
    is2016Pre(false), 
    is2016Post(false), 
    is2017(false), 
    is2018(false), 
    isData(false), 
    isMC(false), 
    isZeeJet(false), 
    isZmmJet(false), 
    isGamJet(false), 
    isGamJetFake(false), 
    isMultiJet(false), 
    isWqq(false), 
    isQCD(false), 
    isMG(false){

    if (outName.find("2016Pre")!= string::npos) is2016Pre   = true;
    if (outName.find("2016Post")!= string::npos) is2016Post = true;
    if (outName.find("2017")!= string::npos) is2017 = true;
    if (outName.find("2018")!= string::npos) is2018 = true;

    if (outName.find("Data")!= string::npos) isData = true;
    if (outName.find("MC")!= string::npos)   isMC = true;

    if(outName.find("ZeeJet")!= string::npos)    isZeeJet   = true;
    if(outName.find("ZmmJet")!= string::npos)    isZmmJet    = true;
    if(outName.find("GamJetFake")!= string::npos)    isGamJetFake    = true;
    else if(outName.find("GamJet")!= string::npos)    isGamJet    = true;
    if(outName.find("MultiJet")!= string::npos)  isMultiJet  = true;
    if(outName.find("Wqq")!= string::npos)       isWqq       = true;

    if(outName.find("QCD")!= string::npos)    isQCD    = true;
    if(outName.find("MC")!= string::npos)     isMG     = true;
}

void GlobalFlag::printFlag(){
    if(isDebug   )cout<<"isDebug   = true"<<'\n';
    if(is2016Pre   )cout<<"is2016Pre   = true"<<'\n';
    if(is2016Post   )cout<<"is2016Post   = true"<<'\n';
    if(is2017   )cout<<"is2017   = true"<<'\n';
    if(is2018   )cout<<"is2018   = true"<<'\n';
    if(isData    )cout<<"isData    = true"<<'\n';
    if(isMC      )cout<<"isMC      = true"<<'\n';
    if(isZeeJet  )cout<<"isZeeJet  = true"<<'\n';
    if(isZmmJet  )cout<<"isZmmJet  = true"<<'\n';
    if(isGamJet  )cout<<"isGamJet  = true"<<'\n';
    if(isGamJetFake  )cout<<"isGamJetFake  = true"<<'\n';
    if(isMultiJet)cout<<"isMultiJet= true"<<'\n';
    if(isWqq     )cout<<"isWqq     = true"<<'\n';

    if(isQCD     )cout<<"isQCD     = true"<<'\n';
    if(isMG      )cout<<"isMG      = true"<<'\n';
}


