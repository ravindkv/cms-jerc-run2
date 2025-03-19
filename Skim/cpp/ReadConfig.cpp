#include "ReadConfig.h"

ReadConfig::ReadConfig(const std::string& commonConfigPath, const std::string& channelConfigPath) {
    commonConfig_ = loadConfig(commonConfigPath);
    channelConfig_ = loadConfig(channelConfigPath);
}

nlohmann::json ReadConfig::loadConfig(const std::string& configPath) {
    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Could not open config file: " + configPath);
    }
    nlohmann::json config;
    configFile >> config;
    return config;
}

std::vector<std::string> ReadConfig::getFilterList(const std::string& runPeriod) const {
    if (!commonConfig_.contains("filters") || !commonConfig_["filters"].contains(runPeriod)) {
        throw std::runtime_error("Filters for run period " + runPeriod + " not found in common config.");
    }
    return commonConfig_["filters"][runPeriod].get<std::vector<std::string>>();
}

std::vector<std::string> ReadConfig::getCommonTreeBranches() const {
    if (!commonConfig_.contains("commonTreeBranches")) {
        throw std::runtime_error("Common config does not contain commonTreeBranches.");
    }
    return commonConfig_["commonTreeBranches"].get<std::vector<std::string>>();
}

std::vector<std::string> ReadConfig::getChannelTreeBranches() const {
    if (!channelConfig_.contains("treeBranches")) {
        return std::vector<std::string>(); // Not an error if none are specified.
    }
    return channelConfig_["treeBranches"].get<std::vector<std::string>>();
}

std::vector<std::string> ReadConfig::getCommonMCBranches() const {
    if (!commonConfig_.contains("commonMCBranches")) {
        return std::vector<std::string>();
    }
    return commonConfig_["commonMCBranches"].get<std::vector<std::string>>();
}

std::vector<std::string> ReadConfig::getChannelMCBranches() const {
    if (!channelConfig_.contains("mcBranches")) {
        return std::vector<std::string>();
    }
    return channelConfig_["mcBranches"].get<std::vector<std::string>>();
}

std::vector<std::string> ReadConfig::getTriggerList(const std::string& runPeriod) const {
    if (!channelConfig_.contains("triggers") || !channelConfig_["triggers"].contains(runPeriod)) {
        throw std::runtime_error("Triggers for run period " + runPeriod + " not found in channel config.");
    }
    return channelConfig_["triggers"][runPeriod].get<std::vector<std::string>>();
}

