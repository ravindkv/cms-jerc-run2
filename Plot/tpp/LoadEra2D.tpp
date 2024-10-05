#include "TDRStyle.h"
#include "LoadEra2D.h"

template<typename T>
LoadEra2D<T>::LoadEra2D() : 
  channel_(""), 
  year_(""), 
  histDir_(""), 
  dataEraOrMCBin_(""),
  histName_("")
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
void LoadEra2D<T>::setDataEraOrMCBin(const std::string & dataEraOrMCBin) {
  dataEraOrMCBin_ = dataEraOrMCBin;
}

template<typename T>
void LoadEra2D<T>::setHistDir(const std::string & histDir) {
  histDir_ = histDir;
}

template<typename T>
void LoadEra2D<T>::setHistName(const std::string & histName) {
  histName_ = histName;
}

// Load Data Histograms
template<typename T>
void LoadEra2D<T>::drawHist2D(TFile* outRootFile, const std::string &outputFile) {

  std::string fileName;
  if (dataEraOrMCBin_.find("Data_")!= std::string::npos){
    fileName = inputJson_[channel_][year_]["Data"][dataEraOrMCBin_];
  }
  else if (dataEraOrMCBin_.find("MC_")!= std::string::npos){
    fileName = inputJson_[channel_][year_]["MC"][dataEraOrMCBin_];
  }
  else{
    std::cerr << "Error: check dataEraOrMCBin_ =  " << dataEraOrMCBin_ << std::endl;
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
    gROOT->cd();  // Change the directory to ROOT's global directory
    outRootFile->cd();
    TCanvas canvas("c", "Data and MC Ratio", 600, 600);
    canvas.cd();
    TDRStyle tdrS;
    tdrS.setTDRStyle();
    TLegend *leg = new TLegend(0.25, 0.80, 0.90, 0.90, "", "brNDC");
    tdrS.setStyle(leg);

    //tdrS.setStyle(hist, 0.0, 1.5);
    //tdrS.setColor(hist, i);
    hist->Draw("colz");
    leg->AddEntry(hist, hist->GetName(), "L");
    leg->Draw();

    canvas.Update();
    canvas.SaveAs(outputFile.c_str());
    outRootFile->Write();
    file.Close();  // Now it's safe to close the file since the histogram has been cloned
  } 
  else {
    std::cerr << "Error: Could not retrieve histogram " << path << " from " << fileName << std::endl;
    return;
  }
}


