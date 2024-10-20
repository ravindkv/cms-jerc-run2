#pragma once

#ifndef HELPER_H
#define HELPER_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <TGraphErrors.h>
#include <TH1.h> // For TH1 histograms

class Helper {
public:
    // Function to retrieve keys from a JSON object
    static std::vector<std::string> getEras(const nlohmann::json& inputJson, const std::string& channel, const std::string& year, const std::string& key);

    // General function to calculate ratio and fill a TGraphErrors
    template<typename T>
    static void calculateHistRatio(T* dataHist, T* mcHist, TGraphErrors* ratioGraph);

    // General function to combine multiple histograms
    template<typename T>
    static T* combineHists(const std::vector<T*>& hists);
};

// Include the template implementation here
#include "../cpp/Helper.tpp"

#endif // HELPER_H
