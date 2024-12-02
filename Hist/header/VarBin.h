#ifndef VARBIN_H
#define VARBIN_H

#include <vector>
#include "GlobalFlag.h"

class VarBin {
public:
    explicit VarBin(const GlobalFlag& globalFlags);

    // Variable bin width 
    const std::vector<double>& getBinsPt() const;
    const std::vector<double>& getBinsEta() const;
    const std::vector<double>& getBinsPhi() const;
    const std::vector<double>& getBinsPhiRebin() const;
    const std::vector<double>& getBinsMass() const;

    // Fixed bin width
    const std::vector<double>& getRangePt() const;
    const std::vector<double>& getRangeEta() const;
    const std::vector<double>& getRangePhi() const;
    const std::vector<double>& getRangePhiRebin() const;
    const std::vector<double>& getRangeMass() const;
    const std::vector<double>& getRangeMassJet1() const;
    const std::vector<double>& getRangeMassJetSum() const;

    const std::vector<double>& getRangeRun() const;
    const std::vector<double>& getRangeDeltaR() const;
    const std::vector<double>& getRangeDeltaPhi() const;
    const std::vector<double>& getRangeResponse() const;
    const std::vector<double>& getRangeCountRef() const;

private:
    // Helper method for printing bins
    void printBins(const std::string& binName, const std::vector<double>& bins) const;

    // Reference to GlobalFlag instance
    const GlobalFlag& globalFlags_;
    const GlobalFlag::Year year_;
    const GlobalFlag::Channel channel_;
    const bool isDebug_;

    // Bin vectors initialized in the constructor
    std::vector<double> binsPt_;
    std::vector<double> binsEta_;
    std::vector<double> binsPhi_;
    std::vector<double> binsPhiRebin_;
    std::vector<double> binsMass_;

    std::vector<double> rangePt_;
    std::vector<double> rangeEta_;
    std::vector<double> rangePhi_;
    std::vector<double> rangePhiRebin_;
    std::vector<double> rangeMass_;
    std::vector<double> rangeMassJet1_;
    std::vector<double> rangeMassJetSum_;

    std::vector<double> rangeRun_;
    std::vector<double> rangeDeltaR_;
    std::vector<double> rangeDeltaPhi_;
    std::vector<double> rangeResponse_;
    std::vector<double> rangeCountRef_;

    void InitializeBins();
};

#endif // VARBIN_H

