#include "HistBarrel.h"

#include <TDirectory.h>
#include "Helper.h"

HistBarrel::HistBarrel(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin)
{
    InitializeHistograms(origDir, directoryName, varBin);
}

void HistBarrel::InitializeHistograms(TDirectory* origDir, const std::string& directoryName, const VarBin& varBin)
{
    // Use the Helper method to get or create the directory
    std::string dirName = directoryName + "/HistBarrel";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    newDir->cd();

    // Retrieve binning configurations
    std::vector<double> binsPt  = varBin.getBinsPt();
    std::vector<double> binsPhi  = varBin.getBinsPhi();
    const int nPt  = binsPt.size() - 1;
    const int nPhi  = binsPhi.size() - 1;

    std::vector<double> rangeDeltaRadius  = varBin.getRangeDeltaR();
    int drN = rangeDeltaRadius.at(0);
    double drMin = rangeDeltaRadius.at(1);
    double drMax = rangeDeltaRadius.at(2);

    std::vector<double> rangeDeltaPhi  = varBin.getRangeDeltaPhi();
    int deltaPhiN = rangeDeltaPhi.at(0);
    double deltaPhiMin = rangeDeltaPhi.at(1);
    double deltaPhiMax = rangeDeltaPhi.at(2);

    // Initialize 1D Histograms
    hist_.h1EventInDeltaPhiJetRef = std::make_unique<TH1D>(
        "h1EventInDeltaPhiJetRef", "", deltaPhiN, deltaPhiMin, deltaPhiMax);

    hist_.h1EventInDeltaRadiusJetRef = std::make_unique<TH1D>(
        "h1EventInDeltaRadiusJetRef", "", drN, drMin, drMax);

    // Initialize TProfile Histograms
    hist_.p1GenJet1PtOverRefPtInRefPt = std::make_unique<TProfile>(
        "p1GenJet1PtOverRefPtInRefPt", "", nPt, binsPt.data());

    hist_.p1Jet1PtOverRefPtInRefPt = std::make_unique<TProfile>(
        "p1Jet1PtOverRefPtInRefPt", "", nPt, binsPt.data());

    hist_.p1Jet1PtOverGenJet1PtInGenJet1Pt = std::make_unique<TProfile>(
        "p1Jet1PtOverGenJet1PtInGenJet1Pt", "", nPt, binsPt.data());

    std::cout << "Initialized HistBarrel histograms in directory: " << dirName << std::endl;

    // Return to the original directory
    origDir->cd();
}

void HistBarrel::Fill(const TLorentzVector& p4Ref, const TLorentzVector& p4Jet1, const TLorentzVector& p4GenJet1, double weight)
{
    // Fill 1D Histograms
    double deltaPhi = Helper::DELTAPHI(p4Ref.Phi(), p4Jet1.Phi());
    hist_.h1EventInDeltaPhiJetRef->Fill(deltaPhi, weight);
    hist_.h1EventInDeltaRadiusJetRef->Fill(p4Ref.DeltaR(p4Jet1), weight);

    if (p4Ref.Pt() > 0) {
        hist_.p1Jet1PtOverRefPtInRefPt->Fill(p4Ref.Pt(), p4Jet1.Pt() / p4Ref.Pt(), weight);
        if (p4GenJet1.Pt() > 0) {
            hist_.p1GenJet1PtOverRefPtInRefPt->Fill(p4Ref.Pt(), p4GenJet1.Pt() / p4Ref.Pt(), weight);
            hist_.p1Jet1PtOverGenJet1PtInGenJet1Pt->Fill(p4GenJet1.Pt(), p4Jet1.Pt() / p4GenJet1.Pt(), weight);
        }

    }
}

