#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

class ReadConfig {
public:
    ReadConfig(const std::string& commonConfigPath, const std::string& channelConfigPath);

    // Accessor functions to retrieve lists from the configs.
    std::vector<std::string> getFilterList(const std::string& runPeriod) const;
    std::vector<std::string> getCommonTreeBranches() const;
    std::vector<std::string> getChannelTreeBranches() const;
    std::vector<std::string> getCommonMCBranches() const;
    std::vector<std::string> getChannelMCBranches() const;
    std::vector<std::string> getTriggerList(const std::string& runPeriod) const;

private:
    nlohmann::json commonConfig_;
    nlohmann::json channelConfig_;

    nlohmann::json loadConfig(const std::string& configPath);
};

