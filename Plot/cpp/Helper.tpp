// Helper.tpp

#include <cmath> // For sqrt, pow

// Implementation of the getEras function
std::vector<std::string> Helper::getEras(const nlohmann::json& inputJson, const std::string& channel, const std::string& year, const std::string& key) {
    std::vector<std::string> dataEras;
    if (inputJson.contains(channel) && inputJson[channel].contains(year) && inputJson[channel][year].contains(key)) {
        for (const auto& element : inputJson[channel][year][key].items()) {
            dataEras.push_back(element.key());
        }
    }
    return dataEras;
}

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

std::string Helper::dirToName(const std::string& dir) {
    std::string filename = dir;
    // Replace all '/' with '_'
    std::replace(filename.begin(), filename.end(), '/', '_');
    // Append .pdf extension
    filename += ".pdf";
    return filename;
}
