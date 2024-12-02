#include "VarCut.h"

void VarCut::print() const {
    std::cout << "Configurations:\n";
    std::cout << "  applyJer = " << std::boolalpha << applyJer << "\n";
    std::cout << "  tTreeCatchSize = " << tTreeCatchSize << "\n";
    std::cout << "  cutFlowBinMargin = " << cutFlowBinMargin << "\n";
    std::cout << "  coutTabWidth = " << coutTabWidth << "\n";
}

