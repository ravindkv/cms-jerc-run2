#include "TdrStyle.h"
#include "CoreEraXY.h"

template<typename T>
CoreEraXY<T>::CoreEraXY() : 
  channel_(""), 
  year_(""), 
  varName_(""),
  histDir_(""), 
  histName_(""),
  varIsOnXaxis_(true),
  tdrStyle_(std::make_shared<TdrStyle>())
 {
  varBins_.clear();
  dataHists_.clear();
  mcHists_.clear();
}

// Clean up each cloned histogram
template<typename T>
CoreEraXY<T>::~CoreEraXY() {
  for (auto hist : dataHists_) {
      delete hist; 
  }
  for (auto hist : mcHists_) {
      delete hist; 
  }
}

template<typename T>
void CoreEraXY<T>::setInput(const nlohmann::json &inputJson, const std::string & channel, const std::string & year){
  inputJson_ = inputJson;
  channel_ = channel;
  year_ = year;
}

template<typename T>
void CoreEraXY<T>::setFigConfigEraXY(const FigConfigEraXY & params) {
  tdrStyle_->setFigConfig(params);
  histDir_  = params.histDir;
  histName_ = params.histName;
  varBins_ = params.varBins;
  varName_ = params.varName;
  varIsOnXaxis_ = params.isVarOnX;
}

template<typename T>
void CoreEraXY<T>::loadHists(const std::string& sourceType, const std::string& dataEra, const std::vector<std::string>& mcHtBins) {
    // Determine which bins to use based on sourceType (Data or MC)
    std::vector<std::string> bins;

    if (sourceType == "Data") {
        bins.push_back(dataEra);  // For Data, use the single dataEra
    } else if (sourceType == "MC") {
        bins = mcHtBins;  // For MC, use the list of mcHtBins
    }

    for (const auto& bin : bins) {
        std::string fileName = (sourceType == "Data") ? inputJson_[channel_][year_]["Data"][bin]
                                                      : inputJson_[channel_][year_]["MC"][bin];

        std::string path = (histDir_.empty()) ? histName_ : histDir_ + "/" + histName_;
        TFile file(fileName.c_str());
        if (file.IsZombie()) {
            std::cerr << "Error: Could not open file " << fileName << std::endl;
            continue;
        }

        T* hist = (T*)file.Get(path.c_str());
        if (!hist) {
            std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
            file.Close();
            continue;
        }

        gROOT->cd();  // Change to ROOT's global directory

        std::vector<TProfile*> histsPerBin;

        for (int i = 0; i < varBins_.size() - 1; ++i) {
            TProfile* clonedHist = projectAndClone(hist, bin, i);
            if (clonedHist) {
                if (tdrStyle_->getIsNorm() && clonedHist->Integral() > 0.0)
                    clonedHist->Scale(1 / clonedHist->Integral());

                tdrStyle_->setStyle(clonedHist);
                histsPerBin.push_back(clonedHist);
            }
        }

        // Store histograms for MC differently than for Data
        if (sourceType == "MC") {
            mcHists_.push_back(Helper::combineHists(histsPerBin));
        } else if (sourceType == "Data") {
            dataHists_.insert(dataHists_.end(), histsPerBin.begin(), histsPerBin.end());
        }

        file.Close();
    }
}


