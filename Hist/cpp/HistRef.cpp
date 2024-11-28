#include "HistRef.h"
#include "Helper.h"

HistRef::HistRef(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin)
{
    InitializeHistograms(origDir, directoryName, varBin);
}

void HistRef::InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin)
{
    // Use the Helper method to get or create the directory
    std::string dirName = directoryName + "/HistBarrel";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    newDir->cd();

    std::vector<double> binsPt  = varBin.getBinsPt();
    std::vector<double> binsEta = varBin.getBinsEta();
    std::vector<double> binsPhi = varBin.getBinsPhi();
    std::vector<double> binsMass = varBin.getBinsMass();

    const int nPt  = binsPt.size()  - 1;
    const int nEta = binsEta.size() - 1;
    const int nPhi = binsPhi.size() - 1;
    const int nMass= binsMass.size() - 1;

    std::vector<double> rangeCountRef = varBin.getRangeCountRef();
    int countRefN   = rangeCountRef.at(0);
    int countRefMin = rangeCountRef.at(1);
    int countRefMax = rangeCountRef.at(2);
    std::vector<double> rangePhiRebin = varBin.getRangePhiRebin();
    int phiRebinN   = rangePhiRebin.at(0);
    int phiRebinMin = rangePhiRebin.at(1);
    int phiRebinMax = rangePhiRebin.at(2);

    // Initialize 1D Histograms
    hist_.h1EventInRefPt = std::make_unique<TH1D>("h1EventInRefPt", "", nPt, binsPt.data());
    hist_.h1EventInRefMass = std::make_unique<TH1D>("h1EventInRefMass", "", nMass, binsMass.data());
    hist_.h1EventInRefPtForRefBarrel = std::make_unique<TH1D>("h1EventInRefPtForRefBarrel", "",nPt, binsPt.data());
    hist_.h1EventInGenRefPtForGenRefBarrel = std::make_unique<TH1D>("h1EventInGenRefPtForGenRefBarrel", "", nPt, binsPt.data());
    
    // Initialize 2D Histograms
    hist_.h2EventInRefPhiRefEta = std::make_unique<TH2D>(
        "h2EventInRefPhiRefEta", "", nEta, binsEta.data(), nPhi, binsPhi.data());
    
    // Rebinning histogram: increasing the range in eta and phi
    hist_.h2EventInRefPhiRebinRefEta = std::make_unique<TH2D>(
        "h2EventInRefPhiRebinRefEta", "", nEta, binsEta.data(), phiRebinN, phiRebinMin, phiRebinMax);
    
    hist_.h2EventInRefPhiRefEtaForRefPt110 = std::make_unique<TH2D>(
        "h2EventInRefPhiRefEtaForRefPt110", "", nEta, binsEta.data(), nPhi, binsPhi.data());
    
    hist_.h2EventInRefPhiRebinRefEtaForRefPt110 = std::make_unique<TH2D>(
        "h2EventInRefPhiRebinRefEtaForRefPt110", "", nEta, binsEta.data(), phiRebinN, phiRebinMin, phiRebinMax);
    
    hist_.h2EventInCountRefRefPt = std::make_unique<TH2D>(
        "h2EventInCountRefRefPt", "", nPt, binsPt.data(), countRefN, countRefMin, countRefMax);
    
    // Initialize TProfile Histograms
    hist_.p1RefMassInRefPt = std::make_unique<TProfile>(
        "p1RefMassInRefPt", "", nPt, binsPt.data());
    hist_.p1RefPtOverGenRefPtInGenRefPt = std::make_unique<TProfile>(
        "p1RefPtOverGenRefPtInGenRefPt", "", nPt, binsPt.data());
    
    hist_.p1GenRefPtOverRefPtInRefPt = std::make_unique<TProfile>(
        "p1GenRefPtOverRefPtInRefPt", "", nPt, binsPt.data());
    
    std::cout << "Initialized HistRef histograms in directory: " << dirName << std::endl;
    origDir->cd();
}

void HistRef::Fill(const int& nRef, const TLorentzVector& p4Ref, const TLorentzVector& p4GenRef, 
                    double weight)
{
    double ptRef = p4Ref.Pt();
    hist_.h2EventInCountRefRefPt->Fill(ptRef, nRef, weight);
    
    // Fill h1EventInRefPt
    hist_.h1EventInRefPt->Fill(ptRef, weight);
    hist_.h1EventInRefMass->Fill(p4Ref.M(), weight);
    hist_.p1RefMassInRefPt->Fill(p4Ref.Pt(), p4Ref.M(), weight);
    
    hist_.h2EventInRefPhiRefEta->Fill(p4Ref.Eta(), p4Ref.Phi(), weight);
    hist_.h2EventInRefPhiRebinRefEta->Fill(p4Ref.Eta(), p4Ref.Phi(), weight);
    
    // Conditional filling for RefPt >= 110
    if (ptRef >= 110) {
        hist_.h2EventInRefPhiRefEtaForRefPt110->Fill(p4Ref.Eta(), p4Ref.Phi(), weight);
        hist_.h2EventInRefPhiRebinRefEtaForRefPt110->Fill(p4Ref.Eta(), p4Ref.Phi(), weight);
    }
    
	// Gen objects handling
	if (p4GenRef.Pt() > 0 && fabs(p4GenRef.Eta()) < 1.3) {
		hist_.h1EventInGenRefPtForGenRefBarrel->Fill(p4GenRef.Pt(), weight);
	}
    if (fabs(p4Ref.Eta()) < 1.3) {
        hist_.h1EventInRefPtForRefBarrel->Fill(ptRef, weight);
        if (p4GenRef.Pt() > 0) {
           hist_.p1RefPtOverGenRefPtInGenRefPt->Fill(p4GenRef.Pt(), ptRef / p4GenRef.Pt(), weight);
           hist_.p1GenRefPtOverRefPtInRefPt->Fill(ptRef, p4GenRef.Pt() / ptRef, weight);
        }
    }
}
