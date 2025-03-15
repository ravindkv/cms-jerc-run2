#include "RunGamJetFake.h"

RunGamJetFake::RunGamJetFake(GlobalFlag& globalFlags)
    : RunBase(globalFlags) {
}

std::string RunGamJetFake::getConfigPath() const {
    return "config/RunGamJetFake.json";
}

int RunGamJetFake::Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) {
    // Simply call the common Run implementation from RunBase.
    return RunBase::Run(nanoT, fout);
}

