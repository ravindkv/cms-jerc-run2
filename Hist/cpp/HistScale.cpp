#include "HistScale.h"

/**
 * @brief Constructs a HistScale object and initializes histograms.
 * 
 * @param origDir Pointer to the original ROOT directory.
 * @param directoryName Name of the directory within the ROOT file to store histograms.
 * @param varBin Object defining variable binning.
 */
HistScale::HistScale(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin)
{
    InitializeHistograms(origDir, directoryName, varBin);
}

/**
 * @brief Initializes all histograms and sets up the internal structures.
 * 
 * @param origDir Pointer to the original ROOT directory.
 * @param directoryName Name of the directory within the ROOT file to store histograms.
 * @param varBin Object defining variable binning.
 */
void HistScale::InitializeHistograms(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin)
{
    // Create a subdirectory for HistScale histograms
    std::string dirName = directoryName + "/HistScale";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    newDir->cd();

    // Define binning from VarBin
    const std::vector<double> binsPt  = varBin.getBinsPt();
    const std::vector<double> binsPhi  = varBin.getBinsPhi();
    const int nPt  = binsPt.size()  - 1;
    const int nPhi  = binsPhi.size()  - 1;

    corrNames_ = {
        "Nano", "Corr"
    };
    // Initialize histograms for Electron1 Pt
    for (const auto& corrName : corrNames_) {
        std::string histNamePt = "h1EventInElectron1Pt" + corrName;
        histElectron1Pt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }

    // Initialize histograms for Muon1 Pt
    for (const auto& corrName : corrNames_) {
        std::string histNamePt = "h1EventInMuon1Pt" + corrName;
        histMuon1Pt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }

    // Initialize histograms for Photon1 Pt
    for (const auto& corrName : corrNames_) {
        std::string histNamePt = "h1EventInPhoton1Pt" + corrName;
        histPhoton1Pt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }

    // Initialize correction names (should match those in ScaleJetMet)
    jecNames_ = {
        "Nano", "Raw", "L1RcCorr", "L2RelCorr", "L2L3ResCorr", "JerCorr", "Corr"
    };

    // Initialize histograms for Jet1 Pt
    for (const auto& corrName : jecNames_) {
        std::string histNamePt = "h1EventInJet1Pt" + corrName;
        histJet1Pt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }

    // Initialize histograms for JetSum Pt
    for (const auto& corrName : jecNames_) {
        std::string histNamePt = "h1EventInJetSumPt" + corrName;
        histJetSumPt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }

    // Initialize histograms for MET Pt
    for (const auto& corrName : corrNames_) {
        std::string histNamePt = "h1EventInMetPt" + corrName;
        histMetPt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }
    // Initialize histograms for MET Phi
    for (const auto& corrName : corrNames_) {
        std::string histNamePhi = "h1EventInMetPhi" + corrName;
        histMetPhi_[corrName] = std::make_unique<TH1D>(
            histNamePhi.c_str(), "", nPhi, binsPhi.data());
    }

    // Initialize histograms for Jet + MET pTs
    for (const auto& corrName : corrNames_) {
        std::string histNamePt = "h1EventInJetMetSumPt" + corrName;
        histJetMetSumPt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }

    std::cout << "Initialized HistScale histograms in directory: " << dirName << std::endl;
    origDir->cd();
}


// Fill
void HistScale::FillElectron(const ScaleElectron& scaleElectron, double weight)
{
    const auto& p4MapElectron1 = scaleElectron.getP4MapElectron1();

    // Fill Electron1 histograms
    for (const auto& corrName : corrNames_) {
        const auto& p4Electron1Iter = p4MapElectron1.find(corrName);
        if (p4Electron1Iter != p4MapElectron1.end()) {
            const auto& p4Electron1 = p4Electron1Iter->second;
            histElectron1Pt_[corrName]->Fill(p4Electron1.Pt(), weight);
        }
    }

}

void HistScale::FillMuon(const ScaleMuon& scaleMuon, double weight)
{
    const auto& p4MapMuon1 = scaleMuon.getP4MapMuon1();

    // Fill Muon1 histograms
    for (const auto& corrName : corrNames_) {
        const auto& p4Muon1Iter = p4MapMuon1.find(corrName);
        if (p4Muon1Iter != p4MapMuon1.end()) {
            const auto& p4Muon1 = p4Muon1Iter->second;
            histMuon1Pt_[corrName]->Fill(p4Muon1.Pt(), weight);
        }
    }

}

void HistScale::FillPhoton(const ScalePhoton& scalePhoton, double weight)
{
    const auto& p4MapPhoton1 = scalePhoton.getP4MapPhoton1();

    // Fill Photon1 histograms
    for (const auto& corrName : corrNames_) {
        const auto& p4Photon1Iter = p4MapPhoton1.find(corrName);
        if (p4Photon1Iter != p4MapPhoton1.end()) {
            const auto& p4Photon1 = p4Photon1Iter->second;
            histPhoton1Pt_[corrName]->Fill(p4Photon1.Pt(), weight);
        }
    }

}

void HistScale::FillJetMet(const ScaleJetMet& scaleJetMet, double weight)
{
    const auto& p4MapJet1 = scaleJetMet.getP4MapJet1();
    const auto& getP4MapJetSum = scaleJetMet.getP4MapJetSum();

    // Fill Jet1 histograms
    for (const auto& corrName : jecNames_) {
        const auto& p4Jet1Iter = p4MapJet1.find(corrName);
        if (p4Jet1Iter != p4MapJet1.end()) {
            const auto& p4Jet1 = p4Jet1Iter->second;
            histJet1Pt_[corrName]->Fill(p4Jet1.Pt(), weight);
        }
    }

    // Fill JetSum histograms
    for (const auto& corrName : jecNames_) {
        const auto& p4JetSumIter = getP4MapJetSum.find(corrName);
        if (p4JetSumIter != getP4MapJetSum.end()) {
            const auto& p4JetSum = p4JetSumIter->second;
            histJetSumPt_[corrName]->Fill(p4JetSum.Pt(), weight);
        }
    }

    const auto& p4MapMet = scaleJetMet.getP4MapMet();

    // Fill MET histograms
    for (const auto& corrName : corrNames_) {
        const auto& p4MetIter = p4MapMet.find(corrName);
        if (p4MetIter != p4MapMet.end()) {
            const auto& p4Met = p4MetIter->second;
            histMetPt_[corrName]->Fill(p4Met.Pt(), weight);
            histMetPhi_[corrName]->Fill(p4Met.Phi(), weight);
        }
    }
    // Fill Jet+MET histograms
    for (const auto& corrName : corrNames_) {
        const auto& p4MetIter = p4MapMet.find(corrName);
        if (p4MetIter == p4MapMet.end()) continue;
        const auto& p4JetSumIter = getP4MapJetSum.find(corrName);
        if (p4JetSumIter == getP4MapJetSum.end()) continue;
        const auto& p4Met = p4MetIter->second;
        const auto& p4JetSum = p4JetSumIter->second;
        histJetMetSumPt_[corrName]->Fill((p4Met + p4JetSum).Pt(), weight);
    }
}


