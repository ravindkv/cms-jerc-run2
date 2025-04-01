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

    bool passHlt(const std::shared_ptr<SkimTree>& skimT);
    bool passHltWithPt(const std::shared_ptr<SkimTree>& skimT, const double& pt);
    bool passHltWithPtEta(const std::shared_ptr<SkimTree>& skimT, const double& pt, const double& eta);
    std::vector<std::string> getPassedHlts(){return passedHlts_;}
    std::string getPassedHlt(){return passedHlt_;}
    
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
    std::vector<std::string> passedHlts_{};
    std::string passedHlt_;
};

#endif // PICKEVENT_H

