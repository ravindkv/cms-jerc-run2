#include "HistTime.h"
#include "Helper.h"

HistTime::HistTime(TDirectory *origDir,  const std::string& directoryName, const VarBin& varBin, const std::vector<int>& pTRefs)
    : pTRefs_(pTRefs), runN_(200), runMin_(271030), runMax_(325200)
{
    InitializeHistograms(origDir, directoryName, varBin);
}

void HistTime::InitializeHistograms(TDirectory *origDir, const std::string& directoryName, const VarBin& varBin) {

    // Use the Helper method to get or create the directory
    std::string dirName = directoryName + "/HistTime";
    TDirectory* newDir = Helper::createTDirectory(origDir, dirName);
    newDir->cd();

    std::vector<double> rangeRun  = varBin.getRangeRun();
    runN_ = rangeRun.at(0);
    runMin_ = rangeRun.at(1);
    runMax_ = rangeRun.at(2);

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
            ";Run Number; DB Balance",
            runN_,
            runMin_,
            runMax_
        );
        
        std::string p1MpfRespName = "p1MpfRespInRunFor" + suffix;
        rh.p1MpfRespInRun = std::make_unique<TProfile>(
            p1MpfRespName.c_str(),
            ";Run Number; MPF Response",
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

        std::string p1JetChemefName = "p1JetChemefInRunFor" + suffix;
        rh.p1JetChemefInRun = std::make_unique<TProfile>(
            p1JetChemefName.c_str(),
            ";Run Number;Jet chEmEF",
            runN_,
            runMin_,
            runMax_
        );

        std::string p1JetMuefName = "p1JetMuefInRunFor" + suffix;
        rh.p1JetMuefInRun = std::make_unique<TProfile>(
            p1JetMuefName.c_str(),
            ";Run Number;Jet muEF",
            runN_,
            runMin_,
            runMax_
        );
        
        // Add to histMap_
        histMap_[pTRef] = std::move(rh);
        
        // Optionally, print initialization confirmation
        std::cout << "Initialized HistTime histograms for pTRef" << pTRef <<" in "<<dirName << std::endl;
    }
    origDir->cd();
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

            // Fill p1JetChemefInRun
            rh.p1JetChemefInRun->Fill(skimT->run, skimT->Jet_chEmEF[iJet1], weight);

            // Fill p1JetMuefInRun
            rh.p1JetMuefInRun->Fill(skimT->run, skimT->Jet_muEF[iJet1], weight);
        }
    }
}

