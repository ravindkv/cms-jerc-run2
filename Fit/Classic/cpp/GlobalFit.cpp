#include "GlobalFit.h"
#include "Containers.h"
#include "Helper.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

// Define the extern variables here:
namespace ClassicFit {
  std::map<TString, DataContainer*>       gMyData;
  std::map<TString, ShapeContainer*>        gShapes;
  std::map<TString, SystematicContainer*>   gSources;

  TF1* gJesFit               = nullptr;
  int  gNFittedDataPoints    = 0;
  int  gFitCounter           = 0;

  // Initialize static members
  TString GlobalFit::gCurrentObs = "";
  bool GlobalFit::gDebug = true;  // default; may be overwritten in constructor

  // ------------------ GlobalFit Implementation ------------------

  GlobalFit::GlobalFit(const std::string& runName,
                       const std::string& mode,
                       double etaMin,
                       double etaMax)
  {
      // Set instance and static debug flag (so static routines can print messages)
      kDebug = true;
      gDebug = kDebug;
      if (kDebug) {
          std::cout << "========: GlobalFit() :========" << std::endl;
      }
      m_runName = runName;
      m_mode    = mode;
      m_etaMin  = Form("%02.0f", 10.0 * etaMin);
      m_etaMax  = Form("%02.0f", 10.0 * etaMax);
      Helper::printLine("Running on: run=" + m_runName + ", mode=" + m_mode +
                        ", etaMin=" + m_etaMin + ", etaMax=" + m_etaMax, colGreen);
  }

  void GlobalFit::setInput(std::vector<TString> sampleList,
                           std::vector<TString> hdmMethods,
                           std::vector<TString> typesList,
                           TString outputFileName)
  {
      m_sampleList = sampleList;
      m_hdmMethods = hdmMethods;
      m_typesList  = typesList;
      m_outputFileName = outputFileName;

      Helper::print("  --> samples:", colGreen); 
      Helper::printLine(m_sampleList);
      Helper::print("  --> hdmMethods:", colGreen);
      Helper::printLine(m_hdmMethods);
      Helper::print("  --> typesList:", colGreen);
      Helper::printLine(m_typesList);

      // Build the list of input histogram names
      for (const auto& sample : m_sampleList) {
          for (const auto& method : m_hdmMethods) {
              for (const auto& t : m_typesList) {
                  m_inputHnames.push_back(t + "_" + sample + "_" + method);
              }
          }
      }
      openFiles();
  }

  GlobalFit::~GlobalFit()
  {
      // Close input files
      for (auto& [fname, filePtr] : m_inputFiles) {
          if (kDebug) {
              Helper::printLine("Closing file for " + fname, colYellow);
          }
          if (filePtr) filePtr->Close();
      }
      // Close output files
      for (auto& [fname, filePtr] : m_outputFiles) {
          if (kDebug) {
              Helper::printLine("Closing file for " + fname, colYellow);
          }
          if (filePtr) filePtr->Close();
      }
  }

  void GlobalFit::openFiles()
  {
      if (kDebug) {
          std::cout << "========: openFiles() :========" << std::endl;
      }
      for (const auto& [name, fname] : FitConfig::kInputFnames) {
          if (kDebug) {
              Helper::printLoading("file", name, fname);
          }
          m_inputFiles[name] = TFile::Open(replaceDefault(fname), "UPDATE");
          assert(m_inputFiles[name] && !m_inputFiles[name]->IsZombie());
          if (kDebug) {
              Helper::printLine("successfully", colYellow);
          }
      }
      m_outputFiles["output"] = TFile::Open(replaceDefault(m_outputFileName), "RECREATE");
  }

  TString GlobalFit::replaceDefault(TString name)
  {
      if (kDebug) {
          std::cout << "========: replaceDefault() :========" << std::endl;
      }
      name.ReplaceAll("RUN",     m_runName.c_str());
      name.ReplaceAll("MODE",    m_mode.c_str());
      name.ReplaceAll("ETAMIN",  m_etaMin);
      name.ReplaceAll("ETAMAX",  m_etaMax);
      return name;
  }

