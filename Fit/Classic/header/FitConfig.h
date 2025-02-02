#pragma once

#include "Helper.h"
#include "Containers.h"
#include <map>
#include <vector>
#include <string>

namespace ClassicFit {

class FitConfig {
 public:
  // Prevent instantiation.
  FitConfig() = delete;

  // Basic debug and function range constants
  static constexpr double kFuncRangeMin = 10.0;   // Fit range min
  static constexpr double kFuncRangeMax = 6500.0; // Fit range max

  // Types vector
  static const std::vector<TString> kAllTypes;

  // Input filenames
  static const std::map<TString, TString> kInputFnames;

  // Reference objects map
  static const std::map<TString, std::map<TString, TString>> kReferenceObjMap;

  // Input histograms map
  static const std::map<TString, std::map<TString, TString>> kInputHnamesMap;

  // FSR map
  static const std::map<TString, std::map<TString, TString>> kKfsrHnamesMap;

  // Sources map
  static const std::map<TString, std::map<TString, TString>> kSourcesHnamesMap;

  // Shape map
  static const std::map<TString, std::map<TString, TString>> kShapesMap;
};

}  // namespace ClassicFit

