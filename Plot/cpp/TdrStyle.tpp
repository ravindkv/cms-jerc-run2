// TdrStyle.tpp

#include "TdrStyle.h"
#include <iostream>
#include <cmath> // for sqrt, pow

// Helper struct for static_assert in templates
template <class>
struct always_false : std::false_type {};

// Constructor
TdrStyle::TdrStyle()
    : xTitle_(""), yTitle_(""), zTitle_(""), rTitle_(""),
      xMin_(0.0), xMax_(0.0),
      yMin_(0.0), yMax_(0.0),
      rMin_(0.0), rMax_(0.0),
      xLog_(false), yLog_(false), zLog_(false),
      isNorm_(false), default_(-9999)
{
    hexColors = {
        "#3f90da", "#ffa90e", "#bd1f01",
        "#94a4a2", "#832db6", "#a96b59",
        "#e76300", "#b9ac70", "#717581", "#92dadd"
    };
}

// Set the TDR style
void TdrStyle::setTdrStyle() {
    TStyle *tdrStyle = new TStyle("tdrStyle", "Style for P-TDR");

    // Set canvas properties
    tdrStyle->SetCanvasBorderMode(0);
    tdrStyle->SetCanvasColor(kWhite);
    tdrStyle->SetCanvasDefH(600);
    tdrStyle->SetCanvasDefW(600);
    tdrStyle->SetCanvasDefX(0);
    tdrStyle->SetCanvasDefY(0);

    // Set pad properties
    tdrStyle->SetPadBorderMode(0);
    tdrStyle->SetPadColor(kWhite);
    tdrStyle->SetPadGridX(false);
    tdrStyle->SetPadGridY(false);
    tdrStyle->SetGridColor(0);
    tdrStyle->SetGridStyle(3);
    tdrStyle->SetGridWidth(1);

    // Set frame properties
    tdrStyle->SetFrameBorderMode(0);
    tdrStyle->SetFrameBorderSize(1);
    tdrStyle->SetFrameFillColor(0);
    tdrStyle->SetFrameLineColor(1);
    tdrStyle->SetFrameLineStyle(1);
    tdrStyle->SetFrameLineWidth(1);

    // Set histogram properties
    tdrStyle->SetHistLineColor(1);
    tdrStyle->SetHistLineStyle(0);
    tdrStyle->SetHistLineWidth(1);

    tdrStyle->SetEndErrorSize(2);
    tdrStyle->SetMarkerStyle(20);

    // Set fit/function properties
    tdrStyle->SetOptFit(1);
    tdrStyle->SetFuncColor(2);
    tdrStyle->SetFuncStyle(1);
    tdrStyle->SetFuncWidth(1);

    // Set statistics box properties
    tdrStyle->SetOptStat(0);
    tdrStyle->SetStatColor(kWhite);
    tdrStyle->SetStatFont(42);
    tdrStyle->SetStatFontSize(0.025);
    tdrStyle->SetStatTextColor(1);
    tdrStyle->SetStatBorderSize(1);

    // Set margins
    tdrStyle->SetPadTopMargin(0.05);
    tdrStyle->SetPadBottomMargin(0.13);
    tdrStyle->SetPadLeftMargin(0.16);
    tdrStyle->SetPadRightMargin(0.02);

    // Set axis titles and labels
    tdrStyle->SetTitleColor(1, "XYZ");
    tdrStyle->SetTitleFont(42, "XYZ");
    tdrStyle->SetTitleSize(0.06, "XYZ");
    tdrStyle->SetTitleXOffset(0.9);
    tdrStyle->SetTitleYOffset(1.25);
    tdrStyle->SetLabelColor(1, "XYZ");
    tdrStyle->SetLabelFont(42, "XYZ");
    tdrStyle->SetLabelOffset(0.007, "XYZ");
    tdrStyle->SetLabelSize(0.05, "XYZ");

    //From Mikko --------
    tdrStyle->SetAxisColor(1, "XYZ");
    tdrStyle->SetStripDecimals(kTRUE);
    tdrStyle->SetTickLength(0.03, "XYZ");
    tdrStyle->SetNdivisions(510, "XYZ");
    tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
    tdrStyle->SetPadTickY(1);
    //Change for log plots:
    tdrStyle->SetOptLogx(0);
    tdrStyle->SetOptLogy(0);
    tdrStyle->SetOptLogz(0);
    //Postscript options:
    tdrStyle->SetPaperSize(20.,20.);
    tdrStyle->SetHatchesLineWidth(5);
    tdrStyle->SetHatchesSpacing(0.05);
    //-------------------

    tdrStyle->cd();
}

