#pragma once

#ifndef VARBIN_H
#define VARBIN_H

#include <iostream>
#include <vector>

class VarBin {
public:
    // Inclusive jets pT, eta binning
    static std::vector<double> getBinsPt();
    static std::vector<double> getBinsEta();
    static std::vector<double> getBinsPhi();

};
    
#endif // VARBIN_H

