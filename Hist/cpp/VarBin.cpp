#include "VarBin.h"
#include <iostream>
#include <string>
#include <TMath.h>

// Constructor implementation
VarBin::VarBin(const GlobalFlag& globalFlags)
    :globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()){

    InitializeBins();
}

void VarBin::InitializeBins(){

    binsPt_ = {};
    if (channel_ == GlobalFlag::Channel::ZeeJet || channel_ == GlobalFlag::Channel::ZmmJet) {
        binsPt_ = { 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 
                    50.0, 60.0, 70.0, 85.0, 105.0, 130.0, 175.0, 
                    230.0, 300.0, 400.0, 500.0, 700.0, 1000.0, 1500.0 };
    }
    else if (channel_ == GlobalFlag::Channel::GamJet) {
        binsPt_ = {15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 50.0, 
                    60.0, 70.0, 85.0, 105.0, 130.0, 175.0, 230.0, 
                    300.0, 400.0, 500.0, 600.0, 700.0, 850.0, 1000.0, 1200.0, 1450.0, 1750.0}; 
    }
    else if (channel_ == GlobalFlag::Channel::MultiJet) {
        binsPt_ = {10.0, 15.0, 21.0, 28.0, 37.0, 49.0, 64.0, 
                    84.0, 114.0, 153.0, 196.0, 245.0, 300.0, 362.0, 
                    430.0, 507.0, 592.0, 686.0, 790.0, 905.0, 1032.0, 
                    1172.0, 1327.0, 1497.0, 1684.0, 1890.0, 2116.0, 2366.0, 
                    2640.0, 2941.0, 3273.0, 5220.0};
    }
    else{
        binsPt_ = {15, 25, 35, 50, 75, 100, 130, 170, 230, 300, 500, 1000 };
    }

    binsEta_ = {
        -5.191, -3.839, -3.489, -3.139, -2.964, -2.853,
        -2.650, -2.500, -2.322, -2.172, -1.930, -1.653,
        -1.479, -1.305, -1.044, -0.783, -0.522, -0.261,
        0.000, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479,
        1.653, 1.930, 2.172, 2.322, 2.500, 2.650, 2.853,
        2.964, 3.139, 3.489, 3.839, 5.191
    };
    binsPhi_ = {
        -3.142, -3.054, -2.967, -2.880, -2.793, -2.705,
        -2.618, -2.531, -2.443, -2.356, -2.269, -2.182,
        -2.094, -2.007, -1.920, -1.833, -1.745, -1.658,
        -1.571, -1.484, -1.396, -1.309, -1.222, -1.134,
        -1.047, -0.960, -0.873, -0.785, -0.698, -0.611,
        -0.524, -0.436, -0.349, -0.262, -0.175, -0.087,
        0.000, 0.087, 0.175, 0.262, 0.349, 0.436, 0.524,
        0.611, 0.698, 0.785, 0.873, 0.960, 1.047, 1.134,
        1.222, 1.309, 1.396, 1.484, 1.571, 1.658, 1.745,
        1.833, 1.920, 2.007, 2.094, 2.182, 2.269, 2.356,
        2.443, 2.531, 2.618, 2.705, 2.793, 2.880, 2.967,
        3.054, 3.142
    };
    binsPhiRebin_ = binsPhi_;

    binsMass_ = {
        60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 
        81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
        101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 
        116, 117, 118, 119, 120 
    };
    if (channel_ == GlobalFlag::Channel::GamJet) {
        binsMass_ = { -3, -2, -1, 0, 1, 2, 3 };
    }
    
    // Fixed width (N, min, max)
    rangePt_  = {100, 0, 1500};
    rangeEta_ = {30,-1.305,+1.305};
    rangePhi_ = {72,-TMath::Pi(),TMath::Pi()};
    rangePhiRebin_ = {300,-TMath::Pi(),TMath::Pi()};
    rangeMass_  = {100, 0, 200};
    rangeMassJet1_  = {100, 0, 150};
    rangeMassJetSum_  = {100, 0, 15000};

    rangeRun_ = { 200, 271036, 325175 };//2016 to 2018

    rangeDeltaR_ = { 50, 0, 10 };
    rangeDeltaPhi_ = { 100, 0, 5 };
    rangeResponse_ = { 100, 0, 5 };
    rangeCountRef_ = { 5, -0.5, 4.5 };
    rangeFraction_ = { 50, 0.0, 1.0};
    rangeIndex_ = { 30, -0.5, 29.5};
}

