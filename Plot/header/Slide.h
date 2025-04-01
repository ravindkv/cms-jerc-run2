#pragma once

#ifndef SLIDE_H
#define SLIDE_H

#include <vector>
#include <map>
#include <string>
#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Slide {
public:
    Slide(const std::string& latexFile);
    ~Slide();

    void startDocument(const std::string& title);
    double calculateWidth(int totalPlots);

    void addTextSlide(const std::vector<std::pair<std::string, std::vector<std::string>>>& textsForSlide, const std::string& slideTitle = "");
    void addPlotSlide(const std::vector<std::string>& plotsForSlide, const std::string& slideTitle = "");
    void addCenteredTextSlide(const std::string& centeredText);

    void makeSlideFromJson(const std::string &jsonContent);
    void makeSlideFromJsonFile(const std::string &jsonFilePath);
    
    void endDocument();

private:
    std::ofstream file_;
    std::string formatString(const std::string &str);
    void addLatexSlide(const std::string& slideTitle, const std::string& latexContent);
    std::string processJson(const json& j, int indent = 0);
    std::vector<std::string> splitLatexContent(const std::string &latexContent, size_t maxRows);
};

#endif // SLIDE_H

