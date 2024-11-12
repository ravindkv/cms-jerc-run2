// HistCutflow.cpp
#include "HistCutflow.h"
#include <iostream>

HistCutflow::HistCutflow(const std::string& name, const std::vector<std::string>& cuts, TDirectory* parentDir)
    : histName(name), cutNames(cuts) {

    // Ensure the parent directory exists
    if (!parentDir) {
        std::cerr << "Error: Parent directory is null. Cannot create cutflow histogram.\n";
        return;
    }

    // Create the histogram within the parent directory
    parentDir->cd();
    h1EventsInCutflow = std::make_unique<TH1D>(histName.c_str(), "", 
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

