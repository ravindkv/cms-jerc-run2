#pragma once

#include "RunBase.h"

class RunWqqe : public RunBase {
public:
    explicit RunWqqe(GlobalFlag& globalFlags);
    ~RunWqqe() = default;

    // Inherit Run() from RunBase
    int Run(std::shared_ptr<NanoTree>& nanoT, TFile* fout) override final;

protected:
    std::string getConfigPath() const override;
};

