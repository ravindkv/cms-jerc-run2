#pragma once

#include <memory>
#include <cassert>
#include <iostream>
#include <string>
#include "TF1.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "Helper.h"

/**
 * @brief Represents the base loading object that manages several TGraphErrors.
 */
class LoadBase {
public:
    LoadBase(const TString& name, TGraphErrors* graph);

    // Delete copy and assignment operators.
    LoadBase(const LoadBase&) = delete;
    LoadBase& operator=(const LoadBase&) = delete;

    // Setters.
    void setName(const TString& x)      { m_name = x; }
    void setRaw(TGraphErrors* x)        { m_raw.reset(x); }
    void setInput(TGraphErrors* x)      { m_input.reset(x); }
    void setOutput(TGraphErrors* x)     { m_output.reset(x); }
    void setVariation(TGraphErrors* x)  { m_variation.reset(x); }

    // Getters.
    std::string getColor() const        { return colBlue; } // Uses Helper color definitions.
    TString getName() const             { return m_name; }
    TGraphErrors* getRaw() const        { return m_raw.get(); }
    TGraphErrors* getInput() const      { return m_input.get(); }
    TGraphErrors* getOutput() const     { return m_output.get(); }
    TGraphErrors* getVariation() const  { return m_variation.get(); }

    /**
     * @brief Gets the number of points in the input graph.
     * @throws std::runtime_error if input/output graphs are invalid or mismatched.
     */
    int getN() const {
        if (!getInput() || !getOutput() || getInput()->GetN() != getOutput()->GetN()) {
            throw std::runtime_error("LoadBase::getN() failed: invalid or mismatched graphs.");
        }
        return getInput()->GetN();
    }

private:
    TString m_name;
    std::unique_ptr<TGraphErrors> m_raw, m_input, m_output, m_variation;
};

