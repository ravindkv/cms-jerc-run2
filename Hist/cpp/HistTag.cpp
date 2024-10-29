#include "HistTag.h"

HistTag::HistTag(TFile* fout, const std::string& directoryName, int nPtBins, const std::vector<double> binsPt, GlobalFlag& globalFlags)
    : 
     threshBtagDeepFlavB_(1.0),
     threshBtagDeepFlavCvL_(1.0),
     threshBtagDeepFlavCvB_(1.0),
     threshBtagDeepFlavG_(1.0),
     threshBtagDeepFlavQG_(1.0),
     threshBtagDeepFlavUDS_(1.0),
    year_(globalFlags.getYear()),
    isMC_(globalFlags.isMC()),
    isDebug_(globalFlags.isDebug())
{
    if (year_ == GlobalFlag::Year::Year2016Pre) {
        threshBtagDeepFlavB_    = 0.6502;
        threshBtagDeepFlavCvL_  = 0.270;
        threshBtagDeepFlavCvB_  = 0.256;
        threshBtagDeepFlavQG_    = 0.5;
        threshBtagDeepFlavG_    = 0.5;
    }
    if (year_ == GlobalFlag::Year::Year2016Post) {
        threshBtagDeepFlavB_    = 0.6377;
        threshBtagDeepFlavCvL_  = 0.269;
        threshBtagDeepFlavCvB_  = 0.247;
        threshBtagDeepFlavQG_    = 0.5;
        threshBtagDeepFlavG_    = 0.5;
    }
    if (year_ == GlobalFlag::Year::Year2017) {
        threshBtagDeepFlavB_    = 0.7476;
        threshBtagDeepFlavCvL_  = 0.520;
        threshBtagDeepFlavCvB_  = 0.050; 
        threshBtagDeepFlavQG_    = 0.5;
        threshBtagDeepFlavG_    = 0.5;
    }
    if (year_ == GlobalFlag::Year::Year2018) {
        threshBtagDeepFlavB_    = 0.7100;
        threshBtagDeepFlavCvL_  = 0.282;
        threshBtagDeepFlavCvB_  = 0.267;
        threshBtagDeepFlavQG_    = 0.5;
        threshBtagDeepFlavG_    = 0.5;
    }

    // Initialize histograms and the mvar_ map
    InitializeHistograms(fout, directoryName, nPtBins, binsPt);
}

void HistTag::InitializeHistograms(TFile* fout, const std::string& directoryName, int nPtBins, const std::vector<double> binsPt) {
    // Create the directory within the ROOT file
    fout->mkdir(directoryName.c_str());
    fout->cd(directoryName.c_str());

    // Define the list of variables to initialize
    std::vector<std::string> variables = {
        "h1EventInRefPt",
        "h1EventInGenJet1Pt",
        "p1MpfRespInRefPt",
        "p1DbRespInRefPt",
        "p1MpfResp1InRefPt",
        "p1MpfRespNInRefPt",
        "p1MpfRespUInRefPt",
        "p1RhoInRefPt",
        "p1Jet1PtORefPtInRefPt",
        "p1GenJet1PtORefPtInRefPt",
        "p1Jet1PtOGenJet1PtInGenJet1Pt"
    };

    // Initialize tag and flavor identifiers
    atag_ = {"i", "b", "c", "q", "g", "n"};
    aflv_ = {"i", "b", "c", "q", "s", "ud", "g", "n"};

    // Loop over all combinations of variables, tags, and flavors to initialize histograms
    for (const auto& var : variables) {
        mvar[var] = 1.0;//initialize
        for (const auto& tag : atag_) {
            for (const auto& flv : aflv_) {
                // Construct the histogram name based on the naming convention
                std::string histName = var + "_" + tag + flv;
                
                // Initialize TH1D or TProfile based on the variable type
                if (var == "h1EventInGenJet1Pt" || var == "h1EventInRefPt") {
                    varTagFlvMap_[var][tag][flv] = std::make_unique<TH1D>(
                        histName.c_str(),
                        ";#it{p}_{T} (GeV);Events",
                        nPtBins,
                        binsPt.data()
                    );
                } else {
                    varTagFlvMap_[var][tag][flv] = std::make_unique<TProfile>(
                        histName.c_str(),
                        ";#it{p}_{T} (GeV);Value",
                        nPtBins,
                        binsPt.data()
                    );
                }
            }//aflv_
        }//atag_
    }//variables
}

void HistTag::FillHistograms(
    SkimTree* skimT,
    double ptRef,
    int iJet1,
    int iGenJet,
    double weight
) {
    // Determine the flavor of the jet
    int flv = (isMC_ ? skimT->GenJet_partonFlavour[iGenJet] : 99);

    // Evaluate tagging conditions based on jet properties
    bool isb = (skimT->Jet_btagDeepFlavB[iJet1] > threshBtagDeepFlavB_);
    bool isc = (skimT->Jet_btagDeepFlavCvL[iJet1] > threshBtagDeepFlavCvL_ 
                && skimT->Jet_btagDeepFlavCvB[iJet1] > threshBtagDeepFlavCvB_ 
                && !isb);
    bool isq = (skimT->Jet_btagDeepFlavQG[iJet1] > threshBtagDeepFlavQG_ 
                && !isb 
                && !isc);
    bool isg = (skimT->Jet_btagDeepFlavG[iJet1] > threshBtagDeepFlavG_ 
                && !isb 
                && !isc);
    bool isn = (!isb && !isc && !isq && !isg);

    // Loop over all variables to fill the corresponding histograms
    for (const auto& var_pair : mvar) {
        const std::string& var = var_pair.first;
        double val = var_pair.second;

        for (const auto& tag : atag_) {
            for (const auto& flv_str : aflv_) {
                // Apply the selection criteria based on flavor and tag
                if (((flv_str == "i") ||
                    (flv_str == "b" && abs(flv) == 5) ||
                    (flv_str == "c" && abs(flv) == 4) ||
                    (flv_str == "q" && abs(flv) <= 3 && flv != 0) ||
                    (flv_str == "s" && abs(flv) == 3) ||
                    (flv_str == "ud" && abs(flv) <= 2 && flv != 0) ||
                    (flv_str == "g" && flv == 21) ||
                    (flv_str == "n" && flv == 0)) &&
                    ((tag == "i") ||
                    (tag == "b" && isb) ||
                    (tag == "c" && isc) ||
                    (tag == "q" && isq) ||
                    (tag == "g" && isg) ||
                    (tag == "n" && isn))) {

                    // Determine the x-axis value based on the variable
                    double x = ptRef;
                    if (var == "p1Jet1PtOGenJet1PtInGenJet1Pt") {
                        //x = p4GenJet1.Pt();
                    }
                    if ((var == "p1Jet1PtORefPtInRefPt" || var == "p1GenJet1PtORefPtInRefPt") && iGenJet == -1) {
                        x = 0.0;
                    }

                    // Retrieve the histogram from the map
                    TH1* h = varTagFlvMap_[var][tag][flv_str].get();
                    if (!h) {
                        std::cerr << "Error: Missing histogram for var: " << var << ", tag: " << tag 
                                  << ", flv: " << flv_str << std::endl;
                        assert(h);
                        continue; // Skip filling if histogram is missing
                    }

                    // Fill the histogram based on its type
                    if (var == "h1EventInRefPt" || var == "h1EventInGenJet1Pt") {
                        // Fill TH1D histograms
                        static_cast<TH1D*>(h)->Fill(x, weight);
                    } else {
                        // Fill TProfile histograms
                        static_cast<TProfile*>(h)->Fill(x, val, weight);
                    }
                }
            }
        }
    }
}