void TdrStyle::CMS_lumi(TPad* pad, int iPeriod, int iPosX) {
    bool outOfFrame = false;
    if (iPosX / 10 == 0) {
        outOfFrame = true;
    }
    int alignY_ = 3;
    int alignX_ = 2;
    if (iPosX / 10 == 0) alignX_ = 1;
    if (iPosX == 0) alignX_ = 1;
    if (iPosX == 0) alignY_ = 1;
    if (iPosX / 10 == 1) alignX_ = 1;
    if (iPosX / 10 == 2) alignX_ = 2;
    if (iPosX / 10 == 3) alignX_ = 3;
    int align_ = 10 * alignX_ + alignY_;

    pad->cd();
    
    TString lumiText = lumi_;

    TLatex latex;
    latex.SetNDC();
    latex.SetTextAngle(0);
    latex.SetTextColor(kBlack);

    float extraTextSize = extraOverCmsTextSize_ * cmsTextSize_;

    latex.SetTextFont(42);
    latex.SetTextAlign(31);
    latex.SetTextSize(lumiTextSize_ * pad->GetTopMargin());
    latex.DrawLatex(1 - pad->GetRightMargin(), 1 - pad->GetTopMargin() + lumiTextOffset_ * pad->GetTopMargin(), lumiText);

    if (outOfFrame) {
        latex.SetTextFont(cmsTextFont_);
        latex.SetTextAlign(11);
        latex.SetTextSize(cmsTextSize_ * pad->GetTopMargin());
        latex.DrawLatex(pad->GetLeftMargin(), 1 - pad->GetTopMargin() + lumiTextOffset_ * pad->GetTopMargin(), cmsText_);
    }

    float posX_ = (iPosX % 10 <= 1) ?
        pad->GetLeftMargin() + relPosX_ * (1 - pad->GetLeftMargin() - pad->GetRightMargin()) :
        (iPosX % 10 == 2) ?
        pad->GetLeftMargin() + 0.5 * (1 - pad->GetLeftMargin() - pad->GetRightMargin()) :
        1 - pad->GetRightMargin() - relPosX_ * (1 - pad->GetLeftMargin() - pad->GetRightMargin());

    float posY_ = 1 - pad->GetTopMargin() - relPosY_ * (1 - pad->GetTopMargin() - pad->GetBottomMargin());

    if (!outOfFrame) {
        latex.SetTextFont(cmsTextFont_);
        latex.SetTextSize(cmsTextSize_ * pad->GetTopMargin());
        latex.SetTextAlign(align_);
        latex.DrawLatex(posX_, posY_, cmsText_);
        if (writeExtraText_) {
            latex.SetTextFont(extraTextFont_);
            latex.SetTextAlign(align_);
            latex.SetTextSize(extraTextSize * pad->GetTopMargin());
            latex.DrawLatex(posX_, posY_ - relExtraDY_ * cmsTextSize_ * pad->GetTopMargin(), extraText_);
        }
    }
    else if (writeExtraText_) {
        posX_ = (iPosX == 0) ? pad->GetLeftMargin() + relPosX_ * (1 - pad->GetLeftMargin() - pad->GetRightMargin()) : 0;
        latex.SetTextFont(extraTextFont_);
        latex.SetTextSize(extraTextSize * pad->GetTopMargin());
        latex.SetTextAlign(align_);
        latex.DrawLatex(posX_, 1 - pad->GetTopMargin() + lumiTextOffset_ * pad->GetTopMargin(), extraText_);
    }
}


