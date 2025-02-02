#pragma once
// Purpose: Container objects for GlobalFit inputs
// Author: Andrea Malara, Ravindra Verma, ChatGPT o1

#include <memory>
#include <cassert>
#include <iostream>
#include <string>
#include "TF1.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "Helper.h"

namespace ClassicFit {

/**
 * \class DataContainer
 * \brief Holds TGraphErrors for raw, input, output, variation
 */
class DataContainer {
public:
    DataContainer(const TString& name,
                  const TString& type,
                  const TString& hname,
                  TGraphErrors* graph);

    // Setters
    void setName(const TString& x)      { m_name = x; }
    void setType(const TString& x)      { m_type = x; }
    void setHname(const TString& x)     { m_hname = x; }
    void setRaw(TGraphErrors* x)        { m_raw.reset(x); }
    void setInput(TGraphErrors* x)      { m_input.reset(x); }
    void setOutput(TGraphErrors* x)     { m_output.reset(x); }
    void setVariation(TGraphErrors* x)  { m_variation.reset(x); }

    // Getters
    std::string getColor() const        { return colBlue; } // from utils.h
    TString getName() const            { return m_name; }
    TString getType() const            { return m_type; }
    TString getHname() const           { return m_hname; }
    TGraphErrors* getRaw() const       { return m_raw.get(); }
    TGraphErrors* getInput() const     { return m_input.get(); }
    TGraphErrors* getOutput() const    { return m_output.get(); }
    TGraphErrors* getVariation() const { return m_variation.get(); }

    int getN() const {
        assert(getInput() && getOutput() && getInput()->GetN() == getOutput()->GetN());
        return getInput()->GetN();
    }

    friend std::ostream& operator<<(std::ostream& os, const DataContainer&);

private:
    // Member variables
    TString m_name, m_type, m_hname;
    std::unique_ptr<TGraphErrors> m_raw, m_input, m_output, m_variation;
};

/**
 * \class ShapeContainer
 * \brief Holds a TF1 shape function for "Resp" or other types
 */
class ShapeContainer {
public:
    ShapeContainer(const TString& name,
                   const TString& form,
                   const TString& appliesTo,
                   int index,
                   bool isPositive);

    // Setters
    void setName(const TString& x)       { m_name = x; }
    void setForm(const TString& x)       { m_form = x; }
    void setAppliesTo(const TString& x)  { m_appliesTo = x; }
    void setIndex(int x)                 { m_index = x; }
    void setIsPositive(bool x)           { m_isPositive = x; }
    void setFunc(std::unique_ptr<TF1> x) { m_func = std::move(x); }

    // Getters
    std::string getColor() const        { return colMagenta; }
    TString getName() const            { return m_name; }
    TString getForm() const            { return m_form; }
    TString getAppliesTo() const       { return m_appliesTo; }
    int getIndex() const               { return m_index; }
    bool isPositive() const            { return m_isPositive; }
    TF1* getFunc() const               { return m_func.get(); }

    friend std::ostream& operator<<(std::ostream& os, const ShapeContainer&);

private:
    TString m_name, m_form, m_appliesTo;
    int  m_index      = -1;
    bool m_isPositive = false;
    std::unique_ptr<TF1> m_func;
};

/**
 * \class SystematicContainer
 * \brief Holds a TH1D representing a systematic shift
 */
class SystematicContainer {
public:
    SystematicContainer(const TString& name,
                        const TString& appliesTo,
                        int index,
                        const TString& hname,
                        TH1D* hist);

    // Setters
    void setName(const TString& x)       { m_name = x; }
    void setAppliesTo(const TString& x)  { m_appliesTo = x; }
    void setHname(const TString& x)      { m_hname = x; }
    void setIndex(int x)                 { m_index = x; }
    void setHist(TH1D* x) {
        m_hist.reset(x);
        if (m_hist) m_hist->SetDirectory(0);
    }

    // Getters
    std::string getColor() const        { return colCyan; }
    TString getName() const            { return m_name; }
    TString getAppliesTo() const       { return m_appliesTo; }
    TString getHname() const           { return m_hname; }
    int getIndex() const               { return m_index; }
    TH1D* getHist() const              { return m_hist.get(); }

    int getN() const {
        if (!m_hist) return 0;
        return m_hist->GetNbinsX();
    }

    friend std::ostream& operator<<(std::ostream& os, const SystematicContainer&);

private:
    TString m_name, m_appliesTo, m_hname;
    int m_index = -1;
    std::unique_ptr<TH1D> m_hist;
};

} // namespace ClassicFit

