#include "TDRStyle.h"
#include <iostream>

TDRStyle::TDRStyle() {
    setTDRStyle();
}

void TDRStyle::setTDRStyle() {
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

    tdrStyle->cd();
}

void TDRStyle::CMS_lumi(TPad* pad, int iPeriod, int iPosX) {
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