// Helper method to print bin values
void VarBin::printBins(const std::string& binName, const std::vector<double>& bins) const {
    if (isDebug_) {
        std::cout << "=======: VarBin::" << binName << " :======\n";
        for (const auto& bin : bins) {
            std::cout << bin << ", ";
        }
        std::cout << "\n";
    }
}

// Getters
const std::vector<double>& VarBin::getBinsPt() const {
    printBins("getBinsPt()", binsPt_);
    return binsPt_;
}
const std::vector<double>& VarBin::getBinsEta() const {
    printBins("getBinsEta()", binsEta_);
    return binsEta_;
}
const std::vector<double>& VarBin::getBinsPhi() const {
    printBins("getBinsPhi()", binsPhi_);
    return binsPhi_;
}
const std::vector<double>& VarBin::getBinsPhiRebin() const {
    printBins("getBinsPhiRebin()", binsPhiRebin_);
    return binsPhiRebin_;
}
const std::vector<double>& VarBin::getBinsMass() const {
    printBins("getBinsMass()", binsMass_);
    return binsMass_;
}


const std::vector<double>& VarBin::getRangePt() const {
    printBins("getRangePt()", rangePt_);
    return rangePt_;
}
const std::vector<double>& VarBin::getRangeEta() const {
    printBins("getRangeEta()", rangeEta_);
    return rangeEta_;
}
const std::vector<double>& VarBin::getRangePhi() const {
    printBins("getRangePhi()", rangePhi_);
    return rangePhi_;
}
const std::vector<double>& VarBin::getRangePhiRebin() const {
    printBins("getRangePhiRebin()", rangePhiRebin_);
    return rangePhiRebin_;
}
const std::vector<double>& VarBin::getRangeMass() const {
    printBins("getRangeMass()", rangeMass_);
    return rangeMass_;
}
const std::vector<double>& VarBin::getRangeMassJet1() const {
    printBins("getRangeMassJet1()", rangeMassJet1_);
    return rangeMassJet1_;
}
const std::vector<double>& VarBin::getRangeMassJetSum() const {
    printBins("getRangeMassJetSum()", rangeMassJetSum_);
    return rangeMassJetSum_;
}



const std::vector<double>& VarBin::getRangeRun() const {
    printBins("getRangeRun()", rangeRun_);
    return rangeRun_;
}
const std::vector<double>& VarBin::getRangeDeltaR() const {
    printBins("getRangeDeltaR()", rangeDeltaR_);
    return rangeDeltaR_;
}
const std::vector<double>& VarBin::getRangeDeltaPhi() const {
    printBins("getRangeDeltaPhi()", rangeDeltaPhi_);
    return rangeDeltaPhi_;
}
const std::vector<double>& VarBin::getRangeResponse() const {
    printBins("getRangeResponse()", rangeResponse_);
    return rangeResponse_;
}
const std::vector<double>& VarBin::getRangeCountRef() const {
    printBins("getRangeCountRef()", rangeCountRef_);
    return rangeCountRef_;
}
const std::vector<double>& VarBin::getRangeFraction() const {
    printBins("getRangeFraction()", rangeFraction_);
    return rangeFraction_;
}
const std::vector<double>& VarBin::getRangeIndex() const {
    printBins("getRangeIndex()", rangeIndex_);
    return rangeIndex_;
}
