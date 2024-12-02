#pragma once

#include <iostream>

class VarCut {
public:
    // All magic numbers with clear comments
    static constexpr int tTreeCatchSize = 52428800; // Buffer size for TTree cache (50 MB)
    static constexpr unsigned long cutFlowBinMargin = 0.5; // Bin margin for cut flow
    static constexpr int coutTabWidth = 5; // Tab width for console output

    static constexpr bool applyJer = false; // Apply JER correction

    // Print function to display all configuration variables
    void print() const;
};

