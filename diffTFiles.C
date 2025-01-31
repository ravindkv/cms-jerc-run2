
//$ g++ -o /afs/cern.ch/user/r/rverma/public/diffTFiles diffTFiles.C `root-config --cflags --glibs`
// ./diffTFiles file1.root file2.root ... fileN.root

#include <TFile.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TList.h>
#include <TClass.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TROOT.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

//-------------------------------------------------//
// General templated function to calculate ratio
// ratioHist = (hist / refHist).
//-------------------------------------------------//
template<typename T>
void calculateHistRatio(
    T* hist, T* refHist, TGraphErrors* ratioGraph)
{
    int nBins = hist->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        double thisVal  = hist->GetBinContent(i);
        double thisErr  = hist->GetBinError(i);
        double refVal   = refHist->GetBinContent(i);
        double refErr   = refHist->GetBinError(i);
        double ratioVal = 0.;
        double ratioErr = 0.;
        double binCenter = hist->GetXaxis()->GetBinCenter(i);
        double binWidth  = hist->GetXaxis()->GetBinWidth(i);

        if (refVal != 0.) {
            ratioVal = thisVal / refVal;
            // Error propagation:
            //   ratioErr^2 = (thisErr/refVal)^2 + (thisVal * refErr / refVal^2)^2
            ratioErr = std::sqrt(
                std::pow(thisErr / refVal, 2) +
                std::pow(thisVal * refErr / (refVal * refVal), 2));
        }

        ratioGraph->SetPoint(i - 1, binCenter, ratioVal);
        // If you want x-errors, set them to binWidth/2:
        ratioGraph->SetPointError(i - 1, binWidth * 0.5, ratioErr);
    }
}

// Forward declarations
void compareDirectories(const std::vector<TFile*>& files,
                       TDirectory* refDir,
                       const std::string& path,
                       const char* pdfFileName,
                       TCanvas* c1);

void compareHistograms(const std::vector<TH1*>& hists,
                       const std::string& objPath,
                       const char* pdfFileName,
                       TCanvas* c1);

//-------------------------------------------------//
// Compare an arbitrary number of files.
//-------------------------------------------------//
void compareRootFiles(const std::vector<std::string>& fileNames)
{
    // Open TFiles
    std::vector<TFile*> files;
    files.reserve(fileNames.size());
    for (auto& fn : fileNames) {
        TFile* f = TFile::Open(fn.c_str());
        if (!f || f->IsZombie()) {
            std::cerr << "Failed to open file: " << fn << std::endl;
            return;
        }
        files.push_back(f);
    }
    std::cout<<"Comparing "<<files.size()<<'\n';
    // PDF output
    const char* pdfFileName = "comparison.pdf";
    TCanvas* c1 = new TCanvas("c1", "", 800, 800);
    c1->Print(Form("%s[", pdfFileName)); // open multi-page PDF

    // Compare directories. Use the directory structure of the first file as reference.
    compareDirectories(files, files[0], "", pdfFileName, c1);

    c1->Print(Form("%s]", pdfFileName)); // close multi-page PDF

    // Cleanup
    delete c1;
    for (auto f : files) {
        f->Close();
        delete f;
    }
}

