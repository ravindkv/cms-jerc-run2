#include "RunMultiJet.h"

RunMultiJet::RunMultiJet(GlobalFlag& globalFlags)
    : RunBase(globalFlags) {
}

std::string RunMultiJet::getConfigPath() const {
    return "config/RunMultiJet.json";
}

int RunMultiJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) {
    // Simply call the common Run implementation from RunBase.
    return RunBase::Run(nanoT, fout);
}

