#pragma once

#include "RunBase.h"

class RunGamJet : public RunBase {
public:
    explicit RunGamJet(GlobalFlag& globalFlags);
    ~RunGamJet() = default;

    // Inherit Run() from RunBase
    int Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) override final;

protected:
    std::string getConfigPath() const override;
};

