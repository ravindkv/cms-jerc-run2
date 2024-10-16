#include "EventPick.h"

// Constructor implementation
EventPick::EventPick(GlobalFlag& globalFlags) : 
    globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()){
}

// Destructor
EventPick::~EventPick() {
    std::cout << "Destructor of EventPick" << '\n';
}

// Helper function for printing debug messages
void EventPick::printDebug(const std::string& message) const {
    if (isDebug_) {
        std::cout << message << '\n';
    }
}

// Function to check High-Level Triggers (HLT)
auto EventPick::passHLT(const std::shared_ptr<SkimTree>& tree) const -> bool {
    bool pass_HLT = false;

    if (channel_ == GlobalFlag::Channel::ZeeJet) {
        if (year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post) {
            pass_HLT = tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
            printDebug("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ: " +
                       std::to_string(tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ));
        } else if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
            pass_HLT = tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
            printDebug("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL: " +
                       std::to_string(tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL));
        }
    }

    if (channel_ == GlobalFlag::Channel::ZmmJet) {
        if (year_ == GlobalFlag::Year::Year2016Pre || year_ == GlobalFlag::Year::Year2016Post) {
            pass_HLT = tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
            printDebug("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ: " +
                       std::to_string(tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ));
        } else if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
            pass_HLT = tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
            printDebug("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8: " +
                       std::to_string(tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8));
        }
    }

    if (channel_ == GlobalFlag::Channel::GamJet) {
        // Combine all HLT conditions into a vector for easier management
        std::vector<bool> hlt_conditions = {
            tree->HLT_Photon200,
            tree->HLT_Photon175,
            tree->HLT_Photon150,
            tree->HLT_Photon120,
            tree->HLT_Photon90,
            tree->HLT_Photon75,
            tree->HLT_Photon50,
            tree->HLT_Photon33,
            tree->HLT_Photon110EB_TightID_TightIso,
            tree->HLT_Photon100EBHE10,
            tree->HLT_Photon30EB_TightID_TightIso,
            tree->HLT_Photon90_R9Id90_HE10_IsoM,
            tree->HLT_Photon75_R9Id90_HE10_IsoM,
            tree->HLT_Photon50_R9Id90_HE10_IsoM,
            tree->HLT_Photon30_HoverELoose,
            tree->HLT_Photon20_HoverELoose
        };

        // Check if any of the HLT conditions are true
        pass_HLT = std::any_of(hlt_conditions.begin(), hlt_conditions.end(), [](bool hlt) { return hlt; });

        // Debugging output
        if (isDebug_) {
            printDebug("HLT Photon Triggers:");
            printDebug("HLT_Photon200: " + std::to_string(tree->HLT_Photon200));
            printDebug("HLT_Photon175: " + std::to_string(tree->HLT_Photon175));
            printDebug("HLT_Photon150: " + std::to_string(tree->HLT_Photon150));
            printDebug("HLT_Photon120: " + std::to_string(tree->HLT_Photon120));
            printDebug("HLT_Photon90: " + std::to_string(tree->HLT_Photon90));
            printDebug("HLT_Photon75: " + std::to_string(tree->HLT_Photon75));
            printDebug("HLT_Photon50: " + std::to_string(tree->HLT_Photon50));
            printDebug("HLT_Photon33: " + std::to_string(tree->HLT_Photon33));
            printDebug("HLT_Photon110EB_TightID_TightIso: " + std::to_string(tree->HLT_Photon110EB_TightID_TightIso));
            printDebug("HLT_Photon100EBHE10: " + std::to_string(tree->HLT_Photon100EBHE10));
            printDebug("HLT_Photon30EB_TightID_TightIso: " + std::to_string(tree->HLT_Photon30EB_TightID_TightIso));
            printDebug("HLT_Photon90_R9Id90_HE10_IsoM: " + std::to_string(tree->HLT_Photon90_R9Id90_HE10_IsoM));
            printDebug("HLT_Photon75_R9Id90_HE10_IsoM: " + std::to_string(tree->HLT_Photon75_R9Id90_HE10_IsoM));
            printDebug("HLT_Photon50_R9Id90_HE10_IsoM: " + std::to_string(tree->HLT_Photon50_R9Id90_HE10_IsoM));
            printDebug("HLT_Photon30_HoverELoose: " + std::to_string(tree->HLT_Photon30_HoverELoose));
            printDebug("HLT_Photon20_HoverELoose: " + std::to_string(tree->HLT_Photon20_HoverELoose));
        }
    }//channel_ == GlobalFlag::Channel::GamJet

    if (channel_ == GlobalFlag::Channel::MCTruth || channel_ == GlobalFlag::Channel::VetoMap) {
        pass_HLT = tree->HLT_MC;
        if(isDebug_)
            printDebug("HLT_MC: " + std::to_string(tree->HLT_MC));
    }

    return pass_HLT;
}

