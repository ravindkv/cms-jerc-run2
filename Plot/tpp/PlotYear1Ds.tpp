#include <iostream>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TPad.h>
#include <TROOT.h>
#include <TLatex.h> // For labeling
#include "PlotYear1Ds.h"
#include "Helper.h"

template<typename T>
PlotYear1Ds<T>::PlotYear1Ds()
    : channel_(""),
      year_(""),
      histDir_(""),
      tdrStyle_(std::make_shared<TdrStyle>()),
      isOverlayMC_(false)
{
    histNames_.clear();
}

template<typename T>
PlotYear1Ds<T>::~PlotYear1Ds()
{
    // Clean up MC histograms
    for (auto& pair : mapDataHists_) {
        for (auto* hist : pair.second) {
            delete hist;
        }
        pair.second.clear();
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
void PlotYear1Ds<T>::setInput(const nlohmann::json& inputJson, const std::string& channel, const std::string& year)
{
    inputJson_ = inputJson;
    channel_   = channel;
    year_      = year;
}

template<typename T>
void PlotYear1Ds<T>::setFigConfigYear1Ds(const FigConfigYear1Ds& params)
{
    tdrStyle_->setFigConfig(params);
    histDir_   = params.histDir;
    trigDirs_ = params.trigDirs;
    histNames_ = params.histNames;
}

template<typename T>
void PlotYear1Ds<T>::loadHists(const std::string& sourceType, const std::vector<std::string>& bins)
{
    for (const auto& bin : bins) {
        const std::string fileName = inputJson_[channel_][year_][sourceType][bin];

        TFile file(fileName.c_str());
        if (file.IsZombie()) {
            std::cerr << "Error: Could not open file " << fileName << std::endl;
            continue;
        }

        for (const auto& histName : histNames_) {
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

            gROOT->cd();
            T* clonedHist = static_cast<T*>(hist->Clone((bin + histName).c_str()));

            // Push this MC histogram onto the vector in the map
            if (sourceType == "Data") {
                mapDataHists_[histName].push_back(clonedHist);
            } else if (sourceType == "MC") {
                mapMcHists_[histName].push_back(clonedHist);
            }
        }
        file.Close();
    }
}

template<typename T>
void PlotYear1Ds<T>::drawHists(const std::vector<T*>& hists, const bool& isMC)
{
    if (hists.empty()) {
        std::cerr << "Error: Histograms vector is empty in drawHists(...)." << std::endl;
        return;
    }

    // Create a legend whose vertical size depends on # of hists
    double legYmin = 0.85 - 0.05 * hists.size();
    double legXmin = isMC ? 0.25: 0.35;
    double legXmax = isMC ? 0.35: 0.90;

    TLegend* legend = new TLegend(legXmin, legYmin, legXmax, 0.90, "", "brNDC");
    tdrStyle_->setStyle(legend);
    if(isMC) legend->AddEntry((TObject*)0, "MC", "");
    else legend->AddEntry((TObject*)0, "Data", "");

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

        // Add legend entry
        if (isMC) {
            legend->AddEntry(hist, "", "L");
        } else {
            legend->AddEntry(hist, hist->GetName(), "E1");
        }
    }

    // Optional label for era
    TLatex label;
    label.SetTextSize(0.035);
    label.DrawLatexNDC(0.30, 0.92, year_.c_str());

    // Draw the legend last
    legend->Draw();
}

template<typename T>
void PlotYear1Ds<T>::overlayDataWithMcInRatio(const std::string& outPdfName)
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
        if (mapDataHists_.find(histName) != mapDataHists_.end() &&
            mapMcHists_.find(histName) != mapMcHists_.end())
        {
            // Grab pointers
            T* mergedDataHist = Helper::combineHists(mapDataHists_[histName]);
            T* mergedMcHist = Helper::combineHists(mapMcHists_[histName]);
            if (!mergedMcHist) {
                std::cerr << "Error: Could not create merged MC histogram." << std::endl;
                return;
            }

            // Optional normalization
            if (tdrStyle_->getIsNorm() && mergedDataHist->Integral() > 0.0) {
                mergedDataHist->Scale(1.0 / mergedDataHist->Integral());
            }
            if (tdrStyle_->getIsNorm() && mergedMcHist->Integral() > 0.0) {
                mergedMcHist->Scale(1.0 / mergedMcHist->Integral());
            }
            mergedDataHist->SetName(histName.c_str());
            mergedMcHist->SetName(histName.c_str());
            dataHists.push_back(mergedDataHist);
            mcHists.push_back(mergedMcHist);

            // Prepare ratio TGraphErrors
            auto* ratioGraph = new TGraphErrors(mergedDataHist->GetNbinsX());
            Helper::calculateHistRatio(mergedDataHist, mergedMcHist, ratioGraph);
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

