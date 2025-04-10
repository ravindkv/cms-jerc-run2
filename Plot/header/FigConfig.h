#pragma once

#ifndef FIGCONFIG_H
#define FIGCONFIG_H

#include <string>
#include <vector>

// Base configuration structure
struct FigConfigBase {
    std::string histDir;
    std::vector<std::string> trigDirs;
    std::string xTitle;
    std::string yTitle;
    std::string caption;
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    bool xLog;
    bool yLog;
    bool forAN;
    bool isNorm;

    virtual void print() const; // Declaration only, implementation will be in .cpp file
};

// Derived configuration for Eras1D
struct FigConfigEras1D : public FigConfigBase {
    std::string histName;
    std::string rTitle;
    double rMin;
    double rMax;

    void print() const override; // Declaration only
};
// Derived configuration for Era1Ds
struct FigConfigEra1Ds : public FigConfigBase {
    std::vector<std::string> histNames;
    std::string rTitle;
    double rMin;
    double rMax;

    void print() const override; // Declaration only
};
// Derived configuration for Year1Ds
struct FigConfigYear1Ds : public FigConfigBase {
    std::vector<std::string> histNames;
    std::string rTitle;
    double rMin;
    double rMax;

    void print() const override; // Declaration only
};

// Derived configuration for Era2D
struct FigConfigEra2D : public FigConfigBase {
    std::string histName;
    std::string zTitle;
    bool zLog;

    void print() const override; // Declaration only
};

// Derived configuration for ErasXY
struct FigConfigErasXY : public FigConfigEras1D {
    std::string histName;
    std::string varName;
    double varMin;
    double varMax;
    bool zLog;
    bool isVarOnX;

    void print() const override; // Declaration only
};

// Derived configuration for EraXYs
struct FigConfigEraXYs : public FigConfigEras1D {
    std::string histName;
    std::string varName;
    std::vector<double> varBins;
    bool isVarOnX;

    void print() const override; // Declaration only
};

// Derived configuration for Time1Ds
struct FigConfigTime1Ds : public FigConfigBase {
    std::vector<std::string> histNames;

    void print() const override; // Declaration only
};
#endif // FIGCONFIG_H

