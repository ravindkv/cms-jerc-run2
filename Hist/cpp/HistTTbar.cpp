#include "HistTTbar.h"
#include "Helper.h"

HistTTbar::HistTTbar(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin)
{
    InitializeHistograms(origDir, directoryName, varBin);
}

void HistTTbar::InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin)
{
    // Use the Helper method to get or create the directory
    std::string dirName = directoryName + "/HistTTbar";
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

    std::vector<double> rangeMassTop = varBin.getRangeMassTop();
    const int nMassTop= rangeMassTop.at(0); 
    const int minMassTop= rangeMassTop.at(1); 
    const int maxMassTop= rangeMassTop.at(2); 

    std::vector<double> rangeChiSqr = varBin.getRangeChiSqr();
    const int nChiSqr= rangeChiSqr.at(0); 
    const int minChiSqr= rangeChiSqr.at(1); 
    const int maxChiSqr= rangeChiSqr.at(2); 

    std::vector<double> rangeRun  = varBin.getRangeRun();
    const int runN = rangeRun.at(0);
    const int runMin = rangeRun.at(1);
    const int runMax = rangeRun.at(2);

    // Initialize 1D Histograms
    hist_.h1EventInHadWPt = std::make_unique<TH1D>("h1EventInHadWPt", "", nPt, binsPt.data());
    hist_.h1EventInAvgHadPt = std::make_unique<TH1D>("h1EventInAvgHadPt", "", nPt, binsPt.data());
    hist_.h1EventInHadWEta = std::make_unique<TH1D>("h1EventInHadWEta", "", nEta, binsEta.data());
    hist_.h1EventInHadWPhi = std::make_unique<TH1D>("h1EventInHadWPhi", "", nPhi, binsPhi.data());
    hist_.h1EventInHadWMass = std::make_unique<TH1D>("h1EventInHadWMass", "", nMass, binsMass.data());
    
    hist_.h1EventInHadTPt = std::make_unique<TH1D>("h1EventInHadTPt", "", nPt, binsPt.data());
    hist_.h1EventInHadTEta = std::make_unique<TH1D>("h1EventInHadTEta", "", nEta, binsEta.data());
    hist_.h1EventInHadTPhi = std::make_unique<TH1D>("h1EventInHadTPhi", "", nPhi, binsPhi.data());
    hist_.h1EventInHadTMass = std::make_unique<TH1D>("h1EventInHadTMass", "", nMassTop, minMassTop, maxMassTop);
    hist_.h1EventInChiSqr = std::make_unique<TH1D>("h1EventInChiSqr", "", nChiSqr, minChiSqr, maxChiSqr);
    
    hist_.h1EventInLepTPt = std::make_unique<TH1D>("h1EventInLepTPt", "", nPt, binsPt.data());
    hist_.h1EventInLepTEta = std::make_unique<TH1D>("h1EventInLepTEta", "", nEta, binsEta.data());
    hist_.h1EventInLepTPhi = std::make_unique<TH1D>("h1EventInLepTPhi", "", nPhi, binsPhi.data());
    hist_.h1EventInLepTMass = std::make_unique<TH1D>("h1EventInLepTMass", "", nMassTop, minMassTop, maxMassTop);
    
    // Initialize TProfile Histograms
    hist_.p1HadWMassInHadWPt = std::make_unique<TProfile>(
        "p1HadWMassInHadWPt", "", nPt, binsPt.data());
    hist_.p1HadWMassInAvgHadPt = std::make_unique<TProfile>(
        "p1HadWMassInAvgHadPt", "", nPt, binsPt.data());
    hist_.p1HadWMassInHadWEta = std::make_unique<TProfile>(
        "p1HadWMassInHadWEta", "", nEta, binsEta.data());
    hist_.p1HadWMassInHadWPhi = std::make_unique<TProfile>(
        "p1HadWMassInHadWPhi", "", nPhi, binsPhi.data());
    
    hist_.p1HadTMassInHadTPt = std::make_unique<TProfile>(
        "p1HadTMassInHadTPt", "", nPt, binsPt.data());
    hist_.p1HadTMassInHadTEta = std::make_unique<TProfile>(
        "p1HadTMassInHadTEta", "", nEta, binsEta.data());
    hist_.p1HadTMassInHadTPhi = std::make_unique<TProfile>(
        "p1HadTMassInHadTPhi", "", nPhi, binsPhi.data());
    
    hist_.p1LepTMassInLepTPt = std::make_unique<TProfile>(
        "p1LepTMassInLepTPt", "", nPt, binsPt.data());
    hist_.p1LepTMassInLepTEta = std::make_unique<TProfile>(
        "p1LepTMassInLepTEta", "", nEta, binsEta.data());
    hist_.p1LepTMassInLepTPhi = std::make_unique<TProfile>(
        "p1LepTMassInLepTPhi", "", nPhi, binsPhi.data());

    hist_.p1HadWMassInRun = std::make_unique<TProfile>(
        "p1HadWMassInRun", "", runN, runMin, runMax);
    hist_.p1HadTMassInRun = std::make_unique<TProfile>(
        "p1HadTMassInRun", "", runN, runMin, runMax);
    hist_.p1LepTMassInRun = std::make_unique<TProfile>(
        "p1LepTMassInRun", "", runN, runMin, runMax);
    
    std::cout << "Initialized HistTTbar histograms in directory: " << dirName << std::endl;
    origDir->cd();
}

