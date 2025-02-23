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
 * @brief Manages shape loading, including functional forms and related parameters.
 */
class LoadShape {
public:
    LoadShape(const TString& name,
              const TString& form,
              int index,
              bool isPositive);

    // Delete copy and assignment operators.
    LoadShape(const LoadShape&) = delete;
    LoadShape& operator=(const LoadShape&) = delete;

    // Setters.
    void setName(const TString& x)     { m_name = x; }
    void setForm(const TString& x)     { m_form = x; }
    void setIndex(int x)               { m_index = x; }
    void setIsPositive(bool x)         { m_isPositive = x; }
    void setFunc(std::unique_ptr<TF1> x) { m_func = std::move(x); }

    // Getters.
    std::string getColor() const       { return colMagenta; }
    TString getName() const            { return m_name; }
    TString getForm() const            { return m_form; }
    int getIndex() const               { return m_index; }
    bool isPositive() const            { return m_isPositive; }
    TF1* getFunc() const               { return m_func.get(); }

private:
    TString m_name, m_form;
    int  m_index = -1;
    bool m_isPositive = false;
    std::unique_ptr<TF1> m_func;
};

