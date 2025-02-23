#include "RunL3Res.h"
#include "Config.h"
#include "LoadAll.h"
#include "FitModel.h"
#include "Minimizer.h"
#include "ResultWriter.h"

#include <iostream>

RunL3Res::RunL3Res(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}

void RunL3Res::Run()
{
    // Load configuration.
    std::string configFile = "input/fit_config.json";
    Config config(configFile);
    if (!config.isValid()) {
        std::cerr << "Invalid configuration." << std::endl;
        return;
    }
    if(globalFlags_.isDebug) {
        cout<<"Running L3Res Fit\n";
    } 
    // Load data.
    // Note: LoadAll now uses smart pointers internally.
    LoadAll loadAll(config, globalFlags_);
    loadAll.loadBase();
    loadAll.loadSyst();
    loadAll.loadShape();
    loadAll.loadNorm();
    
    // Setup the model.
    FitModel model(config, loadAll.getShape(), loadAll.getNorm(), globalFlags_);
    double fitMin = 10.0;
    double fitMax = 1000.0;
    model.setupFitFunction(fitMin, fitMax);
    
    // Run the minimizer.
    Minimizer minimizer(model, globalFlags_);
    minimizer.runFit(100);
    const FitResults& results = minimizer.getResults();
    
    // Write output.
    ResultWriter writer(results, model, "fit_output.root", loadAll, globalFlags_);
    writer.storeResults();
}

