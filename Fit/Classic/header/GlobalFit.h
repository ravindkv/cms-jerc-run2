#pragma once
/**
 *  \file GlobalFit.h
 *  \brief Manages the global fitting process, including loading data, shapes, systematics, and running the fitter.
 *
 *  Pre-requisites:
 *    - reprocess.C : produce rootfiles/jecdata[X].root input file
 *    - softrad3.C  : produce FSR+ISR corrections for HDM (MPF+DB)
 *    - globalFitSyst.C : produce uncertainty shapes
 *    - globalFitSettings.h : input definitions and configurable settings
 *  Post-processing:
 *    - globalFitPulls.C : plot pull distributions
 *    - createL2L3ResTextFile.C : produce simplified (L2)L3Res text file
 *    - (minitools/mergerL2L3ResTextFiles.C : combine L3Res with L2Res)
 *
 *  Author: Mikko Voutilainen, Andrea Malara, Ravindra Verma, ChatGPT o3-mini-high
 */

#include "FitConfig.h"
#include "Containers.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TMinuit.h"
#include "TFitter.h"
#include "TMatrixD.h"
#include <string>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include <functional>

namespace ClassicFit {

  /**
   * \class GlobalFit
   * \brief Manages the global fitting process.
   */
  class GlobalFit {
  public:
      GlobalFit(const std::string& runName,
                const std::string& mode,
                double etaMin,
                double etaMax);
      ~GlobalFit();

      void openFiles();
      void run();

      // Sub-steps of run():
      void loadInputs();
      void loadSystematics();
      void loadShapes();
      void loadReference();
      void loadFSR();
      void scaleFSR();
      void setupFitFunction();
      void doGlobalFit();
      void storeFitOutput();

      // Utility
      TString replaceDefault(TString name);

      // Public configuration members
      bool kDebug;  // (non-static; used in member functions)
      // Static configuration used by static fitting routines:
      static bool gDebug;  // used in static methods (set e.g. in constructor)
      static constexpr int kNumberOfFitIterations = 1;
      double m_fitMin = 15.;  // fit range minimum
      double m_fitMax = 2000.;

      static constexpr bool kUseJESref       = true;
      static constexpr bool kPenalizeFitPars = true;
      static constexpr double kGlobalErrMin    = 0.0;
      static constexpr double kScaleFullSimShape = 0.01;

      static TString gCurrentObs; // tracks the current observable ("Resp", etc.)

      // Public members
      std::string m_runName;
      std::string m_mode;
      TString m_etaMin;
      TString m_etaMax;

      std::map<TString, TFile*> m_inputFiles;
      std::map<TString, TFile*> m_outputFiles;

      // Histograms requested
      std::vector<TString> m_inputHnames;

      // Keep track of shape "types"
      std::set<TString> m_shapeTypes;

      std::map<TString, TH1D*> m_referenceObjects;
      std::map<TString, SystematicContainer*> m_fsrs;

      TFitter* m_fitter = nullptr;
      std::unique_ptr<TMatrixD> m_errorMatrix;

      int m_nFitPars      = 0;
      int m_nNuisancePars = 0;
      int m_nTotPars      = 0;
      int m_nTotalPoints  = 0;

      std::vector<TString> m_sampleList{};
      std::vector<TString> m_hdmMethods{};
      std::vector<TString> m_typesList {};
      TString m_outputFileName = "";

      void setInput(std::vector<TString> sampleList,
                    std::vector<TString> hdmMethods,
                    std::vector<TString> typesList,
                    TString outputFileName);

      // ------------------ Fitting functions as static members ------------------
      /// \brief A dummy fitting function (not used directly).
      static Double_t jesFit(Double_t* x, Double_t* p);

      /// \brief Returns a std::function (for use with TF1) wrapping the JES fit.
      static std::function<double(Double_t*, Double_t*)>
      jesFitWrapper(TH1D* hjesref, std::map<TString, ShapeContainer*> shapeMap);

      /// \brief The function called by the minimizer to compute chi2.
      static void jesFitter(Int_t& npar, Double_t* grad, Double_t& chi2,
                            Double_t* par, Int_t flag = 1);
  };

  // Global containers and function pointers used in the fit:
  extern std::map<TString, DataContainer*>       gMyData;
  extern std::map<TString, ShapeContainer*>        gShapes;
  extern std::map<TString, SystematicContainer*>   gSources;
  extern TF1* gJesFit;
  extern int gNFittedDataPoints;
  extern int gFitCounter;

} // namespace ClassicFit

