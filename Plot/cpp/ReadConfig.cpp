#include "ReadConfig.h"
#include <fstream>
#include <iostream>

ReadConfig::ReadConfig(const std::string& configFilePath) {
    readConfigFile(configFilePath);
}

void ReadConfig::readConfigFile(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cerr << "Error: Cannot open configuration file " << configFilePath << std::endl;
        exit(EXIT_FAILURE);
    }

    nlohmann::json configJson;
    configFile >> configJson;

    for (auto& [era, configGroup] : configJson.items()) {
        if (era == "ConfigEras1D") {
            readFigConfig(configGroup, figConfigVecEras1D_);
        } else if (era == "ConfigEra2D") {
            readFigConfig(configGroup, figConfigVecEra2D_);
        } else if (era == "ConfigErasXY") {
            readFigConfig(configGroup, figConfigVecErasXY_);
        } else if (era == "ConfigEraXY") {
            readFigConfig(configGroup, figConfigVecEraXY_);
        }
    }
}

// Template function to read configurations for any era
template<typename T>
void ReadConfig::readFigConfig(const nlohmann::json& configGroup, std::vector<T>& configVec) {
    configVec.clear();
    for (auto& [name, params] : configGroup.items()) {
        T figConfig;
        figConfig.histDir = params.value("histDir", "");
        figConfig.histName = params.value("histName", "");
        figConfig.xTitle = params.value("xTitle", "");
        figConfig.yTitle = params.value("yTitle", "");
        figConfig.xMin = params.value("xMin", 0.0);
        figConfig.xMax = params.value("xMax", 0.0);
        figConfig.yMin = params.value("yMin", 0.0);
        figConfig.yMax = params.value("yMax", 0.0);
        figConfig.xLog = params.value("xLog", false);
        figConfig.yLog = params.value("yLog", false);
        figConfig.isNorm = params.value("isNorm", false);
        
        // Special handling for specific types
        if constexpr (std::is_same_v<T, FigConfigEras1D> || 
                     std::is_same_v<T, FigConfigErasXY> || 
                     std::is_same_v<T, FigConfigEraXY>) {
            figConfig.rTitle = params.value("rTitle", "");
            figConfig.rMin = params.value("rMin", 0.0);
            figConfig.rMax = params.value("rMax", 2.0);
        }
        if constexpr (std::is_same_v<T, FigConfigEra2D>) {
            figConfig.zTitle = params.value("zTitle", "");
            figConfig.zLog = params.value("zLog", false);
        }
        if constexpr (std::is_same_v<T, FigConfigErasXY>) {
            figConfig.varName = params.value("varName", "");
            figConfig.varMin = params.value("varMin", 0.0);
            figConfig.varMax = params.value("varMax", 2.0);
            figConfig.isVarOnX = params.value("isVarOnX", false);
        }
        if constexpr (std::is_same_v<T, FigConfigEraXY>) {
            figConfig.varName = params.value("varName", "");
            figConfig.varBins = params.value("varBins", std::vector<double>{});
            figConfig.isVarOnX = params.value("isVarOnX", false);
        }

        configVec.push_back(figConfig);
    }
}

const std::vector<FigConfigEras1D>& ReadConfig::getFigConfigVecEras1D() const {
    return figConfigVecEras1D_;
}
const std::vector<FigConfigEra2D>& ReadConfig::getFigConfigVecEra2D() const {
    return figConfigVecEra2D_;
}
const std::vector<FigConfigErasXY>& ReadConfig::getFigConfigVecErasXY() const {
    return figConfigVecErasXY_;
}
const std::vector<FigConfigEraXY>& ReadConfig::getFigConfigVecEraXY() const {
    return figConfigVecEraXY_;
}

