#pragma once

#include "RunBase.h"

class RunWqqm : public RunBase {
public:
    explicit RunWqqm(GlobalFlag& globalFlags);
    ~RunWqqm() = default;

    // Inherit Run() from RunBase
    int Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) override final;

protected:
    std::string getConfigPath() const override;
};

