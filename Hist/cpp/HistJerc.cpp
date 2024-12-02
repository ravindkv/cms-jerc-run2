#include "HistJerc.h"

/**
 * @brief Constructs a HistJerc object and initializes histograms.
 * 
 * @param origDir Pointer to the original ROOT directory.
 * @param directoryName Name of the directory within the ROOT file to store histograms.
 * @param varBin Object defining variable binning.
 */
HistJerc::HistJerc(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin)
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
void HistJerc::InitializeHistograms(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin)
{
    // Create a subdirectory for HistJerc histograms
    std::string dirName = directoryName + "/HistJerc";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    newDir->cd();

    // Define binning from VarBin
    const std::vector<double> binsPt  = varBin.getBinsPt();
    const std::vector<double> binsPhi  = varBin.getBinsPhi();
    const int nPt  = binsPt.size()  - 1;
    const int nPhi  = binsPhi.size()  - 1;

    const std::vector<double> rangeMassJet1 = varBin.getRangeMassJet1();
    int massJet1N   = static_cast<int>(rangeMassJet1.at(0));
    double massJet1Min = rangeMassJet1.at(1);
    double massJet1Max = rangeMassJet1.at(2);

    const std::vector<double> rangeMassJetSum = varBin.getRangeMassJetSum();
    int massJetSumN   = static_cast<int>(rangeMassJetSum.at(0));
    double massJetSumMin = rangeMassJetSum.at(1);
    double massJetSumMax = rangeMassJetSum.at(2);

    // Initialize correction names (should match those in ApplyJerc)
    jecNames_ = {
        "Nano", "Raw", "L1RcCorr", "L2RelCorr", "L2L3ResCorr", "JerCorr"
    };

    // Initialize histograms for Jet1 Pt
    for (const auto& corrName : jecNames_) {
        std::string histNamePt = "h1EventInJet1Pt" + corrName;
        histJet1Pt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }

    // Initialize histograms for Jet1 Mass
    for (const auto& corrName : jecNames_) {
        std::string histNameMass = "h1EventInJet1Mass" + corrName;
        histJet1Mass_[corrName] = std::make_unique<TH1D>(
            histNameMass.c_str(), "", massJet1N, massJet1Min, massJet1Max);
    }

    // Initialize histograms for JetSum Pt
    for (const auto& corrName : jecNames_) {
        std::string histNamePt = "h1EventInJetSumPt" + corrName;
        histJetSumPt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }

    // Initialize histograms for JetSum Mass
    for (const auto& corrName : jecNames_) {
        std::string histNameMass = "h1EventInJetSumMass" + corrName;
        histJetSumMass_[corrName] = std::make_unique<TH1D>(
            histNameMass.c_str(), "", massJetSumN, massJetSumMin, massJetSumMax);
    }


    //MET from Nano and type1 corrected
    metNames_ = {
        "Nano", "Type1Corr"
    };
    // Initialize histograms for MET Pt
    for (const auto& corrName : metNames_) {
        std::string histNamePt = "h1EventInMetPt" + corrName;
        histMetPt_[corrName] = std::make_unique<TH1D>(
            histNamePt.c_str(), "", nPt, binsPt.data());
    }
    // Initialize histograms for MET Phi
    for (const auto& corrName : metNames_) {
        std::string histNamePhi = "h1EventInMetPhi" + corrName;
        histMetPhi_[corrName] = std::make_unique<TH1D>(
            histNamePhi.c_str(), "", nPhi, binsPhi.data());
    }

    std::cout << "Initialized HistJerc histograms in directory: " << dirName << std::endl;
    origDir->cd();
}

/**
 * @brief Fills the JEC histograms based on the corrected jet variables.
 * 
 * @param appliedJerc Instance of ApplyJerc containing corrected jet variables.
 * @param weight Event weight.
 */
void HistJerc::Fill(const ApplyJerc& appliedJerc, double weight)
{
    const auto& p4MapJet1 = appliedJerc.getP4MapJet1();
    const auto& getP4MapJetSum = appliedJerc.getP4MapJetSum();

    // Fill Jet1 histograms
    for (const auto& corrName : jecNames_) {
        const auto& p4Jet1Iter = p4MapJet1.find(corrName);
        if (p4Jet1Iter != p4MapJet1.end()) {
            const auto& p4Jet1 = p4Jet1Iter->second;
            histJet1Pt_[corrName]->Fill(p4Jet1.Pt(), weight);
            histJet1Mass_[corrName]->Fill(p4Jet1.M(), weight);
        }
    }

    // Fill JetSum histograms
    for (const auto& corrName : jecNames_) {
        const auto& p4JetSumIter = getP4MapJetSum.find(corrName);
        if (p4JetSumIter != getP4MapJetSum.end()) {
            const auto& p4JetSum = p4JetSumIter->second;
            histJetSumPt_[corrName]->Fill(p4JetSum.Pt(), weight);
            histJetSumMass_[corrName]->Fill(p4JetSum.M(), weight);
        }
    }

    const auto& p4MapMet = appliedJerc.getP4MapMet();

    // Fill MET histograms
    for (const auto& corrName : metNames_) {
        const auto& p4MetIter = p4MapMet.find(corrName);
        if (p4MetIter != p4MapMet.end()) {
            const auto& p4Met = p4MetIter->second;
            histMetPt_[corrName]->Fill(p4Met.Pt(), weight);
            histMetPhi_[corrName]->Fill(p4Met.Phi(), weight);
        }
    }
}