// Helper function for projecting and cloning histograms
template<typename T>
TProfile* CoreEraXY<T>::projectAndClone(T* hist, const std::string& bin, int i) {
    std::string nameBin;
    TProfile* clonedHist = nullptr;

    if (varIsOnXaxis_) {
        auto binIdx = hist->GetYaxis()->FindBin(varBins_.at(i));
        auto binLow = hist->GetYaxis()->GetBinLowEdge(binIdx);
        auto binHigh = hist->GetYaxis()->GetBinUpEdge(binIdx);
        nameBin = Form("%0.1f %s %0.1f", binLow, varName_.c_str(), binHigh);
        clonedHist = (TProfile*)hist->ProjectionX(nameBin.c_str(), binIdx, binIdx)->Clone((nameBin + ": " + bin).c_str());
    } else {
        auto binIdx = hist->GetXaxis()->FindBin(varBins_.at(i));
        auto binLow = hist->GetXaxis()->GetBinLowEdge(binIdx);
        auto binHigh = hist->GetXaxis()->GetBinUpEdge(binIdx);
        nameBin = Form("%0.1f %s %0.1f", binLow, varName_.c_str(), binHigh);
        clonedHist = (TProfile*)hist->ProjectionY(nameBin.c_str(), binIdx, binIdx)->Clone((nameBin + ": " + bin).c_str());
    }

    return clonedHist;
}


// Helper function to draw histograms (Data/Mc), set styles, and handle the legend
template<typename T>
void CoreEraXY<T>::drawHists(const std::vector<TProfile*>& hists) {
  if (hists.empty()) {
    std::cerr << "Error: Histograms vector is empty." << std::endl;
    return;
  }

  double legYmin = 0.85 - 0.05*hists.size();
  TLegend *leg = new TLegend(0.25, legYmin, 0.90, 0.90, "", "brNDC");
  tdrStyle_->setStyle(leg);
  if(tdrStyle_->getXLog())gPad->SetLogx(true);
  if(tdrStyle_->getYLog())gPad->SetLogy(true);

  for (size_t i = 0; i < hists.size(); i++) {
    auto hist = hists.at(i);
    if (hist != nullptr) {
      tdrStyle_->setColor(hist, i);
      hist->Draw(i == 0 ? "Pz" : "Pz SAME");
      leg->AddEntry(hist, hist->GetName(), "L");
    } else {
      std::cerr << "Error: Histogram is nullptr." << std::endl;
      return;
    }
  }

  leg->Draw();
}

// Overlay Data with Mc and Plot Ratio
template<typename T>
void CoreEraXY<T>::overlayDataWithMcInRatio(const std::string &outputFile) {
  TCanvas canvas("c", "Data and Mc Ratio", 600, 600);
  canvas.cd();
  tdrStyle_->setTdrStyle();

  if (dataHists_.size() > 0 && mcHists_.size() == 0) {
    drawHists(dataHists_);
  }
  
  if (mcHists_.size() > 0 && dataHists_.size() == 0) {
    drawHists(mcHists_);
  }
  
  if (mcHists_.size() > 0 && dataHists_.size() > 0) {
    TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.3, 1.0, 1.0);
    pad1->SetBottomMargin(0.00);
    pad1->Draw();
    pad1->cd();

    drawHists(dataHists_);
    canvas.cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.3);
    pad2->SetTopMargin(0.0);
    pad2->SetBottomMargin(0.4);
    pad2->Draw();
    pad2->cd();

    if(tdrStyle_->getXLog())gPad->SetLogx(true);
    TProfile* mergedMcHist = Helper::combineHists(mcHists_);
    if (!mergedMcHist) {
      std::cerr << "Error: Could not create merged Mc histogram." << std::endl;
      return;
    }

    std::vector<TGraphErrors*> ratioGraphs;
    for (auto dataHist : dataHists_) {
      TGraphErrors* ratioGraph = new TGraphErrors(dataHist->GetNbinsX());
      Helper::calculateHistRatio(dataHist, mergedMcHist, ratioGraph);
      ratioGraphs.push_back(ratioGraph);
    }

    for (int i = 0; i < dataHists_.size(); i++) {
      auto rGraph = ratioGraphs.at(i);
      tdrStyle_->setStyleRatio(rGraph);
      tdrStyle_->setColor(rGraph, i);
      rGraph->Draw(i == 0 ? "APz" : "Pz SAME");
    }
  }

  canvas.Update();
  canvas.SaveAs(outputFile.c_str());
}

