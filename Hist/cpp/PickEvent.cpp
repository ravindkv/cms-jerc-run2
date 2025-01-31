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

    if (channel_ == GlobalFlag::Channel::GamJet) {
        const auto& trigs = trigDetail_.getTrigMapRangePt();
        std::vector<std::string> passedTriggers; // To store all passed triggers

        // Loop over all triggers and collect those that pass
        for (const auto& [trigName, trigRangePt] : trigs) {
            const auto trigValue = static_cast<int>(skimT->getTrigValue(trigName));

            if (trigValue &&
                pt >= trigRangePt.ptMin &&
                pt < trigRangePt.ptMax) {
                
                passedTriggers.emplace_back(trigName);
                printDebug(trigName + ", pt = " + std::to_string(pt) + 
                           " : " + std::to_string(trigValue));
            }
        }

        if (passedTriggers.empty()) {
            // No triggers passed
            return false;
        }

        if (passedTriggers.size() > 1) {
            std::cerr << "Warning: Multiple HLTs fired for pt = " << pt 
                      << ". Possible duplicated range." 
                      << std::endl;
        }

        // If at least one trigger passed, set the first one as the passed HLT
        passedHlt_ = passedTriggers[0];
        return true;
    }

    return false;
}

auto PickEvent::passHltWithPtEta(const std::shared_ptr<SkimTree>& skimT, 
                                 const double& pt, 
                                 const double& eta) -> bool {
    printDebug("<- PickEvent::passHltWithPtEta ->");

    if (channel_ == GlobalFlag::Channel::MultiJet) {
        const auto& trigs = trigDetail_.getTrigMapRangePtEta();
        std::vector<std::string> passedTriggers; // To store all passed triggers

        // Loop over all triggers and collect those that pass
        for (const auto& [trigName, trigRangePtEta] : trigs) {
            const auto trigValue = static_cast<int>(skimT->getTrigValue(trigName));

            if (trigValue &&
                pt >= trigRangePtEta.ptMin &&
                pt < trigRangePtEta.ptMax &&
                std::abs(eta) >= trigRangePtEta.absEtaMin &&
                std::abs(eta) < trigRangePtEta.absEtaMax) {
                
                passedTriggers.emplace_back(trigName);
                printDebug(trigName + ", pt = " + std::to_string(pt) + 
                           ", eta = " + std::to_string(eta) + 
                           " : " + std::to_string(trigValue));
            }
        }

        if (passedTriggers.empty()) {
            return false;
        }

        if (passedTriggers.size() > 1) {
            std::cerr << "Warning: Multiple HLTs fired for pt = " << pt 
                      << ", eta = " << eta << ". Possible duplicated range." 
                      << std::endl;
        }

        // If at least one trigger passed, set the first one as the passed HLT
        passedHlt_ = passedTriggers[0];
        return true;
    }

    return false;
}

// Function to apply event filters
auto PickEvent::passFilter(const std::shared_ptr<SkimTree>& skimT) const -> bool {
    printDebug("<- PickEvent::passFilter ->");
    for (const auto& filterName : skimT->getFilterNames()){
        const auto& filterValue = static_cast<int>(skimT->getFilterValue(filterName));
        //exit the loop when a filter is NOT true
        if (!filterValue){
            printDebug(filterName + ": "+ std::to_string(filterValue));
            return false;
        }
    }//filter loop
    return true;
}


