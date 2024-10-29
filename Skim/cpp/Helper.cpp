
#include "Helper.h"

std::vector<std::vector<std::string>> Helper::splitVector(const std::vector<std::string>& strings, int n) {
    int size = strings.size() / n;  // Size of each small vector
    int remainder = strings.size() % n;  // Remaining elements
    std::vector<std::vector<std::string>> smallVectors;
    int index = 0;
    for (int i = 0; i < n; ++i) {
        if (i < remainder) {
            smallVectors.push_back(std::vector<std::string>(
                        strings.begin() + index, strings.begin() + index + size + 1));
            index += size + 1;
        } else {
            smallVectors.push_back(std::vector<std::string>(
                        strings.begin() + index, strings.begin() + index + size));
            index += size;
        }
    }
    return smallVectors;
}

std::vector<std::string> Helper::splitString(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start)); // Last token
    
    return tokens;
}

// Function to print progress during event processing
void Helper::printProgress(Long64_t jentry, Long64_t nentries,
                              std::chrono::time_point<std::chrono::high_resolution_clock>& startClock,
                              double& totTime){
    bool isDebug_ = false;
    if (isDebug_) {
        std::cout << "\n=== Event: " << jentry << " ===\n" << '\n';
    }
    if (nentries > 100 && jentry % (nentries / 100) == 0) {  // Print progress every 1%
        auto currentTime = std::chrono::high_resolution_clock::now();
        totTime += std::chrono::duration<double>(currentTime - startClock).count();
        int sec = static_cast<int>(totTime) % 60;
        int min = static_cast<int>(totTime) / 60;
        std::cout << std::setw(5) << (100 * jentry / nentries) << "% "
                  << std::setw(5) << min << "m " << sec << "s" << '\n';
        startClock = currentTime;  // Reset clock after printing progress
    }
}

// Function to print histogram bins and their statistics
void Helper::printCutflow(const TH1D* hist){
    // Print the header
    std::cout << "---------: Cutflow Summary :--------" << '\n';
    std::cout << std::setw(20) << "CUT" << std::setw(10) << "ENTRIES" << std::setw(15) << "REDUCED (%)" << '\n';

    int nBins = hist->GetNbinsX();
    double previous = hist->GetBinContent(1);

    // Print the first bin's content (no change for the first bin)
    std::cout << std::setw(20) << hist->GetXaxis()->GetBinLabel(1)
              << std::setw(10) << previous
              << std::setw(15) << "N/A" << '\n';

    // Loop over the remaining bins
    for (int i = 2; i <= nBins; ++i) {
        double current = hist->GetBinContent(i);
        double change = (previous != 0) ? ((previous - current) / previous) * 100.0 : 0.0;

        // Print the cut name, entries, and percentage change
        std::cout << std::setw(20) << hist->GetXaxis()->GetBinLabel(i)
                  << std::setw(10) << current
                  << std::setw(10) << " => "
                  << std::fixed << std::setprecision(1)
                  << change << " %" << '\n';

        previous = current;
    }
}

std::string Helper::WildcardToRegex(const std::string& wildcard) {
    std::string regex;
    regex.reserve(wildcard.size() * 2); // Reserve space to avoid reallocations
    regex += '^'; // Anchor to start of the string
    for (char c : wildcard) {
        switch (c) {
            case '*':
                regex += ".*";
                break;
            case '?':
                regex += '.';
                break;
            // Escape regex special characters
            case '.':
            case '^':
            case '$':
            case '+':
            case '{':
            case '}':
            case '[':
            case ']':
            case '|':
            case '(':
            case ')':
            case '\\':
            case '/':
                regex += '\\';
                regex += c;
                break;
            default:
                regex += c;
                break;
        }
    }
    regex += '$'; // Anchor to end of the string
    return regex;
}


std::vector<std::string> Helper::GetMatchingBranchNames(TTree* tree, const std::vector<std::string>& patterns) {
    std::vector<std::string> matchingBranches;

    // Check if the tree is valid
    if (!tree) {
        std::cerr << "Error: TTree pointer is null." << std::endl;
        return matchingBranches;
    }

    // Convert wildcard patterns to std::regex patterns
    std::vector<std::regex> regexPatterns;
    regexPatterns.reserve(patterns.size());

    for (const auto& pat : patterns) {
        std::string regexPattern = WildcardToRegex(pat);

        try {
            regexPatterns.emplace_back(regexPattern);
        } catch (const std::regex_error& e) {
            std::cerr << "Invalid regex pattern: " << regexPattern << " from pattern: " << pat << std::endl;
            // Handle error as needed, e.g., continue to the next pattern
        }
    }

    // Retrieve the list of branches from the tree
    TObjArray* branches = tree->GetListOfBranches();
    if (!branches) {
        std::cerr << "Error: No branches found in the tree." << std::endl;
        return matchingBranches;
    }

    // Iterate over each branch and check if it matches any of the regex patterns
    TIter next(branches);
    TBranch* branch;
    while ((branch = static_cast<TBranch*>(next()))) {
        std::string branchName = branch->GetName();
        for (const auto& regexPat : regexPatterns) {
            if (std::regex_match(branchName, regexPat)) {
                matchingBranches.emplace_back(branchName);
                break; // Move to the next branch after a match is found
            }
        }
    }
    std::cout<<"Matched branch names:"<<'\n';
    for (const auto name: matchingBranches)
        std::cout<<name<<'\n';
    return matchingBranches;
}
