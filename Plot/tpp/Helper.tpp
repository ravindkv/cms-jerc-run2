
#include <cmath> // For sqrt, pow

// General templated function to calculate ratio and fill a TGraphErrors
template<typename T>
void Helper::calculateHistRatio(T* dataHist, T* mcHist, TGraphErrors* ratioGraph) {
    int nBins = dataHist->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        double dataVal = dataHist->GetBinContent(i);
        double mcVal = mcHist->GetBinContent(i);
        double ratioVal = 0.0;
        double ratioErr = 0.0;
        double binWidth = dataHist->GetXaxis()->GetBinWidth(i);  // Get the bin width
        if(mcVal > 0 && dataVal > 0){
            ratioVal = dataVal / mcVal;
            ratioErr = sqrt(pow(dataHist->GetBinError(i) / mcVal, 2) +
                            pow(dataVal * mcHist->GetBinError(i) / pow(mcVal, 2), 2));
        }
        ratioGraph->SetPoint(i - 1, dataHist->GetXaxis()->GetBinCenter(i), ratioVal);
        ratioGraph->SetPointError(i - 1, binWidth / 2.0, ratioErr);  // Set x-error to half the bin width
    }
}

// General templated function to combine multiple histograms
template<typename T>
T* Helper::combineHists(const std::vector<T*>& hists) {
    T* combinedHist = nullptr;
    for (const auto &entry : hists) {
        if (entry->GetEntries() == 0) {
            std::cerr << "Warning: A histogram in the combine function has no entries." << std::endl;
            continue;
        }
        if (!combinedHist) {
            combinedHist = (T*)entry->Clone();
        } else {
            combinedHist->Add(entry);
        }
    }
    return combinedHist;
}

template<typename T>
T* Helper::sumHistsFromTrigDirs(TFile& file, const std::string& histDir, const std::vector<std::string> trigDirs, const std::string& histName){
    T* summedHist = nullptr; // Pointer to hold the summed histogram

    for (const auto& trigDir : trigDirs) {
        // Construct the full path to the histogram
        std::string path = histDir.empty() ? 
                           histName : 
                           (trigDir + "/" + histDir + "/" + histName);

        // Retrieve the histogram from the file
        T* hist = static_cast<T*>(file.Get(path.c_str()));
        if (!hist) {
            std::cerr << "Warning: Could not retrieve histogram " << path
                      << " from " << file.GetName() << std::endl;
            continue;
        }

        // Clone the histogram to avoid modifying the original
        T* clonedHist = static_cast<T*>(hist->Clone());
        if (!clonedHist) {
            std::cerr << "Error: Failed to clone histogram " << path << std::endl;
            continue;
        }

        if (!summedHist) {
            // Initialize the summed histogram with the first cloned histogram
            summedHist = static_cast<T*>(clonedHist->Clone((histName + "_summed").c_str()));
            if (!summedHist) {
                std::cerr << "Error: Failed to clone summed histogram for " << histName << std::endl;
                delete clonedHist;
                return nullptr;
            }
        } else {
            // Add the cloned histogram to the summed histogram
            summedHist->Add(clonedHist);
            delete clonedHist; // Clean up the cloned histogram after adding
        }
    }

    return summedHist;
}

