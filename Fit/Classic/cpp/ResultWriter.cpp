#include "ResultWriter.h"
#include "Helper.h"
#include <iostream>
#include <cassert>
#include <TDirectory.h>

ResultWriter::ResultWriter(const FitResults& res, FitModel& model, const std::string& outputFilename,
                           const LoadAll& loadAll, GlobalFlag& flags)
    : results(res), fitModel(model), loadAll_(loadAll), globalFlags_(flags)
{
    if (globalFlags_.isDebug) {
        std::cout << "[ResultWriter] Opening output file: " << outputFilename << std::endl;
    }
    outputFile = TFile::Open(outputFilename.c_str(), "RECREATE");
    assert(outputFile && !outputFile->IsZombie());
    if (globalFlags_.isDebug) {
        std::cout << "[ResultWriter] Output file opened successfully." << std::endl;
    }
}

ResultWriter::~ResultWriter() {
    if (outputFile) {
        if (globalFlags_.isDebug) {
            std::cout << "[ResultWriter] Closing output file." << std::endl;
        }
        outputFile->Close();
    }
}

void ResultWriter::storeResults() {
    if (globalFlags_.isDebug) {
        std::cout << "[ResultWriter] Storing fit results to output file." << std::endl;
    }
    outputFile->cd();
    
    // Write the fit function and corresponding graph.
    TF1* jesFit = fitModel.getFitFunction();
    jesFit->Write("jesFit_func", TObject::kOverwrite);
    
    TGraphErrors* graph = new TGraphErrors();
    TMatrixD errorMatrix(results.errorMatrix);
    Helper::funcToGraph(jesFit, errorMatrix, graph);
    graph->Write("jesFit_graph", TObject::kOverwrite);
    
    if (globalFlags_.isDebug) {
        std::cout << "[ResultWriter] Fit Chi2: " << results.chi2 
                  << " with " << results.nDataPoints << " data points." << std::endl;
    }
    
    // Create a directory for pre-fit inputs.
    TDirectory* prefitDir = outputFile->mkdir("prefit");
    prefitDir->cd();
    
    // Write pre-fit Base objects.
    const auto& baseMap = loadAll_.getBase();
    for (const auto &entry : baseMap) {
        const std::string &name = entry.first;
        // Assume LoadBase has a method getGraph() that returns a TGraphErrors*.
        TGraphErrors* g = entry.second->getInput();
        if (g) {
            g->Write(("base_" + name).c_str(), TObject::kOverwrite);
            if (globalFlags_.isDebug) {
                std::cout << "[ResultWriter] Wrote prefit base object: " << name << std::endl;
            }
        }
    }
    
    // Write pre-fit Systematic objects.
    const auto& systMap = loadAll_.getSyst();
    for (const auto &entry : systMap) {
        const std::string &name = entry.first;
        // Assume LoadSyst has a method getHist() that returns a TH1D*.
        TH1D* h = entry.second->getHist();
        if (h) {
            h->Write(("syst_" + name).c_str(), TObject::kOverwrite);
            if (globalFlags_.isDebug) {
                std::cout << "[ResultWriter] Wrote prefit systematic object: " << name << std::endl;
            }
        }
    }
    
    // Write pre-fit Shape objects.
    const auto& shapeMap = loadAll_.getShape();
    for (const auto &entry : shapeMap) {
        const std::string &name = entry.first;
        // Assume LoadShape has a method getFunc() that returns a TF1*.
        TF1* f = entry.second->getFunc();
        if (f) {
            f->Write(("shape_" + name).c_str(), TObject::kOverwrite);
            if (globalFlags_.isDebug) {
                std::cout << "[ResultWriter] Wrote prefit shape object: " << name << std::endl;
            }
        }
    }
    
    // Write pre-fit Normalization objects.
    const auto& normMap = loadAll_.getNorm();
    for (const auto &entry : normMap) {
        const std::string &name = entry.first;
        // Assume LoadNorm has a method getHist() that returns a TH1D*.
        TH1D* h = entry.second->getHist();
        if (h) {
            h->Write(("norm_" + name).c_str(), TObject::kOverwrite);
            if (globalFlags_.isDebug) {
                std::cout << "[ResultWriter] Wrote prefit norm object: " << name << std::endl;
            }
        }
    }
    
    // Return to the main directory and write all changes.
    outputFile->cd();
    outputFile->Write();
}

