#include "TDRStyle.h"
#include "LoadEraXY.h"

template<typename T>
LoadEraXY<T>::LoadEraXY() : 
  channel_(""), 
  year_(""), 
  dataEra_(""),
  varName_(""),
  histDir_(""), 
  histName_(""),
  varIsOnXaxis_(true),
  tdrStyle_(std::make_shared<TDRStyle>())
 {
  mcHtBins_.clear();
  varBins_.clear();
  dataHists_.clear();
  mcHists_.clear();
}

// Clean up each cloned histogram
template<typename T>
LoadEraXY<T>::~LoadEraXY() {
  for (auto hist : dataHists_) {
      delete hist; 
  }
  for (auto hist : mcHists_) {
      delete hist; 
  }
}

template<typename T>
void LoadEraXY<T>::setInputJson(const nlohmann::json &inputJson) {
  inputJson_ = inputJson;
}

template<typename T>
void LoadEraXY<T>::setChannel(const std::string & channel) {
  channel_ = channel;
}

template<typename T>
void LoadEraXY<T>::setYear(const std::string & year) {
  year_ = year;
}

template<typename T>
void LoadEraXY<T>::setMcHtBins(const std::vector<std::string>& htBins) {
  mcHtBins_ = htBins;
}

template<typename T>
void LoadEraXY<T>::setDataEra(const std::string& dataEra) {
  dataEra_ = dataEra;
}

template<typename T>
void LoadEraXY<T>::setHistDir(const std::string & histDir) {
  histDir_ = histDir;
}

template<typename T>
void LoadEraXY<T>::setHistName(const std::string & histName) {
  histName_ = histName;
}

template<typename T>
void LoadEraXY<T>::setFigConfig(const FigConfig & params) {
  tdrStyle_->setFigConfig(params);
  varBins_ = params.varBins;
  varName_ = params.varName;
  varIsOnXaxis_ = params.isVarOnX;
}

// Load Data Histograms
template<typename T>
void LoadEraXY<T>::loadDataHists() {

  std::string fileName = inputJson_[channel_][year_]["Data"][dataEra_];
  std::string path = histDir_ + "/" + histName_;

  TFile file(fileName.c_str());
  if (file.IsZombie()) {
    std::cerr << "Error: Could not open file " << fileName << std::endl;
    return;
  }

  T* hist = (T*)file.Get(path.c_str());
  if (hist) {
    gROOT->cd();  // Change the directory to ROOT's global directory
  	for (int i = 0; i < varBins_.size()-1; ++i) {
      TH1D* clonedHist;
      string nameBin = Form("%0.1f %s %0.1f", varBins_.at(i), varName_.c_str(), varBins_.at(i+1));
			if (varIsOnXaxis_){
        int startBin = hist->GetXaxis()->FindBin(varBins_.at(i));
        int endBin   = hist->GetXaxis()->FindBin(varBins_.at(i + 1));
				clonedHist = (TH1D*)hist->ProjectionX(nameBin.c_str(), startBin, endBin)->Clone(dataEra_.c_str()); 
			}
			else{
        int startBin = hist->GetYaxis()->FindBin(varBins_.at(i));
        int endBin   = hist->GetYaxis()->FindBin(varBins_.at(i + 1));
				clonedHist = (TH1D*)hist->ProjectionY(nameBin.c_str(), startBin, endBin)->Clone(dataEra_.c_str()); 
			}
      if(tdrStyle_->getIsNorm() && clonedHist->Integral()>0.0)
        clonedHist->Scale(1/clonedHist->Integral());
      tdrStyle_->setStyle(clonedHist);
      dataHists_.push_back(clonedHist);
		}
  } else {
    std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
  }
  file.Close();  // Now it's safe to close the file since the histogram has been cloned
  
}

// Load Mc Histograms
template<typename T>
void LoadEraXY<T>::loadMcHists(){
  for (int i = 0; i < varBins_.size()-1; ++i) {
    std::vector<TH1D*> mcHistsPerBin;
    for (const auto& htBin : mcHtBins_) {
      std::string fileName = inputJson_[channel_][year_]["Mc"][htBin];
      std::string path = histDir_ + "/" + histName_;
      TFile file(fileName.c_str());
      if (file.IsZombie()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
      }
      T* hist = (T*)file.Get(path.c_str());
      if (hist) {
        gROOT->cd();  // Change the directory to ROOT's global directory
        TH1D* clonedHist;
        string nameBin = Form("%0.1f %s %0.1f", varBins_.at(i), varName_.c_str(), varBins_.at(i+1));
      	if (varIsOnXaxis_){
          int startBin = hist->GetXaxis()->FindBin(varBins_.at(i));
          int endBin   = hist->GetXaxis()->FindBin(varBins_.at(i + 1));
      		clonedHist = (TH1D*)hist->ProjectionX(nameBin.c_str(), startBin, endBin)->Clone(htBin.c_str()); 
      	}
      	else{
          int startBin = hist->GetYaxis()->FindBin(varBins_.at(i));
          int endBin   = hist->GetYaxis()->FindBin(varBins_.at(i + 1));
      		clonedHist = (TH1D*)hist->ProjectionY(nameBin.c_str(), startBin, endBin)->Clone(htBin.c_str()); 
      	}
      if(tdrStyle_->getIsNorm() && clonedHist->Integral()>0.0)
        clonedHist->Scale(1/clonedHist->Integral());
      tdrStyle_->setStyle(clonedHist);
        mcHistsPerBin.push_back(clonedHist);
      }//hist
      else {
        std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
      }
      file.Close();  // Now it's safe to close the file since the histogram has been cloned
    }//mcHtBins
  mcHists_.push_back(combineHists(mcHistsPerBin));
  }//varBins
}

// Helper function to draw histograms (Data/Mc), set styles, and handle the legend
template<typename T>
void LoadEraXY<T>::drawHists(const std::vector<TH1D*>& hists) {
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
void LoadEraXY<T>::overlayDataWithMcInRatio(TFile* outRootFile, const std::string &outputFile) {
  outRootFile->cd();
  TCanvas canvas("c", "Data and Mc Ratio", 600, 600);
  canvas.cd();
  tdrStyle_->setTDRStyle();

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
    TH1D* mergedMcHist = combineHists(mcHists_);
    if (!mergedMcHist) {
      std::cerr << "Error: Could not create merged Mc histogram." << std::endl;
      return;
    }

    std::vector<TGraphErrors*> ratioGraphs;
    for (auto dataHist : dataHists_) {
      TGraphErrors* ratioGraph = new TGraphErrors(dataHist->GetNbinsX());
      calculateHistRatio(dataHist, mergedMcHist, ratioGraph);
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
  outRootFile->Write();
}

// Combine multiple histograms
template<typename T>
TH1D* LoadEraXY<T>::combineHists(const std::vector<TH1D*>& hists) {
  TH1D* combinedHist = nullptr;
  for (const auto &entry : hists) {
    if (entry->GetEntries() == 0) {
      std::cerr << "Warning: A histogram in the combine function has no entries." << std::endl;
      continue;
    }
    if (!combinedHist) {
      combinedHist = (TH1D*)entry->Clone();
    } else {
      combinedHist->Add(entry);
    }
  }
  return combinedHist;
}

// Function to calculate ratio and fill a TGraphErrors
template<typename T>
void LoadEraXY<T>::calculateHistRatio(TH1D* dataHist, TH1D* mcHist, TGraphErrors* ratioGraph) {
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
