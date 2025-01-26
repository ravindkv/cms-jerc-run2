// TrigDetail.cpp
#include "TrigDetail.h"
#include <iostream>

// Constructor implementation
TrigDetail::TrigDetail(const GlobalFlag& globalFlags)
    :globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()){

    InitializeList();
}

void TrigDetail::InitializeList(){
    // We know we will insert ~9 or so triggers for GamJet
    // and ~20-30 triggers for MultiJet. Adjust as needed.
    trigMapRangePt_.reserve(15);//approx number
    trigMapRangePtEta_.reserve(30);

    //Year2016Pre
    if (year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post) {
        // ZeeJet channel
        if(channel_ == GlobalFlag::Channel::ZeeJet){
            trigList_ = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ"};
        }        

        // ZmmJet channel
        if(channel_ == GlobalFlag::Channel::ZmmJet){
            trigList_ = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"};
        }        

        // GamJet channel
        if(channel_ == GlobalFlag::Channel::GamJet){
            trigMapRangePt_["HLT_Photon175"]                   = TrigRangePt{230 ,4000};
            trigMapRangePt_["HLT_Photon165_R9Id90_HE10_IsoM"]  = TrigRangePt{175 ,230};
            trigMapRangePt_["HLT_Photon120_R9Id90_HE10_IsoM"]  = TrigRangePt{130 ,175};
            trigMapRangePt_["HLT_Photon90_R9Id90_HE10_IsoM"]   = TrigRangePt{105 ,130};
            trigMapRangePt_["HLT_Photon75_R9Id90_HE10_IsoM"]   = TrigRangePt{85  ,105};
            trigMapRangePt_["HLT_Photon50_R9Id90_HE10_IsoM"]   = TrigRangePt{60  ,85 };
            trigMapRangePt_["HLT_Photon36_R9Id90_HE10_IsoM"]   = TrigRangePt{40  ,60 };
            trigMapRangePt_["HLT_Photon30_R9Id90_HE10_IsoM"]   = TrigRangePt{35  ,40 };
            trigMapRangePt_["HLT_Photon22_R9Id90_HE10_IsoM"]   = TrigRangePt{20  ,35 };
        }
    }
    //Year2017
    else if (year_ == GlobalFlag::Year::Year2017){
        // ZeeJet channel
        if(channel_ == GlobalFlag::Channel::ZeeJet){
            trigList_ = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"};
        }        

        // ZmmJet channel
        if(channel_ == GlobalFlag::Channel::ZmmJet){
            trigList_ = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8"};
        }        

        // GamJet channel
        if(channel_ == GlobalFlag::Channel::GamJet){
            trigMapRangePt_["HLT_Photon200"]                   = TrigRangePt{230 ,4000};
            trigMapRangePt_["HLT_Photon165_R9Id90_HE10_IsoM"]  = TrigRangePt{175 ,230};
            trigMapRangePt_["HLT_Photon120_R9Id90_HE10_IsoM"]  = TrigRangePt{130 ,175};
            trigMapRangePt_["HLT_Photon90_R9Id90_HE10_IsoM"]   = TrigRangePt{105 ,130};
            trigMapRangePt_["HLT_Photon75_R9Id90_HE10_IsoM"]   = TrigRangePt{85  ,105};
            trigMapRangePt_["HLT_Photon50_R9Id90_HE10_IsoM"]   = TrigRangePt{60  ,85 };
            trigMapRangePt_["HLT_Photon30_HoverELoose"]        = TrigRangePt{35  ,60 };
            trigMapRangePt_["HLT_Photon20_HoverELoose"]        = TrigRangePt{20  ,35 };
        }
    } 
    
    else if (year_ == GlobalFlag::Year::Year2018){
        // ZeeJet channel
        if(channel_ == GlobalFlag::Channel::ZeeJet){
            trigList_ = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"};
        }        

        // ZmmJet channel
        if(channel_ == GlobalFlag::Channel::ZmmJet){
            trigList_ = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8"};
        }        

        // GamJet channel
        if(channel_ == GlobalFlag::Channel::GamJet){
            trigMapRangePt_["HLT_Photon200"]                     = TrigRangePt{230 ,4000};
            trigMapRangePt_["HLT_Photon110EB_TightID_TightIso"]  = TrigRangePt{130 ,230};
            trigMapRangePt_["HLT_Photon100EB_TightID_TightIso"]  = TrigRangePt{105 ,130};
            trigMapRangePt_["HLT_Photon90_R9Id90_HE10_IsoM"]     = TrigRangePt{95  ,105};
            trigMapRangePt_["HLT_Photon75_R9Id90_HE10_IsoM"]     = TrigRangePt{85  ,95 };
            trigMapRangePt_["HLT_Photon50_R9Id90_HE10_IsoM"]     = TrigRangePt{60  ,85 };
            trigMapRangePt_["HLT_Photon30_HoverELoose"]          = TrigRangePt{35  ,60 };
            trigMapRangePt_["HLT_Photon20_HoverELoose"]          = TrigRangePt{20  ,35 };
        }
    }

    if(channel_ == GlobalFlag::Channel::MultiJet){
        //DiJet, Multijet channels
        //trigMapRangePtEta_["HLT_DiPFJetAve40"]       = TrigRangePtEta{40, 85, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet40"]            = TrigRangePtEta{40, 85, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet60"]            = TrigRangePtEta{85, 100, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet80"]            = TrigRangePtEta{100, 155, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet140"]           = TrigRangePtEta{155, 210, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet200"]           = TrigRangePtEta{210, 300, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet260"]           = TrigRangePtEta{300, 400, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet320"]           = TrigRangePtEta{400, 500, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet400"]           = TrigRangePtEta{500, 600, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet450"]           = TrigRangePtEta{500, 600, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet500"]           = TrigRangePtEta{600, 6500, 0, 5.2};
        trigMapRangePtEta_["HLT_PFJet550"]           = TrigRangePtEta{700, 6500, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve40"]       = TrigRangePtEta{40, 85, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve60"]       = TrigRangePtEta{85, 100, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve80"]       = TrigRangePtEta{100, 155, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve140"]      = TrigRangePtEta{155, 210, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve200"]      = TrigRangePtEta{210, 300, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve260"]      = TrigRangePtEta{300, 400, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve320"]      = TrigRangePtEta{400, 500, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve400"]      = TrigRangePtEta{500, 600, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve500"]      = TrigRangePtEta{600, 6500, 0, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve60_HFJEC"] = TrigRangePtEta{85, 100, 3.139, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve80_HFJEC"] = TrigRangePtEta{100, 125, 3.139, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve100_HFJEC"]= TrigRangePtEta{125, 180, 3.139, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve160_HFJEC"]= TrigRangePtEta{180, 250, 3.139, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve220_HFJEC"]= TrigRangePtEta{250, 350, 2.964, 5.2};
        //trigMapRangePtEta_["HLT_DiPFJetAve300_HFJEC"]= TrigRangePtEta{350, 6500, 2.964, 5.2};
    }
}

const std::vector<std::string> TrigDetail::getTrigNames() const{
    std::vector<std::string> tNames;
    if(channel_ == GlobalFlag::Channel::ZeeJet){
        tNames = trigList_;
    }        
    else if(channel_ == GlobalFlag::Channel::ZmmJet){
        tNames = trigList_;
    }        
    else if(channel_ == GlobalFlag::Channel::GamJet){
        for (const auto& trig : trigMapRangePt_) {
            tNames.push_back(trig.first);
        }
    }
    else if(channel_ == GlobalFlag::Channel::MultiJet){
        for (const auto& trig : trigMapRangePtEta_) {
            tNames.push_back(trig.first);
        }
    }
    return tNames;
}

// Destructor
TrigDetail::~TrigDetail() {
    // No dynamic memory to clean up in this class
}

