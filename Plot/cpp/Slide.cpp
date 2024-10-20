#include "Slide.h"
#include <iostream>
#include <cmath>

Slide::Slide(const std::string& latexFile) {
    file_.open(latexFile);
    if (!file_.is_open()) {
        std::cerr << "Error: Could not open LaTeX file: " << latexFile << std::endl;
    }
}

Slide::~Slide() {
    if (file_.is_open()) {
        file_.close();
    }
}

double Slide::calculateWidth(int totalPlots) {
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

    return plotWidth;
}

void Slide::startDocument(const std::string& title) {
    file_ << "\\documentclass{beamer}\n";
    file_ << "\\usepackage{graphicx}\n";  // Needed for including graphics
    file_ << "\\begin{document}\n";
    file_ << "\\title{" << title << "}\n";
    file_ << "\\author{Ravindra Verma (HIP)}\n";
    file_ << "\\institute{JERC Meeting}\n";
    file_ << "\\frame{\\titlepage}\n";
}

// Function to escape special characters in a string for LaTeX
std::string Slide::formatString(const std::string &str) {
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



void Slide::addTextSlide(const std::vector<std::pair<std::string, std::vector<std::string>>>& textsForSlide, const std::string& slideTitle) {
    file_ << "\n\\begin{frame}{" << slideTitle << "}\n";

    // Determine the total number of items for font size adjustment
    size_t totalItems = 0;
    for (const auto& entry : textsForSlide) {
        totalItems += entry.second.size(); // Count the number of triggers
    }

    // Determine the font size based on the total number of items
    std::string fontSize;
    if (totalItems <= 5) {
        fontSize = "\\normalsize"; // Default size
    } else if (totalItems <= 8) {
        fontSize = "\\footnotesize"; // Even smaller size
    } else if (totalItems <= 10) {
        fontSize = "\\scriptsize"; // Even smaller size
    } else {
        fontSize = "\\tiny"; // Very small size
    }

    // Loop through each year and its corresponding triggers while maintaining insertion order
    file_ << "\\begin{itemize}\n";
    for (const auto& entry : textsForSlide) {
        file_ << "  \\item " << formatString(entry.first) << "\n"; // Year as the main item
        file_ << "  \\begin{itemize}\n";  // Begin nested itemize for triggers
        file_ << "    "<<fontSize << "\n";
        for (const auto& text : entry.second) {
            file_ << "    \\item " << formatString(text) << "\n"; // Triggers as sub-items
        }
        file_ << "  \\end{itemize}\n";  // End nested itemize
    }
    file_ << "\\end{itemize}\n";
    file_ << "\\end{frame}\n\n";
}


void Slide::addPlotSlide(const std::vector<std::string>& plotsForSlide, const std::string& slideTitle) {
    file_ << "\n\\begin{frame}{" << slideTitle << "}\n";

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
    file_ << "\\end{frame}\n\n";
}

void Slide::addCenteredTextSlide(const std::string& centeredText) {
    // Start writing the slide structure
    file_ << "\\begin{frame}\n";
    file_ << "    \\centering\n";
    file_ << "    {\\Large " << centeredText << "}\n"; // Adding centered large text
    file_ << "\\end{frame}\n\n";
}

void Slide::endDocument() {
    file_ << "\\end{document}\n";
}

