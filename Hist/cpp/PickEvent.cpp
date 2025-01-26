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

auto PickEvent::passHltWithPt(const std::shared_ptr<SkimTree>& skimT, const double& pt) const -> bool {
    if (channel_ == GlobalFlag::Channel::GamJet) {
        const auto& trigs = trigDetail_.getTrigMapRangePt();
        //Loop over the triggers
        for (const auto& [trigName, trigRangePt] : trigs){
            const auto& trigValue = static_cast<int>(skimT->getTrigValue(trigName));
             //exit the loop when an event fires any trigger
            if (trigValue && pt>=trigRangePt.ptMin && pt< trigRangePt.ptMax){
                printDebug(trigName + ", pt = "+std::to_string(pt)+" : "+ std::to_string(trigValue));
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
        //exit the loop when a filter is NOT true
        if (!filterValue){
            printDebug(filterName + ": "+ std::to_string(filterValue));
            return false;
        }
    }//filter loop
    return true;
}


