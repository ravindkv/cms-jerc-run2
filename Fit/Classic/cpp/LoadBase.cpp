#include "LoadBase.h"
#include "Helper.h"
#include <iostream>
#include <memory>

LoadBase::LoadBase(const TString& name, TGraphErrors* graph)
{
    setName(name);
    // Remove zero points from the original graph.
    Helper::removeZerosFromGraph(graph);
    // Clone graphs for raw, input, output, and variation.
    setRaw(static_cast<TGraphErrors*>(graph->Clone(name + "_raw")));
    setInput(static_cast<TGraphErrors*>(graph->Clone(name + "_in")));
    setOutput(static_cast<TGraphErrors*>(graph->Clone(name + "_out")));
    setVariation(static_cast<TGraphErrors*>(graph->Clone(name + "_variation")));
}
