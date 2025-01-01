#ifndef HISTZEEJET_H
#define HISTZEEJET_H

#include <iostream>
#include <cmath>

// ROOT includes
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TLorentzVector.h>

// User-defined includes
#include "SkimTree.h"
#include "PickEvent.h"
#include "PickObject.h"
#include "ScaleEvent.h"
#include "ScaleObject.h"
#include "GlobalFlag.h"
#include "VarCut.h"

class RunZeeJet{
public:
    // Constructor accepting a reference to GlobalFlag
    explicit RunZeeJet(GlobalFlag& globalFlags);
    ~RunZeeJet() = default;

    int Run(std::shared_ptr<SkimTree>& skimT, PickEvent* pickEvent, PickObject* pickObject, ScaleEvent* scaleEvent, ScaleObject* scaleObject, TFile* fout);

private:
    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;

    // Add any private member variables or methods here if needed
};

#endif // HISTZEEJET_H

