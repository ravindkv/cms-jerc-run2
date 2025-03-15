#include "TrigDetail.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

// Constructor implementation
TrigDetail::TrigDetail(const GlobalFlag& globalFlags)
    : globalFlags_(globalFlags),
      year_(globalFlags_.getYear()),
      channel_(globalFlags_.getChannel()),
      isDebug_(globalFlags_.isDebug()) {

    loadConfig();
}

void TrigDetail::loadConfig() {
    // Reserve capacity as before (optional)
    trigMapRangePt_.reserve(15);
    trigMapRangePtEta_.reserve(30);
    
    // Get string representations for year and channel.
    std::string yearStr = globalFlags_.getYearStr();
    std::string channelStr = globalFlags_.getChannelStr();
    
    // Build the file name for the current channel's configuration.
    std::string configFile = "config/TrigDetail" + channelStr + ".json";
    std::ifstream ifs(configFile);
    if (!ifs.is_open()) {
        std::cerr << "Error: Could not open " << configFile << std::endl;
        return;
    }
    
    json j;
    try {
        ifs >> j;
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error in " << configFile << ": " << e.what() << std::endl;
        return;
    }
    
    // Ensure the JSON contains the current year.
    if (!j.contains(yearStr)) {
        std::cerr << "Error: " << configFile << " does not contain configuration for year " << yearStr << std::endl;
        return;
    }
    
    // Retrieve the configuration for the active year.
    auto yearConfig = j.at(yearStr);
    
    // For channels with a simple trigger list.
    if (channel_ == GlobalFlag::Channel::ZeeJet ||
        channel_ == GlobalFlag::Channel::ZmmJet ||
        channel_ == GlobalFlag::Channel::Wqqe ||
        channel_ == GlobalFlag::Channel::Wqqm) {
        
        trigList_ = yearConfig.get<std::vector<std::string>>();
    }
    // For channels with a trigger map using only pt (e.g., GamJet).
    else if (channel_ == GlobalFlag::Channel::GamJet) {
        for (auto& item : yearConfig.items()) {
            const std::string& key = item.key();
            json value = item.value();
            double ptMin = value.at("ptMin").get<double>();
            double ptMax = value.at("ptMax").get<double>();
            trigMapRangePt_[key] = TrigRangePt{ ptMin, ptMax };
        }
    }
    // For channels with a trigger map using pt and eta (e.g., MultiJet).
    else if (channel_ == GlobalFlag::Channel::MultiJet) {
        for (auto& item : yearConfig.items()) {
            const std::string& key = item.key();
            json value = item.value();
            int trigPt = value.at("trigPt").get<int>();
            double ptMin = value.at("ptMin").get<double>();
            double ptMax = value.at("ptMax").get<double>();
            double absEtaMin = value.at("absEtaMin").get<double>();
            double absEtaMax = value.at("absEtaMax").get<double>();
            trigMapRangePtEta_[key] = TrigRangePtEta{ trigPt, ptMin, ptMax, absEtaMin, absEtaMax };
        }
    }
}

const std::vector<std::string> TrigDetail::getTrigNames() const {
    std::vector<std::string> tNames;
    if (channel_ == GlobalFlag::Channel::ZeeJet ||
        channel_ == GlobalFlag::Channel::ZmmJet ||
        channel_ == GlobalFlag::Channel::Wqqe ||
        channel_ == GlobalFlag::Channel::Wqqm) {
        tNames = trigList_;
    } else if (channel_ == GlobalFlag::Channel::GamJet) {
        for (const auto& trig : trigMapRangePt_) {
            tNames.push_back(trig.first);
        }
    } else if (channel_ == GlobalFlag::Channel::MultiJet) {
        for (const auto& trig : trigMapRangePtEta_) {
            tNames.push_back(trig.first);
        }
    }
    return tNames;
}

// Destructor
TrigDetail::~TrigDetail() {
    // No dynamic memory to clean up in this class
}

