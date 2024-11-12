// TrigList.h
#ifndef TRIGLIST_H
#define TRIGLIST_H

#include <string>
#include <map>
#include <utility>

// Define the Range struct
struct Range {
    double ptMin;
    double ptMax;
    double absEtaMin;
    double absEtaMax;
};

// Forward declaration of SkimTree
class SkimTree;

// Define the TrigList class
class TrigList {
public:
    // Constructor accepting a SkimTree pointer to initialize trigDetails_
    TrigList(const SkimTree* skimT);

    // Destructor
    ~TrigList();

    // Method to get the trigDetails map
    const std::map<std::string, std::pair<const bool*, Range>>& getTrigDetails() const;

private:
    // trigDetails map
    std::map<std::string, std::pair<const bool*, Range>> trigDetails_;
};

#endif // TRIGLIST_H

