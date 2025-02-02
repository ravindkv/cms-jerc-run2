#include "Containers.h"
#include "FitConfig.h"
#include "Helper.h"

#include <iostream>
#include <memory>

namespace ClassicFit {

// ------------ DataContainer ------------

DataContainer::DataContainer(const TString& name,
                             const TString& type,
                             const TString& hname,
                             TGraphErrors* graph)
{
    setName(name);
    setType(type);
    setHname(hname);

    Helper::removeZerosFromGraph(graph);
    setRaw(static_cast<TGraphErrors*>(graph->Clone(name + "_raw")));
    setInput(static_cast<TGraphErrors*>(graph->Clone(name + "_in")));
    setOutput(static_cast<TGraphErrors*>(graph->Clone(name + "_out")));
    setVariation(static_cast<TGraphErrors*>(graph->Clone(name + "_variation")));
}

std::ostream& operator<<(std::ostream& os, const DataContainer& dc)
{
    Helper::printLine("name: " + dc.getName(), dc.getColor());
    Helper::printLine("  --> type: " + dc.getType(), dc.getColor());
    Helper::print("  --> hname: " + dc.getHname() + colReset, dc.getColor());
    return os;
}

// ------------ ShapeContainer ------------

ShapeContainer::ShapeContainer(const TString& name,
                               const TString& form,
                               const TString& appliesTo,
                               int index,
                               bool isPositive)
{
    setName(name);
    setForm(form);
    setAppliesTo(appliesTo);
    setIndex(index);
    setIsPositive(isPositive);

    auto func = std::make_unique<TF1>("f1_" + name + "_" + appliesTo,
                                      form, FitConfig::kFuncRangeMin, FitConfig::kFuncRangeMax);
    setFunc(std::move(func));
}

std::ostream& operator<<(std::ostream& os, const ShapeContainer& sc)
{
    Helper::printLine("name: " + sc.getName(), sc.getColor());
    Helper::printLine("  --> form: " + sc.getForm(), sc.getColor());
    Helper::printLine("  --> appliesTo: " + sc.getAppliesTo(), sc.getColor());
    Helper::printLine("  --> index: " + std::to_string(sc.getIndex()), sc.getColor());
    Helper::print("  --> isPositive: " + std::to_string(sc.isPositive()) + colReset, sc.getColor());
    return os;
}

// ------------ SystematicContainer ------------

SystematicContainer::SystematicContainer(const TString& name,
                                         const TString& appliesTo,
                                         int index,
                                         const TString& hname,
                                         TH1D* hist)
{
    setName(name);
    setAppliesTo(appliesTo);
    setIndex(index);
    setHname(hname);
    setHist(static_cast<TH1D*>(hist->Clone(name)));
}

std::ostream& operator<<(std::ostream& os, const SystematicContainer& sc)
{
    Helper::printLine("name: " + sc.getName(), sc.getColor());
    Helper::printLine("  --> appliesTo: " + sc.getAppliesTo(), sc.getColor());
    Helper::printLine("  --> hname: " + sc.getHname(), sc.getColor());
    Helper::print("  --> index: " + std::to_string(sc.getIndex()) + colReset, sc.getColor());
    return os;
}

} // namespace ClassicFit

