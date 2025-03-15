#include "RunZmmJet.h"

RunZmmJet::RunZmmJet(GlobalFlag& globalFlags)
    : RunBase(globalFlags) {
}

std::string RunZmmJet::getConfigPath() const {
    return "config/RunZmmJet.json";
}

int RunZmmJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) {
    // Simply call the common Run implementation from RunBase.
    return RunBase::Run(nanoT, fout);
}

