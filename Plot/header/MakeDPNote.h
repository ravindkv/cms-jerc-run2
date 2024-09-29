#ifndef MAKEDPPNOTE_H
#define MAKEDPPNOTE_H

#include <vector>
#include <string>
#include <fstream>

class MakeDPNote {
public:
    MakeDPNote(const std::string& latexFile);
    ~MakeDPNote();

    void startDocument(const std::string& title);
    double calculateWidth(int totalPlots);

    void addSlide(const std::vector<std::string>& plotsForSlide, const std::string& slideTitle = "");
    
    void endDocument();

private:
    std::ofstream file_;
};

#endif // DPNOTE_H

