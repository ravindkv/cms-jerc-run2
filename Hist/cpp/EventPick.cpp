#include "EventPick.h"

// Constructor implementation
EventPick::EventPick(GlobalFlag& globalFlags) : 
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
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
            printDebug("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ: " +
                       std::to_string(tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ));
        } else if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
            pass_HLT = tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
            printDebug("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL: " +
                       std::to_string(tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL));
        }
    }

    if (channel_ == GlobalFlag::Channel::ZmmJet) {
        if (year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post) {
            pass_HLT = tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
            printDebug("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ: " +
                       std::to_string(tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ));
        } else if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
            pass_HLT = tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
            printDebug("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8: " +
                       std::to_string(tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8));
        }
    }

    if (channel_ == GlobalFlag::Channel::GamJet) {
        // Combine all HLT conditions into a vector for easier management
        std::vector<bool> hlt_conditions = {
            tree->HLT_Photon200,
            tree->HLT_Photon175,
            tree->HLT_Photon150,
            tree->HLT_Photon120,
            tree->HLT_Photon90,
            tree->HLT_Photon75,
            tree->HLT_Photon50,
            tree->HLT_Photon33,
            tree->HLT_Photon110EB_TightID_TightIso,
            tree->HLT_Photon100EBHE10,
            tree->HLT_Photon30EB_TightID_TightIso,
            tree->HLT_Photon90_R9Id90_HE10_IsoM,
            tree->HLT_Photon75_R9Id90_HE10_IsoM,
            tree->HLT_Photon50_R9Id90_HE10_IsoM,
            tree->HLT_Photon30_HoverELoose,
            tree->HLT_Photon20_HoverELoose
        };

        // Check if any of the HLT conditions are true
        pass_HLT = std::any_of(hlt_conditions.begin(), hlt_conditions.end(), [](bool hlt) { return hlt; });

        // Debugging output
        if (isDebug_) {
            printDebug("HLT Photon Triggers:");
            printDebug("HLT_Photon200: " + std::to_string(tree->HLT_Photon200));
            printDebug("HLT_Photon175: " + std::to_string(tree->HLT_Photon175));
            printDebug("HLT_Photon150: " + std::to_string(tree->HLT_Photon150));
            printDebug("HLT_Photon120: " + std::to_string(tree->HLT_Photon120));
            printDebug("HLT_Photon90: " + std::to_string(tree->HLT_Photon90));
            printDebug("HLT_Photon75: " + std::to_string(tree->HLT_Photon75));
            printDebug("HLT_Photon50: " + std::to_string(tree->HLT_Photon50));
            printDebug("HLT_Photon33: " + std::to_string(tree->HLT_Photon33));
            printDebug("HLT_Photon110EB_TightID_TightIso: " + std::to_string(tree->HLT_Photon110EB_TightID_TightIso));
            printDebug("HLT_Photon100EBHE10: " + std::to_string(tree->HLT_Photon100EBHE10));
            printDebug("HLT_Photon30EB_TightID_TightIso: " + std::to_string(tree->HLT_Photon30EB_TightID_TightIso));
            printDebug("HLT_Photon90_R9Id90_HE10_IsoM: " + std::to_string(tree->HLT_Photon90_R9Id90_HE10_IsoM));
            printDebug("HLT_Photon75_R9Id90_HE10_IsoM: " + std::to_string(tree->HLT_Photon75_R9Id90_HE10_IsoM));
            printDebug("HLT_Photon50_R9Id90_HE10_IsoM: " + std::to_string(tree->HLT_Photon50_R9Id90_HE10_IsoM));
            printDebug("HLT_Photon30_HoverELoose: " + std::to_string(tree->HLT_Photon30_HoverELoose));
            printDebug("HLT_Photon20_HoverELoose: " + std::to_string(tree->HLT_Photon20_HoverELoose));
        }
    }//channel_ == GlobalFlag::Channel::GamJet

    if (channel_ == GlobalFlag::Channel::MCTruth || channel_ == GlobalFlag::Channel::VetoMap) {
        pass_HLT = tree->HLT_MC;
        if(isDebug_)
            printDebug("HLT_MC: " + std::to_string(tree->HLT_MC));
    }

    return pass_HLT;
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


