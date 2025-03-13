#include "TrigDetail.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

// Constructor implementation
TrigDetail::TrigDetail(const GlobalFlag& globalFlags)
    :globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()){

    InitializeList();
}

void TrigDetail::InitializeList() {
    // Reserve capacity as before (optional)
    trigMapRangePt_.reserve(15);
    trigMapRangePtEta_.reserve(30);
    
    // Open and parse the JSON configuration file
    std::ifstream ifs("config/TrigDetail.json");
    if (!ifs.is_open()) {
        std::cerr << "Error: Could not open config/TrigDetail.json" << std::endl;
        return;
    }
    json j;
    try {
        ifs >> j;
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return;
    }
    
    // Use GlobalFlag helper functions for string representations.
    std::string yearStr = globalFlags_.getYearStr();
    std::string channelStr = globalFlags_.getChannelStr();
    
    // Look up the channel configuration under "Channels"
    try {
        json channelConf = j.at("Channels").at(channelStr);
        
        // For channels with a base trigger list (simple list)
        if (channelConf.contains("baseTrigList")) {
            trigList_ = channelConf.at("baseTrigList").get<std::vector<std::string>>();
            // Append year-specific trigger list if available
            if (channelConf.contains("Years") && channelConf["Years"].contains(yearStr)) {
                json yearConf = channelConf["Years"].at(yearStr);
                if (yearConf.contains("trigList")) {
                    auto yearTrigList = yearConf.at("trigList").get<std::vector<std::string>>();
                    trigList_.insert(trigList_.end(), yearTrigList.begin(), yearTrigList.end());
                }
            }
        }
        
        // For channels with a base trigger map using only pt (e.g., GamJet)
        if (channelConf.contains("baseTrigMapRangePt")) {
            json baseMap = channelConf.at("baseTrigMapRangePt");
            for (auto& item : baseMap.items()) {
                const std::string& key = item.key();
                json value = item.value();
                double ptMin = value.at("ptMin").get<double>();
                double ptMax = value.at("ptMax").get<double>();
                trigMapRangePt_[key] = TrigRangePt{ ptMin, ptMax };
            }
            // Merge year-specific overrides if available
            if (channelConf.contains("Years") && channelConf["Years"].contains(yearStr)) {
                json yearConf = channelConf["Years"].at(yearStr);
                if (yearConf.contains("trigMapRangePt")) {
                    json yearMap = yearConf.at("trigMapRangePt");
                    for (auto& item : yearMap.items()) {
                        const std::string& key = item.key();
                        json value = item.value();
                        double ptMin = value.at("ptMin").get<double>();
                        double ptMax = value.at("ptMax").get<double>();
                        // Overwrite or add the key from the year-specific configuration
                        trigMapRangePt_[key] = TrigRangePt{ ptMin, ptMax };
                    }
                }
            }
        }
        
        // For channels with a base trigger map using pt and eta (e.g., MultiJet)
        if (channelConf.contains("baseTrigMapRangePtEta")) {
            json baseMap = channelConf.at("baseTrigMapRangePtEta");
            for (auto& item : baseMap.items()) {
                const std::string& key = item.key();
                json value = item.value();
                int trigPt = value.at("trigPt").get<int>();
                double ptMin = value.at("ptMin").get<double>();
                double ptMax = value.at("ptMax").get<double>();
                double absEtaMin = value.at("absEtaMin").get<double>();
                double absEtaMax = value.at("absEtaMax").get<double>();
                trigMapRangePtEta_[key] = TrigRangePtEta{ trigPt, ptMin, ptMax, absEtaMin, absEtaMax };
            }
            // Merge any year-specific changes
            if (channelConf.contains("Years") && channelConf["Years"].contains(yearStr)) {
                json yearConf = channelConf["Years"].at(yearStr);
                if (yearConf.contains("trigMapRangePtEta")) {
                    json yearMap = yearConf.at("trigMapRangePtEta");
                    for (auto& item : yearMap.items()) {
                        const std::string& key = item.key();
                        json value = item.value();
                        int trigPt = value.at("trigPt").get<int>();
                        double ptMin = value.at("ptMin").get<double>();
                        double ptMax = value.at("ptMax").get<double>();
                        double absEtaMin = value.at("absEtaMin").get<double>();
                        double absEtaMax = value.at("absEtaMax").get<double>();
                        // Overwrite or add the key from the year-specific configuration
                        trigMapRangePtEta_[key] = TrigRangePtEta{ trigPt, ptMin, ptMax, absEtaMin, absEtaMax };
                    }
                }
            }
        }
        
        // Optionally, you can also merge Data/MC specific configurations here
        // For example, if channelConf contains a "Data" block with additional triggers.
        
    } catch (json::exception& e) {
        std::cerr << "Error reading configuration for channel " << channelStr
                  << " and year " << yearStr << ": " << e.what() << std::endl;
    }
}

const std::vector<std::string> TrigDetail::getTrigNames() const{
    std::vector<std::string> tNames;
    if(channel_ == GlobalFlag::Channel::ZeeJet
      || channel_ == GlobalFlag::Channel::ZmmJet
      || channel_ == GlobalFlag::Channel::Wqqe
      || channel_ == GlobalFlag::Channel::Wqqm
    ){
        tNames = trigList_;
    }        
    else if(channel_ == GlobalFlag::Channel::GamJet){
        for (const auto& trig : trigMapRangePt_) {
            tNames.push_back(trig.first);
        }
    }
    else if(channel_ == GlobalFlag::Channel::MultiJet){
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

