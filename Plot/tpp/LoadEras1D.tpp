#include "TDRStyle.h"
#include "LoadEras1D.h"

template<typename T>
LoadEras1D<T>::LoadEras1D() : 
  channel_(""), 
  year_(""), 
  histDir_(""), 
  histName_("")
 {
  mcHTBins_.clear();
  dataEras_.clear();
  dataHists_.clear();
  mcHists_.clear();
}

// Clean up each cloned histogram
template<typename T>
LoadEras1D<T>::~LoadEras1D() {
  for (auto hist : dataHists_) {
      delete hist; 
  }
  for (auto hist : mcHists_) {
      delete hist; 
  }
}

template<typename T>
void LoadEras1D<T>::setInputJson(const nlohmann::json &inputJson) {
  inputJson_ = inputJson;
}

template<typename T>
void LoadEras1D<T>::setChannel(const std::string & channel) {
  channel_ = channel;
}

template<typename T>
void LoadEras1D<T>::setYear(const std::string & year) {
  year_ = year;
}

template<typename T>
void LoadEras1D<T>::setMCHTBins(const std::vector<std::string>& htBins) {
  mcHTBins_ = htBins;
}

template<typename T>
void LoadEras1D<T>::setDataEras(const std::vector<std::string>& dataEras) {
  dataEras_ = dataEras;
}

template<typename T>
void LoadEras1D<T>::setHistDir(const std::string & histDir) {
  histDir_ = histDir;
}

template<typename T>
void LoadEras1D<T>::setHistName(const std::string & histName) {
  histName_ = histName;
}

// Load Data Histograms
template<typename T>
void LoadEras1D<T>::loadDataHists() {
  for (const auto& era : dataEras_) {
    std::string fileName = inputJson_[channel_][year_]["Data"][era];
    std::string path = histDir_ + "/" + histName_;

    TFile file(fileName.c_str());
    if (file.IsZombie()) {
      std::cerr << "Error: Could not open file " << fileName << std::endl;
      return;
    }
    T* hist = (T*)file.Get(path.c_str());
    if (hist) {
      gROOT->cd();  // Change the directory to ROOT's global directory
      T* clonedHist = (T*)hist->Clone(era.c_str());  // Clone the histogram
      dataHists_.push_back(clonedHist);
    } else {
      std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
    }
    file.Close();  // Now it's safe to close the file since the histogram has been cloned
  }
}

// Load MC Histograms
template<typename T>
void LoadEras1D<T>::loadMCHists() {
  for (const auto& htBin : mcHTBins_) {
    std::string fileName = inputJson_[channel_][year_]["MC"][htBin];
    std::string path = histDir_ + "/" + histName_;

    TFile file(fileName.c_str());
    if (file.IsZombie()) {
      std::cerr << "Error: Could not open file " << fileName << std::endl;
      return;
    }
    T* hist = (T*)file.Get(path.c_str());
    if (hist) {
      gROOT->cd();  // Change the directory to ROOT's global directory
      T* clonedHist = (T*)hist->Clone(htBin.c_str());  // Clone the histogram
      mcHists_.push_back(clonedHist);
    } else {
      std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
    }
    file.Close();  // Now it's safe to close the file since the histogram has been cloned
  }
}

// Helper function to draw histograms (Data/MC), set styles, and handle the legend
template<typename T>
void LoadEras1D<T>::drawHists(TDRStyle &tdrS, const std::vector<T*>& hists) {
  if (hists.empty()) {
    std::cerr << "Error: Histograms vector is empty." << std::endl;
    return;
  }

  double legYmin = 0.85 - 0.05*hists.size();
  TLegend *leg = new TLegend(0.25, legYmin, 0.90, 0.90, "", "brNDC");
  tdrS.setStyle(leg);

  for (size_t i = 0; i < hists.size(); i++) {
    auto hist = hists.at(i);
    if (hist != nullptr) {
      tdrS.setStyle(hist, 0.0, 1.5);
      tdrS.setColor(hist, i);
      hist->Draw(i == 0 ? "Pz" : "Pz SAME");
      leg->AddEntry(hist, hist->GetName(), "L");
    } else {
      std::cerr << "Error: Histogram is nullptr." << std::endl;
      return;
    }
  }

  leg->Draw();
}

// Overlay Data with MC and Plot Ratio
template<typename T>
void LoadEras1D<T>::overlayDataWithMCInRatio(TFile* outRootFile, const std::string &outputFile) {
  outRootFile->cd();
  TCanvas canvas("c", "Data and MC Ratio", 600, 600);
  canvas.cd();
  TDRStyle tdrS;
  tdrS.setTDRStyle();

  if (dataHists_.size() > 0 && mcHists_.size() == 0) {
    drawHists(tdrS, dataHists_);
  }
  
  if (mcHists_.size() > 0 && dataHists_.size() == 0) {
    drawHists(tdrS, mcHists_);
  }
  
  if (mcHists_.size() > 0 && dataHists_.size() > 0) {
    TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.3, 1.0, 1.0);
    pad1->SetBottomMargin(0.00);
    pad1->Draw();
    pad1->cd();

    drawHists(tdrS, dataHists_);
    canvas.cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.3);
    pad2->SetTopMargin(0.0);
    pad2->SetBottomMargin(0.4);
    pad2->Draw();
    pad2->cd();

    T* mergedMCHist = combineHists(mcHists_);
    if (!mergedMCHist) {
      std::cerr << "Error: Could not create merged MC histogram." << std::endl;
      return;
    }

    std::vector<TGraphErrors*> ratioGraphs;
    for (auto dataHist : dataHists_) {
      TGraphErrors* ratioGraph = new TGraphErrors(dataHist->GetNbinsX());
      calculateHistRatio(dataHist, mergedMCHist, ratioGraph);
      ratioGraphs.push_back(ratioGraph);
    }

    for (int i = 0; i < dataHists_.size(); i++) {
      auto rGraph = ratioGraphs.at(i);
      tdrS.setStyleRatio(rGraph, 0.9, 1.1);
      tdrS.setColor(rGraph, i);
      rGraph->Draw(i == 0 ? "APz" : "Pz SAME");
    }
  }

  canvas.Update();
  canvas.SaveAs(outputFile.c_str());
  outRootFile->Write();
}

// Combine multiple histograms
template<typename T>
T* LoadEras1D<T>::combineHists(const std::vector<T*>& hists) {
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

// Function to calculate ratio and fill a TGraphErrors
template<typename T>
void LoadEras1D<T>::calculateHistRatio(T* dataHist, T* mcHist, TGraphErrors* ratioGraph) {
  int nBins = dataHist->GetNbinsX();
  for (int i = 1; i <= nBins; ++i) {
    double dataVal = dataHist->GetBinContent(i);
    double mcVal = mcHist->GetBinContent(i);
    double ratioVal = 0.0;
    double ratioErr = 0.0;
    if(mcVal>0 && dataVal >0){
      ratioVal = dataVal / mcVal;
      ratioErr = sqrt(pow(dataHist->GetBinError(i) / mcVal, 2) +
                           pow(dataVal * mcHist->GetBinError(i) / pow(mcVal, 2), 2));
    }
    ratioGraph->SetPoint(i - 1, dataHist->GetXaxis()->GetBinCenter(i), ratioVal);
    ratioGraph->SetPointError(i - 1, 0.0, ratioErr);
  }
}

