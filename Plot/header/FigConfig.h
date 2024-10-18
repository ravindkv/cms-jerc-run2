#ifndef FIGCONFIG_H
#define FIGCONFIG_H

#include <string>
#include <vector>

struct FigConfig {
    std::string dirName;
    std::string xTitle;
    std::string yTitle;
    std::string zTitle;
    std::string rTitle;//for ratio
    std::string varName;//for XY projection
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double rMin;
    double rMax;
    double varMin;
    double varMax;
    std::vector<double>varBins;
    bool xLog;
    bool yLog;
    bool zLog;
    bool isNorm;
    bool isVarOnX;
   
    void print() const;
};

#endif // FIGCONFIG_H

