#pragma once

#include "RunBase.h"

class RunZmmJet : public RunBase {
public:
    explicit RunZmmJet(GlobalFlag& globalFlags);
    ~RunZmmJet() = default;

    // Inherit Run() from RunBase
    int Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) override final;

protected:
    std::string getConfigPath() const override;
};

