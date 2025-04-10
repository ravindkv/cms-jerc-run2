#include <iostream>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TPad.h>
#include <TROOT.h>
#include <TLatex.h> // For labeling
#include "PlotEra1Ds.h"
#include "Helper.h"

template<typename T>
PlotEra1Ds<T>::PlotEra1Ds()
    : channel_(""),
      year_(""),
      dataEra_(""),
      histDir_(""),
      tdrStyle_(std::make_shared<TdrStyle>()),
      isOverlayMC_(false)
{
    histNames_.clear();
}

template<typename T>
PlotEra1Ds<T>::~PlotEra1Ds()
{
    // Clean up Data histograms
    for (auto& pair : mapDataHist_) {
        delete pair.second;
        pair.second = nullptr;
    }

    // Clean up MC histograms
    for (auto& pair : mapMcHists_) {
        for (auto* hist : pair.second) {
            delete hist;
        }
        pair.second.clear();
    }
}

template<typename T>
void PlotEra1Ds<T>::setInput(const nlohmann::json& inputJson, const std::string& channel, const std::string& year)
{
    inputJson_ = inputJson;
    channel_   = channel;
    year_      = year;
}

template<typename T>
void PlotEra1Ds<T>::setFigConfigEra1Ds(const FigConfigEra1Ds& params)
{
    tdrStyle_->setFigConfig(params);
    histDir_   = params.histDir;
    trigDirs_ = params.trigDirs;
    histNames_ = params.histNames;
}

template<typename T>
void PlotEra1Ds<T>::loadHist(const std::string& sourceType, const std::string& era)
{
    dataEra_ = era;
    const std::string fileName = inputJson_[channel_][year_][sourceType][era];

    // Open the ROOT file
    TFile file(fileName.c_str());
    if (file.IsZombie()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return;
    }

    // Loop over histogram names to retrieve each one
    for (const auto& histName : histNames_) {
        // Construct the histogram path
        const std::string path = histDir_.empty() ? histName : (histDir_ + "/" + histName);

        // Retrieve the histogram
        T* hist = nullptr;
        if(!trigDirs_.empty()){
            hist = Helper::sumHistsFromTrigDirs<T>(file, histDir_, trigDirs_, histName);
        } else{
            hist = static_cast<T*>(file.Get(path.c_str()));
        }
        if (!hist) {
            std::cerr << "Error: Could not retrieve histogram " << path
                      << " from " << fileName << std::endl;
            continue;
        }

        // Clone the histogram into ROOT's global directory (avoid file closing issues)
        gROOT->cd();
        T* clonedHist = static_cast<T*>(hist->Clone(histName.c_str()));

        // Store it in the Data map
        mapDataHist_[histName] = clonedHist;
    }

    file.Close();
}

template<typename T>
void PlotEra1Ds<T>::loadHists(const std::string& sourceType, const std::vector<std::string>& bins)
{
    for (const auto& mcBin : bins) {
        const std::string fileName = inputJson_[channel_][year_][sourceType][mcBin];

        TFile file(fileName.c_str());
        if (file.IsZombie()) {
            std::cerr << "Error: Could not open file " << fileName << std::endl;
            continue;
        }

        for (const auto& histName : histNames_) {
            const std::string path = histDir_.empty() ? histName : (histDir_ + "/" + histName);

            T* hist = static_cast<T*>(file.Get(path.c_str()));
            if (!hist) {
                std::cerr << "Error: Could not retrieve histogram " << path
                          << " from " << fileName << std::endl;
                continue;
            }

            gROOT->cd();
            T* clonedHist = static_cast<T*>(hist->Clone((mcBin + histName).c_str()));

            // Push this MC histogram onto the vector in the map
            mapMcHists_[histName].push_back(clonedHist);
        }

        file.Close();
    }
}

