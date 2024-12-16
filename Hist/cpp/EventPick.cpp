#include "EventPick.h"

// Constructor implementation
EventPick::EventPick(GlobalFlag& globalFlags) : 
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isMC_(globalFlags_.isMC()),
    isDebug_(globalFlags_.isDebug()){
}

// Destructor
EventPick::~EventPick() {
    std::cout << "Destructor of EventPick" << '\n';
}

// Helper function for printing debug messages
void EventPick::printDebug(const std::string& message) const {
    if (isDebug_) {
        std::cout << message << '\n';
    }
}

// Function to check High-Level Triggers (HLT)
auto EventPick::passHLT(const std::shared_ptr<SkimTree>& tree) const -> bool {
    bool pass_HLT = false;

    if (channel_ == GlobalFlag::Channel::ZeeJet) {
        if (year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post) {
            pass_HLT = tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
        } else if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
            pass_HLT = tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
        }
    }//ZeeJet

    else if (channel_ == GlobalFlag::Channel::ZmmJet) {
        if (year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post) {
            pass_HLT = tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
        } else if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
            pass_HLT = tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
        }
    }//ZmmJet

    else if (channel_ == GlobalFlag::Channel::GamJet) {
        // Combine all HLT conditions into a vector for easier management
        std::vector<bool> hlts = {
                    tree->HLT_Photon200,
                    tree->HLT_Photon175,
                    tree->HLT_Photon150,
                    tree->HLT_Photon120,
                    tree->HLT_Photon90, 
                    tree->HLT_Photon75, 
                    tree->HLT_Photon50, 
                    tree->HLT_Photon33, 
                    tree->HLT_Photon20, 
                    tree->HLT_Photon90_R9Id90_HE10_IsoM,
                    tree->HLT_Photon75_R9Id90_HE10_IsoM,
                    tree->HLT_Photon50_R9Id90_HE10_IsoM
        };
        if (year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post) {
            hlts.push_back(tree->HLT_Photon36);
            hlts.push_back(tree->HLT_Photon30);
            hlts.push_back(tree->HLT_Photon22);
            hlts.push_back(tree->HLT_Photon165_R9Id90_HE10_IsoM);
            hlts.push_back(tree->HLT_Photon120_R9Id90_HE10_IsoM);
            hlts.push_back(tree->HLT_Photon36_R9Id90_HE10_IsoM);
            hlts.push_back(tree->HLT_Photon30_R9Id90_HE10_IsoM);
            hlts.push_back(tree->HLT_Photon22_R9Id90_HE10_IsoM); 
        }
        else if (year_ == GlobalFlag::Year::Year2017){
            hlts.push_back(tree->HLT_Photon165_R9Id90_HE10_IsoM);
            hlts.push_back(tree->HLT_Photon120_R9Id90_HE10_IsoM);
            hlts.push_back(tree->HLT_Photon60_HoverELoose);
            hlts.push_back(tree->HLT_Photon50_HoverELoose);
            hlts.push_back(tree->HLT_Photon40_HoverELoose);
            hlts.push_back(tree->HLT_Photon30_HoverELoose);
            hlts.push_back(tree->HLT_Photon20_HoverELoose); 
        } 
        else if (year_ == GlobalFlag::Year::Year2018){
            hlts.push_back(tree->HLT_Photon120EB_TightID_TightIso);
            hlts.push_back(tree->HLT_Photon110EB_TightID_TightIso);
            hlts.push_back(tree->HLT_Photon100EB_TightID_TightIso);
            hlts.push_back(tree->HLT_Photon30_HoverELoose);
            hlts.push_back(tree->HLT_Photon20_HoverELoose); 
        }
        // Check if any of the HLT conditions are true
        pass_HLT = std::any_of(hlts.begin(), hlts.end(), [](bool hlt) {
            return hlt;
        });
        if (isDebug_){
            for (auto hlt: hlts) std::cout << "HLT value: " << hlt << "\n";
        }
    }//GamJet

    printDebug("pass_HLT = " + std::to_string(pass_HLT));
    return pass_HLT;
}


