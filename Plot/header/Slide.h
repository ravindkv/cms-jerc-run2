#pragma once

#ifndef SLIDE_H
#define SLIDE_H

#include <vector>
#include <map>
#include <string>
#include <fstream>

class Slide {
public:
    Slide(const std::string& latexFile);
    ~Slide();

    void startDocument(const std::string& title);
    double calculateWidth(int totalPlots);

    void addTextSlide(const std::vector<std::pair<std::string, std::vector<std::string>>>& textsForSlide, const std::string& slideTitle = "");
    void addPlotSlide(const std::vector<std::string>& plotsForSlide, const std::string& slideTitle = "");
    void addCenteredTextSlide(const std::string& centeredText);
    
    void endDocument();

private:
    std::ofstream file_;
    std::string formatString(const std::string &str);
};

#endif // SLIDE_H

