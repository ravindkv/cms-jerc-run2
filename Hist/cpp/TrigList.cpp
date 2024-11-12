// TrigList.cpp
#include "TrigList.h"
#include "SkimTree.h" // Ensure this includes the definition of SkimTree
#include <iostream>

// Constructor
TrigList::TrigList(const SkimTree* skimT) {
    if (!skimT) {
        std::cerr << "Error: SkimTree pointer is null in TrigList constructor.\n";
        return;
    }

    trigDetails_["HLT_DiPFJetAve40"]       = std::make_pair(&skimT->HLT_DiPFJetAve40,       Range{40, 85, 0, 5.2});
    // Initialize trigDetails_ map
    /*
    trigDetails_["HLT_PFJet40"]            = std::make_pair(&skimT->HLT_PFJet40,            Range{40, 85, 0, 5.2});
    trigDetails_["HLT_PFJet60"]            = std::make_pair(&skimT->HLT_PFJet60,            Range{85, 100, 0, 5.2});
    trigDetails_["HLT_PFJet80"]            = std::make_pair(&skimT->HLT_PFJet80,            Range{100, 155, 0, 5.2});
    trigDetails_["HLT_PFJet140"]           = std::make_pair(&skimT->HLT_PFJet140,           Range{155, 210, 0, 5.2});
    trigDetails_["HLT_PFJet200"]           = std::make_pair(&skimT->HLT_PFJet200,           Range{210, 300, 0, 5.2});
    trigDetails_["HLT_PFJet260"]           = std::make_pair(&skimT->HLT_PFJet260,           Range{300, 400, 0, 5.2});
    trigDetails_["HLT_PFJet320"]           = std::make_pair(&skimT->HLT_PFJet320,           Range{400, 500, 0, 5.2});
    trigDetails_["HLT_PFJet400"]           = std::make_pair(&skimT->HLT_PFJet400,           Range{500, 600, 0, 5.2});
    trigDetails_["HLT_PFJet450"]           = std::make_pair(&skimT->HLT_PFJet450,           Range{500, 600, 0, 5.2});
    trigDetails_["HLT_PFJet500"]           = std::make_pair(&skimT->HLT_PFJet500,           Range{600, 6500, 0, 5.2});
    trigDetails_["HLT_PFJet550"]           = std::make_pair(&skimT->HLT_PFJet550,           Range{700, 6500, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve40"]       = std::make_pair(&skimT->HLT_DiPFJetAve40,       Range{40, 85, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve60"]       = std::make_pair(&skimT->HLT_DiPFJetAve60,       Range{85, 100, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve80"]       = std::make_pair(&skimT->HLT_DiPFJetAve80,       Range{100, 155, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve140"]      = std::make_pair(&skimT->HLT_DiPFJetAve140,      Range{155, 210, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve200"]      = std::make_pair(&skimT->HLT_DiPFJetAve200,      Range{210, 300, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve260"]      = std::make_pair(&skimT->HLT_DiPFJetAve260,      Range{300, 400, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve320"]      = std::make_pair(&skimT->HLT_DiPFJetAve320,      Range{400, 500, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve400"]      = std::make_pair(&skimT->HLT_DiPFJetAve400,      Range{500, 600, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve500"]      = std::make_pair(&skimT->HLT_DiPFJetAve500,      Range{600, 6500, 0, 5.2});
    trigDetails_["HLT_DiPFJetAve60_HFJEC"] = std::make_pair(&skimT->HLT_DiPFJetAve60_HFJEC, Range{85, 100, 3.139, 5.2});
    trigDetails_["HLT_DiPFJetAve80_HFJEC"] = std::make_pair(&skimT->HLT_DiPFJetAve80_HFJEC, Range{100, 125, 3.139, 5.2});
    trigDetails_["HLT_DiPFJetAve100_HFJEC"]= std::make_pair(&skimT->HLT_DiPFJetAve100_HFJEC,Range{125, 180, 3.139, 5.2});
    trigDetails_["HLT_DiPFJetAve160_HFJEC"]= std::make_pair(&skimT->HLT_DiPFJetAve160_HFJEC,Range{180, 250, 3.139, 5.2});
    trigDetails_["HLT_DiPFJetAve220_HFJEC"]= std::make_pair(&skimT->HLT_DiPFJetAve220_HFJEC,Range{250, 350, 2.964, 5.2});
    trigDetails_["HLT_DiPFJetAve300_HFJEC"]= std::make_pair(&skimT->HLT_DiPFJetAve300_HFJEC,Range{350, 6500, 2.964, 5.2});
    */
}

// Destructor
TrigList::~TrigList() {
    // No dynamic memory to clean up in this class
}

// Method to get the trigDetails map
const std::map<std::string, std::pair<const Bool_t*, Range>>& TrigList::getTrigDetails() const {
    return trigDetails_;
}

