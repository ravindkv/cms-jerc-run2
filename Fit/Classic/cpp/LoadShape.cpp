#include "LoadShape.h"
#include "Helper.h"
#include <iostream>
#include <memory>

LoadShape::LoadShape(const TString& name,
                     const TString& form,
                     int index,
                     bool isPositive)
{
    setName(name);
    setForm(form);
    setIndex(index);
    setIsPositive(isPositive);
    // Create a TF1 with the given functional form and fit range.
    auto func = std::make_unique<TF1>("f1_" + name,
                                      form,
                                      kFuncRangeMin,
                                      kFuncRangeMax);
    setFunc(std::move(func));
}

