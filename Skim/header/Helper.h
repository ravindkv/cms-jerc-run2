#pragma once

#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <iostream>
#include <regex>
#include <vector>
#include <chrono>
#include <iomanip>
#include "Rtypes.h"
#include "TH1D.h"
#include "TTree.h"
#include "TBranch.h"
#include "TObjArray.h"

class Helper {
public:
    //Function to split a vector into smallare vectors
    static std::vector<std::vector<std::string>> splitVector(const std::vector<std::string>& strings, std::size_t n);
    
    // Function to split a string into smaller vectors of string 
    static std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);


    static void initProgress(); 
    static void printProgress(Long64_t jentry, Long64_t nentries, 
                       std::chrono::time_point<std::chrono::high_resolution_clock>& startClock, 
                       double& totTime);
    
    static void printCutflow(const TH1D* hist);

    /**
     * @brief Converts a wildcard pattern to a regular expression pattern.
     *
     * This function transforms wildcard characters '*' and '?' into their
     * corresponding regular expression equivalents '.*' and '.', respectively.
     * It also escapes other regex-special characters to ensure accurate matching.
     *
     * @param wildcard The wildcard pattern string.
     * @return A std::string containing the equivalent regular expression pattern.
     */
    static std::string WildcardToRegex(const std::string& wildcard);


    /**
     * @brief Retrieves branch names from a TTree that match any of the provided wildcard patterns.
     *
     * @param tree Pointer to the TTree from which to retrieve branch names.
     * @param patterns A vector of wildcard patterns to match branch names against.
     * @return A vector of branch names that match any of the provided patterns.
     */
    static std::vector<std::string> GetMatchingBranchNames(TTree* tree, const std::vector<std::string>& patterns);

    //All magic numbers
    static const int tTreeCatchSize = 52428800;//50*1024*1024
    static constexpr unsigned long cutFlowBinMargin = 0.5;
    static const int coutTabWidth5 = 5; 
    static const int coutTabWidth10 = 10; 
    static const int coutTabWidth15 = 15; 
    static const int coutTabWidth20 = 20; 

    static const int hundred = 100; 
    static const int sixty = 60; 
};

#endif // HELPER_H

