#include "HistObjectId.h"

/**
 * @brief Constructs a HistObjectId object and initializes histograms.
 * 
 * @param origDir Pointer to the original ROOT directory.
 * @param directoryName Name of the directory within the ROOT file to store histograms.
 * @param varBin Object defining variable binning.
 */
HistObjectId::HistObjectId(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin)
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
void HistObjectId::InitializeHistograms(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin)
{
    // Create a subdirectory for HistObjectId histograms
    std::string dirName = directoryName + "/HistObjectId";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    newDir->cd();

    // Define binning from VarBin
    const std::vector<double> binsPt  = varBin.getBinsPt();
    const std::vector<double> binsEta  = varBin.getBinsEta();
    const std::vector<double> binsPhi  = varBin.getBinsPhi();
    const int nPt   = binsPt.size()  - 1;
    const int nEta  = binsEta.size()  - 1;
    const int nPhi  = binsPhi.size()  - 1;

    std::vector<double> rangeCountRef = varBin.getRangeCountRef();
    int countN   = rangeCountRef.at(0);
    double countMin = rangeCountRef.at(1);
    double countMax = rangeCountRef.at(2);

    std::vector<double> rangeR9 = varBin.getRangeResponse();
    int r9N   = rangeR9.at(0);
    double r9Min = rangeR9.at(1);
    double r9Max = rangeR9.at(2);

    //Ele
    histEle_.h2EventInElePtEleEta = std::make_unique<TH2D>(
        "h2EventInElePtEleEta", "", nEta, binsEta.data(), nPt, binsPt.data());

    //Photon
    histPho_.h2EventInPhoEtaPhoPt = std::make_unique<TH2D>(
        "h2EventInPhoEtaPhoPt", "", nPt, binsPt.data(), nEta, binsEta.data());
    histPho_.h2EventInPhoCountPhoPt = std::make_unique<TH2D>(
        "h2EventInPhoCountPhoPt", "", nPt, binsPt.data(), countN, countMin, countMax);
    histPho_.h2EventInPhoR9PhoPt = std::make_unique<TH2D>(
        "h2EventInPhoR9PhoPt", "", nPt, binsPt.data(), r9N, r9Min, r9Max);
    histPho_.h2EventInPhoHoePhoPt = std::make_unique<TH2D>(
        "h2EventInPhoHoePhoPt", "", nPt, binsPt.data(), r9N, r9Min, r9Max/100);
    histPho_.h2EventInPhoIdPhoPt = std::make_unique<TH2D>(
        "h2EventInPhoIdPhoPt", "", nPt, binsPt.data(), countN, countMin, countMax);

    //Muon
    histMu_.h2EventInMuPtMuEta = std::make_unique<TH2D>(
        "h2EventInMuPtMuEta", "", nEta, binsEta.data(), nPt, binsPt.data());

    //Jet
    histJet_.h2EventInJetPtJetEta = std::make_unique<TH2D>(
        "h2EventInJetPtJetEta", "", nEta, binsEta.data(), nPt, binsPt.data());

    //Met
    histMet_.h2EventInMetPtMetPhi = std::make_unique<TH2D>(
        "h2EventInMetPtMetPhi", "", nPhi, binsPhi.data(), nPt, binsPt.data());

    std::cout << "Initialized HistObjectId histograms in directory: " << dirName << std::endl;
    origDir->cd();
}


// Fill
void HistObjectId::FillElectron(const SkimTree& skimT, double weight)
{
    for (int ind = 0; ind < skimT.nElectron; ++ind) {
        double pt = skimT.Electron_pt[ind];
        double eta = skimT.Electron_eta[ind];
        histEle_.h2EventInElePtEleEta->Fill(eta, pt, weight);
    }
}

void HistObjectId::FillPhoton(const SkimTree& skimT, double weight)
{
    for (int ind = 0; ind < skimT.nPhoton; ++ind) {
        double pt = skimT.Photon_pt[ind];
        double eta = skimT.Photon_eta[ind];
        histPho_.h2EventInPhoEtaPhoPt->Fill(pt, eta, weight);
        histPho_.h2EventInPhoCountPhoPt->Fill(pt, skimT.nPhoton, weight);
        histPho_.h2EventInPhoR9PhoPt->Fill(pt, skimT.Photon_r9[ind], weight);
        histPho_.h2EventInPhoHoePhoPt->Fill(pt, skimT.Photon_hoe[ind], weight);
        histPho_.h2EventInPhoIdPhoPt->Fill(pt, skimT.Photon_cutBased[ind], weight);
    }
}

void HistObjectId::FillMuon(const SkimTree& skimT, double weight)
{
    for (int ind = 0; ind < skimT.nMuon; ++ind) {
        double pt = skimT.Muon_pt[ind];
        double eta = skimT.Muon_eta[ind];
        histMu_.h2EventInMuPtMuEta->Fill(eta, pt, weight);
    }
}

void HistObjectId::FillJet(const SkimTree& skimT, double weight)
{
    for (int ind = 0; ind < skimT.nJet; ++ind) {
        double pt = skimT.Jet_pt[ind];
        double eta = skimT.Jet_eta[ind];
        histJet_.h2EventInJetPtJetEta->Fill(eta, pt, weight);
    }
}

void HistObjectId::FillMet(const SkimTree& skimT, double weight)
{
    double pt   = skimT.ChsMET_pt;
    double phi  = skimT.ChsMET_phi;
    histMet_.h2EventInMetPtMetPhi->Fill(phi, pt, weight);
}
