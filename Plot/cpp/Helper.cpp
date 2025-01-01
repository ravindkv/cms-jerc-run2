#include "Helper.h"
#include <cmath> // For sqrt, pow

// Implementation of the getEras function
std::vector<std::string> Helper::getEras(const nlohmann::json& inputJson, const std::string& channel, const std::string& year, const std::string& key) {
    std::vector<std::string> dataEras;
    if (inputJson.contains(channel) && inputJson[channel].contains(year) && inputJson[channel][year].contains(key)) {
        for (const auto& element : inputJson[channel][year][key].items()) {
            dataEras.push_back(element.key());
        }
    }
    return dataEras;
}

std::string Helper::dirToName(const std::string& dir) {
    std::string filename = dir;
    // Replace all '/' with '_'
    std::replace(filename.begin(), filename.end(), '/', '_');
    // Append .pdf extension
    return filename;
}

// Splits a string `str` by the given `delimiter` and returns a vector of tokens
std::vector<std::string> Helper::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
