#ifndef EVENTPICK_H
#define EVENTPICK_H

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <iomanip>
#include <chrono>

#include <TFile.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TProfile2D.h>

#include "SkimTree.h"
#include "GlobalFlag.h"

class EventPick{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit EventPick(GlobalFlag& globalFlags);

    bool passFilter(const std::shared_ptr<SkimTree>& tree) const;
    bool passHLT(const std::shared_ptr<SkimTree>& tree) const;
    
    std::vector<std::string> getTrigNames() const;
    std::map<std::string, const Bool_t*> getTrigValues() const;

    void printProgress(Long64_t jentry, Long64_t nentries, 
                       std::chrono::time_point<std::chrono::high_resolution_clock>& startClock, 
                       double& totTime) const;
    
    void printBins(const TH1D* hist) const;
    void printInfo(const TObject* obj) const;
    void scanDirectory(TDirectory* dir, const std::string& path) const;
    void scanTFile(TFile* file) const;

    ~EventPick();

private:
    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;
    void printDebug(const std::string& message) const;
};

#endif // EVENTPICK_H