// Template function to set style
template<typename T>
void TdrStyle::setStyle(T* obj) {
    if constexpr (std::is_same_v<T, TH1D>) {
        obj->SetLineWidth(3);
        obj->GetYaxis()->CenterTitle();
        obj->GetXaxis()->SetTitleOffset(1.0);
        obj->GetYaxis()->SetTitleOffset(1.0);
        obj->GetXaxis()->SetTitleSize(0.05);
        obj->GetYaxis()->SetTitleSize(0.05);
        obj->GetXaxis()->SetMoreLogLabels();
        obj->GetXaxis()->SetTitle(xTitle_.c_str());
        obj->GetYaxis()->SetTitle(yTitle_.c_str());
        if(xMin_ != default_ || xMax_ != default_){
            obj->GetXaxis()->SetRangeUser(xMin_, xMax_);
        }
        if(yMin_ != default_ || yMax_ != default_){
            obj->SetMinimum(yMin_);
            obj->SetMaximum(yMax_);
        }
    }
    else if constexpr (std::is_same_v<T, TProfile>) {
        obj->SetLineWidth(3);
        obj->SetMarkerStyle(20);
        obj->GetYaxis()->CenterTitle();
        obj->GetXaxis()->SetTitleOffset(1.0);
        obj->GetYaxis()->SetTitleOffset(1.15);
        obj->GetXaxis()->SetTitleSize(0.05);
        obj->GetYaxis()->SetTitleSize(0.07);
        obj->GetXaxis()->SetLabelSize(0.05);
        obj->GetYaxis()->SetLabelSize(0.05);
        obj->GetXaxis()->SetMoreLogLabels();
        obj->GetXaxis()->SetTitle(xTitle_.c_str());
        obj->GetYaxis()->SetTitle(yTitle_.c_str());
        if(xMin_ != default_ || xMax_ != default_){
            obj->GetXaxis()->SetRangeUser(xMin_, xMax_);
        }
        if(yMin_ != default_ || yMax_ != default_){
            obj->SetMinimum(yMin_);
            obj->SetMaximum(yMax_);
        }
    }
    else if constexpr (std::is_same_v<T, TH2D> || 
                       std::is_same_v<T, TH2F> ||
                       std::is_same_v<T, TProfile2D>
                      ) {
        obj->GetXaxis()->CenterTitle();
        obj->GetYaxis()->CenterTitle();
        obj->GetZaxis()->CenterTitle();
        obj->GetXaxis()->SetTitleOffset(1.0);
        obj->GetYaxis()->SetTitleOffset(1.6);
        obj->GetZaxis()->SetTitleOffset(1.5);
        obj->GetXaxis()->SetTitleSize(0.05);
        obj->GetYaxis()->SetTitleSize(0.05);
        obj->GetZaxis()->SetTitleSize(0.05);
        obj->GetXaxis()->SetLabelSize(0.05);
        obj->GetYaxis()->SetLabelSize(0.05);
        obj->GetZaxis()->SetLabelSize(0.05);
        obj->GetYaxis()->SetNoExponent();
        obj->GetXaxis()->SetMoreLogLabels();
        obj->GetYaxis()->SetMoreLogLabels();
        obj->GetZaxis()->SetMoreLogLabels();
        obj->GetXaxis()->SetTitle(xTitle_.c_str());
        obj->GetYaxis()->SetTitle(yTitle_.c_str());
        obj->GetZaxis()->SetTitle(zTitle_.c_str());
        if(xMin_ != default_ || xMax_ != default_){
            obj->GetXaxis()->SetRangeUser(xMin_, xMax_);
        }
        if(yMin_ != default_ || yMax_ != default_){
            obj->GetYaxis()->SetRangeUser(yMin_, yMax_);
        }
    }
    else if constexpr (std::is_same_v<T, TLegend>) {
        obj->SetFillStyle(kNone);
        obj->SetBorderSize(0);
        obj->SetTextSize(0.040);
    }
    else {
        static_assert(always_false<T>::value, "Unsupported type for setStyle");
    }
}

