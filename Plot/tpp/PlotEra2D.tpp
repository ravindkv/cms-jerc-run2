#include "TdrStyle.h"
#include "PlotEra2D.h"

template<typename T>
PlotEra2D<T>::PlotEra2D() : 
  channel_(""), 
  year_(""), 
  histDir_(""), 
  dataEraOrMcBin_(""),
  histName_(""),
  tdrStyle_(std::make_shared<TdrStyle>())
 {
}
// Clean up each cloned histogram
template<typename T>
PlotEra2D<T>::~PlotEra2D() {
}

template<typename T>
void PlotEra2D<T>::setInput(const nlohmann::json &inputJson, const std::string & channel, const std::string & year){
  inputJson_ = inputJson;
  channel_ = channel;
  year_ = year;
}

template<typename T>
void PlotEra2D<T>::setDataEraOrMcBin(const std::string & dataEraOrMcBin) {
  dataEraOrMcBin_ = dataEraOrMcBin;
}


template<typename T>
void PlotEra2D<T>::setFigConfigEra2D(const FigConfigEra2D & params) {
  tdrStyle_->setFigConfig(params);
  histDir_  = params.histDir;
  histName_ = params.histName;
}

// Load Data Histograms
template<typename T>
void PlotEra2D<T>::drawHist2D(const std::string &outPdfName) {

  std::string fileName;
  if (dataEraOrMcBin_.find("Data_")!= std::string::npos){
    fileName = inputJson_[channel_][year_]["Data"][dataEraOrMcBin_];
  }
  else if (dataEraOrMcBin_.find("MC_")!= std::string::npos){
    fileName = inputJson_[channel_][year_]["MC"][dataEraOrMcBin_];
  }
  else{
    std::cerr << "Error: check dataEraOrMcBin_ =  " << dataEraOrMcBin_ << std::endl;
    return;
  }
  
  std::string path = histDir_ + "/" + histName_;
  if(histDir_=="") path = histDir_ + histName_;
  TFile file(fileName.c_str());
  if (file.IsZombie()) {
    std::cerr << "Error: Could not open file " << fileName << std::endl;
    return;
  }
  T* hist = (T*)file.Get(path.c_str());
  if (hist){
    tdrStyle_->setTdrStyle();
    tdrStyle_->setStyle(hist);
    gROOT->cd();  // Change the directory to ROOT's global directory
    TCanvas canvas("c", "Data and Mc Ratio", 600, 600);
    canvas.cd();
    gPad->SetTopMargin(0.1);
    gPad->SetRightMargin(0.22);
    if(tdrStyle_->getXLog())gPad->SetLogx(true);
    if(tdrStyle_->getYLog())gPad->SetLogy(true);
    if(tdrStyle_->getZLog())gPad->SetLogz(true);

    //TLegend *leg = new TLegend(0.25, 0.80, 0.90, 0.90, "", "brNDC");
    //tdrStyle_->setStyle(leg);

    hist->SetTitle(dataEraOrMcBin_.c_str());
    hist->Draw("COLZ");
    //leg->AddEntry(hist, hist->GetName(), "L");
    //leg->Draw();

    canvas.Update();
    canvas.SaveAs(outPdfName.c_str());
    file.Close();  // Now it's safe to close the file since the histogram has been cloned
  } 
  else {
    std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
    return;
  }
}


