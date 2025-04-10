#include "PlotTime1Ds.h"
#include "Helper.h"

template<typename T>
PlotTime1Ds<T>::PlotTime1Ds() : 
  channel_(), 
  year_(), 
  histDir_(""), 
  yMin_(0.0),
  yMax_(2.0),
  tdrStyle_(std::make_shared<TdrStyle>())
{
    histNames_.clear(),
    dataHists_.clear();
}

// Clean up each cloned histogram
template<typename T>
PlotTime1Ds<T>::~PlotTime1Ds() {
    for(auto &histMap : dataHists_) {
        for(auto &yearMap : histMap.second) {
            for(auto hist : yearMap.second) {
                delete hist;
            }
        }
    }
}

template<typename T>
void PlotTime1Ds<T>::setInput(const nlohmann::json &inputJson, const std::string & channel, const std::string& year){
    inputJson_ = inputJson;
    channel_ = channel;
    year_ = year;
}

template<typename T>
void PlotTime1Ds<T>::setFigConfigTime1Ds(const FigConfigTime1Ds & params) {
    tdrStyle_->setFigConfig(params);
    histDir_  = params.histDir;
    trigDirs_ = params.trigDirs;
    histNames_ = params.histNames;
    yMin_ = params.yMin;
    yMax_ = params.yMax;
}

template<typename T>
void PlotTime1Ds<T>::loadHists(const std::string& sourceType) {
    for (const auto& histName : histNames_) {
        auto eras = Helper::getEras(inputJson_, channel_, year_, sourceType);
        for (const auto& era : eras) {
            // Access the file name from JSON
            if(!inputJson_.contains(channel_) || !inputJson_[channel_].contains(year_)) {
                std::cerr << "Error: Missing channel_ or year_ in JSON: " << channel_ << ", " << year_ << std::endl;
                continue;
            }
            
            std::string fileName;
            try {
                fileName = inputJson_[channel_][year_][sourceType][era];
            }
            catch (nlohmann::json::exception& e) {
                std::cerr << "Error accessing JSON for " << channel_ << ", " << year_ << ", " << sourceType << ", " << era << ", " << histName << ": " << e.what() << std::endl;
                continue;
            }

            // Construct the histogram path
            std::string path = (histDir_.empty()) ? histName : histDir_ + "/" + histName;

            // Open the ROOT file
            TFile file(fileName.c_str());
            if (file.IsZombie()) {
                std::cerr << "Error: Could not open file " << fileName << std::endl;
                continue;
            }
            // Retrieve the histogram
            T* hist = nullptr;
            if(!trigDirs_.empty()){
                hist = Helper::sumHistsFromTrigDirs<T>(file, histDir_, trigDirs_, histName);
            } else{
                hist = static_cast<T*>(file.Get(path.c_str()));
            }
            if (!hist) {
                std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
                file.Close();
                continue;
            }

            gROOT->cd();  // Change to ROOT's global directory
            T* clonedHist = (T*)hist->Clone((histName + "_" + era).c_str());  // Clone the histogram

            // Normalize if needed
            if (tdrStyle_->getIsNorm() && clonedHist->Integral() > 0.0)
                clonedHist->Scale(1 / clonedHist->Integral());

            // Apply style settings
            tdrStyle_->setStyle(clonedHist);

            // Store the histograms in the appropriate map
            dataHists_[histName][year_].push_back(clonedHist);

            // Close the file after cloning the histogram
            file.Close();
            
        }
    }
}

