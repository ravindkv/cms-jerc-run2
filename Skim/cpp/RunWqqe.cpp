#include "RunWqqe.h"

RunWqqe::RunWqqe(GlobalFlag& globalFlags)
    : RunBase(globalFlags) {
}

std::string RunWqqe::getConfigPath() const {
    return "config/RunWqqe.json";
}

int RunWqqe::Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) {
    // Simply call the common Run implementation from RunBase.
    return RunBase::Run(nanoT, fout);
}

