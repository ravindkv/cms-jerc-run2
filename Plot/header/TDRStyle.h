#ifndef TDRSTYLE_H
#define TDRSTYLE_H

#include "TStyle.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"

class TDRStyle {
public:
    // Constructor
    TDRStyle();

    // Set style methods
    void setTDRStyle();
    void CMS_lumi(TPad* pad, int iPeriod = 3, int iPosX = 10);
    
    // Setters for additional parameters
    void setLumiText(const TString& lumiText) { lumi_ = lumiText; }
    void setExtraText(const TString& extraText) { extraText_ = extraText; }
    void setWriteExtraText(bool writeExtraText) { writeExtraText_ = writeExtraText; }

private:
    // Global variables
    TString cmsText_ = "CMS";
    float cmsTextFont_ = 61;  // default is helvetic-bold

    bool writeExtraText_ = false;
    TString extraText_ = "Preliminary";
    float extraTextFont_ = 52;  // default is helvetica-italics

    // Text sizes and offsets
    float lumiTextSize_ = 0.6;
    float lumiTextOffset_ = 0.2;
    float cmsTextSize_ = 0.75;
    float cmsTextOffset_ = 0.1;

    float relPosX_ = 0.045;
    float relPosY_ = 0.035;
    float relExtraDY_ = 1.2;

    float extraOverCmsTextSize_ = 0.76;

    TString lumi_ = "20.1 fb^{-1}"; // default luminosity
};

#endif // TDRSTYLE_H

