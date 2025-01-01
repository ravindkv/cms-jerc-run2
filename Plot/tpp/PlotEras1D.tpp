#include "PlotEras1D.h"
#include "Helper.h"


template<typename T>
PlotEras1D<T>::PlotEras1D() : 
  channel_(""), 
  year_(""), 
  histDir_(""), 
  histName_(""),
  tdrStyle_(std::make_shared<TdrStyle>())
 {
  dataHists_.clear();
  mcHists_.clear();
}

// Clean up each cloned histogram
template<typename T>
PlotEras1D<T>::~PlotEras1D() {
  for (auto hist : dataHists_) {
      delete hist; 
  }
  for (auto hist : mcHists_) {
      delete hist; 
  }
}

template<typename T>
void PlotEras1D<T>::setInput(const nlohmann::json &inputJson, const std::string & channel, const std::string & year){
  inputJson_ = inputJson;
  channel_ = channel;
  year_ = year;
}

template<typename T>
void PlotEras1D<T>::setFigConfigEras1D(const FigConfigEras1D & params) {
  tdrStyle_->setFigConfig(params);
  histDir_  = params.histDir;
  histName_ = params.histName;
}

// Generic load function for both Data and MC
template<typename T>
void PlotEras1D<T>::loadHists(const std::string& sourceType, const std::vector<std::string>& bins) {
    for (const auto& bin : bins) {
        // Select the file name based on sourceType
        std::string fileName = (sourceType == "Data") ? inputJson_[channel_][year_]["Data"][bin]
                                                      : inputJson_[channel_][year_]["MC"][bin];

        // Construct the histogram path
        std::string path = (histDir_.empty()) ? histName_ : histDir_ + "/" + histName_;

        // Open the ROOT file
        TFile file(fileName.c_str());
        if (file.IsZombie()) {
            std::cerr << "Error: Could not open file " << fileName << std::endl;
            continue;
        }

        // Retrieve the histogram
        T* hist = (T*)file.Get(path.c_str());
        if (!hist) {
            std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
            file.Close();
            continue;
        }

        gROOT->cd();  // Change to ROOT's global directory
        T* clonedHist = (T*)hist->Clone(bin.c_str());  // Clone the histogram

        // Apply style settings
        tdrStyle_->setStyle(clonedHist);

        // Store the histograms in the appropriate vector
        if (sourceType == "Data") {
            dataHists_.push_back(clonedHist);
        } else if (sourceType == "MC") {
            mcHists_.push_back(clonedHist);
        }

        // Close the file after cloning the histogram
        file.Close();
    }
}

// Helper function to draw histograms (Data/Mc), set styles, and handle the legend
template<typename T>
void PlotEras1D<T>::drawHists(const std::vector<T*>& hists) {
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
    // Clone the histogram and cast it back to type T*
    T* hist = static_cast<T*>(hists.at(i)->Clone(hists.at(i)->GetName()));
    if (hist != nullptr) {
      // Normalize 
      if (tdrStyle_->getIsNorm() && hist->Integral() > 0.0){
        hist->Scale(1./hist->Integral());
      }
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
void PlotEras1D<T>::overlayDataWithMcInRatio(const std::string &outPdfName) {
  tdrStyle_->setTdrStyle();
  TCanvas canvas("c", "Data and Mc Ratio", 600, 600);
  canvas.cd();

  if (!dataHists_.empty() && mcHists_.empty()) {
    drawHists(dataHists_);
  }
  
  if (!mcHists_.empty() && dataHists_.empty()) {
    drawHists( mcHists_);
  }
  
  if (!mcHists_.empty() && !dataHists_.empty()) {
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

    T* mergedMcHist = Helper::combineHists(mcHists_);
    if (!mergedMcHist) {
      std::cerr << "Error: Could not create merged Mc histogram." << std::endl;
      return;
    }

    std::vector<TGraphErrors*> ratioGraphs;
    for (auto dataHist : dataHists_) {
      // Normalize 
      if (tdrStyle_->getIsNorm() && dataHist->Integral() > 0.0){
        dataHist->Scale(1./dataHist->Integral());
      }
      if (tdrStyle_->getIsNorm() && mergedMcHist->Integral() > 0.0){
        mergedMcHist->Scale(1./mergedMcHist->Integral());
      }
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
  canvas.SaveAs(outPdfName.c_str());
}

