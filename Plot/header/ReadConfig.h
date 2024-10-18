#ifndef READCONFIG_H
#define READCONFIG_H

#include <string>
#include <map>
#include "FigConfig.h" // Assuming you have this struct defined
#include <nlohmann/json.hpp> // Include the JSON library header

class ReadConfig {
public:
    ReadConfig(const std::string& configFilePath);
    const std::map<std::string, FigConfig>& getFigConfigMap() const;

private:
    void readConfigFile(const std::string& configFilePath);
    std::map<std::string, FigConfig> figConfigMap_;
};

#endif // READCONFIG_H

