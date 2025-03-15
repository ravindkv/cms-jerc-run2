#pragma once

#include "RunBase.h"

class RunZeeJet : public RunBase {
public:
    explicit RunZeeJet(GlobalFlag& globalFlags);
    ~RunZeeJet() = default;

    // Inherit Run() from RunBase
    int Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) override final;

protected:
    std::string getConfigPath() const override;
};

