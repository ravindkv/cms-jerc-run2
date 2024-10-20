#include "CoreEras1D.h"
#include "Helper.h"


template<typename T>
CoreEras1D<T>::CoreEras1D() : 
  channel_(""), 
  year_(""), 
  histDir_(""), 
  histName_(""),
  tdrStyle_(std::make_shared<TdrStyle>())
 {
  mcHtBins_.clear();
  dataEras_.clear();
  dataHists_.clear();
  mcHists_.clear();
}

// Clean up each cloned histogram
template<typename T>
CoreEras1D<T>::~CoreEras1D() {
  for (auto hist : dataHists_) {
      delete hist; 
  }
  for (auto hist : mcHists_) {
      delete hist; 
  }
}

template<typename T>
void CoreEras1D<T>::setInputJson(const nlohmann::json &inputJson) {
  inputJson_ = inputJson;
}

template<typename T>
void CoreEras1D<T>::setChannel(const std::string & channel) {
  channel_ = channel;
}

template<typename T>
void CoreEras1D<T>::setYear(const std::string & year) {
  year_ = year;
}

template<typename T>
void CoreEras1D<T>::setMcHtBins(const std::vector<std::string>& htBins) {
  mcHtBins_ = htBins;
}

template<typename T>
void CoreEras1D<T>::setDataEras(const std::vector<std::string>& dataEras) {
  dataEras_ = dataEras;
}

template<typename T>
void CoreEras1D<T>::setFigConfigEras1D(const FigConfigEras1D & params) {
  tdrStyle_->setFigConfig(params);
  histDir_  = params.histDir;
  histName_ = params.histName;
}

// Load Data Histograms
template<typename T>
void CoreEras1D<T>::loadDataHists() {
  for (const auto& era : dataEras_) {
    std::string fileName = inputJson_[channel_][year_]["Data"][era];
    std::string path = histDir_ + "/" + histName_;
    if(histDir_=="") path = histDir_ + histName_;

    TFile file(fileName.c_str());
    if (file.IsZombie()) {
      std::cerr << "Error: Could not open file " << fileName << std::endl;
      return;
    }
    T* hist = (T*)file.Get(path.c_str());
    if (hist) {
      gROOT->cd();  // Change the directory to ROOT's global directory
      T* clonedHist = (T*)hist->Clone(era.c_str());  // Clone the histogram
      if(tdrStyle_->getIsNorm() && clonedHist->Integral()>0.0)
        clonedHist->Scale(1/clonedHist->Integral());
      tdrStyle_->setStyle(clonedHist);
      dataHists_.push_back(clonedHist);
    } else {
      std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
    }
    file.Close();  // Now it's safe to close the file since the histogram has been cloned
  }
}

// Load Mc Histograms
template<typename T>
void CoreEras1D<T>::loadMcHists() {
  for (const auto& htBin : mcHtBins_) {
    std::string fileName = inputJson_[channel_][year_]["MC"][htBin];
    std::string path = histDir_ + "/" + histName_;
    if(histDir_=="") path = histDir_ + histName_;

    TFile file(fileName.c_str());
    if (file.IsZombie()) {
      std::cerr << "Error: Could not open file " << fileName << std::endl;
      return;
    }
    T* hist = (T*)file.Get(path.c_str());
    if (hist) {
      gROOT->cd();  // Change the directory to ROOT's global directory
      T* clonedHist = (T*)hist->Clone(htBin.c_str());  // Clone the histogram
      if(tdrStyle_->getIsNorm() && clonedHist->Integral()>0.0)
        clonedHist->Scale(1/clonedHist->Integral());
      tdrStyle_->setStyle(clonedHist);
      mcHists_.push_back(clonedHist);
    } else {
      std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
    }
    file.Close();  // Now it's safe to close the file since the histogram has been cloned
  }
}

// Helper function to draw histograms (Data/Mc), set styles, and handle the legend
template<typename T>
void CoreEras1D<T>::drawHists(const std::vector<T*>& hists) {
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
void CoreEras1D<T>::overlayDataWithMcInRatio(const std::string &outPdfName) {
  TCanvas canvas("c", "Data and Mc Ratio", 600, 600);
  canvas.cd();
  tdrStyle_->setTdrStyle();

  if (dataHists_.size() > 0 && mcHists_.size() == 0) {
    drawHists(dataHists_);
  }
  
  if (mcHists_.size() > 0 && dataHists_.size() == 0) {
    drawHists( mcHists_);
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

    T* mergedMcHist = Helper::combineHists(mcHists_);
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
  canvas.SaveAs(outPdfName.c_str());
}

