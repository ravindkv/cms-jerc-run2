#pragma once

#ifndef NOTE_H
#define NOTE_H

#include <vector>
#include <map>
#include <string>
#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Note {
public:
    Note(const std::string& latexFile);
    ~Note();

    void startDocument(const std::string& title);
    double calculateWidth(int totalPlots);

    void addPlotNote(const std::vector<std::string>& plotsForNote, const std::string& figCaption);

    void endDocument();

private:
    std::ofstream file_;
    std::string formatString(const std::string &str);
};

#endif // NOTE_H

