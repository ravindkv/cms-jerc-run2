#include "FigConfig.h"
#include <iostream>

// Base print function for FigConfigBase
void FigConfigBase::print() const {
    std::cout << "\nFigure configuration:\n"
              << "histDir: " << histDir << "\n"
              << "histName: " << histName << "\n"
              << "xTitle: " << xTitle << "\n"
              << "yTitle: " << yTitle << "\n"
              << "xMin: " << xMin << "\n"
              << "xMax: " << xMax << "\n"
              << "yMin: " << yMin << "\n"
              << "yMax: " << yMax << "\n"
              << "xLog: " << (xLog ? "true" : "false") << "\n"
              << "yLog: " << (yLog ? "true" : "false") << "\n"
              << "isNorm: " << (isNorm ? "true" : "false") << std::endl;
}

// Print function for FigConfigEras1D
void FigConfigEras1D::print() const {
    FigConfigBase::print();
    std::cout << "rTitle: " << rTitle << "\n"
              << "rMin: " << rMin << "\n"
              << "rMax: " << rMax << std::endl;
}

// Print function for FigConfigEra2D
void FigConfigEra2D::print() const {
    FigConfigBase::print();
    std::cout << "zTitle: " << zTitle << "\n"
              << "zLog: " << (zLog ? "true" : "false") << std::endl;
}

// Print function for FigConfigErasXY
void FigConfigErasXY::print() const {
    FigConfigEras1D::print();
     std::cout<< "varName: " << varName << "\n"
              << "varMin: " << varMin << "\n"
              << "varMax: " << varMax << "\n"
              << "isVarOnX: " << (isVarOnX ? "true" : "false") << std::endl;
}

// Print function for FigConfigEraXY
void FigConfigEraXY::print() const {
    FigConfigEras1D::print();
     std::cout<< "varName: " << varName << "\n"
              << "varBinsSize: " << varBins.size() << "\n"
              << "isVarOnX: " << (isVarOnX ? "true" : "false") << std::endl;
}

// Print function for FigConfigTime1D
void FigConfigTime1D::print() const {
    FigConfigBase::print();
    std::cout << "histNames:";
    for(const auto& histName: histNames){
        std::cout << ", " << histName;
    }
    std::cout << "\n";

}
