#include "HistTime.h"

HistTime::HistTime(TFile* fout, const std::string& directoryName, const std::vector<int>& pTRefs, GlobalFlag &globalFlags)
    : pTRefs_(pTRefs), globalFlags_(globalFlags), runN_(200), runMin_(271030), runMax_(325200)
{
    InitializeHistograms(fout, directoryName);
}

void HistTime::InitializeHistograms(TFile* fout, const std::string& directoryName) {
    // Create the directory within the ROOT file
    fout->mkdir(directoryName.c_str());
    fout->cd(directoryName.c_str());
    if (globalFlags_.getYear()== GlobalFlag::Year::Year2016Pre){
        runMin_ = 271036;
        runMax_ = 284044;
    }
    if (globalFlags_.getYear()== GlobalFlag::Year::Year2016Post){
        runMin_ = 271036;
        runMax_ = 284044;
    }
    if (globalFlags_.getYear()== GlobalFlag::Year::Year2017){
        runMin_ = 294927;
        runMax_ = 306462;
    }
    if (globalFlags_.getYear()== GlobalFlag::Year::Year2018){
        runMin_ = 314472;
        runMax_ = 325175;
    }
    std::cout<<"HistTime: Run range = "<<runMin_<<", "<<runMax_<<'\n';
    
    // Initialize histograms for each pTRef threshold
    for (const auto& pTRef : pTRefs_) {
        // Create RunHistograms struct
        RunHistograms rh;
        
        // Define histogram suffix based on pTRef
        std::string suffix = "RefPt" + std::to_string(pTRef);
        
        // Initialize TH1D histograms
        std::string h1Name = "h1EventInRunFor" + suffix;
        rh.h1EventInRun = std::make_unique<TH1D>(
            h1Name.c_str(),
            ";Run Number;Events",
            runN_,
            runMin_,
            runMax_
        );
        
        // Initialize TProfile histograms
        std::string p1DbRespName = "p1DbRespInRunFor" + suffix;
        rh.p1DbRespInRun = std::make_unique<TProfile>(
            p1DbRespName.c_str(),
            ";Run Number;Balance Factor",
            runN_,
            runMin_,
            runMax_
        );
        
        std::string p1MpfRespName = "p1MpfRespInRunFor" + suffix;
        rh.p1MpfRespInRun = std::make_unique<TProfile>(
            p1MpfRespName.c_str(),
            ";Run Number;Momentum Fraction",
            runN_,
            runMin_,
            runMax_
        );
        
        std::string p1JetChhefName = "p1JetChhefInRunFor" + suffix;
        rh.p1JetChhefInRun = std::make_unique<TProfile>(
            p1JetChhefName.c_str(),
            ";Run Number;Jet chHEF",
            runN_,
            runMin_,
            runMax_
        );
        
        std::string p1JetNehefName = "p1JetNehefInRunFor" + suffix;
        rh.p1JetNehefInRun = std::make_unique<TProfile>(
            p1JetNehefName.c_str(),
            ";Run Number;Jet neHEF",
            runN_,
            runMin_,
            runMax_
        );
        
        std::string p1JetNeemefName = "p1JetNeemefInRunFor" + suffix;
        rh.p1JetNeemefInRun = std::make_unique<TProfile>(
            p1JetNeemefName.c_str(),
            ";Run Number;Jet neEmEF",
            runN_,
            runMin_,
            runMax_
        );
        
        // Add to histMap_
        histMap_[pTRef] = std::move(rh);
        
        // Optionally, print initialization confirmation
        std::cout << "Initialized histograms for pTRef " << pTRef << std::endl;
    }
}

void HistTime::Fill(SkimTree* skimT, int iJet1, double bal, double mpf, double ptRef, double weight) {
    // Loop over each pTRef threshold
    for (const auto& pTRef : pTRefs_) {
        if (ptRef > pTRef) {
            // Retrieve the corresponding RunHistograms
            auto it = histMap_.find(pTRef);
            if (it == histMap_.end()) {
                std::cerr << "Error: pTRef " << pTRef << " not found in histMap_." << std::endl;
                continue;
            }
            RunHistograms& rh = it->second;
            
            // Fill h1EventInRun
            rh.h1EventInRun->Fill(skimT->run, weight);
            
            // Fill p1DbRespInRun
            rh.p1DbRespInRun->Fill(skimT->run, bal, weight);
            
            // Fill p1MpfRespInRun
            rh.p1MpfRespInRun->Fill(skimT->run, mpf, weight);
            
            // Fill p1JetChhefInRun
            rh.p1JetChhefInRun->Fill(skimT->run, skimT->Jet_chHEF[iJet1], weight);
            
            // Fill p1JetNehefInRun
            rh.p1JetNehefInRun->Fill(skimT->run, skimT->Jet_neHEF[iJet1], weight);
            
            // Fill p1JetNeemefInRun
            rh.p1JetNeemefInRun->Fill(skimT->run, skimT->Jet_neEmEF[iJet1], weight);
        }
    }
}