  void GlobalFit::loadInputs()
  {
      if (kDebug) {
          std::cout << "========: loadInputs() :========" << std::endl;
      }
      for (const auto& [name, info] : FitConfig::kInputHnamesMap) {
          if (Helper::findInVector(m_inputHnames, name) < 0) {
              Helper::printLine("Skipping hist: " + name, colYellow);
              continue;
          }
          TString hname = replaceDefault(info.at("hname"));
          if (kDebug) {
              Helper::printLoading("hist", name, hname);
          }
          std::unique_ptr<TGraphErrors> g(
              static_cast<TGraphErrors*>(m_inputFiles.at(info.at("fname"))->Get(hname))
          );
          gMyData[name] = new DataContainer(name, info.at("type"), hname, g.get());
          m_nTotalPoints += gMyData[name]->getN();
          if (kDebug) {
              Helper::printLine("successfully", colYellow);
              std::cout << *gMyData[name] << std::endl;
          }
      }
  }

  void GlobalFit::loadSystematics()
  {
      if (kDebug) {
          std::cout << "========: loadSystematics() :========" << std::endl;
      }
      for (const auto& [name, info] : FitConfig::kSourcesHnamesMap) {
          TString appliesTo = info.at("appliesTo");
          if (Helper::findInVector(m_inputHnames, appliesTo) < 0) {
              Helper::printLine("Skipping source: " + name + 
                                " index: " + std::to_string(Helper::findInVector(m_inputHnames, appliesTo)), colYellow);
              continue;
          }
          TString hname = replaceDefault(info.at("hname"));
          if (kDebug) {
              Helper::printLoading("source", name, hname);
          }
          std::unique_ptr<TH1D> h;
          if (Helper::findInString("scale", name.Data())) {
              h.reset(new TH1D(name, name, 10000, 10, 5000));
              double scaleVal = std::stod(std::string(hname.ReplaceAll("scale", "").Data()));
              hname = name;
              for (int i = 1; i <= h->GetNbinsX(); ++i) {
                  h->SetBinContent(i, scaleVal);
                  h->SetBinError(i, scaleVal);
              }
          } else {
              h.reset(static_cast<TH1D*>(m_inputFiles.at(info.at("fname"))->Get(hname)));
          }
          gSources[name] = new SystematicContainer(name, appliesTo, static_cast<int>(gSources.size()), hname, h.get());
          assert(gSources[name]);
          if (kDebug) {
              Helper::printLine("successfully", colYellow);
              std::cout << *gSources[name] << std::endl;
          }
      }
  }

  void GlobalFit::loadShapes()
  {
      if (kDebug) {
          std::cout << "========: loadShapes() :========" << std::endl;
      }
      for (const auto& [name, info] : FitConfig::kShapesMap) {
          TString appliesTo = info.at("appliesTo");
          if (Helper::findInVector(m_typesList, appliesTo) < 0) {
              Helper::printLine("Skipping shape: " + name + 
                                " index: " + std::to_string(Helper::findInVector(m_typesList, appliesTo)), colYellow);
              continue;
          }
          if (kDebug) {
              Helper::printLoading("shape", name, "");
          }
          int index = static_cast<int>(gShapes.size());
          if (info.at("appliesTo") != "Resp") {
              // reuse index from existing shape if that is the intended design
              index = gShapes[info.at("type")]->getIndex();
          }
          gShapes[name] = new ShapeContainer(
              name,
              info.at("form"),
              info.at("appliesTo"),
              index,
              std::atoi(info.at("ispositive").Data())
          );
          assert(gShapes[name]);
          if (kDebug) {
              Helper::printLine("successfully", colYellow);
              std::cout << *gShapes[name] << std::endl;
          }
          m_shapeTypes.insert(info.at("type"));
      }
  }

  void GlobalFit::loadReference()
  {
      if (kDebug) {
          std::cout << "========: loadReference() :========" << std::endl;
      }
      for (const auto& [name, info] : FitConfig::kReferenceObjMap) {
          TString hname = replaceDefault(info.at("hname"));
          if (kDebug) {
              Helper::printLoading("hist", name, hname);
          }
          m_referenceObjects[name] = static_cast<TH1D*>(
              m_inputFiles.at(info.at("fname"))->Get(hname)->Clone(name)
          );
          assert(m_referenceObjects[name]);
          m_referenceObjects[name]->SetDirectory(0);
          if (kDebug) {
              Helper::printLine("successfully", colYellow);
          }
      }
  }

  void GlobalFit::loadFSR()
  {
      if (kDebug) {
          std::cout << "========: loadFSR() :========" << std::endl;
      }
      for (const auto& [name, info] : FitConfig::kKfsrHnamesMap) {
          TString hname = replaceDefault(info.at("hname"));
          if (kDebug) {
              Helper::printLoading("kfsr", name, hname);
          }
          std::unique_ptr<TH1D> h(
              static_cast<TH1D*>(m_inputFiles.at(info.at("fname"))->Get(hname))
          );
          m_fsrs[name] = new SystematicContainer(name, info.at("appliesTo"), -1, hname, h.get());
          assert(m_fsrs[name]);
          if (kDebug) {
              Helper::printLine("successfully", colYellow);
              std::cout << *m_fsrs[name] << std::endl;
          }
      }
  }