// Template function to set style for ratio plots
template<typename T>
void TdrStyle::setStyleRatio(T* obj) {
    if constexpr (std::is_same_v<T, TGraphErrors>) {
        if (obj->GetHistogram()) { // Check if histogram exists
            obj->GetHistogram()->SetTitle("");
            // X-axis styling
            obj->GetHistogram()->GetXaxis()->SetTitleSize(0.12);
            obj->GetHistogram()->GetXaxis()->SetLabelSize(0.12);
            obj->GetHistogram()->GetXaxis()->SetLabelFont(42);
            obj->GetHistogram()->GetXaxis()->SetTitleOffset(1.2);
            obj->GetHistogram()->GetXaxis()->SetLabelOffset(0.01);

            // Y-axis styling
            obj->GetHistogram()->GetYaxis()->SetTitleSize(0.13);
            obj->GetHistogram()->GetYaxis()->SetLabelSize(0.12);
            obj->GetHistogram()->GetYaxis()->SetLabelFont(42);
            obj->GetHistogram()->GetYaxis()->SetNdivisions(6, 5, 0);
            obj->GetHistogram()->GetYaxis()->SetTitleOffset(0.6);
            obj->GetHistogram()->GetYaxis()->SetLabelOffset(0.01);
            obj->GetHistogram()->GetYaxis()->CenterTitle();

            // Additional styling
            obj->SetMarkerStyle(20);  // Set marker style for points

            // Optional: Log scale or no exponent for x-axis
            obj->GetHistogram()->GetXaxis()->SetMoreLogLabels();
            obj->GetHistogram()->GetXaxis()->SetNoExponent();
            obj->GetHistogram()->GetXaxis()->SetTitle(xTitle_.c_str());
            obj->GetHistogram()->GetYaxis()->SetTitle(rTitle_.c_str());
            if(xMin_ != default_ || xMax_ != default_){
                obj->GetHistogram()->GetXaxis()->SetRangeUser(xMin_, xMax_);
            }
            if(rMin_ != default_ || rMax_ != default_){
                obj->GetHistogram()->GetYaxis()->SetRangeUser(rMin_, rMax_);
            }
        }
    }
    else if constexpr (std::is_same_v<T, TProfile>) {
        obj->GetXaxis()->SetTitleSize(0.12);
        obj->GetXaxis()->SetLabelSize(0.12);
        obj->GetXaxis()->SetLabelFont(42);
        obj->GetXaxis()->SetTitleOffset(1.1);
        obj->GetXaxis()->SetLabelOffset(0.01);
        obj->SetMarkerStyle(20);
        obj->GetYaxis()->SetTitleSize(0.13);
        obj->GetYaxis()->SetLabelSize(0.12);
        obj->GetYaxis()->SetLabelFont(42);
        obj->GetYaxis()->SetNdivisions(6, 5, 0);
        obj->GetYaxis()->SetTitleOffset(0.6);
        obj->GetYaxis()->SetLabelOffset(0.01);
        obj->GetYaxis()->CenterTitle();
        obj->GetXaxis()->SetMoreLogLabels();
        obj->GetXaxis()->SetNoExponent();
        obj->GetXaxis()->SetTitle(xTitle_.c_str());
        obj->GetYaxis()->SetTitle(rTitle_.c_str());
        if(xMin_ != default_ || xMax_ != default_){
            obj->GetXaxis()->SetRangeUser(xMin_, xMax_);
        }
        if(rMin_ != default_ || rMax_ != default_){
            obj->GetYaxis()->SetRangeUser(rMin_, rMax_);
        }
    }
    else {
        static_assert(always_false<T>::value, "Unsupported type for setStyleRatio");
    }
}

// Template function to set color
template<typename T>
void TdrStyle::setColor(T* obj, int index) {
    // Retrieve color from hex code
    if (hexColors.empty()) {
        std::cerr << "Error: hexColors vector is empty." << std::endl;
        return;
    }
    int color = TColor::GetColor(hexColors[index % hexColors.size()].c_str());
    if constexpr (std::is_same_v<T, TH1D> || 
                  std::is_same_v<T, TProfile> || 
                  std::is_same_v<T, TGraphErrors>) {
        obj->SetLineColor(color);
        obj->SetMarkerColor(color); // Example marker color
        // obj->SetFillColor(color); // Uncomment if needed
    }
    else {
        static_assert(always_false<T>::value, "Unsupported type for setColor");
    }
}

// Template function to set figure configuration
template <typename T>
void TdrStyle::setFigConfig(const T& params) {
    xTitle_ = params.xTitle;
    yTitle_ = params.yTitle;
    xMin_ = params.xMin;
    xMax_ = params.xMax;
    yMin_ = params.yMin;
    yMax_ = params.yMax;
    xLog_ = params.xLog;
    yLog_ = params.yLog;
    isNorm_ = params.isNorm;

    // Special handling for specific types
    if constexpr (std::is_same_v<T, FigConfigEras1D> || 
                  std::is_same_v<T, FigConfigErasXY> || 
                  std::is_same_v<T, FigConfigEraXY>) {
        rTitle_ = params.rTitle;
        rMin_ = params.rMin;
        rMax_ = params.rMax;
    }
    if constexpr (std::is_same_v<T, FigConfigEra2D>) {
        zLog_ = params.zLog;
        zTitle_ = params.zTitle;
    }
}