void HistTTbar::Fill(const TLorentzVector& p4HadW, const TLorentzVector& p4HadT, const TLorentzVector& p4LepT, double avgHadPt, double chiSqr, double run, double weight)
{
    // Fill h1EventInHadWPt
    hist_.h1EventInHadWPt->Fill(p4HadW.Pt(), weight);
    hist_.h1EventInAvgHadPt->Fill(avgHadPt, weight);
    hist_.h1EventInHadWEta->Fill(p4HadW.Eta(), weight);
    hist_.h1EventInHadWPhi->Fill(p4HadW.Phi(), weight);
    hist_.h1EventInHadWMass->Fill(p4HadW.M(), weight);

    hist_.h1EventInHadTPt->Fill(p4HadT.Pt(), weight);
    hist_.h1EventInHadTEta->Fill(p4HadT.Eta(), weight);
    hist_.h1EventInHadTPhi->Fill(p4HadT.Phi(), weight);
    hist_.h1EventInHadTMass->Fill(p4HadT.M(), weight);

    hist_.h1EventInLepTPt->Fill(p4LepT.Pt(), weight);
    hist_.h1EventInLepTEta->Fill(p4LepT.Eta(), weight);
    hist_.h1EventInLepTPhi->Fill(p4LepT.Phi(), weight);
    hist_.h1EventInLepTMass->Fill(p4LepT.M(), weight);

    hist_.h1EventInChiSqr->Fill(chiSqr, weight);

    hist_.p1HadWMassInHadWPt->Fill(p4HadW.Pt(), p4HadW.M(), weight);
    hist_.p1HadWMassInAvgHadPt->Fill(avgHadPt, p4HadW.M(), weight);
    hist_.p1HadWMassInHadWEta->Fill(p4HadW.Eta(), p4HadW.M(), weight);
    hist_.p1HadWMassInHadWPhi->Fill(p4HadW.Phi(), p4HadW.M(), weight);
    
    hist_.p1HadTMassInHadTPt->Fill(p4HadT.Pt(), p4HadT.M(), weight);
    hist_.p1HadTMassInHadTEta->Fill(p4HadT.Eta(), p4HadT.M(), weight);
    hist_.p1HadTMassInHadTPhi->Fill(p4HadT.Phi(), p4HadT.M(), weight);
    
    hist_.p1LepTMassInLepTPt->Fill(p4LepT.Pt(), p4LepT.M(), weight);
    hist_.p1LepTMassInLepTEta->Fill(p4LepT.Eta(), p4LepT.M(), weight);
    hist_.p1LepTMassInLepTPhi->Fill(p4LepT.Phi(), p4LepT.M(), weight);
    
    hist_.p1HadWMassInRun->Fill(run, p4HadW.M(), weight);
    hist_.p1HadTMassInRun->Fill(run, p4HadT.M(), weight);
    hist_.p1LepTMassInRun->Fill(run, p4LepT.M(), weight);
}
