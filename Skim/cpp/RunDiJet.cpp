#include "RunDiJet.h"

RunDiJet::RunDiJet(GlobalFlag& globalFlags)
    : RunBase(globalFlags) {
}

std::string RunDiJet::getConfigPath() const {
    return "config/RunDiJet.json";
}

int RunDiJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) {
    // Simply call the common Run implementation from RunBase.
    return RunBase::Run(nanoT, fout);
}

