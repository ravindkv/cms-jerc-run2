// HistCutflow.h
#ifndef HISTCUTFLOW_H
#define HISTCUTFLOW_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <TH1D.h>
#include <TDirectory.h>

class HistCutflow {
public:
    // Constructor
    HistCutflow(const std::string& name, const std::vector<std::string>& cuts, TDirectory* parentDir);

    // Destructor
    ~HistCutflow();

    // Method to fill the histogram for a specific cut
    void fill(const std::string& cutName, double weight = 1.0);

    // Accessor for the underlying histogram (optional)
    TH1D* getHistogram() const { return h1EventsInCutflow.get(); }

private:
    std::string histName;
    std::vector<std::string> cutNames;
    std::unique_ptr<TH1D> h1EventsInCutflow;
    std::map<std::string, int> cutToBinMap;
};

#endif // HISTCUTFLOW_H