// Function to apply event filters
auto EventPick::passFilter(const std::shared_ptr<SkimTree>& tree) const -> bool {
    bool pass = tree->Flag_goodVertices &&
                tree->Flag_globalSuperTightHalo2016Filter &&
                tree->Flag_HBHENoiseFilter &&
                tree->Flag_HBHENoiseIsoFilter &&
                tree->Flag_EcalDeadCellTriggerPrimitiveFilter &&
                tree->Flag_BadPFMuonFilter &&
                tree->Flag_eeBadScFilter;

    if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
        pass = pass && tree->Flag_ecalBadCalibFilter;
    }

    // Debugging output
    if (isDebug_) {
        printDebug("Event Filters:");
        printDebug("Flag_goodVertices: " + std::to_string(tree->Flag_goodVertices));
        printDebug("Flag_globalSuperTightHalo2016Filter: " + std::to_string(tree->Flag_globalSuperTightHalo2016Filter));
        printDebug("Flag_HBHENoiseFilter: " + std::to_string(tree->Flag_HBHENoiseFilter));
        printDebug("Flag_HBHENoiseIsoFilter: " + std::to_string(tree->Flag_HBHENoiseIsoFilter));
        printDebug("Flag_EcalDeadCellTriggerPrimitiveFilter: " + std::to_string(tree->Flag_EcalDeadCellTriggerPrimitiveFilter));
        printDebug("Flag_BadPFMuonFilter: " + std::to_string(tree->Flag_BadPFMuonFilter));
        printDebug("Flag_eeBadScFilter: " + std::to_string(tree->Flag_eeBadScFilter));
        if (year_ == GlobalFlag::Year::Year2017 || year_ == GlobalFlag::Year::Year2018) {
            printDebug("Flag_ecalBadCalibFilter: " + std::to_string(tree->Flag_ecalBadCalibFilter));
        }
    }

    return pass;
}

// Function to print progress during event processing
void EventPick::printProgress(Long64_t jentry, Long64_t nentries,
                              std::chrono::time_point<std::chrono::high_resolution_clock>& startClock,
                              double& totTime) const {
    if (isDebug_) {
        std::cout << "\n=== Event: " << jentry << " ===\n" << '\n';
    }
    if (nentries > 100 && jentry % (nentries / 100) == 0) {  // Print progress every 1%
        auto currentTime = std::chrono::high_resolution_clock::now();
        totTime += std::chrono::duration<double>(currentTime - startClock).count();
        int sec = static_cast<int>(totTime) % 60;
        int min = static_cast<int>(totTime) / 60;
        std::cout << std::setw(5) << (100 * jentry / nentries) << "% "
                  << std::setw(5) << min << "m " << sec << "s" << '\n';
        startClock = currentTime;  // Reset clock after printing progress
    }
}

// Function to print histogram bins and their statistics
void EventPick::printBins(const TH1D* hist) const {
    // Print the header
    std::cout << "---------: Cutflow Summary :--------" << '\n';
    std::cout << std::setw(20) << "CUT" << std::setw(10) << "ENTRIES" << std::setw(15) << "REDUCED (%)" << '\n';

    int nBins = hist->GetNbinsX();
    double previous = hist->GetBinContent(1);

    // Print the first bin's content (no change for the first bin)
    std::cout << std::setw(20) << hist->GetXaxis()->GetBinLabel(1)
              << std::setw(10) << previous
              << std::setw(15) << "N/A" << '\n';

    // Loop over the remaining bins
    for (int i = 2; i <= nBins; ++i) {
        double current = hist->GetBinContent(i);
        double change = (previous != 0) ? ((previous - current) / previous) * 100.0 : 0.0;

        // Print the cut name, entries, and percentage change
        std::cout << std::setw(20) << hist->GetXaxis()->GetBinLabel(i)
                  << std::setw(10) << current
                  << std::setw(10) << " => "
                  << std::fixed << std::setprecision(1)
                  << change << " %" << '\n';

        previous = current;
    }
}

// Function to print information about ROOT objects
void EventPick::printInfo(const TObject* obj) const {
    if (const auto* tree = dynamic_cast<const TTree*>(obj)) {
        std::cout << std::setw(15) << "TTree: " << std::setw(35) << tree->GetName()
                  << std::setw(15) << tree->GetEntries() << '\n';
    } else if (const TH1* hist = dynamic_cast<const TH1*>(obj)) {
        std::cout << std::setw(15) << hist->ClassName() << ": " << std::setw(35) << hist->GetName()
                  << std::setw(15) << hist->GetEntries()
                  << std::setw(15) << hist->GetMean()
                  << std::setw(15) << hist->GetRMS() << '\n';
    } else if (const auto* prof = dynamic_cast<const TProfile*>(obj)) {
        std::cout << std::setw(15) << "TProfile: " << std::setw(35) << prof->GetName()
                  << std::setw(15) << prof->GetEntries()
                  << std::setw(15) << prof->GetMean()
                  << std::setw(15) << prof->GetRMS() << '\n';
    } else if (const auto* prof2d = dynamic_cast<const TProfile2D*>(obj)) {
        std::cout << std::setw(15) << "TProfile2D: " << std::setw(35) << prof2d->GetName()
                  << std::setw(15) << prof2d->GetEntries()
                  << std::setw(15) << prof2d->GetMean()
                  << std::setw(15) << prof2d->GetRMS() << '\n';
    } else {
        std::cout << std::setw(15) << obj->ClassName() << ": " << std::setw(35) << obj->GetName()
                  << '\n';
    }
}

// Function to scan a directory and its contents recursively
void EventPick::scanDirectory(TDirectory* dir, const std::string& path) const {
    std::string currentPath = path + dir->GetName() + "/";
    std::cout << "\nDirectory: " << currentPath << '\n';

    TIter next(dir->GetListOfKeys());
    TKey* key = nullptr;

    while ((key = dynamic_cast<TKey*>(next()))) {
        TObject* obj = key->ReadObj();

        if (obj->InheritsFrom(TDirectory::Class())) {
            scanDirectory(dynamic_cast<TDirectory*>(obj), currentPath);  // Recursive call for subdirectories
        } else {
            printInfo(obj);
        }
    }
}

// Function to scan a ROOT file and its directories
void EventPick::scanTFile(TFile* file) const {
    std::cout << "\n-----------: Scanning All Directories and Printing Entries, Mean, RMS :------------\n" << '\n';
    scanDirectory(file, "");
}

