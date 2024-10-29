#include "HistRef.h"

HistRef::HistRef(TFile* fout, const std::string& directoryName,
                 int nPtBins, const double* binsPt)
{
    InitializeHistograms(fout, directoryName, nPtBins, binsPt);
}

void HistRef::InitializeHistograms(TFile* fout, const std::string& directoryName,
                                   int nPtBins, const double* binsPt)
{
    // Create the directory within the ROOT file
    if (!fout->GetDirectory(directoryName.c_str())) {
        fout->mkdir(directoryName.c_str());
    }
    fout->cd(directoryName.c_str());
    
    // Initialize 1D Histograms
    hist_.h1EventInRefPt = std::make_unique<TH1D>("h1EventInRefPt", "", 197, 15, 1000);
    hist_.h1EventInRefMass = std::make_unique<TH1D>("h1EventInRefMass", "", 197, 15, 1000);
    hist_.h1EventInRefPtBarrel = std::make_unique<TH1D>("h1EventInRefPtBarrel", "", 197, 15, 1000);
    hist_.h1EventInGenRefPt = std::make_unique<TH1D>("h1EventInGenRefPt", "", nPtBins, binsPt);
    
    // Initialize 2D Histograms
    hist_.h2EventInRefEtaRefPhi = std::make_unique<TH2D>(
        "h2EventInRefEtaRefPhi", "", 30, -1.305, 1.305, 72, -TMath::Pi(), TMath::Pi());
    
    // Rebinning histogram: increasing the range in eta and phi
    hist_.h2EventInRefEtaRefPhiRebin = std::make_unique<TH2D>(
        "h2EventInRefEtaRefPhiRebin", "", 360, -3.132, 3.132, 360, -TMath::Pi(), TMath::Pi());
    
    hist_.h2EventInRefEtaRefPhiForRefPt110 = std::make_unique<TH2D>(
        "h2EventInRefEtaRefPhiForRefPt110", "", 30, -1.305, 1.305, 72, -TMath::Pi(), TMath::Pi());
    
    hist_.h2EventInRefEtaRefPhiForRefPt110Rebin = std::make_unique<TH2D>(
        "h2EventInRefEtaRefPhiForRefPt110Rebin", "", 360, -3.132, 3.132, 360, -TMath::Pi(), TMath::Pi());
    
    hist_.h2EventInRefPtCountRef = std::make_unique<TH2D>(
        "h2EventInRefPtCountRef", "", nPtBins, binsPt, 5, 0, 5);
    
    // Initialize TProfile Histograms
    hist_.p1RefMassInRefPt = std::make_unique<TProfile>(
        "p1RefMassInRefPt", "", nPtBins, binsPt);
    hist_.p1RefPtOverGenRefPtInGenRefPt = std::make_unique<TProfile>(
        "p1RefPtOverGenRefPtInGenRefPt", "", nPtBins, binsPt);
    
    hist_.p1GenRefPtOverRefPtInRefPt = std::make_unique<TProfile>(
        "p1GenRefPtOverRefPtInRefPt", "", nPtBins, binsPt);
    
    std::cout << "Initialized HistRef histograms in directory: " << directoryName << std::endl;
}

void HistRef::Fill(const int& nRef, const TLorentzVector& p4Ref, const TLorentzVector& p4GenRef, 
                    double weight)
{
    double ptRef = p4Ref.Pt();
    hist_.h2EventInRefPtCountRef->Fill(ptRef, nRef, weight);
    
    // Fill h1EventInRefPt
    hist_.h1EventInRefPt->Fill(ptRef, weight);
    hist_.h1EventInRefMass->Fill(p4Ref.M(), weight);
    hist_.p1RefMassInRefPt->Fill(p4Ref.Pt(), p4Ref.M(), weight);
    
    // Fill h2EventInRefEtaRefPhi and h2EventInRefEtaRefPhiRebin
    hist_.h2EventInRefEtaRefPhi->Fill(p4Ref.Eta(), p4Ref.Phi(), weight);
    hist_.h2EventInRefEtaRefPhiRebin->Fill(p4Ref.Eta(), p4Ref.Phi(), weight);
    
    // Conditional filling for RefPt >= 110
    if (ptRef >= 110) {
        hist_.h2EventInRefEtaRefPhiForRefPt110->Fill(p4Ref.Eta(), p4Ref.Phi(), weight);
        hist_.h2EventInRefEtaRefPhiForRefPt110Rebin->Fill(p4Ref.Eta(), p4Ref.Phi(), weight);
    }
    
	// Gen objects handling
	if (p4GenRef.Pt() > 0 && fabs(p4GenRef.Eta()) < 1.3) {
		hist_.h1EventInGenRefPt->Fill(p4GenRef.Pt(), weight);
	}
    if (fabs(p4Ref.Eta()) < 1.3) {
        hist_.h1EventInRefPtBarrel->Fill(ptRef, weight);
        if (p4GenRef.Pt() > 0) {
           hist_.p1RefPtOverGenRefPtInGenRefPt->Fill(p4GenRef.Pt(), ptRef / p4GenRef.Pt(), weight);
           hist_.p1GenRefPtOverRefPtInRefPt->Fill(ptRef, p4GenRef.Pt() / ptRef, weight);
        }
    }
}