  void GlobalFit::scaleFSR()
  {
      if (kDebug) {
          std::cout << "========: scaleFSR() :========" << std::endl;
      }
      for (auto& [name, fsr] : m_fsrs) {
          auto dt = gMyData.at(fsr->getAppliesTo());
          TGraphErrors* raw       = dt->getRaw();
          TGraphErrors* input     = dt->getInput();
          TGraphErrors* output    = dt->getOutput();
          TGraphErrors* variation = dt->getVariation();
          for (int i = 0; i < raw->GetN(); ++i) {
              double pt    = raw->GetX()[i];
              double r     = raw->GetY()[i];
              double shift = fsr->getHist()->GetBinContent(fsr->getHist()->FindBin(pt));
              input->SetPoint(i, pt, r + shift);
              output->SetPoint(i, pt, r + shift);
              variation->SetPoint(i, pt, r + shift);
          }
      }
  }

  void GlobalFit::setupFitFunction()
  {
      if (kDebug) {
          std::cout << "========: setupFitFunction() :========" << std::endl;
      }
      if (kDebug) {
          Helper::printLine("Loaded " + std::to_string(gMyData.size()) + " hists", colGreen);
          Helper::printLine("Loaded " + std::to_string(gShapes.size()) + " shapes", colGreen);
          Helper::printLine("Loaded " + std::to_string(gSources.size()) + " sources", colGreen);
      }
      m_nFitPars      = static_cast<int>(m_shapeTypes.size());
      m_nNuisancePars = static_cast<int>(gSources.size());
      m_nTotPars      = m_nFitPars + m_nNuisancePars;

      Helper::printLine("Global fit has " + std::to_string(m_nTotPars) + " total parameters:", colBlue);
      Helper::printLine("  --> " + std::to_string(m_nFitPars) + " fit parameters", colBlue);
      Helper::printLine("  --> " + std::to_string(m_nNuisancePars) + " nuisance parameters", colBlue);
      if (kPenalizeFitPars) {
          Helper::printLine("  --> Fit parameters have Gaussian prior", colBlue);
      }

      // Use the static method from GlobalFit.
      auto wrapper = GlobalFit::jesFitWrapper(m_referenceObjects.at("hjesref"), gShapes);
      gJesFit      = new TF1("jesFit", wrapper, m_fitMin, m_fitMax, m_nFitPars);

      m_fitter = new TFitter(m_nTotPars);
      // Note: pass the static function pointer.
      m_fitter->SetFCN(GlobalFit::jesFitter);
      for (int i = 0; i < m_nTotPars; ++i) {
          m_fitter->SetParameter(i, "", 1.0, (i < m_nFitPars ? 0.01 : 1.0), -100.0, 100.0);
      }
  }

