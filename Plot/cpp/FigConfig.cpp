#include "FigConfig.h"
#include <iostream>

// Base print function for FigConfigBase
void FigConfigBase::print() const {
    std::cout << "\nFigure configuration:\n"
              << "histDir: " << histDir << "\n"
              << "xTitle: " << xTitle << "\n"
              << "yTitle: " << yTitle << "\n"
              << "caption: " << caption << "\n"
              << "xMin: " << xMin << "\n"
              << "xMax: " << xMax << "\n"
              << "yMin: " << yMin << "\n"
              << "yMax: " << yMax << "\n"
              << "xLog: " << (xLog ? "true" : "false") << "\n"
              << "yLog: " << (yLog ? "true" : "false") << "\n"
              << "forAN: " << (forAN ? "true" : "false") << "\n"
              << "isNorm: " << (isNorm ? "true" : "false") << std::endl;
    std::cout << "trigDirs:";
    for(const auto& trigDir: trigDirs){
        std::cout << ", " << trigDir;
    }
}

// Print function for FigConfigEras1D
void FigConfigEras1D::print() const {
    FigConfigBase::print();
    std::cout << "histName: " << histName << "\n"
              << "rTitle: " << rTitle << "\n"
              << "rMin: " << rMin << "\n"
              << "rMax: " << rMax << std::endl;
}

// Print function for FigConfigEra1Ds
void FigConfigEra1Ds::print() const {
    FigConfigBase::print();
    std::cout << "histNames:";
    for(const auto& histName: histNames){
        std::cout << ", " << histName;
    }
    std::cout << "\n";
    std::cout << "rTitle: " << rTitle << "\n"
              << "rMin: " << rMin << "\n"
              << "rMax: " << rMax << std::endl;
}

// Print function for FigConfigYear1Ds
void FigConfigYear1Ds::print() const {
    FigConfigBase::print();
    std::cout << "histNames:";
    for(const auto& histName: histNames){
        std::cout << ", " << histName;
    }
    std::cout << "\n";
    std::cout << "rTitle: " << rTitle << "\n"
              << "rMin: " << rMin << "\n"
              << "rMax: " << rMax << std::endl;
}
// Print function for FigConfigEra2D
void FigConfigEra2D::print() const {
    FigConfigBase::print();
    std::cout << "histName: " << histName << "\n";
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

// Print function for FigConfigEraXYs
void FigConfigEraXYs::print() const {
    FigConfigEras1D::print();
     std::cout<< "varName: " << varName << "\n"
              << "varBinsSize: " << varBins.size() << "\n"
              << "isVarOnX: " << (isVarOnX ? "true" : "false") << std::endl;
}

// Print function for FigConfigTime1Ds
void FigConfigTime1Ds::print() const {
    FigConfigBase::print();
    std::cout << "histNames:";
    for(const auto& histName: histNames){
        std::cout << ", " << histName;
    }
    std::cout << "\n";

}
