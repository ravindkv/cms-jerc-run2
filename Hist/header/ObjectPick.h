#ifndef OBJECTPICK_H
#define OBJECTPICK_H

#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>

#include <TH1F.h>
#include <TH1D.h>
#include <TLorentzVector.h>

#include "SkimTree.h"
#include "GlobalFlag.h"

class ObjectPick{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit ObjectPick(GlobalFlag& globalFlags);
    ~ObjectPick();

    // Set the SkimTree pointer
    void setTree(const std::shared_ptr<SkimTree>& skimTree);

    // Clear picked objects
    void clearObjects();

    // Reco objects
    void pickElectrons();
    void pickMuons();
    void pickPhotons();
    void pickRefs();

    // Gen objects
    void pickGenElectrons();
    void pickGenMuons();
    void pickGenPhotons();
    void pickGenRefs();

    // Accessors for picked objects
    const std::vector<int>& getPickedElectrons() const;
    const std::vector<int>& getPickedMuons() const;
    const std::vector<int>& getPickedPhotons() const;
    const std::vector<TLorentzVector>& getPickedRefs() const;

    const std::vector<int>& getPickedGenElectrons() const;
    const std::vector<int>& getPickedGenMuons() const;
    const std::vector<int>& getPickedGenPhotons() const;
    const std::vector<TLorentzVector>& getPickedGenRefs() const;

    const double getBtagThresh()const;
    const double getCtagThresh()const;
    const double getQtagThresh()const;
    const double getGtagThresh()const;


private:
    // Pointer to SkimTree
    std::shared_ptr<SkimTree> skimTree_;

    // Reco objects
    std::vector<int> pickedElectrons_;
    std::vector<int> pickedMuons_;
    std::vector<int> pickedPhotons_;
    std::vector<TLorentzVector> pickedRefs_;

    // Gen objects
    std::vector<int> pickedGenElectrons_;
    std::vector<int> pickedGenMuons_;
    std::vector<int> pickedGenPhotons_;
    std::vector<TLorentzVector> pickedGenRefs_;

    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;

    // Helper function for debug printing
    void printDebug(const std::string& message) const;
    double bTagThresh_;
    double cTagThresh_;
    double qTagThresh_;
    double gTagThresh_;
};

#endif  // OBJECTPICK_H

