#pragma once

#ifndef PLOT_ERA1DS_H
#define PLOT_ERA1DS_H

#include <map>
#include <memory>
#include <string>
#include <vector>

// Forward declarations to limit includes in header if desired.
// You can also include them here if you prefer:
#include <nlohmann/json.hpp>

class TFile;
class TLegend;
class TCanvas;
class TGraphErrors;
class TH1;
class TPad;

// If you need them for template resolution or inline methods, you may keep the includes here.
#include "FigConfig.h"
#include "TdrStyle.h"

/**
 * @class PlotEra1Ds
 * @brief A class to load, store, and plot 1D histograms for different eras (Data/MC).
 *
 * @tparam T A ROOT histogram type (e.g., TH1F, TH1D, etc.).
 */
template <typename T>
class PlotEra1Ds {
public:
    /// Default constructor
    PlotEra1Ds();

    /// Destructor: Cleans up all dynamically allocated histograms.
    ~PlotEra1Ds();

    /**
     * @brief Set the input JSON, channel, and year.
     *
     * @param inputJson A JSON object containing file paths and configurations.
     * @param channel   The channel name (e.g., "muon", "electron").
     * @param year      The year (e.g., "2017", "2018").
     */
    void setInput(const nlohmann::json& inputJson, const std::string& channel, const std::string& year);

    /**
     * @brief Set the figure configuration for plotting.
     *
     * @param params A FigConfigEra1Ds struct holding directory names, histogram names, style configs, etc.
     */
    void setFigConfigEra1Ds(const FigConfigEra1Ds& params);

    /**
     * @brief Load one set of histograms (e.g., data or one MC era).
     *
     * @param sourceType A key in the JSON to identify the source type (e.g., "Data", "DYJets").
     * @param era        An identifier for the era (e.g., "2017B", "2017C").
     */
    void loadHist(const std::string& sourceType, const std::string& era);

    /**
     * @brief Load histograms for multiple bins (usually for multiple MC processes).
     *
     * @param sourceType A key in the JSON to identify the source type (e.g., "MC").
     * @param bins       A list of bins (sub-categories, e.g., "TT", "DY", "WJets", etc.).
     */
    void loadHists(const std::string& sourceType, const std::vector<std::string>& bins);

    /**
     * @brief Draw (and optionally normalize) a vector of histograms.
     *
     * @param hists A vector of histogram pointers to draw.
     * @param isMC  A boolean indicating whether these histograms are MC or Data (affects style and legend).
     */
    void drawHists(const std::vector<T*>& hists, const bool& isMC);

    /**
     * @brief Overlay Data with MC on the same canvas and draw a ratio pad below.
     *
     * @param outPdfName The output PDF name where the plot will be saved.
     */
    void overlayDataWithMcInRatio(const std::string& outPdfName);

    /**
     * @brief Decide whether to overlay MC histograms on the same pad as Data.
     *
     * @param flag True if MC should be drawn on top of Data; false otherwise.
     */
    void setOverlayMC(bool flag) { isOverlayMC_ = flag; }

private:
    /// A JSON object with the input file paths and configurations.
    nlohmann::json inputJson_;

    /// The physics channel (e.g., "mu", "ele"), set via setInput().
    std::string channel_;

    /// The year (e.g., "2017", "2018"), set via setInput().
    std::string year_;

    /// The era (e.g., "2017B", "2017C").
    std::string dataEra_;

    /// The directory path inside the ROOT file where histograms are stored.
    std::string histDir_;
    std::vector<std::string> trigDirs_;

    /// List of histogram names to retrieve from the ROOT file.
    std::vector<std::string> histNames_;

    /// Map from histogram name to the cloned Data histogram.
    std::map<std::string, T*> mapDataHist_;

    /// Map from histogram name to a vector of cloned MC histograms (could be merged later).
    std::map<std::string, std::vector<T*>> mapMcHists_;

    /// Custom styling.
    std::shared_ptr<TdrStyle> tdrStyle_;

    /// If true, MC histograms are overlaid on the same pad as Data.
    bool isOverlayMC_{false};
};

#include "../tpp/PlotEra1Ds.tpp"

#endif // PLOT_ERA1DS_H

