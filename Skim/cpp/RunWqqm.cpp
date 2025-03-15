#include "RunWqqm.h"

RunWqqm::RunWqqm(GlobalFlag& globalFlags)
    : RunBase(globalFlags) {
}

std::string RunWqqm::getConfigPath() const {
    return "config/RunWqqm.json";
}

int RunWqqm::Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) {
    // Simply call the common Run implementation from RunBase.
    return RunBase::Run(nanoT, fout);
}

