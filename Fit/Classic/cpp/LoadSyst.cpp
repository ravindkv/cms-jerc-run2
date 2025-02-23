#include "LoadSyst.h"
#include "Helper.h"
#include <iostream>
#include <memory>

LoadSyst::LoadSyst(const TString& name, TH1D* hist)
{
    setName(name);
    // Clone the histogram and decouple it from the ROOT directory.
    setHist(static_cast<TH1D*>(hist->Clone(name)));
}

