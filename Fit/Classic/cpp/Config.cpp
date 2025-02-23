#include "Config.h"
#include <fstream>
#include <iostream>

Config::Config(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Error opening config file: " << filename << std::endl;
        return;
    }
    try {
        ifs >> configJson;
    } catch (const std::exception &e) {
        std::cerr << "Exception reading config file: " << e.what() << std::endl;
    }
}

bool Config::isValid() const {
    return !configJson.empty();
}

const nlohmann::json& Config::getRootFile() const {
    return configJson["input_file"];
}

const nlohmann::json& Config::getBase() const {
    return configJson["input_base"];
}

const nlohmann::json& Config::getSyst() const {
    return configJson["input_syst"];
}

const nlohmann::json& Config::getNorm() const {
    return configJson["input_norm"];
}

const nlohmann::json& Config::getShape() const {
    return configJson["input_shape"];
}

const nlohmann::json& Config::getFsr() const {
    return configJson["fsr"];
}

