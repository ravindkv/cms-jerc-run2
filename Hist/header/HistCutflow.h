// HistCutflow.h
#ifndef HISTCUTFLOW_H
#define HISTCUTFLOW_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <TH1D.h>
#include <TDirectory.h>

class HistCutflow {
public:
    // Constructor
    HistCutflow(TDirectory* origDir, const std::string& directoryName, const std::vector<std::string>& cuts);

    // Destructor
    ~HistCutflow();

    // Method to fill the histogram for a specific cut
    void fill(const std::string& cutName, double weight = 1.0);

    // Accessor for the underlying histogram (optional)
    TH1D* getHistogram() const { return h1EventInCutflow_.get(); }

    /**
     * @brief Creates a fraction cutflow histogram from the current cutflow histogram.
     *
     * Each bin in the returned histogram represents the fraction of entries
     * relative to the previous bin. The first bin is set to 1.0 as a reference.
     *
     * @return std::unique_ptr<TH1D> Pointer to the fraction cutflow histogram.
     *         Returns nullptr if the input histogram is invalid.
     */
    void fillFractionCutflow() const;

    /**
     * @brief Prints a summary of the histogram cutflow with statistics.
     *
     * The summary includes each cut's name, the number of entries, and the percentage
     * of entries remaining relative to the previous cut.
     *
     * @param outputStream Reference to the output stream (default is std::cout).
     *                     Allows redirection of output to files or other streams.
     */
    void printCutflow(std::ostream& outputStream = std::cout) const;

private:
    std::vector<std::string> cutNames_;
    std::unique_ptr<TH1D> h1EventInCutflow_;
    std::unique_ptr<TH1D> h1EventFractionInCutflow_;
    std::map<std::string, int> cutToBinMap;

    /**
     * @brief Validates the input histogram.
     *
     * Checks if the histogram is non-null and has at least one bin.
     *
     * @return bool True if the histogram is valid, false otherwise.
     */
    bool validateHistogram() const;

};

#endif // HISTCUTFLOW_H

