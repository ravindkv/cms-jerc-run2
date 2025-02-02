#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <cmath>

#include "TF1.h"
#include "TH1D.h"
#include "TMatrixD.h"
#include "TGraphErrors.h"

// Text color definitions (inline ensures a single definition in modern C++)
inline constexpr const char* colRed     = "\x1b[0;31m";
inline constexpr const char* colGreen   = "\x1b[0;32m";
inline constexpr const char* colYellow  = "\x1b[0;33m";
inline constexpr const char* colBlue    = "\x1b[0;34m";
inline constexpr const char* colMagenta = "\x1b[0;35m";
inline constexpr const char* colCyan    = "\x1b[0;36m";
inline constexpr const char* colBold    = "\033[1m";
inline constexpr const char* colReset   = "\x1b[0m";

namespace ClassicFit {
namespace Helper {

std::string nWhiteSpaces(int n);
TString completeWhiteSpaces(const TString& text, int ntot = 20);

bool findInString(const std::string& search, const std::string& str);

template <typename T>
int findInVector(const std::vector<T>& vec, const T& el) {
    auto it = std::find(vec.begin(), vec.end(), el);
    return (it != vec.end()) ? static_cast<int>(std::distance(vec.begin(), it)) : -1;
}

template <typename T>
bool findInMap(const std::map<TString, T>& mp, const TString& el) {
    return (mp.find(el) != mp.end());
}

template <typename T>
std::string toStringWithPrecision(const T value, const int n = 3) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << value;
    return out.str();
}

// Print utilities
// General template for printing
template <typename T>
void print(const T& text, const std::string& color = "")
{
    std::cout << color << text << colReset;
}

// Overload for std::vector<T> to print space-separated values
template <typename T>
void print(const std::vector<T>& vec, const std::string& color = "")
{
    std::cout << color;
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << (i + 1 < vec.size() ? " " : "");
    }
    std::cout << colReset;
}

template <typename T>
void printLine(const T& text, const std::string& color = "") {
    print(text, color);
    std::cout << std::endl;
}

void printLoading(TString type, TString name, TString objname, const std::string& color = colYellow);
void printParameter(TString name, double val, double err, const std::string& color = colBlue);

// Graph/Histo Utilities
void removeZerosFromGraph(TGraphErrors* g);
double oPlus(double a, double b);

// Fit error and transformations
double fitError(TF1* func, TMatrixD errMatrix, double x, double k = 1);
void funcToGraph(TF1* func, TMatrixD errMatrix, TGraphErrors* graph, double k = 1);
void funcToHist(TF1* func, TMatrixD errMatrix, TH1D* hist, double k = 1);

void multiplyGraph(TGraphErrors* graph, double scale);
void multiplyGraph(TGraphErrors* graph, TF1* func);
void propagateErrorToGraph(TGraphErrors* graph, std::vector<TF1*> funcs, TMatrixD errMatrix);

} // namespace Helper
} // namespace ClassicFit

