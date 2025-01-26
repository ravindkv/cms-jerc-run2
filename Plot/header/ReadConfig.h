#pragma once

#ifndef READCONFIG_H
#define READCONFIG_H

#include <string>
#include <vector>
#include "FigConfig.h" // Assuming you have this struct defined
#include <nlohmann/json.hpp> // Include the JSON library header

class ReadConfig {
public:
    ReadConfig(const std::string& configFilePath);
    const std::vector<FigConfigEras1D>& getFigConfigVecEras1D() const;
    const std::vector<FigConfigEra1Ds>& getFigConfigVecEra1Ds() const;
    const std::vector<FigConfigYear1Ds>& getFigConfigVecYear1Ds() const;
    const std::vector<FigConfigEra2D>& getFigConfigVecEra2D() const;
    const std::vector<FigConfigErasXY>& getFigConfigVecErasXY() const;
    const std::vector<FigConfigEraXYs>& getFigConfigVecEraXYs() const;
    const std::vector<FigConfigTime1Ds>& getFigConfigVecTime1Ds() const;

private:
    void readConfigFile(const std::string& configFilePath);

    template<typename T>
    void readFigConfig(const nlohmann::json& configGroup, std::vector<T>& configVec);

    std::vector<FigConfigEras1D> figConfigVecEras1D_;
    std::vector<FigConfigEra1Ds> figConfigVecEra1Ds_;
    std::vector<FigConfigYear1Ds> figConfigVecYear1Ds_;
    std::vector<FigConfigEra2D> figConfigVecEra2D_;
    std::vector<FigConfigErasXY> figConfigVecErasXY_;
    std::vector<FigConfigEraXYs> figConfigVecEraXYs_;
    std::vector<FigConfigTime1Ds> figConfigVecTime1Ds_;
};

#endif // READCONFIG_H

