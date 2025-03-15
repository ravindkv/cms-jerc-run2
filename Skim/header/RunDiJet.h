#pragma once

#include "RunBase.h"

class RunDiJet : public RunBase {
public:
    explicit RunDiJet(GlobalFlag& globalFlags);
    ~RunDiJet() = default;

    // Inherit Run() from RunBase
    int Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) override final;

protected:
    std::string getConfigPath() const override;
};

