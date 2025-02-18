#include "VarCut.h"

// Constructor implementation
VarCut::VarCut(const GlobalFlag& globalFlags)
    :globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()){

    InitializeCuts();
}

void VarCut::InitializeCuts() {
    // Configure lepton cuts
    leptonCuts_.mass = 1.77682; // default tau mass
    if (channel_ == GlobalFlag::Channel::Wqqe) {
        leptonCuts_.mass = 0.00051099891; // electron mass
    }
    if (channel_ == GlobalFlag::Channel::Wqqm) {
        leptonCuts_.mass = 0.105658367; // muon mass
    }

    // Configure jet cuts
    jetCuts_.minPt = 12;
    jetCuts_.maxEta = 2.4;
    jetCuts_.maxEtaLight = 1.3;
    jetCuts_.minId = 6;
    jetCuts_.nJet = 4;
    if (channel_ == GlobalFlag::Channel::Wqqe || channel_ == GlobalFlag::Channel::Wqqm) {
        jetCuts_.minPt = 30;
        if (year_ == GlobalFlag::Year::Year2017) {
            jetCuts_.maxEta = 2.5;
        }
    }

    // Configure b-jet cuts
    bJetCuts_.nBJet = 2;
    bJetCuts_.minDisc = 0.0;
    if (year_ == GlobalFlag::Year::Year2016Pre) {
        bJetCuts_.minDisc = 0.6502;
    } else if (year_ == GlobalFlag::Year::Year2016Post) {
        bJetCuts_.minDisc = 0.6377;
    } else if (year_ == GlobalFlag::Year::Year2017) {
        bJetCuts_.minDisc = 0.7476;
    } else if (year_ == GlobalFlag::Year::Year2018) {
        bJetCuts_.minDisc = 0.7100;
    }

    if(isDebug_) print();
}

void VarCut::print() const {
    std::cout << "=== VarCut Debug Info ===" << std::endl;
    
    std::cout << "\n-- Jet Cuts --" << std::endl;
    std::cout << "  minPt       = " << jetCuts_.minPt << std::endl;
    std::cout << "  maxEta      = " << jetCuts_.maxEta << std::endl;
    std::cout << "  maxEtaLight = " << jetCuts_.maxEtaLight << std::endl;
    std::cout << "  minId       = " << jetCuts_.minId << std::endl;
    std::cout << "  nJet       = " << jetCuts_.nJet << std::endl;
    
    std::cout << "\n-- Lepton Cuts --" << std::endl;
    std::cout << "  mass        = " << leptonCuts_.mass << std::endl;
    
    std::cout << "\n-- B-Jet Cuts --" << std::endl;
    std::cout << "  minDisc     = " << bJetCuts_.minDisc << std::endl;
    std::cout << "  nBJet     = " << bJetCuts_.nBJet << std::endl;
    
    std::cout << "\n-- Other Cuts --" << std::endl;
    
    std::cout << "\n-- Global/Static Settings --" << std::endl;
    std::cout << "  applyJer         = " << std::boolalpha << applyJer << std::endl;
    std::cout << "  tTreeCatchSize   = " << tTreeCatchSize << std::endl;
    std::cout << "  cutFlowBinMargin = " << cutFlowBinMargin << std::endl;
    std::cout << "  coutTabWidth     = " << coutTabWidth << std::endl;
    std::cout << "  MassW            = " << MassW << std::endl;
    std::cout << "  MassT            = " << MassT << std::endl;
}

