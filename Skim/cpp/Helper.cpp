
#include "Helper.h"

void Helper::initProgress(){
    std::cout<<"---------------------------"<<'\n';
    std::cout<<std::setw(coutTabWidth10)
             <<"Progress"<<std::setw(coutTabWidth10)
             <<"Time"
             <<'\n';
    std::cout<<"---------------------------"<<'\n';
}

// Function to print progress during event processing
void Helper::printProgress(Long64_t jentry, Long64_t nentries,
                              std::chrono::time_point<std::chrono::high_resolution_clock>& startClock,
                              double& totTime){
    bool isDebug_ = false;
    if (isDebug_) {
        std::cout << "\n=== Event: " << jentry << " ===\n" << '\n';
    }
    if (nentries > hundred && jentry % (nentries / hundred) == 0) {  // Print progress every 1%
        auto currentTime = std::chrono::high_resolution_clock::now();
        totTime += std::chrono::duration<double>(currentTime - startClock).count();
        int sec = static_cast<int>(totTime) % sixty;
        int min = static_cast<int>(totTime) / sixty;
        std::cout << std::setw(coutTabWidth5) << (hundred * jentry / nentries) << "% "
                  << std::setw(coutTabWidth5) << min << "m " << sec << "s" << '\n';
        startClock = currentTime;  // Reset clock after printing progress
    }
}

// Function to print histogram bins and their statistics
void Helper::printCutflow(const TH1D* hist){
    // Print the header
    std::cout << "---------: Cutflow Summary :--------" << '\n';
    std::cout << std::setw(coutTabWidth20) << "CUT" << std::setw(coutTabWidth10) << "ENTRIES" << std::setw(coutTabWidth20) << "REDUCED x times" << '\n';

    int nBins = hist->GetNbinsX();
    double previous = hist->GetBinContent(1);

    // Print the first bin's content (no change for the first bin)
    std::cout << std::setw(coutTabWidth20) << hist->GetXaxis()->GetBinLabel(1)
              << std::setw(coutTabWidth10) << previous
              << std::setw(coutTabWidth15) << "N/A" << '\n';

    // Loop over the remaining bins
    for (int i = 2; i <= nBins; ++i) {
        double current = hist->GetBinContent(i);
        //double change = (previous != 0) ? ((current) / previous) * hundred : 0.0;
        double xTimes = (current != 0) ? ((previous) / current) : 0.0;

        // Print the cut name, entries, and percentage change
        std::cout << std::setw(coutTabWidth20) << hist->GetXaxis()->GetBinLabel(i)
                  << std::setw(coutTabWidth10) << current
                  << std::setw(coutTabWidth10) << " => "
                  << std::fixed << std::setprecision(1)
                  //<< change << " %" << '\n';
                  << xTimes << '\n';

        previous = current;
    }
}