  void GlobalFit::doGlobalFit()
  {
      if (kDebug) {
          std::cout << "========: doGlobalFit() :========" << std::endl;
      }
      for (int i = 0; i < kNumberOfFitIterations; ++i) {
          m_fitter->ExecuteCommand("MINI", nullptr, 0);
      }
      gNFittedDataPoints -= m_nNuisancePars;
      if (kPenalizeFitPars) {
          gNFittedDataPoints -= m_nFitPars;
      }
      assert(gNFittedDataPoints == m_nTotalPoints);

      m_errorMatrix = std::make_unique<TMatrixD>(m_nTotPars, m_nTotPars);
      gMinuit->mnemat(m_errorMatrix->GetMatrixArray(), m_nTotPars);

      // Evaluate global chi2
      std::vector<Double_t> tmpPar(m_nTotPars);
      std::vector<Double_t> grad(m_nTotPars);
      Double_t chi2Gbl = 0.0;
      for (int i = 0; i < m_nTotPars; ++i) {
          tmpPar[i] = m_fitter->GetParameter(i);
      }
      GlobalFit::jesFitter(m_nTotPars, grad.data(), chi2Gbl, tmpPar.data(), 1);

      // Decompose the chi2
      Double_t chi2Src  = 0.0;
      Double_t chi2Par  = 0.0;
      Double_t chi2Data = 0.0;
      int nParTrue   = 0;
      int nSrcTrue   = 0;

      for (int i = 0; i < m_nTotPars; ++i) {
          double val = m_fitter->GetParameter(i);
          double err = m_fitter->GetParError(i);
          if (std::fabs(val) != 0.0 || std::fabs(err - 1.0) > 1e-2) {
              if (i < m_nFitPars) {
                  ++nParTrue;
                  chi2Par += val * val;
              } else {
                  ++nSrcTrue;
                  chi2Src += val * val;
              }
          }
      }
      assert(m_nFitPars == nParTrue);
      assert(m_nNuisancePars == nSrcTrue);

      // Calculate chi2 from data
      for (auto& [dtName, dt] : gMyData) {
          TGraphErrors* graphOutput = dt->getOutput();
          assert(graphOutput);
          gCurrentObs = dt->getType();
          for (int j = 0; j < graphOutput->GetN(); ++j) {
              double x  = graphOutput->GetX()[j];
              double y  = graphOutput->GetY()[j];
              double ey = graphOutput->GetEY()[j];
              chi2Data += std::pow((y - gJesFit->Eval(x)) / ey, 2);
          }
      }
      // Check consistency:
      double expected = chi2Data + chi2Src + chi2Par;
      assert(std::fabs(chi2Gbl - expected) < 1e-6);
      assert(gNFittedDataPoints == (m_nTotalPoints + m_nFitPars + m_nNuisancePars));

      Helper::printLine("Output GlobalFit", colBlue);
      Helper::printLine("  --> " + std::to_string(m_nTotalPoints) + " data points used", colBlue);
      Helper::printLine("  --> " + std::to_string(m_nFitPars) + " fit parameters", colBlue);
      Helper::printLine("  --> " + std::to_string(nSrcTrue)   + " nuisances", colBlue);
      Helper::printLine(Form("  --> fitting range [%1.0f,%1.0f]", gJesFit->GetXmin(), gJesFit->GetXmax()), colBlue);
      Helper::printLine(Form("  --> Total     chi2/NDF  = %1.1f / %d", chi2Gbl, gNFittedDataPoints), colBlue);
      Helper::printLine(Form("  --> Data      chi2/NDF  = %1.1f / %d", chi2Data, m_nTotalPoints), colBlue);
      Helper::printLine(Form("  --> Nuisance  chi2/Nsrc = %1.1f / %d", chi2Src, m_nFitPars), colBlue);
      Helper::printLine(Form("  --> Parameter chi2/Npar = %1.1f / %d", chi2Par, m_nNuisancePars), colBlue);

      Helper::printLine("Fitted parameters (for Resp):", colBlue);
      for (auto& [shapeName, shape] : gShapes) {
          if (shape->getAppliesTo() != "Resp") continue;
          double val = m_fitter->GetParameter(shape->getIndex());
          double err = m_fitter->GetParError(shape->getIndex());
          Helper::printParameter(shapeName, val, err);
      }

      Helper::printLine("Nuisance parameters:", colBlue);
      for (auto& [srcName, src] : gSources) {
          double val = m_fitter->GetParameter(m_nFitPars + src->getIndex());
          double err = m_fitter->GetParError(m_nFitPars + src->getIndex());
          Helper::printParameter(srcName, val, err);
      }
  }

