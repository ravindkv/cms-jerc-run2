#include "TDRStyle.h"
#include "LoadEra2D.h"

template<typename T>
LoadEra2D<T>::LoadEra2D() : 
  channel_(""), 
  year_(""), 
  histDir_(""), 
  dataEraOrMcBin_(""),
  histName_(""),
  tdrStyle_(std::make_shared<TDRStyle>())
 {
}
// Clean up each cloned histogram
template<typename T>
LoadEra2D<T>::~LoadEra2D() {
}

template<typename T>
void LoadEra2D<T>::setInputJson(const nlohmann::json &inputJson) {
  inputJson_ = inputJson;
}

template<typename T>
void LoadEra2D<T>::setChannel(const std::string & channel) {
  channel_ = channel;
}

template<typename T>
void LoadEra2D<T>::setYear(const std::string & year) {
  year_ = year;
}

template<typename T>
void LoadEra2D<T>::setDataEraOrMcBin(const std::string & dataEraOrMcBin) {
  dataEraOrMcBin_ = dataEraOrMcBin;
}

template<typename T>
void LoadEra2D<T>::setHistDir(const std::string & histDir) {
  histDir_ = histDir;
}

template<typename T>
void LoadEra2D<T>::setHistName(const std::string & histName) {
  histName_ = histName;
}

template<typename T>
void LoadEra2D<T>::setFigConfig(const FigConfig & params) {
  tdrStyle_->setFigConfig(params);
}

// Load Data Histograms
template<typename T>
void LoadEra2D<T>::drawHist2D(TFile* outRootFile, const std::string &outPdfName) {

  std::string fileName;
  if (dataEraOrMcBin_.find("Data_")!= std::string::npos){
    fileName = inputJson_[channel_][year_]["Data"][dataEraOrMcBin_];
  }
  else if (dataEraOrMcBin_.find("Mc_")!= std::string::npos){
    fileName = inputJson_[channel_][year_]["Mc"][dataEraOrMcBin_];
  }
  else{
    std::cerr << "Error: check dataEraOrMcBin_ =  " << dataEraOrMcBin_ << std::endl;
    return;
  }
  
  std::string path = histDir_ + "/" + histName_;
  TFile file(fileName.c_str());
  if (file.IsZombie()) {
    std::cerr << "Error: Could not open file " << fileName << std::endl;
    return;
  }
  T* hist = (T*)file.Get(path.c_str());
  if (hist){
    tdrStyle_->setTDRStyle();
    if(tdrStyle_->getIsNorm() && hist->Integral()>0.0)
      hist->Scale(1/hist->Integral());
    //tdrStyle_->setStyle(hist);
    gROOT->cd();  // Change the directory to ROOT's global directory
    outRootFile->cd();
    TCanvas canvas("c", "Data and Mc Ratio", 600, 600);
    canvas.cd();
    gPad->SetRightMargin(0.12);
    if(tdrStyle_->getXLog())gPad->SetLogx(true);
    if(tdrStyle_->getYLog())gPad->SetLogy(true);

    TLegend *leg = new TLegend(0.25, 0.80, 0.90, 0.90, "", "brNDC");
    tdrStyle_->setStyle(leg);

    hist->SetTitle(dataEraOrMcBin_.c_str());
    hist->GetXaxis()->SetMoreLogLabels();
    hist->GetXaxis()->SetNoExponent();
    //hist->GetZaxis()->SetRangeUser(1e-4,1.0);
    hist->Draw("COLZ");
    //leg->AddEntry(hist, hist->GetName(), "L");
    //leg->Draw();

    canvas.Update();
    canvas.SaveAs(outPdfName.c_str());
    outRootFile->Write();
    file.Close();  // Now it's safe to close the file since the histogram has been cloned
  } 
  else {
    std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
    return;
  }
}


