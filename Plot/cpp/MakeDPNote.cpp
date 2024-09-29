#include "MakeDPNote.h"
#include <iostream>
#include <cmath>

MakeDPNote::MakeDPNote(const std::string& latexFile) {
    file_.open(latexFile);
    if (!file_.is_open()) {
        std::cerr << "Error: Could not open LaTeX file: " << latexFile << std::endl;
    }
}

MakeDPNote::~MakeDPNote() {
    if (file_.is_open()) {
        file_.close();
    }
}

double MakeDPNote:: calculateWidth(int totalPlots){
    double margin = 0.02;
		double titleFraction = 0.2;
    // Calculate the optimal number of columns (closest integer to the square root of totalPlots)
    int columns = static_cast<int>(std::ceil(std::sqrt(totalPlots)));
    
    // Calculate the number of rows needed
    int rows = (totalPlots + columns - 1) / columns;  // Ensures any leftover plots get a row

    // Calculate plot width based on columns and margin
    double  plotWidth = (1.0 - (columns - 1) * margin) / columns;

   	 // Vertical space: Account for the title reducing available space
    // Assuming titleFraction is the fraction of vertical space taken by the title (e.g., 0.1 for 10%)
    double availableHeight = 1.0 - titleFraction;  // Remaining vertical space after the title
    double plotHeight = availableHeight / rows;           // Divide remaining height by the number of rows
    return plotHeight;
}

void MakeDPNote::startDocument(const std::string& title) {
    file_ << "\\documentclass{beamer}\n";
    file_ << "\\usepackage{graphicx}\n";  // Needed for including graphics
    file_ << "\\begin{document}\n";
    file_ << "\\title{" << title << "}\n";
    file_ << "\\frame{\\titlepage}\n";
}

// Add slide that accepts a map of plot paths and descriptions
void MakeDPNote::addSlide(const std::vector<std::string>& plotsForSlide, const std::string& slideTitle) {
    file_ << "\\begin{frame}{" << slideTitle << "}\n";

    int numPlots = plotsForSlide.size();
    if (numPlots == 0) {
        file_ << "No plots provided.\n";
    } else {
        double width = calculateWidth(numPlots); 
        file_ << "\\begin{figure}\n";
        for (const auto& plot : plotsForSlide) {
            file_ << "\\includegraphics[width=" << width << "\\textwidth]{" << plot << "}\n";
        }
        file_ << "\\end{figure}\n";
    }
    file_ << "\\end{frame}\n";
}

void MakeDPNote::endDocument() {
    file_ << "\\end{document}\n";
}

