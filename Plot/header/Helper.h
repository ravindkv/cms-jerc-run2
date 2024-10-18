#ifndef HELPER_H
#define HELPER_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Helper {
public:
    // Function to retrieve keys from a JSON object
    static std::vector<std::string> getEras(const nlohmann::json& inputJson, const std::string& channel, const std::string& year, const std::string& key);
};

#endif // HELPER_H