//-------------------------------------------------//
// Recursively compare directories among all files.
// We'll use the directory structure from the refDir
// (which is from the first file) to discover objects
// and try to retrieve them from the other files.
//-------------------------------------------------//
void compareDirectories(const std::vector<TFile*>& files,
                       TDirectory* refDir,
                       const std::string& path,
                       const char* pdfFileName,
                       TCanvas* c1)
{
    if (!refDir) return;
    TList* keys = refDir->GetListOfKeys();
    if (!keys) return;

    TIter nextKey(keys);
    TKey* key;
    while ((key = (TKey*) nextKey())) {
        TObject* obj = key->ReadObj();
        if (!obj) continue;

        const char* name = obj->GetName();
        std::string objPath = path.empty() ? name : path + "/" + name;

        // For directories, recurse
        if (obj->InheritsFrom(TDirectory::Class())) {
            TDirectory* subdir = dynamic_cast<TDirectory*>(obj);
            if (!subdir) continue;
            if (std::string(subdir->GetName()).find("HLT_") != std::string::npos) continue;
            
            // We pass the same TDirectory (from file[0]) as the reference,
            // but we want to check if the same subdir exists in all files
            // before recursing, or at least skip if not found in others.
            compareDirectories(files, subdir, objPath, pdfFileName, c1);
        }
        // For histograms
        else if (obj->InheritsFrom(TH1::Class())) {
            // Skip 2D, Profile2D, etc. if desired:
            if (dynamic_cast<TProfile2D*>(obj))  continue;
            if (dynamic_cast<TH2*>(obj))         continue;
            // If you want to skip TProfiles, comment out the next lines:
            // if (dynamic_cast<TProfile*>(obj))   continue;

            // Build a vector of hist pointers
            std::vector<TH1*> hists;
            hists.reserve(files.size());

            // Attempt to retrieve the same object name from each file
            for (auto& f : files) {
                TH1* h = dynamic_cast<TH1*>(f->Get(objPath.c_str()));
                if (!h) {
                    // If not found in one of the files, skip.
                    // Or you can push_back a nullptr if you want to be tolerant.
                    hists.clear();
                    break;
                }
                // Clone to separate them from each file’s memory mgmt
                // (optional, depending on your usage)
                TH1* clone = dynamic_cast<TH1*>(h->Clone(f->GetName()));
                // Store the original name of the file or hist for labeling
                clone->SetDirectory(nullptr);
                hists.push_back(clone);
            }

            // If we have the same histogram in all files
            if (hists.size() == files.size()) {
                compareHistograms(hists, objPath, pdfFileName, c1);
            }

            // Cleanup local clones
            for (auto& hh : hists) {
                if (hh) delete hh;
            }
        }
        else {
            // Do nothing for unsupported object types
            // e.g. TTree, TGraph, etc.
            // std::cout << "Skipping " << objPath << std::endl;
        }
    }
}

