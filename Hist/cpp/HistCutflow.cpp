// HistCutflow.cpp
#include "HistCutflow.h"
#include "Helper.h"
#include <iostream>

HistCutflow::HistCutflow(TDirectory *origDir, const std::string& directoryName, const std::vector<std::string>& cuts)
    : cutNames(cuts) {

    // Use the Helper method to get or create the directory
    std::string dirName = directoryName + "/HistCutflow";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    newDir->cd();

    h1EventsInCutflow = std::make_unique<TH1D>("h1EventInCutflow", "", 
                                              static_cast<int>(cuts.size()), 0.5, cuts.size() + 0.5);

    // Initialize the cut to bin mapping and set bin labels
    for (size_t i = 0; i < cutNames.size(); ++i) {
        // ROOT bins are 1-based
        cutToBinMap[cutNames[i]] = static_cast<int>(i + 1);
        h1EventsInCutflow->GetXaxis()->SetBinLabel(static_cast<int>(i + 1), cutNames[i].c_str());
    }

    // Optionally, set axis titles if needed
    // h1EventsInCutflow->GetXaxis()->SetTitle("Cuts");
    // h1EventsInCutflow->GetYaxis()->SetTitle("Number of Events");
    origDir->cd();
}

HistCutflow::~HistCutflow() {
    // ROOT manages the memory of histograms when associated with a directory
    // If not, uncomment the following line to delete manually
    // h1EventsInCutflow.reset();
}

void HistCutflow::fill(const std::string& cutName, double weight) {
    auto it = cutToBinMap.find(cutName);
    if (it != cutToBinMap.end()) {
        h1EventsInCutflow->Fill(it->second, weight);
    } else {
        std::cerr << "Warning: Cut name \"" << cutName << "\" not found in cutToBinMap.\n";
    }
}