// Helper function to draw histograms (Data/Mc), set styles, and handle the legend
template<typename T>
void PlotTime1Ds<T>::drawHists() {
    if (histNames_.empty()) {
        std::cerr << "Error: No histogram names specified." << std::endl;
        return;
    }

    double legYmin = 0.90 - 0.025*histNames_.size();
    TLegend *leg = new TLegend(0.15, legYmin, 0.95, 0.90, "", "brNDC");
    tdrStyle_->setStyle(leg);
    leg->SetNColumns(2);
    if(tdrStyle_->getXLog())gPad->SetLogx(true);
    if(tdrStyle_->getYLog())gPad->SetLogy(true);

    // Get the EraName to RunNumber map
    std::vector<std::pair<std::string, int>> eraRunMap;
    
    for (size_t i = 0; i < histNames_.size(); ++i) {
        const auto& histName = histNames_[i];
        // Aggregate all histograms of this type across  all  eras
        std::vector<T*> histsToDraw;
        if(dataHists_.find(histName) != dataHists_.end() && dataHists_[histName].find(year_) != dataHists_[histName].end()) {
            for(auto hist : dataHists_[histName][year_]) {
                histsToDraw.push_back(hist);
            }
        }
        

        if(histsToDraw.empty()) {
            std::cerr << "Warning: No histograms found for " << histName << std::endl;
            continue;
        }

        eraRunMap = getEraRunMap(histsToDraw);
        int firstRun = -1;
        int lastRun = -1;
        // Check if the vector is not empty
        if (!eraRunMap.empty()) {
            firstRun = eraRunMap.front().second; // First Run number 
            lastRun = eraRunMap.back().second;   // Last Run number
        } else {
            std::cout << "The eraRunMap vector is empty.\n";
        }

        for(size_t j = 0; j < histsToDraw.size(); ++j){
            auto hist = histsToDraw[j];
            tdrStyle_->setColor(hist, i);
            if(j == 0){
                leg->AddEntry(hist, histName.c_str(), "EP");
            }
            // Draw the first histogram with option, then overlay the rest
            if(j == 0 && i == 0){
                hist->Draw("Pz");
                if(firstRun != -1 &&  lastRun != -1 )
                hist->GetXaxis()->SetRangeUser(firstRun, lastRun);
            }
            else{
                hist->Draw("Pz SAME");
            }
            // Add to legend
        }
        // Draw vertical lines for era breaks and add labels
        if(i==0){
            // Iterate over the map in ascending order of run numbers
            for (const auto& [era, run] : eraRunMap) {
                std::cout << "Run Number: " << run << " | Era Name: " << era << std::endl;
                TLine *line = new TLine(run, yMin_, run, yMax_);
                line->SetLineStyle(2);
                line->SetLineColor(kBlack);
                line->Draw();
                
                TLatex *label = new TLatex(run-1, 0.4*(yMin_ + yMax_), era.c_str());
                label->SetTextAngle(90);
                label->SetTextSize(0.03);
                label->Draw();
            }
        }//i==0
    }// for i

    tdrStyle_->drawTitle();
    leg->Draw();
}

template<typename T>
void PlotTime1Ds<T>::overlayData(const std::string &outPdfName) {
    tdrStyle_->setTdrStyle();
    TCanvas canvas("c", "Energy Fractions over Time", 800, 600);
    canvas.cd();

    drawHists();

    canvas.Update();
    canvas.SaveAs(outPdfName.c_str());
}

// Template function to generate a sorted vector of EraName and RunNumber
template <typename T>
std::vector<std::pair<std::string, int>> PlotTime1Ds<T> ::getEraRunMap(const std::vector<T*>& histsToDraw) {
    std::vector<std::pair<std::string, int>> eraRunVector;

    for (const auto& hist : histsToDraw) {
        if (!hist) {
            std::cerr << "Warning: Encountered a null histogram pointer." << std::endl;
            continue;
        }

        // Extract histogram name
        std::string histName = hist->GetName();

        // Split the histogram name by underscores
        std::vector<std::string> tokens = Helper::splitString(histName, '_');

        if (tokens.size() < 3) {
            std::cerr << "Warning: Histogram name '" << histName 
                      << "' does not contain enough parts to extract era name." << std::endl;
            continue;
        }

        // Extract era name (second element, index 3)
        std::string eraName = tokens[3]; // e.g., "2017B"

        // Determine the run number by finding the rightmost non-zero x-bin
        int nbins = hist->GetNbinsX();
        int runNumber = -1; // Initialize with an invalid run number

        for (int bin = nbins; bin >= 1; --bin) {
            double content = hist->GetBinContent(bin);
            if (content != 0) {
                // Retrieve the x-axis value (run number) of this bin
                runNumber = static_cast<int>(hist->GetBinCenter(bin));
                break;
            }
        }

        if (runNumber == -1) {
            std::cerr << "Warning: Histogram '" << histName 
                      << "' has all zero bins. Unable to determine run number." << std::endl;
            continue;
        }

        // Add to the vector
        eraRunVector.emplace_back(std::make_pair(eraName, runNumber));
    }

    // Sort the vector based on RunNumber in ascending order
    std::sort(eraRunVector.begin(), eraRunVector.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) -> bool {
                  return a.second < b.second;
              });

    return eraRunVector;
}
