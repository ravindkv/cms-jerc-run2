#include "FigConfig.h"
#include <iostream>

void FigConfig::print() const {
    std::cout << "\nFigure configuration:\n"
              << "dirName: " << dirName << "\n"
              << "xTitle: " << xTitle << "\n"
              << "yTitle: " << yTitle << "\n"
              << "zTitle: " << zTitle << "\n"
              << "rTitle: " << rTitle << "\n"
              << "xMin: " << xMin << "\n"
              << "xMax: " << xMax << "\n"
              << "yMin: " << yMin << "\n"
              << "yMax: " << yMax << "\n"
              << "rMin: " << rMin << "\n"
              << "rMax: " << rMax << "\n"
              << "varMin: " << varMin << "\n"
              << "varMax: " << varMax << "\n"
              << "varBinsSize: " << varBins.size() << "\n"
              << "xLog: " << (xLog ? "true" : "false") << "\n"
              << "yLog: " << (yLog ? "true" : "false") << "\n"
              << "zLog: " << (zLog ? "true" : "false") << "\n"
              << "isNorm: " << (isNorm ? "true" : "false") << "\n"
              << "isVarOnX: " << (isVarOnX ? "true" : "false") << std::endl;
}

