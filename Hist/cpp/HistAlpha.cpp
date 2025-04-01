#include "HistAlpha.h"
#include "Helper.h"  // Assuming you have a helper for directory creation

#include <sstream>
#include <iomanip>

HistAlpha::HistAlpha(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin, const std::vector<double>& alphaCuts)
    : alphaCuts_(alphaCuts)
{
    // Create a subdirectory for HistAlpha histograms
    std::string dirName = directoryName + "/HistAlpha";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    if (!newDir) {
        std::cerr << "Error: Could not create directory " << dirName << std::endl;
        return;
    }
    newDir->cd();
    
    // Get binning for pT from VarBin
    binsPt_ = varBin.getBinsPt();
    nPt_ = binsPt_.size() - 1;

    std::vector<double> binsAlpha = varBin.getBinsAlpha();
    int nAlphaForP = binsAlpha.size() -1;

    std::vector<double> rangeAlpha = varBin.getRangeAlpha();
    int nAlpha   = rangeAlpha.at(0);
    double minAlpha = rangeAlpha.at(1);
    double maxAlpha = rangeAlpha.at(2);
    
    h1EventInAlpha_ = std::make_unique<TH1D>("h1EventInAlpha", "", nAlpha, minAlpha, maxAlpha);
    p1DbRespInAlphaForRefPt175to230_ = std::make_unique<TProfile>("h1DbRespInAlphaForRefPt175to230", "", nAlphaForP, binsAlpha.data());
    p1MpfRespInAlphaForRefPt175to230_ = std::make_unique<TProfile>("h1MpfRespInAlphaForRefPt175to230", "", nAlphaForP, binsAlpha.data());
    p1DbRespInMaxAlphaForRefPt175to230_ = std::make_unique<TProfile>("h1DbRespInMaxAlphaForRefPt175to230", "", nAlphaForP, binsAlpha.data());
    p1MpfRespInMaxAlphaForRefPt175to230_ = std::make_unique<TProfile>("h1MpfRespInMaxAlphaForRefPt175to230", "", nAlphaForP, binsAlpha.data());

    // Initialize 2D Histograms
    h2EventInRefPtAlpha_ = std::make_unique<TH2D>(
        "h2EventInRefPtAlpha", "", nAlpha, minAlpha, maxAlpha, nPt_, binsPt_.data());
    p2DbRespInRefPtAlpha_ = std::make_unique<TProfile2D>(
        "p2DbRespInRefPtAlpha", "", nAlpha, minAlpha, maxAlpha, nPt_, binsPt_.data());
    p2MpfRespInRefPtAlpha_ = std::make_unique<TProfile2D>(
        "p2MpfRespInRefPtAlpha", "", nAlpha, minAlpha, maxAlpha, nPt_, binsPt_.data());
    
    // Initialize histograms for each α‐cut value
    for (double alphaCut : alphaCuts_) {
        InitializeHistogramsForAlpha(alphaCut, newDir);
    }
    
    // Return to the original directory
    origDir->cd();
}

void HistAlpha::InitializeHistogramsForAlpha(double alphaCut, TDirectory* dir) {
    // Format the alpha cut value as a string (e.g., "0.30")
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << alphaCut;
    std::string alphaStr = oss.str();
    
    // Create histogram names based on the α‐cut value
    std::string h1Name     = "h1EventInRefPtForAlphaBelow" + alphaStr;
    std::string p1RhoName  = "p1RhoInRefPtForAlphaBelow" + alphaStr;
    std::string p1DpbName  = "p1DpbRespInRefPtForAlphaBelow" + alphaStr;
    std::string p1MpfName  = "p1MpfRespInRefPtForAlphaBelow" + alphaStr;
    
    // Create and store the histograms/TProfiles using the pT binning
    h1EventInRefPtForAlpha_[alphaCut] = std::make_unique<TH1D>(h1Name.c_str(), h1Name.c_str(), nPt_, binsPt_.data());
    p1RhoInRefPtForAlpha_[alphaCut]  = std::make_unique<TProfile>(p1RhoName.c_str(), p1RhoName.c_str(), nPt_, binsPt_.data());
    p1DpbRespInRefPtForAlpha_[alphaCut] = std::make_unique<TProfile>(p1DpbName.c_str(), p1DpbName.c_str(), nPt_, binsPt_.data());
    p1MpfRespInRefPtForAlpha_[alphaCut] = std::make_unique<TProfile>(p1MpfName.c_str(), p1MpfName.c_str(), nPt_, binsPt_.data());
}

void HistAlpha::Fill(double eventAlpha, double ptRef, double rho, double dbResp, double mpfResp, double weight) {
    // Fill the event alpha distribution histogram.
    h1EventInAlpha_->Fill(eventAlpha, weight);
    
    if (ptRef > 175 && ptRef < 230) {
        // Exclusive profiles: one fill per event based on its α value.
        p1DbRespInAlphaForRefPt175to230_->Fill(eventAlpha, dbResp, weight);
        p1MpfRespInAlphaForRefPt175to230_->Fill(eventAlpha, mpfResp, weight);

        // Cumulative profiles: loop over all bins.
        int nbins = p1DbRespInMaxAlphaForRefPt175to230_->GetNbinsX();
        for (int i = 1; i <= nbins; i++) {
            // Get the upper edge of the bin
            double binUpperEdge = p1DbRespInMaxAlphaForRefPt175to230_->GetXaxis()->GetBinUpEdge(i);
            // If the event's α value is less than this upper edge, fill this bin.
            if (eventAlpha < binUpperEdge) {
                // Fill with the bin center (ensuring the entry goes into the correct bin)
                double xval = p1DbRespInMaxAlphaForRefPt175to230_->GetXaxis()->GetBinCenter(i);
                p1DbRespInMaxAlphaForRefPt175to230_->Fill(xval, dbResp, weight);
            }
        }

        // Do the same for the MPF cumulative profile.
        nbins = p1MpfRespInMaxAlphaForRefPt175to230_->GetNbinsX();
        for (int i = 1; i <= nbins; i++) {
            double binUpperEdge = p1MpfRespInMaxAlphaForRefPt175to230_->GetXaxis()->GetBinUpEdge(i);
            if (eventAlpha < binUpperEdge) {
                double xval = p1MpfRespInMaxAlphaForRefPt175to230_->GetXaxis()->GetBinCenter(i);
                p1MpfRespInMaxAlphaForRefPt175to230_->Fill(xval, mpfResp, weight);
            }
        }
    }

    h2EventInRefPtAlpha_->Fill(eventAlpha, ptRef, weight);
    p2DbRespInRefPtAlpha_->Fill(eventAlpha, ptRef, dbResp, weight);
    p2MpfRespInRefPtAlpha_->Fill(eventAlpha, ptRef, mpfResp, weight);
    
    // Loop over each α‐cut and fill the corresponding exclusive histograms.
    for (const auto& entry : h1EventInRefPtForAlpha_) {
        double alphaCut = entry.first;
        if (eventAlpha < alphaCut) {
            entry.second->Fill(ptRef, weight);
            p1RhoInRefPtForAlpha_[alphaCut]->Fill(ptRef, rho, weight);
            p1DpbRespInRefPtForAlpha_[alphaCut]->Fill(ptRef, dbResp, weight);
            p1MpfRespInRefPtForAlpha_[alphaCut]->Fill(ptRef, mpfResp, weight);
        }
    }
}

