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

    for (auto& [name, params] : configJson.items()) {
        FigConfig figConfig;
        figConfig.dirName = params.value("dirName", "");
        figConfig.xTitle = params.value("xTitle", "");
        figConfig.yTitle = params.value("yTitle", "");
        figConfig.zTitle = params.value("zTitle", "");
        figConfig.rTitle = params.value("rTitle", "");
        figConfig.varName = params.value("varName", "");
        figConfig.xMin = params.value("xMin", 0.0);
        figConfig.xMax = params.value("xMax", 0.0);
        figConfig.yMin = params.value("yMin", 0.0);
        figConfig.yMax = params.value("yMax", 0.0);
        figConfig.rMin = params.value("rMin", 0.0);
        figConfig.rMax = params.value("rMax", 2.0);
        figConfig.varMin = params.value("varMin", 0.0);
        figConfig.varMax = params.value("varMax", 2.0);
        figConfig.varBins = params.value("varBins", std::vector<double>{});
        figConfig.xLog = params.value("xLog", false);
        figConfig.yLog = params.value("yLog", false);
        figConfig.zLog = params.value("zLog", false);
        figConfig.isNorm = params.value("isNorm", false);
        figConfig.isVarOnX = params.value("isVarOnX", false);
        figConfigMap_[name] = figConfig;
    }
}

const std::map<std::string, FigConfig>& ReadConfig::getFigConfigMap() const {
    return figConfigMap_;
}

