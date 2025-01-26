
#include "Helper.h"

auto Helper::splitVector(const std::vector<std::string>& strings, std::size_t n) -> std::vector<std::vector<std::string>> {
    std::size_t size = strings.size() / n;  // Size of each small vector
    std::size_t remainder = strings.size() % n;  // Remaining elements
    std::vector<std::vector<std::string>> smallVectors;
    std::size_t index = 0;

    using diff_t = std::vector<std::string>::difference_type;

    for (std::size_t i = 0; i < n; ++i) {
        if (i < remainder) {
            smallVectors.emplace_back(
                strings.begin() + static_cast<diff_t>(index),
                strings.begin() + static_cast<diff_t>(index + size + 1)
            );
            index += size + 1;
        } else {
            smallVectors.emplace_back(
                strings.begin() + static_cast<diff_t>(index),
                strings.begin() + static_cast<diff_t>(index + size)
            );
            index += size;
        }
    }
    return smallVectors;
}



auto Helper::splitString(const std::string& s, const std::string& delimiter) -> std::vector<std::string> {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start)); // Last token
    
    return tokens;
}

void Helper::initProgress(){
    std::cout<<"---------------------------"<<'\n';
    std::cout<<std::setw(coutTabWidth10)
             <<"Progress"<<std::setw(coutTabWidth10)
             <<"Time"
             <<'\n';
    std::cout<<"---------------------------"<<'\n';
}

// Function to print progress during event processing
void Helper::printProgress(Long64_t jentry, Long64_t nentries,
                              std::chrono::time_point<std::chrono::high_resolution_clock>& startClock,
                              double& totTime){
    bool isDebug_ = false;
    if (isDebug_) {
        std::cout << "\n=== Event: " << jentry << " ===\n" << '\n';
    }
    if (nentries > hundred && jentry % (nentries / hundred) == 0) {  // Print progress every 1%
        auto currentTime = std::chrono::high_resolution_clock::now();
        totTime += std::chrono::duration<double>(currentTime - startClock).count();
        int sec = static_cast<int>(totTime) % sixty;
        int min = static_cast<int>(totTime) / sixty;
        std::cout << std::setw(coutTabWidth5) << (hundred * jentry / nentries) << "% "
                  << std::setw(coutTabWidth5) << min << "m " << sec << "s" << '\n';
        startClock = currentTime;  // Reset clock after printing progress
    }
}

// Function to print histogram bins and their statistics
void Helper::printCutflow(const TH1D* hist){
    // Print the header
    std::cout << "---------: Cutflow Summary :--------" << '\n';
    std::cout << std::setw(coutTabWidth20) << "CUT" << std::setw(coutTabWidth10) << "ENTRIES" << std::setw(coutTabWidth20) << "REDUCED x times" << '\n';

    int nBins = hist->GetNbinsX();
    double previous = hist->GetBinContent(1);

    // Print the first bin's content (no change for the first bin)
    std::cout << std::setw(coutTabWidth20) << hist->GetXaxis()->GetBinLabel(1)
              << std::setw(coutTabWidth10) << previous
              << std::setw(coutTabWidth15) << "N/A" << '\n';

    // Loop over the remaining bins
    for (int i = 2; i <= nBins; ++i) {
        double current = hist->GetBinContent(i);
        //double change = (previous != 0) ? ((current) / previous) * hundred : 0.0;
        double xTimes = (current != 0) ? ((previous) / current) : 0.0;

        // Print the cut name, entries, and percentage change
        std::cout << std::setw(coutTabWidth20) << hist->GetXaxis()->GetBinLabel(i)
                  << std::setw(coutTabWidth10) << current
                  << std::setw(coutTabWidth10) << " => "
                  << std::fixed << std::setprecision(1)
                  //<< change << " %" << '\n';
                  << xTimes << '\n';

        previous = current;
    }
}

auto Helper::WildcardToRegex(const std::string& wildcard) -> std::string {
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


auto Helper::GetMatchingBranchNames(TTree* tree, const std::vector<std::string>& patterns) -> std::vector<std::string> {
    std::vector<std::string> matchingBranches;

    // Check if the tree is valid
    if (!tree) {
        std::cerr << "Error: TTree pointer is null." << '\n';
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
            std::cerr << "Invalid regex pattern: " << regexPattern << " from pattern: " << pat << '\n';
            // Handle error as needed, e.g., continue to the next pattern
        }
    }

    // Retrieve the list of branches from the tree
    TObjArray* branches = tree->GetListOfBranches();
    if (!branches) {
        std::cerr << "Error: No branches found in the tree." << '\n';
        return matchingBranches;
    }

    // Iterate over each branch and check if it matches any of the regex patterns
    TIter next(branches);
    TBranch* branch = nullptr;
    while ((branch = dynamic_cast<TBranch*>(next()))) {
        std::string branchName = branch->GetName();
        for (const auto& regexPat : regexPatterns) {
            if (std::regex_match(branchName, regexPat)) {
                matchingBranches.emplace_back(branchName);
                break; // Move to the next branch after a match is found
            }
        }
    }
    std::cout<<"Matched branch names:"<<'\n';
    for (const auto& name: matchingBranches)
        std::cout<<name<<'\n';
    return matchingBranches;
}
