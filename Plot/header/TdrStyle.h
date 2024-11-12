// TdrStyle.h
#pragma once

#include "TStyle.h"
#include "TCanvas.h"
#include <TGraphErrors.h>
#include "TProfile.h"
#include "TProfile2D.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TFrame.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"

#include "FigConfig.h"

#include <string>
#include <vector>
#include <type_traits>

/**
 * @brief TdrStyle class for configuring ROOT plotting styles.
 * 
 * This class provides methods to set styles for various ROOT objects such as histograms,
 * profiles, graphs, and legends. It also includes configuration settings for plot aesthetics.
 */
class TdrStyle {
public:
    /**
     * @brief Constructor for TdrStyle.
     * 
     * Initializes default configuration parameters and color palette.
     */
    TdrStyle();

    /**
     * @brief Sets the overall TDR style for ROOT plots.
     * 
     * Configures global style settings to adhere to TDR (Technical Design Report) standards.
     */
    void setTdrStyle();

    /**
     * @brief Adds CMS-specific luminosity and extra text to a pad.
     * 
     * @param pad Pointer to the TPad where the text will be added.
     * @param iPeriod Luminosity period identifier (default is 3).
     * @param iPosX Position identifier for text placement (default is 10).
     */
    void CMS_lumi(TPad* pad, int iPeriod = 3, int iPosX = 10);

    // Setters for additional parameters
    void setLumiText(const TString& lumiText) { lumi_ = lumiText; }
    void setExtraText(const TString& extraText) { extraText_ = extraText; }
    void setWriteExtraText(bool writeExtraText) { writeExtraText_ = writeExtraText; }

    /**
     * @brief Sets the style for various ROOT objects using templates.
     * 
     * @tparam T Type of the ROOT object (e.g., TH1D, TProfile, TLegend).
     * @param obj Pointer to the ROOT object to style.
     */
    template<typename T>
    void setStyle(T* obj);

    /**
     * @brief Sets the style specifically for ratio plots using templates.
     * 
     * @tparam T Type of the ROOT object (e.g., TGraphErrors, TProfile).
     * @param obj Pointer to the ROOT object to style.
     */
    template<typename T>
    void setStyleRatio(T* obj);

    /**
     * @brief Sets the color for various ROOT objects using templates.
     * 
     * @tparam T Type of the ROOT object (e.g., TH1D, TProfile, TGraphErrors).
     * @param obj Pointer to the ROOT object to color.
     * @param index Index to select the color from the predefined color palette.
     */
    template<typename T>
    void setColor(T* obj, int index);

    /**
     * @brief Configures figure parameters based on provided configuration structures.
     * 
     * @tparam T Type of the configuration structure (e.g., FigConfigEras1D, FigConfigEra2D).
     * @param params Configuration parameters to apply.
     */
    template <typename T>
    void setFigConfig(const T& params);

    // Getters
    bool getXLog() const { return xLog_; }
    bool getYLog() const { return yLog_; }
    bool getZLog() const { return zLog_; }
    bool getIsNorm() const { return isNorm_; }

private:
    // Global variables
    TString cmsText_ = "CMS";
    float cmsTextFont_ = 61;  ///< CMS text font (default: helvetic-bold)

    bool writeExtraText_ = false;       ///< Flag to write extra text
    TString extraText_ = "Preliminary"; ///< Extra text to display
    float extraTextFont_ = 52;          ///< Extra text font (default: helvetica-italics)

    // Text sizes and offsets
    float lumiTextSize_ = 0.6;        ///< Luminosity text size
    float lumiTextOffset_ = 0.2;      ///< Luminosity text offset
    float cmsTextSize_ = 0.75;        ///< CMS text size
    float cmsTextOffset_ = 0.1;       ///< CMS text offset

    float relPosX_ = 0.045;            ///< Relative X position
    float relPosY_ = 0.035;            ///< Relative Y position
    float relExtraDY_ = 1.2;           ///< Relative extra Y delta

    float extraOverCmsTextSize_ = 0.76; ///< Extra over CMS text size

    TString lumi_ = "20.1 fb^{-1}";     ///< Default luminosity text

    std::vector<std::string> hexColors; ///< Vector to store hex color codes

    // Figure configuration
    std::string xTitle_;
    std::string yTitle_;
    std::string zTitle_;
    std::string rTitle_;
    double xMin_, xMax_;
    double yMin_, yMax_;
    double rMin_, rMax_;
    bool xLog_, yLog_, zLog_;
    bool isNorm_;
    int default_;
};

// Include the template implementations
#include "../tpp/TdrStyle.tpp"


