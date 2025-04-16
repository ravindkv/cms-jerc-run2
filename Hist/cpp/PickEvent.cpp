#include "PickEvent.h"

// Constructor implementation
PickEvent::PickEvent(GlobalFlag& globalFlags) : 
    globalFlags_(globalFlags),
    trigDetail_(globalFlags_),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isMC_(globalFlags_.isMC()),
    isDebug_(globalFlags_.isDebug()){
}

// Destructor
PickEvent::~PickEvent() {
    std::cout << "Destructor of PickEvent" << '\n';
}

// Helper function for printing debug messages
void PickEvent::printDebug(const std::string& message) const {
    if (isDebug_) {
        std::cout << message << '\n';
    }
}


bool PickEvent::passHlt(const std::shared_ptr<SkimTree>& skimT){
    printDebug("<- PickEvent::passHlt ->");
    passedHlts_.clear();
    bool isPass = false;
    for (const auto& trigName : skimT->getTrigNames()){
        const auto& trigValue = static_cast<int>(skimT->getTrigValue(trigName));
        if (trigValue) {
            passedHlts_.push_back(trigName);
            printDebug(trigName + ": "+ std::to_string(trigValue));
            isPass = true;
        }
    }
    return isPass; 
}

auto PickEvent::passHltWithPt(const std::shared_ptr<SkimTree>& skimT, 
                              const double& pt) -> bool {
    printDebug("<- PickEvent::passHltWithPt ->");
    bool isPassedHlt = false;
    if (channel_ == GlobalFlag::Channel::GamJet) {
        const auto& trigs = trigDetail_.getTrigMapRangePt();

        // Loop over all triggers and collect those that pass
        for (const auto& [trigName, trigRangePt] : trigs) {
            const auto trigValue = static_cast<int>(skimT->getTrigValue(trigName));
            if (trigValue &&
                pt >= trigRangePt.ptMin &&
                pt < trigRangePt.ptMax) {
                isPassedHlt = true;
                passedHlt_ = trigName;
                printDebug(trigName + ", pt = " + std::to_string(pt) + 
                           " : " + std::to_string(trigValue));
                break;
            }
        }
    }

    return isPassedHlt;
}

auto PickEvent::passHltWithPtEta(const std::shared_ptr<SkimTree>& skimT, 
                                 const double& pt, 
                                 const double& eta) -> bool {
    printDebug("<- PickEvent::passHltWithPtEta ->");

    bool isPassedHlt = false;
    if (channel_ == GlobalFlag::Channel::MultiJet) {
        const auto& trigs = trigDetail_.getTrigMapRangePtEta();

        // Loop over all triggers and collect those that pass
        for (const auto& [trigName, trigRangePtEta] : trigs) {
            const auto trigValue = static_cast<int>(skimT->getTrigValue(trigName));

            if (trigValue &&
                pt >= trigRangePtEta.ptMin &&
                pt < trigRangePtEta.ptMax &&
                std::abs(eta) >= trigRangePtEta.absEtaMin &&
                std::abs(eta) < trigRangePtEta.absEtaMax) {
                isPassedHlt = true;
                passedHlt_ = trigName;
                printDebug(trigName + ", pt = " + std::to_string(pt) + 
                           ", eta = " + std::to_string(eta) + 
                           " : " + std::to_string(trigValue));
                break;
            }
        }
    }

    return isPassedHlt;
}