auto EventPick::passHltWithPt(const std::shared_ptr<SkimTree>& tree, const double& pt) const -> bool {
    bool passHlt = false;

    if (channel_ == GlobalFlag::Channel::GamJet) {
        // Combine all HLT conditions into a vector for easier management
        std::vector<bool> hlts = {};
        if (year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post) {
        	hlts.push_back(tree->HLT_Photon175                  && pt>=230          );
			hlts.push_back(tree->HLT_Photon165_R9Id90_HE10_IsoM && pt>=175 && pt<230);
			hlts.push_back(tree->HLT_Photon120_R9Id90_HE10_IsoM && pt>=130 && pt<175);
			hlts.push_back(tree->HLT_Photon90_R9Id90_HE10_IsoM  && pt>=105 && pt<130);
			hlts.push_back(tree->HLT_Photon75_R9Id90_HE10_IsoM  && pt>=85  && pt<105);
			hlts.push_back(tree->HLT_Photon50_R9Id90_HE10_IsoM  && pt>=60  && pt<85 );
			hlts.push_back(tree->HLT_Photon36_R9Id90_HE10_IsoM  && pt>=40  && pt<60 );
			hlts.push_back(tree->HLT_Photon30_R9Id90_HE10_IsoM  && pt>=35  && pt<40 );
			hlts.push_back(tree->HLT_Photon22_R9Id90_HE10_IsoM  && pt>=20  && pt<35 );
			hlts.push_back(isMC_                         && pt>=40  && pt<60 );
			hlts.push_back(isMC_                         && pt>=35  && pt<40 );
			hlts.push_back(isMC_                         && pt>=20  && pt<35 );
        }

        else if (year_ == GlobalFlag::Year::Year2017){
			hlts.push_back(tree->HLT_Photon200                  && pt>=230         );
            hlts.push_back(tree->HLT_Photon165_R9Id90_HE10_IsoM && pt>=175 && pt<230);
            hlts.push_back(tree->HLT_Photon120_R9Id90_HE10_IsoM && pt>=130 && pt<175);
            hlts.push_back(tree->HLT_Photon90_R9Id90_HE10_IsoM  && pt>=105 && pt<130);
            hlts.push_back(tree->HLT_Photon75_R9Id90_HE10_IsoM  && pt>=85  && pt<105);
            hlts.push_back(tree->HLT_Photon50_R9Id90_HE10_IsoM  && pt>=60  && pt<85 );
            hlts.push_back(tree->HLT_Photon30_HoverELoose       && pt>=35  && pt<60 );
            hlts.push_back(tree->HLT_Photon20_HoverELoose       && pt>=20  && pt<35 );
            hlts.push_back(isMC_                           && pt>=35  && pt<60);
            hlts.push_back(isMC_                           && pt>=20  && pt<35);
        } 

        else if (year_ == GlobalFlag::Year::Year2018){
			hlts.push_back(tree->HLT_Photon200                    && pt>=230          );
            hlts.push_back(tree->HLT_Photon110EB_TightID_TightIso && pt>=130 && pt<230);
            hlts.push_back(tree->HLT_Photon100EB_TightID_TightIso && pt>=105 && pt<130);
            hlts.push_back(tree->HLT_Photon90_R9Id90_HE10_IsoM    && pt>=95  && pt<105);
            hlts.push_back(tree->HLT_Photon75_R9Id90_HE10_IsoM    && pt>=85  && pt<95 );
            hlts.push_back(tree->HLT_Photon50_R9Id90_HE10_IsoM    && pt>=60  && pt<85 );
            hlts.push_back(tree->HLT_Photon30_HoverELoose         && pt>=35  && pt<60 );
            hlts.push_back(tree->HLT_Photon20_HoverELoose         && pt>=20  && pt<35 );
            hlts.push_back(isMC_                           && pt>=35  && pt<60);
            hlts.push_back(isMC_                           && pt>=20  && pt<35);
        }
        // Check if any of the HLT conditions are true
        passHlt = std::any_of(hlts.begin(), hlts.end(), [](bool hlt) {
            return hlt;
        });
    }//GamJet

    printDebug("passHlt = " + std::to_string(passHlt));
    return passHlt;
}

// Function to apply event filters
auto EventPick::passFilter(const std::shared_ptr<SkimTree>& tree) const -> bool {
    bool pass = tree->Flag_goodVertices &&
                tree->Flag_globalSuperTightHalo2016Filter &&
                tree->Flag_HBHENoiseFilter &&
                tree->Flag_HBHENoiseIsoFilter &&
                tree->Flag_EcalDeadCellTriggerPrimitiveFilter &&
                tree->Flag_BadPFMuonFilter &&
                tree->Flag_eeBadScFilter;

    if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
        pass = pass && tree->Flag_ecalBadCalibFilter;
    }

    // Debugging output
    if (isDebug_) {
        printDebug("Event Filters:");
        printDebug("Flag_goodVertices: " + std::to_string(tree->Flag_goodVertices));
        printDebug("Flag_globalSuperTightHalo2016Filter: " + std::to_string(tree->Flag_globalSuperTightHalo2016Filter));
        printDebug("Flag_HBHENoiseFilter: " + std::to_string(tree->Flag_HBHENoiseFilter));
        printDebug("Flag_HBHENoiseIsoFilter: " + std::to_string(tree->Flag_HBHENoiseIsoFilter));
        printDebug("Flag_EcalDeadCellTriggerPrimitiveFilter: " + std::to_string(tree->Flag_EcalDeadCellTriggerPrimitiveFilter));
        printDebug("Flag_BadPFMuonFilter: " + std::to_string(tree->Flag_BadPFMuonFilter));
        printDebug("Flag_eeBadScFilter: " + std::to_string(tree->Flag_eeBadScFilter));
        if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
            printDebug("Flag_ecalBadCalibFilter: " + std::to_string(tree->Flag_ecalBadCalibFilter));
        }
    }

    return pass;
}


