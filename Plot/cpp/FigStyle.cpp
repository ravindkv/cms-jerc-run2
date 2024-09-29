#include "FigStyle.h"

FigStyle::FigStyle() {
  // Initialize hex color codes
    hexColors = {
        "#3f90da", "#ffa90e", "#bd1f01",
        "#94a4a2", "#832db6", "#a96b59",
        "#e76300", "#b9ac70", "#717581", "#92dadd"
    };
}

void FigStyle::setStyle(TH1F* hist, const TString& title, const TString& xLabel, const TString& yLabel) {
    //hist->SetTitle(title);
    hist->GetXaxis()->SetTitle(xLabel);
    hist->GetYaxis()->SetTitle(yLabel);
    hist->SetLineWidth(3);
    hist->SetFillStyle(3001); // Example fill style
    hist->GetYaxis()->CenterTitle();
    hist->GetXaxis()->SetTitleOffset(1.0);
    hist->GetYaxis()->SetTitleOffset(1.0);
    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetTitleSize(0.05);  
}

void FigStyle::setStyle(TProfile* profile, const TString& title, const TString& xLabel, const TString& yLabel) {
    //profile->SetTitle(title);
    profile->GetXaxis()->SetTitle(xLabel);
    profile->GetYaxis()->SetTitle(yLabel);
    profile->SetLineWidth(3);
    profile->SetMarkerStyle(20);
    //profile->SetMarkerColor(kRed); // Example marker color
		profile->GetYaxis()->CenterTitle();
    profile->GetXaxis()->SetTitleOffset(1.0);
    profile->GetYaxis()->SetTitleOffset(1.0);
    profile->GetXaxis()->SetTitleSize(0.05);
    profile->GetYaxis()->SetTitleSize(0.05);
    profile->GetXaxis()->SetTitleSize(0.05);
    profile->GetYaxis()->SetTitleSize(0.05);
}
void FigStyle::setStyleRatio(TProfile* profile, const TString& title, const TString& xLabel, const TString& yLabel) {
    profile->GetXaxis()->SetTitle(xLabel);
    profile->GetYaxis()->SetTitle(yLabel);
    profile->GetXaxis()->SetTitleSize(0.11);
    profile->GetXaxis()->SetLabelSize(0.10);
    profile->GetXaxis()->SetLabelFont(42);
    profile->GetYaxis()->SetRangeUser(0.0, 2.0);
    profile->GetXaxis()->SetTitleOffset(1);
    profile->GetXaxis()->SetLabelOffset(0.01);
    profile->SetMarkerStyle(20); 
    profile->GetYaxis()->SetTitleSize(0.11);
    profile->GetYaxis()->SetLabelSize(0.09);
    profile->GetYaxis()->SetLabelFont(42);
    profile->GetYaxis()->SetNdivisions(6,5,0);
    profile->GetYaxis()->SetTitleOffset(0.6);
    profile->GetYaxis()->SetLabelOffset(0.01);
    profile->GetYaxis()->CenterTitle();
}

void FigStyle::setColor(TH1F* hist, int index) {
    // Retrieve color from hex code
    int color = TColor::GetColor(hexColors[index % hexColors.size()].c_str());
    hist->SetLineColor(color);
    //hist->SetFillColor(color);
}

void FigStyle::setColor(TProfile* profile, int index) {
    // Retrieve color from hex code
    int color = TColor::GetColor(hexColors[index % hexColors.size()].c_str());
    profile->SetLineColor(color);
    //profile->SetFillColor(color);
}
