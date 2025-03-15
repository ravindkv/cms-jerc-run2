#pragma once

#include "RunBase.h"

class RunGamJetFake : public RunBase {
public:
    explicit RunGamJetFake(GlobalFlag& globalFlags);
    ~RunGamJetFake() = default;

    // Inherit Run() from RunBase
    int Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) override final;

protected:
    std::string getConfigPath() const override;
};

