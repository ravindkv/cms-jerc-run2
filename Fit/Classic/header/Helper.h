#pragma once

#include <iostream>
#include <string>
#include <string_view>
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

static constexpr double kFuncRangeMin = 10.0;   // Fit range minimum.
static constexpr double kFuncRangeMax = 6500.0;   // Fit range maximum.

namespace Helper {

/**
 * @brief Generates a string containing n white spaces.
 *
 * @param n Number of white spaces.
 * @return std::string with n white spaces.
 */
std::string nWhiteSpaces(int n);

/**
 * @brief Pads the input text with white spaces until reaching ntot characters.
 *
 * @param text The input string.
 * @param ntot The total desired length.
 * @return TString with padding.
 */
TString completeWhiteSpaces(const TString& text, int ntot = 20);

/**
 * @brief Checks if a substring exists within a string.
 *
 * @param search The substring to search for.
 * @param str The string to search within.
 * @return true if found, false otherwise.
 */
bool findInString(const std::string_view search, const std::string_view str);

/**
 * @brief Template function to find an element in a vector.
 *
 * @tparam T Type of elements in the vector.
 * @param vec The vector to search.
 * @param el The element to find.
 * @return Index of the element if found, -1 otherwise.
 */
template <typename T>
int findInVector(const std::vector<T>& vec, const T& el) {
    auto it = std::find(vec.begin(), vec.end(), el);
    return (it != vec.end()) ? static_cast<int>(std::distance(vec.begin(), it)) : -1;
}

/**
 * @brief Template function to check if a key exists in a map.
 *
 * @tparam T Type of the map’s value.
 * @param mp The map to search.
 * @param el The key to find.
 * @return true if key exists, false otherwise.
 */
template <typename T>
bool findInMap(const std::map<TString, T>& mp, const TString& el) {
    return (mp.find(el) != mp.end());
}

/**
 * @brief Converts a value to a string with fixed precision.
 *
 * @tparam T Type of the value.
 * @param value The value to convert.
 * @param n Number of decimal places.
 * @return std::string representation of the value.
 */
template <typename T>
std::string toStringWithPrecision(const T value, const int n = 3) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << value;
    return out.str();
}

// Print utilities

/**
 * @brief Generic print function.
 *
 * @tparam T Type of the text.
 * @param text The text to print.
 * @param color Optional color formatting.
 */
template <typename T>
void print(const T& text, const std::string& color = "")
{
    std::cout << color << text << colReset;
}

/**
 * @brief Overloaded print function for vectors (prints space-separated values).
 *
 * @tparam T Type of the vector elements.
 * @param vec The vector to print.
 * @param color Optional color formatting.
 */
template <typename T>
void print(const std::vector<T>& vec, const std::string& color = "")
{
    std::cout << color;
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << (i + 1 < vec.size() ? " " : "");
    }
    std::cout << colReset;
}

/**
 * @brief Prints text followed by a newline.
 *
 * @tparam T Type of the text.
 * @param text The text to print.
 * @param color Optional color formatting.
 */
template <typename T>
void printLine(const T& text, const std::string& color = "") {
    print(text, color);
    std::cout << std::endl;
}

/**
 * @brief Prints a loading message with type, name, and object name.
 *
 * @param type The type of loading.
 * @param name The name associated with the loading process.
 * @param objname The object name.
 * @param color Optional color formatting.
 */
void printLoading(TString type, TString name, TString objname, const std::string& color = colYellow);

/**
 * @brief Prints a parameter with its value and error.
 *
 * @param name The parameter name.
 * @param val The value.
 * @param err The error.
 * @param color Optional color formatting.
 */
void printParameter(TString name, double val, double err, const std::string& color = colBlue);

// Graph/Histo Utilities

void removeZerosFromGraph(TGraphErrors* g);
double oPlus(double a, double b);

// Fit error and transformation functions.
double fitError(TF1* func, TMatrixD errMatrix, double x, double k = 1);
void funcToGraph(TF1* func, TMatrixD errMatrix, TGraphErrors* graph, double k = 1);
void funcToHist(TF1* func, TMatrixD errMatrix, TH1D* hist, double k = 1);

void multiplyGraph(TGraphErrors* graph, double scale);
void multiplyGraph(TGraphErrors* graph, TF1* func);
void propagateErrorToGraph(TGraphErrors* graph, std::vector<TF1*> funcs, TMatrixD errMatrix);

} // namespace Helper

