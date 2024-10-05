#ifndef MAKEDPPNOTE_H
#define MAKEDPPNOTE_H

#include <vector>
#include <map>
#include <string>
#include <fstream>

class MakeDPNote {
public:
    MakeDPNote(const std::string& latexFile);
    ~MakeDPNote();

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

#endif // DPNOTE_H