//-------------------------------------------------//
// Compare a vector of TH1 pointers: 
//   - Overlays them all on the upper pad
//   - Makes ratio to the first histogram in the lower pad
//-------------------------------------------------//
void compareHistograms(const std::vector<TH1*>& hists,
                       const std::string& objPath,
                       const char* pdfFileName,
                       TCanvas* c1)
{
    if (hists.empty()) return;
    // Decide if you want log scale or normalization:
    // For example, skip log scale for TProfile, etc.
    bool isLog  = false;
    bool isNorm = true;
    // If you need to do logic here, go for it.

    c1->Clear();
    gStyle->SetOptStat(0);

    // Create an upper pad and a lower pad
    TPad* pad1 = new TPad("pad1","pad1",0,0.3,1,1);
    pad1->SetBottomMargin(0.02);
    pad1->Draw();

    TPad* pad2 = new TPad("pad2","pad2",0,0,1,0.3);
    pad2->SetTopMargin(0.02);
    pad2->SetBottomMargin(0.3);
    pad2->Draw();

    //--------------------
    // Upper pad
    //--------------------
    pad1->cd();
    if (isLog) pad1->SetLogy(true);

    // Optionally normalize all histograms
    if (isNorm) {
        for (auto* h : hists) {
            double integral = h->Integral();
            if (integral > 0.) {
                h->Scale(1. / integral);
            }
        }
    }

    // Setup line colors/styles
    // (Add more if you anticipate more than, say, 9 files)
    static int colors[9] = {
        kBlack, kRed, kBlue, kGreen+2, kMagenta, kOrange+1, kCyan+2, kGray+2, kViolet+2
    };
    static int markers[9] = {
        20, 21, 22, 23, 24, 25, 26, 27, 28
    };

    // Let’s find a max for them all
    double maxVal = 0.;
    for (size_t i=0; i<hists.size(); i++) {
        double thisMax = hists[i]->GetMaximum();
        if (thisMax > maxVal) maxVal = thisMax;
    }

    // If log scale, set min to a small positive
    if (isLog) {
        for (auto* h : hists) {
            if (h->GetMinimum() <= 0) {
                h->SetMinimum(1e-4);
            }
            h->SetMaximum(maxVal * 10.);
        }
    } else {
        // A little margin on top
        for (auto* h : hists) {
            h->SetMaximum(maxVal * 1.2);
        }
    }

    // Draw them
    for (size_t i=0; i<hists.size(); i++) {
        hists[i]->SetLineColor(colors[i % 9]);
        hists[i]->SetLineWidth(2);
        // if you want fill color or marker style, do it here

        if (i == 0) {
            hists[i]->Draw("EP");
        }
        else {
            hists[i]->Draw("EP same");
        }
    }

    // Create a legend
    TLegend* leg = new TLegend(0.2, 0.60, 0.90, 0.90);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);

    // Add each histogram to the legend with stats
    for (size_t i=0; i<hists.size(); i++) {
        TH1* h = hists[i];
        std::ostringstream ostr;
        ostr << " (E=" << (int)h->GetEntries()
             << ", M=" << std::fixed << std::setprecision(1) << h->GetMean()
             << ", R=" << std::fixed << std::setprecision(1) << h->GetRMS()
             << ")";
        leg->AddEntry(h, h->GetName(), "l");
        leg->AddEntry((TObject*)0, ostr.str().c_str(), "");
    }
    leg->Draw();

    // Title on top
    TLatex* latex = new TLatex();
    latex->SetNDC();
    latex->SetTextSize(0.04);
    latex->DrawLatex(0.1, 0.92, objPath.c_str());

    //--------------------
    // Lower pad (ratios)
    //--------------------
    pad2->cd();

    // We'll plot ratio to the *first* histogram:
    TH1* refHist = hists[0];
    int nBins = refHist->GetNbinsX();

    // We can store all ratio TGraphs in a vector
    std::vector<TGraphErrors*> ratios;
    ratios.reserve(hists.size()-1);

    // Create the ratio for each hist vs. the first
    for (size_t i=1; i<hists.size(); i++) {
        TGraphErrors* ratioGraph = new TGraphErrors(nBins);
        calculateHistRatio(hists[i], refHist, ratioGraph);
        // Style
        ratioGraph->SetMarkerColor(colors[i % 9]);
        ratioGraph->SetLineColor(colors[i % 9]);
        ratioGraph->SetMarkerStyle(markers[i % 9]);
        ratios.push_back(ratioGraph);
    }

    // Draw the first ratio
    if (!ratios.empty()) {
        ratios[0]->Draw("AP");
        ratios[0]->SetTitle("");
        ratios[0]->GetHistogram()->GetXaxis()->SetTitleSize(0.12);
        ratios[0]->GetHistogram()->GetXaxis()->SetLabelSize(0.12);
        ratios[0]->GetHistogram()->GetYaxis()->SetTitle("Hist / Ref");
        ratios[0]->GetHistogram()->GetYaxis()->SetTitleSize(0.12);
        ratios[0]->GetHistogram()->GetYaxis()->SetTitleOffset(0.55);
        ratios[0]->GetHistogram()->GetYaxis()->SetLabelSize(0.11);
        ratios[0]->GetHistogram()->GetYaxis()->SetNdivisions(6, 5, 0);
        // You can choose appropriate range:
        ratios[0]->GetHistogram()->GetYaxis()->SetRangeUser(0., 2.);
    }

    // Then overlay the rest of the ratio graphs
    for (size_t i=1; i<ratios.size(); i++) {
        ratios[i]->Draw("P same");
    }

    // Save page to PDF
    c1->Print(pdfFileName);

    // Cleanup ratio objects
    for (auto g : ratios) {
        delete g;
    }
    delete leg;
    delete latex;

    // DO NOT delete pads or the histograms that came from outside
    // since they are managed by the caller (or above).
}

//-------------------------------------------------//
// main()
// Example usage:
//   ./compareNFiles file1.root file2.root [file3.root ...]
//-------------------------------------------------//
int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0]
                  << " file1.root [file2.root ... fileN.root]\n";
        return 1;
    }

    gROOT->SetBatch(true);

    // Collect all filenames into a vector
    std::vector<std::string> fileNames;
    for (int i=1; i<argc; i++) {
        fileNames.push_back(argv[i]);
    }

    std::cout<<"Comparing Names "<<fileNames.size()<<'\n';
    compareRootFiles(fileNames);

    return 0;
}

