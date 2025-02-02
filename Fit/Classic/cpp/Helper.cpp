#include "Helper.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>

namespace ClassicFit {
namespace Helper {

std::string nWhiteSpaces(int n)
{
    return std::string(std::max(0, n), ' ');
}

TString completeWhiteSpaces(const TString& text, int ntot)
{
    return text + nWhiteSpaces(ntot - text.Length());
}

bool findInString(const std::string& search, const std::string& str)
{
    return (str.find(search) != std::string::npos);
}

void printLoading(TString type, TString name, TString objname, const std::string& color)
{
    print("Loading " + type + " for " + completeWhiteSpaces(name) + ": " + objname + "...", color);
    std::cout << colReset << std::endl;
}

void printParameter(TString name, double val, double err, const std::string& color)
{
    printLine("  --> " + completeWhiteSpaces(name) + Form(": %+5.2f +/- %5.2f", val, err), color);
}

void removeZerosFromGraph(TGraphErrors* graph)
{
    for (int i = graph->GetN() - 1; i >= 0; --i) {
        if (graph->GetY()[i] == 0.0 && graph->GetEY()[i] == 0.0) {
            graph->RemovePoint(i);
        }
    }
}

double oPlus(double a, double b)
{
    return std::sqrt(a * a + b * b);
}

double fitError(TF1* func, TMatrixD errMatrix, double x, double k)
{
    int nPars = func->GetNpar();
    std::vector<double> grad(nPars);
    func->GradientPar(&x, grad.data());

    double sumerr2 = 0.0;
    for (int i = 0; i < nPars; ++i) {
        for (int j = 0; j < nPars; ++j) {
            sumerr2 += errMatrix[i][j] * grad[i] * grad[j];
        }
    }
    return func->Eval(x) + k * std::sqrt(sumerr2);
}

void funcToGraph(TF1* func, TMatrixD errMatrix, TGraphErrors* graph, double k)
{
    if (!func || !graph) return;
    std::unique_ptr<TGraph> tmpGraph(new TGraph(func));
    for (int i = 0; i < tmpGraph->GetN(); ++i) {
        double x = tmpGraph->GetX()[i];
        double y = tmpGraph->GetY()[i];
        graph->AddPoint(x, y);
        double dy = fitError(func, errMatrix, x, k) - y;
        graph->SetPointError(i, 0.0, dy);
    }
}

void funcToHist(TF1* func, TMatrixD errMatrix, TH1D* hist, double k)
{
    if (!func || !hist) return;
    for (int j = 1; j <= hist->GetNbinsX(); ++j) {
        double x   = hist->GetBinCenter(j);
        double val = func->Eval(x);
        double err = fitError(func, errMatrix, x, k) - val;
        hist->SetBinContent(j, val);
        hist->SetBinError(j, err);
    }
}

void multiplyGraph(TGraphErrors* graph, double scale)
{
    if (!graph) return;
    for (int i = 0; i < graph->GetN(); ++i) {
        double x  = graph->GetX()[i];
        double y  = graph->GetY()[i];
        double ey = graph->GetEY()[i];
        graph->SetPoint(i, x, scale * y);
        graph->SetPointError(i, graph->GetEX()[i], scale * ey);
    }
}

void multiplyGraph(TGraphErrors* graph, TF1* func)
{
    if (!graph || !func) return;
    for (int i = 0; i < graph->GetN(); ++i) {
        double x   = graph->GetX()[i];
        double y   = graph->GetY()[i];
        double ey  = graph->GetEY()[i];
        double fval= func->Eval(x);
        graph->SetPoint(i, x, fval * y);
        graph->SetPointError(i, graph->GetEX()[i], fval * ey);
    }
}

void propagateErrorToGraph(TGraphErrors* graph, std::vector<TF1*> funcs, TMatrixD errMatrix)
{
    if (!graph) return;
    int nFuncs = static_cast<int>(funcs.size());
    for (int i = 0; i < graph->GetN(); ++i) {
        double x    = graph->GetX()[i];
        double xErr = graph->GetEX()[i];
        double sumerr2 = 0.0;
        for (int ipar = 0; ipar < nFuncs; ++ipar) {
            for (int jpar = 0; jpar < nFuncs; ++jpar) {
                double valI = funcs[ipar]->Eval(x);
                double valJ = funcs[jpar]->Eval(x);
                sumerr2 += valI * valJ * errMatrix[ipar][jpar];
            }
        }
        graph->SetPointError(i, xErr, std::sqrt(sumerr2));
    }
}

} // namespace Helper
} // namespace ClassicFit

