#include "Note.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>

Note::Note(const std::string& latexFile) {
    file_.open(latexFile);
    if (!file_.is_open()) {
        std::cerr << "Error: Could not open LaTeX file: " << latexFile << std::endl;
    }
}

Note::~Note() {
    if (file_.is_open()) {
        file_.close();
    }
}

double Note::calculateWidth(int totalPlots) {
    double plotWidth = 0.2; // Default width
    int rows = 1;
    int columns = 1;

    // Mapping totalPlots to specific grid layouts and widths
    if (totalPlots == 1) {
        rows = 1;
        columns = 1;
        plotWidth = 0.75;
    }
    else if (totalPlots == 2) {
        rows = 1;
        columns = 2;
        plotWidth = 0.45;
    }
    else if (totalPlots == 3 || totalPlots == 4) {
        rows = 2;
        columns = 2;
        plotWidth = 0.38;
    }
    else if (totalPlots == 5 || totalPlots == 6) {
        rows = 2;
        columns = 3;
        plotWidth = 0.3;
    }
    else if (totalPlots >= 7 && totalPlots <= 9) {
        rows = 3;
        columns = 3;
        plotWidth = 0.27;
    }
    else if (totalPlots == 10) {
        rows = 4;
        columns = 3;
        plotWidth = 0.22;
    }
    else {
        // Dynamic calculation for totalPlots > 10 or other unspecified cases
        double margin = 0.02;
        // Calculate the optimal number of columns (closest integer to the square root of totalPlots)
        columns = static_cast<int>(std::ceil(std::sqrt(totalPlots)));

        // Calculate the number of rows needed
        rows = (totalPlots + columns - 1) / columns;  // Ensures any leftover plots get a row

        // Recalculate columns if the total width exceeds 1.0 (slide width)
        plotWidth = (1.0 - (columns - 1) * margin) / columns;
        while (plotWidth <= 0 && columns > 1) {
            columns--;  // Reduce columns to fit the width
            rows = (totalPlots + columns - 1) / columns;  // Update rows accordingly
            plotWidth = (1.0 - (columns - 1) * margin) / columns;
        }

        // If plotWidth is still non-positive, set to a minimal default
        if (plotWidth <= 0) {
            plotWidth = 0.1;  // Minimal width to prevent negative or zero width
        }
    }

    // Vertical space: Account for the title reducing available space
    double margin = 0.02;       // Assuming margin is also used vertically
    double titleFraction = 0.2;
    double availableHeight = 1.0 - titleFraction;  // Remaining vertical space after the title
    double plotHeight = (availableHeight - (rows - 1) * margin) / rows;  // Divide remaining height by the number of rows

    // Debugging output
    std::cout << "Total Plots: " << totalPlots 
              << " | Rows: " << rows 
              << ", Columns: " << columns 
              << ", PlotWidth: " << plotWidth 
              << ", PlotHeight: " << plotHeight 
              << std::endl;

    //return plotWidth;
    return 0.48;
}

void Note::startDocument(const std::string& title) {
    file_ << "\\documentclass{article}\n";
    file_ << "\\usepackage{graphicx}\n";  // Needed for including graphics
    file_ << "\\begin{document}\n";
    file_ << "\\title{" << title << "}\n";
    file_ << "\\author{Ravindra Verma (HIP)}\n";
    file_ << "\\institute{JERC Meeting}\n";
    //file_ << "\\frame{\\titlepage}\n";
}

// Function to escape special characters in a string for LaTeX
std::string Note::formatString(const std::string &str) {
    std::string escapedStr;
    for (char c : str) {
        if (c == '_') {
            escapedStr += "\\_";  // Escape underscore
        } else if (c == '>') {
            escapedStr += "$>$";  // Escape greater than
        } else if (c == '<') {
            escapedStr += "$<$";  // Escape less than
        } else if (c == '|') {
            escapedStr += "$|$";  // Escape vertical bar
        } else if (c == '~') {
            escapedStr += "$\\approx$";  ///Approx
        } else {
            escapedStr += c;
        }
    }
    return escapedStr;
}

void Note::addPlotNote(const std::vector<std::string>& plotsForNote, const std::string& figCaption) {
    //file_ << "\n\\begin{frame}{" << figCaption << "}\n";

    int numPlots = plotsForNote.size();
    if (numPlots == 0) {
        file_ << "No plots provided.\n";
    } else {
        double width = calculateWidth(numPlots); 
        file_ << "\\begin{figure}\n";
        for (const auto& plot : plotsForNote) {
            file_ << "\\includegraphics[width=" << width << "\\textwidth]{" << plot << "}\n";
        }
        file_ << "\\caption{\\cap"<<figCaption<<"}\n";
        file_ << "\\label{fig"<<figCaption<<"}\n";
        file_ << "\\end{figure}\n\n";
    }
    //file_ << "\\end{frame}\n\n";
}

void Note::endDocument() {
    file_ << "\\end{document}\n";
}

