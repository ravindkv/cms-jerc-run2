#include "RunGamJet.h"

RunGamJet::RunGamJet(GlobalFlag& globalFlags)
    : RunBase(globalFlags) {
}

std::string RunGamJet::getConfigPath() const {
    return "config/RunGamJet.json";
}

int RunGamJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) {
    // Simply call the common Run implementation from RunBase.
    return RunBase::Run(nanoT, fout);
}

