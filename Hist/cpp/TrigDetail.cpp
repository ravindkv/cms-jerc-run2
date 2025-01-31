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
        //https://github.com/NestorMancilla/dijet/blob/0ee620884662cadfb7b41ac1914b98ba7d94ea39/histogram_scripts/DijetHistosCombine.C#L673-L694
        double fwdEta0 = 2.964;//2.853; // 40 and 260 up
        double fwdEta = 3.0; // was 3.139; // was 2.853. 80% (100%) on negative (positive) side
        double minEta = 0.0;
        double maxEta = 5.2;
        //Multijet channels
        trigMapRangePtEta_["HLT_PFJet40"]            = TrigRangePtEta{40,  49,  84,  minEta, fwdEta0};
        trigMapRangePtEta_["HLT_PFJet60"]            = TrigRangePtEta{60,  84,  114, minEta, fwdEta};
        trigMapRangePtEta_["HLT_PFJet80"]            = TrigRangePtEta{80,  114, 196, minEta, fwdEta};
        trigMapRangePtEta_["HLT_PFJet140"]           = TrigRangePtEta{140, 196, 272, minEta, fwdEta};
        trigMapRangePtEta_["HLT_PFJet200"]           = TrigRangePtEta{200, 272, 330, minEta, fwdEta0};
        trigMapRangePtEta_["HLT_PFJet260"]           = TrigRangePtEta{260, 330, 395, minEta, fwdEta0};
        trigMapRangePtEta_["HLT_PFJet320"]           = TrigRangePtEta{320, 395, 468, minEta, fwdEta0};
        trigMapRangePtEta_["HLT_PFJet400"]           = TrigRangePtEta{400, 468, 548, minEta, fwdEta0};
        trigMapRangePtEta_["HLT_PFJet450"]           = TrigRangePtEta{450, 548, 686, minEta, fwdEta0};
        trigMapRangePtEta_["HLT_PFJet500"]           = TrigRangePtEta{500, 686,6500, minEta, fwdEta0};

        trigMapRangePtEta_["HLT_PFJetFwd40"]         = TrigRangePtEta{40,  49,  84,  fwdEta0, maxEta};
        trigMapRangePtEta_["HLT_PFJetFwd60"]         = TrigRangePtEta{60,  84,  114, fwdEta, maxEta};
        trigMapRangePtEta_["HLT_PFJetFwd80"]         = TrigRangePtEta{80,  114, 196, fwdEta, maxEta};
        trigMapRangePtEta_["HLT_PFJetFwd140"]        = TrigRangePtEta{140, 196, 272, fwdEta, maxEta};
        trigMapRangePtEta_["HLT_PFJetFwd200"]        = TrigRangePtEta{200, 272, 330, fwdEta0, maxEta};
        trigMapRangePtEta_["HLT_PFJetFwd260"]        = TrigRangePtEta{260, 330, 395, fwdEta0, maxEta};
        trigMapRangePtEta_["HLT_PFJetFwd320"]        = TrigRangePtEta{320, 395, 468, fwdEta0, maxEta};
        trigMapRangePtEta_["HLT_PFJetFwd400"]        = TrigRangePtEta{400, 468, 548, fwdEta0, maxEta};
        trigMapRangePtEta_["HLT_PFJetFwd450"]        = TrigRangePtEta{450, 548, 686, fwdEta0, maxEta};
        trigMapRangePtEta_["HLT_PFJetFwd500"]        = TrigRangePtEta{500, 686,6500, fwdEta0, maxEta};
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

