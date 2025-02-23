#pragma once

#include <string>
#include <map>
#include <nlohmann/json.hpp>

/**
 * @brief Manages configuration loading from a JSON file.
 */
class Config {
public:
    explicit Config(const std::string& filename);
    bool isValid() const;

    // Accessors for configuration sections.
    const nlohmann::json& getRootFile() const;
    const nlohmann::json& getNorm() const;
    const nlohmann::json& getBase() const;
    const nlohmann::json& getSyst() const;
    const nlohmann::json& getShape() const;
    const nlohmann::json& getFsr() const;

private:
    nlohmann::json configJson;
};

