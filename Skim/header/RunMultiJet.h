#pragma once

#include "RunBase.h"

class RunMultiJet : public RunBase {
public:
    explicit RunMultiJet(GlobalFlag& globalFlags);
    ~RunMultiJet() = default;

    // Inherit Run() from RunBase
    int Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) override final;

protected:
    std::string getConfigPath() const override;
};