  void GlobalFit::storeFitOutput()
  {
      if (kDebug) {
          std::cout << "========: storeFitOutput() :========" << std::endl;
      }
      m_outputFiles["output"]->cd();

      gJesFit->SetRange(FitConfig::kFuncRangeMin, FitConfig::kFuncRangeMax);
      gJesFit->SetNpx(static_cast<int>(FitConfig::kFuncRangeMax - FitConfig::kFuncRangeMin));

      double binning[] = {
          10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84, 97,
          114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468,
          507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172,
          1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238,
          2366, 2500, 2640, 2787, 2941, 3103, 3273, 3450, 3637, 3832, 4037,
          4252, 4477, 4713, 4961, 5220, 5492, 5777, 6076, 6389, 6717, 7000
      };
      int nbins = static_cast<int>(sizeof(binning) / sizeof(binning[0]) - 1);

      // Generate TGraphErrors / TH1D for each "type"
      for (const auto& type : FitConfig::kAllTypes) {
          gCurrentObs = type;
          TGraphErrors* graph = new TGraphErrors();
          Helper::funcToGraph(gJesFit, *m_errorMatrix, graph);
          TH1D* hist = new TH1D("jesFit_hist_" + type, ";p_{T} (GeV);" + type, nbins, binning);
          hist->SetDirectory(nullptr);
          Helper::funcToHist(gJesFit, *m_errorMatrix, hist);

          graph->Write("jesFit_graph_" + type, TObject::kOverwrite);
          hist->Write(hist->GetName(), TObject::kOverwrite);
          gJesFit->Write("jesFit_func_" + type, TObject::kOverwrite);
      }
      m_errorMatrix->Write("error_matrix", TObject::kOverwrite);

      // Store data
      for (auto& [name, dt] : gMyData) {
          TGraphErrors* raw       = dt->getRaw();
          TGraphErrors* input     = dt->getInput();
          TGraphErrors* output    = dt->getOutput();
          TGraphErrors* variation = dt->getVariation();

          double scaleVal = (dt->getType() == "Resp") ? 1.0 : 1.0 / kScaleFullSimShape;
          Helper::multiplyGraph(input, scaleVal);
          Helper::multiplyGraph(output, scaleVal);
          Helper::multiplyGraph(variation, scaleVal);

          // collect shape TF1s that apply
          std::vector<TF1*> funcs;
          for (auto& [shapeName, shape] : gShapes) {
              if (shape->getAppliesTo() == dt->getType()) {
                  funcs.push_back(shape->getFunc());
              }
          }
          Helper::propagateErrorToGraph(variation, funcs, *m_errorMatrix);

          raw->Write(name + "_raw", TObject::kOverwrite);
          input->Write(name + "_prefit", TObject::kOverwrite);
          output->Write(name + "_postfit", TObject::kOverwrite);
          variation->Write(name + "_variation", TObject::kOverwrite);
      }

      // Store systematics
      for (auto& [name, src] : gSources) {
          src->getHist()->SetYTitle(name);
          src->getHist()->Write("source_" + name, TObject::kOverwrite);
      }

      // Store shapes
      for (auto& [shapeName, shape] : gShapes) {
          shape->getFunc()->Write("shape_input_" + shapeName, TObject::kOverwrite);
          TF1* prefit = new TF1("shape_prefit_" + shapeName, "[0]*(" + shape->getForm() + ")", FitConfig::kFuncRangeMin, FitConfig::kFuncRangeMax);
          TF1* postfit= new TF1("shape_postfit_" + shapeName,"[0]*(" + shape->getForm() + ")", FitConfig::kFuncRangeMin, FitConfig::kFuncRangeMax);
          prefit->SetParameter(0, 1.0);
          postfit->SetParameter(0, gJesFit->GetParameter(shape->getIndex()));
          prefit->Write(prefit->GetName(), TObject::kOverwrite);
          postfit->Write(postfit->GetName(), TObject::kOverwrite);
      }

      // Store reference objects
      for (auto& [refName, obj] : m_referenceObjects) {
          obj->Write(refName, TObject::kOverwrite);
      }
  }

  void GlobalFit::run()
  {
      loadInputs();
      loadSystematics();
      loadShapes();
      loadReference();
      loadFSR();
      // scaleFSR(); // Uncomment if desired
      setupFitFunction();
      doGlobalFit();
      storeFitOutput();
  }

  // ------------------ Encapsulated Fitting Functions ------------------

  // jesFitWrapper returns a lambda suitable for TF1
  std::function<double(Double_t*, Double_t*)>
  GlobalFit::jesFitWrapper(TH1D* hjesref, std::map<TString, ShapeContainer*> shapeMap)
  {
      if (gDebug) {
          std::cout << "========: GlobalFit::jesFitWrapper() :========" << std::endl;
      }
      return [hjesref, shapeMap](Double_t* x, Double_t* p) -> double {
          double pt = x[0];
          double var = (gCurrentObs == "Resp") ? 1.0 : 0.0;

          double localJESref = 1.0;
          if (kUseJESref && gCurrentObs == "Resp") {
              assert(hjesref);
              localJESref = hjesref->Interpolate(pt);
          }
          for (auto& [shName, shape] : shapeMap) {
              if (shape->getAppliesTo() != "Resp")
                  continue;
              int index = shape->getIndex();
              assert(index >= 0);
              TF1* f1 = shape->getFunc();
              assert(f1);
              double parVal = p[index];
              if (shape->isPositive()) {
                  parVal = std::max(parVal, 0.0);
              }
              var += parVal * f1->Eval(pt) * kScaleFullSimShape;
          }
          return var / localJESref;
      };
  }

