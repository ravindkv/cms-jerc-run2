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


bool PickEvent::passHlt(const std::shared_ptr<SkimTree>& skimT) const {
    for (const auto& trigName : skimT->getTrigNames()){
        const auto& trigValue = static_cast<int>(skimT->getTrigValue(trigName));
        if (isDebug_) {
            std::cout << trigName << ": " << trigValue << std::endl;
        }
        if (trigValue) {
            return true; // An event fired this trigger
        }
    }
    return false; // No trigger fired
}


auto PickEvent::passHltWithPt(const std::shared_ptr<SkimTree>& skimT, const double& pt) const -> bool {
    if (channel_ == GlobalFlag::Channel::GamJet) {
        const auto& trigs = trigDetail_.getTrigMapRangePt();
        //Loop over the triggers
        for (const auto& [trigName, trigRangePt] : trigs){
            const auto& trigValue = static_cast<int>(skimT->getTrigValue(trigName));
             //exit the loop when an event fires any trigger
            printDebug(trigName + ", pt = "+std::to_string(pt)+" : "+ std::to_string(trigValue));
            if (isDebug_) printDebug(trigName + ", pt = "+std::to_string(pt)+" : "+ std::to_string(trigValue));
            if (trigValue && pt>=trigRangePt.ptMin && pt< trigRangePt.ptMax){
               return true;
            }
        }//trig loop
    }//GamJet
    return false;
}

// Function to apply event filters
auto PickEvent::passFilter(const std::shared_ptr<SkimTree>& skimT) const -> bool {
    for (const auto& filterName : skimT->getFilterNames()){
        const auto& filterValue = static_cast<int>(skimT->getFilterValue(filterName));
        if (isDebug_) printDebug(filterName + ": "+ std::to_string(filterValue));
        //exit the loop when a filter is NOT true
        if (!filterValue){
           return false;
        }
    }//filter loop
    return true;
}


