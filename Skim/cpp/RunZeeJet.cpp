#include "RunZeeJet.h"

RunZeeJet::RunZeeJet(GlobalFlag& globalFlags)
    : RunBase(globalFlags) {
}

std::string RunZeeJet::getConfigPath() const {
    return "config/RunZeeJet.json";
}

int RunZeeJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) {
    // Simply call the common Run implementation from RunBase.
    return RunBase::Run(nanoT, fout);
}