template<typename T>
void PlotEra1Ds<T>::drawHists(const std::vector<T*>& hists, const bool& isMC)
{
    if (hists.empty()) {
        std::cerr << "Error: Histograms vector is empty in drawHists(...)." << std::endl;
        return;
    }

    // Create a legend whose vertical size depends on # of hists
    double legYmin = 0.85 - 0.05 * hists.size();
    double legXmin = isMC ? 0.55 : 0.25;
    double legXmax = isMC ? 0.90 : 0.50;

    TLegend* legend = new TLegend(legXmin, legYmin, legXmax, 0.90, "", "brNDC");
    tdrStyle_->setStyle(legend);

    // Set log scales if needed
    if (tdrStyle_->getXLog()) {
        gPad->SetLogx(true);
    }
    if (tdrStyle_->getYLog()) {
        gPad->SetLogy(true);
    }

    // Loop over histograms
    for (std::size_t i = 0; i < hists.size(); ++i) {
        T* originalHist = hists.at(i);
        if (!originalHist) {
            std::cerr << "Error: Null histogram pointer in drawHists(...)." << std::endl;
            continue;
        }

        // Clone to avoid modifying the original
        T* hist = static_cast<T*>(originalHist->Clone(originalHist->GetName()));
        if (!hist) {
            std::cerr << "Error: Could not clone histogram in drawHists(...)." << std::endl;
            continue;
        }

        // Optionally normalize
        if (tdrStyle_->getIsNorm() && hist->Integral() > 0.0) {
            hist->Scale(1.0 / hist->Integral());
        }

        // Assign color/style
        tdrStyle_->setStyle(hist);
        tdrStyle_->setColor(hist, i);

        // Decide the draw option
        bool isEmptyPad = (gPad->GetListOfPrimitives()->GetSize() == 0);
        TString drawOpt = (isMC ? "HIST" : "E1");
        if (!isEmptyPad || i > 0) {
            drawOpt += " SAME";
        }

        hist->Draw(drawOpt);
        tdrStyle_->drawTitle();

        // Add legend entry
        if (isMC) {
            legend->AddEntry(hist, "MC", "L");
        } else {
            legend->AddEntry(hist, hist->GetName(), "EP");
        }
    }

    // Optional label for era
    TLatex label;
    label.SetTextSize(0.035);
    label.DrawLatexNDC(0.30, 0.92, dataEra_.c_str());

    // Draw the legend last
    legend->Draw();
}

template<typename T>
void PlotEra1Ds<T>::overlayDataWithMcInRatio(const std::string& outPdfName)
{
    tdrStyle_->setTdrStyle();

    // Make a canvas
    TCanvas canvas("c", "Data and MC Ratio", 600, 600);
    canvas.cd();

    std::vector<T*> dataHists;
    std::vector<T*> mcHists;
    std::vector<TGraphErrors*> ratioGraphs;

    // Collect pairs of data and merged MC histograms
    for (const auto& histName : histNames_) {
        // Check if data and MC exist for this histName
        if (mapDataHist_.find(histName) != mapDataHist_.end() &&
            mapMcHists_.find(histName) != mapMcHists_.end())
        {
            // Grab pointers
            T* dataHist = mapDataHist_[histName];
            T* mergedMcHist = Helper::combineHists(mapMcHists_[histName]);
            if (!mergedMcHist) {
                std::cerr << "Error: Could not create merged MC histogram." << std::endl;
                return;
            }

            // Optional normalization
            if (tdrStyle_->getIsNorm() && dataHist->Integral() > 0.0) {
                dataHist->Scale(1.0 / dataHist->Integral());
            }
            if (tdrStyle_->getIsNorm() && mergedMcHist->Integral() > 0.0) {
                mergedMcHist->Scale(1.0 / mergedMcHist->Integral());
            }

            dataHists.push_back(dataHist);
            mcHists.push_back(mergedMcHist);

            // Prepare ratio TGraphErrors
            auto* ratioGraph = new TGraphErrors(dataHist->GetNbinsX());
            Helper::calculateHistRatio(dataHist, mergedMcHist, ratioGraph);
            ratioGraphs.push_back(ratioGraph);
        }
    }

    // If only data is found and no MC, just draw data
    if (!dataHists.empty() && mcHists.empty()) {
        drawHists(dataHists, false);
        canvas.SaveAs(outPdfName.c_str());
        return;
    }

    // If only MC is found and no data, just draw MC
    if (!mcHists.empty() && dataHists.empty()) {
        drawHists(mcHists, true);
        canvas.SaveAs(outPdfName.c_str());
        return;
    }

    // If both data and MC exist, draw them with ratio
    if (!mcHists.empty() && !dataHists.empty()) {
        // Top pad
        TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.3, 1.0, 1.0);
        pad1->SetBottomMargin(0.0);
        pad1->Draw();
        pad1->cd();

        // Draw Data
        drawHists(dataHists, false);

        // Optionally overlay MC
        if (isOverlayMC_) {
            pad1->cd();
            drawHists(mcHists, true);
        }

        canvas.cd();

        // Bottom pad for ratio
        TPad* pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.3);
        pad2->SetTopMargin(0.0);
        pad2->SetBottomMargin(0.4);
        pad2->Draw();
        pad2->cd();

        if (tdrStyle_->getXLog()) {
            gPad->SetLogx(true);
        }

        // Draw ratio graphs
        for (std::size_t i = 0; i < ratioGraphs.size(); ++i) {
            TGraphErrors* rGraph = ratioGraphs.at(i);
            tdrStyle_->setStyleRatio(rGraph);
            tdrStyle_->setColor(rGraph, i);
            rGraph->Draw(i == 0 ? "APZ" : "PZ SAME");
        }
    }

    canvas.Update();
    canvas.SaveAs(outPdfName.c_str());
}