  // jesFit is a dummy function (not directly used)
  Double_t GlobalFit::jesFit(Double_t* /*x*/, Double_t* /*p*/)
  {
      return -1000.0;
  }

  // jesFitter is the function called by the minimizer to compute chi2
  void GlobalFit::jesFitter(Int_t& npar, Double_t* grad, Double_t& chi2, Double_t* par, Int_t flag)
  {
      if (gDebug) {
          std::cout << "========: GlobalFit::jesFitter() :========" << std::endl;
      }
      assert(gJesFit);

      int nFitPars       = gJesFit->GetNpar();
      int nNuisancePars  = npar - nFitPars;
      Double_t* fitPars  = &par[0];
      Double_t* nuisPars = &par[nFitPars];

      if (flag) {
          chi2 = 0.0;
          gNFittedDataPoints = 0;

          // Loop over input data
          for (auto& [dtName, dt] : gMyData) {
              std::cout << *dt << std::endl;
              TString dtType = dt->getType();
              TGraphErrors* graphIn  = dt->getInput();
              TGraphErrors* graphOut = dt->getOutput();
              TGraphErrors* graphVar = dt->getVariation();

              for (int bin = 0; bin < graphIn->GetN(); ++bin) {
                  double pt        = graphIn->GetX()[bin];
                  double dataPoint = graphIn->GetY()[bin];
                  double sigma     = graphIn->GetEY()[bin];

                  gCurrentObs = dtType;
                  gJesFit->SetParameters(fitPars);
                  double fitVal = gJesFit->EvalPar(&pt, par);

                  double shift = 0.0;
                  // Sum nuisance shifts
                  for (auto& [srcName, source] : gSources) {
                      if (dtName != source->getAppliesTo()) continue;
                      if (gDebug) {
                          Helper::printLine("adding1 " + srcName + " to shift for " + dtName +
                                              "(" + source->getAppliesTo() + ")", colYellow);
                      }
                      TH1D* hsrc = source->getHist();
                      assert(hsrc);
                      shift += nuisPars[source->getIndex()] *
                               hsrc->GetBinContent(hsrc->FindBin(pt));
                  }
                  // Additional shape shifts for non-Resp observables
                  for (auto& [shapeName, shape] : gShapes) {
                      if (shape->getAppliesTo() == "Resp") continue;
                      if (dtType != shape->getAppliesTo()) continue;
                      if (pt < 40 || pt > 600) continue; // example cut
                      if (gDebug) {
                          Helper::printLine("adding2 " + shapeName + " to shift for " + dtType +
                                              "(" + shape->getAppliesTo() + ") index:" + std::to_string(shape->getIndex()), colYellow);
                      }
                      shift += fitPars[shape->getIndex()] *
                               shape->getFunc()->Eval(pt) * kScaleFullSimShape;
                      std::cout << "  --> index:" << shape->getIndex()
                                << " " << fitPars[shape->getIndex()]
                                << " " << shape->getFunc()->Eval(pt)
                                << " " << shift << std::endl;
                  }

                  std::cout << "dataPoint: " << dataPoint << " shift: " << shift <<
                               " fit: " << fitVal << " sigma: " << sigma <<
                               " err: " << Helper::oPlus(sigma, kGlobalErrMin) << std::endl;

                  double chiVal = (dataPoint + shift - fitVal) /
                                  Helper::oPlus(sigma, kGlobalErrMin);
                  chi2 += chiVal * chiVal;
                  ++gNFittedDataPoints;

                  // Store shifted data
                  assert(graphOut->GetN() == graphIn->GetN());
                  graphOut->SetPoint(bin, pt, dataPoint + shift);
                  graphVar->SetPoint(bin, pt, shift);
              }
          }
          // Add nuisance chi2
          for (int ipar = 0; ipar < nNuisancePars; ++ipar) {
              chi2 += nuisPars[ipar] * nuisPars[ipar];
              ++gNFittedDataPoints;
          }
          // Penalize main fit parameters
          if (kPenalizeFitPars) {
              for (int ipar = 0; ipar < nFitPars; ++ipar) {
                  chi2 += fitPars[ipar] * fitPars[ipar];
                  ++gNFittedDataPoints;
              }
          }
          if ((++gFitCounter) % 100 == 0) {
              std::cout << "." << std::flush;
          }
      } else {
          // flag==0 => do nothing
          if (grad) { /* no gradient computed */ }
          return;
      }
      std::cout << colYellow << "chi2: " << chi2 << colReset << std::endl;
  }

} // namespace ClassicFit

