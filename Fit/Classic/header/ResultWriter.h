#pragma once

#define RESULT_WRITER
#include "Minimizer.h"
#include "FitModel.h"
#include "GlobalFlag.h"  // Include GlobalFlag for debug control
#include "LoadAll.h"     // Include LoadAll for pre-fit inputs
#include <TFile.h>
#include <string>

class ResultWriter {
public:
    // Modified constructor to accept a GlobalFlag and LoadAll reference.
    ResultWriter(const FitResults& res, FitModel& model, const std::string& outputFilename,
                const LoadAll& loadAll, GlobalFlag& globalFlags);
    ~ResultWriter();
    void storeResults();
    
private:
    const FitResults& results;
    FitModel& fitModel;
    TFile* outputFile;
    GlobalFlag& globalFlags_; // Global flags for controlling debug output.
    const LoadAll& loadAll_;  // Reference to LoadAll to access pre-fit input objects.
};

