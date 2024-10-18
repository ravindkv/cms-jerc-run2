#include "Helper.h"

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

