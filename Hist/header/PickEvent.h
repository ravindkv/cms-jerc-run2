#ifndef PICKEVENT_H
#define PICKEVENT_H

#include <iostream>
#include "SkimTree.h"
#include "GlobalFlag.h"
#include "TrigDetail.h"

class PickEvent{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit PickEvent(GlobalFlag& globalFlags);

    bool passFilter(const std::shared_ptr<SkimTree>& skimT) const;
    bool passHlt(const std::shared_ptr<SkimTree>& skimT) const;
    bool passHltWithPt(const std::shared_ptr<SkimTree>& skimT, const double& pt) const;
    
    std::vector<std::string> getTrigNames() const;
    std::unordered_map<std::string, const Bool_t*> getTrigValues() const;

    ~PickEvent();

private:
    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;
    const bool isMC_;
    void printDebug(const std::string& message) const;

    TrigDetail trigDetail_;
};

#endif // PICKEVENT_H

