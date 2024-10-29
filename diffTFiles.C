#include <TFile.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TLatex.h>
#include <iostream>
#include <string>
#include <TList.h>
#include <TClass.h>
#include <TROOT.h>

//$ g++ -o /afs/cern.ch/user/r/rverma/public/diffTFiles diffTFiles.C `root-config --cflags --glibs`
// ./diffTFiles file1.root file2.root

void compareHistograms(TH1* h1, TH1* h2, const std::string& objPath, const char* pdfFileName, TCanvas* c1);
void compareDirectories(TDirectory* dir1, TDirectory* dir2, const std::string& path, const char* pdfFileName, TCanvas* c1);

void compareRootFiles(const char* file1name, const char* file2name) {
    TFile* file1 = TFile::Open(file1name);
    TFile* file2 = TFile::Open(file2name);
    
    if (!file1 || !file2) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }
    
    // Output PDF file
    const char* pdfFileName = "comparison.pdf";
    // Start the multi-page PDF
    TCanvas* c1 = new TCanvas("c1", "", 800, 800);
    c1->Print(Form("%s[", pdfFileName)); // This starts the multi-page PDF
    
    compareDirectories(file1, file2, "", pdfFileName, c1);
    
    c1->Print(Form("%s]", pdfFileName)); // This ends the multi-page PDF
    
    delete c1;
    
    file1->Close();
    file2->Close();
}

void compareDirectories(TDirectory* dir1, TDirectory* dir2, const std::string& path, const char* pdfFileName, TCanvas* c1) {
    TList* keys = dir1->GetListOfKeys();
    TIter nextkey(keys);
    TKey* key;
     
    while ((key = (TKey*)nextkey())) {
        TObject* obj1 = key->ReadObj();
        const char* name = obj1->GetName();
        
        // Build the path to the object
        std::string objPath = path.empty() ? name : path + "/" + name;
        
        // Try to get the object from dir2
        TObject* obj2 = dir2->Get(name);
        if (!obj2) {
            std::cout << "Object " << objPath << " not found in second file." << std::endl;
            continue;
        }
        
        // Check if it's a directory
        if (obj1->InheritsFrom(TDirectory::Class())) {
            // It's a directory, recurse
            TDirectory* subdir1 = (TDirectory*)obj1;
            TDirectory* subdir2 = (TDirectory*)obj2;
            compareDirectories(subdir1, subdir2, objPath, pdfFileName, c1);
        }
        else if (obj1->InheritsFrom(TH1::Class())) {
            // It's a histogram, compare
            TH1* h1 = (TH1*)obj1;
            TH1* h2 = (TH1*)obj2;
            compareHistograms(h1, h2, objPath, pdfFileName, c1);
        }
        else {
            std::cout << "Object " << objPath << " is of unsupported type." << std::endl;
        }
    }
}

void compareHistograms(TH1* h1, TH1* h2, const std::string& objPath, const char* pdfFileName, TCanvas* c1) {
    c1->Clear();
    
    // Create upper and lower pads
    TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetBottomMargin(0.02);
    pad1->Draw();
    TPad* pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0.02);
    pad2->SetBottomMargin(0.3);
    pad2->Draw();
    
    // Upper pad
    pad1->cd();
    h1->SetLineColor(kRed);
    h2->SetLineColor(kBlue);
    
    h1->Draw("hist");
    h2->Draw("hist same");
    
    // Add legend
    TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);
    leg->AddEntry(h1, "Nano", "l");
    leg->AddEntry(h2, "Skim", "l");
    leg->Draw();
    
    // Add title
    TLatex* latex = new TLatex();
    latex->SetNDC();
    latex->SetTextSize(0.04);
    latex->DrawLatex(0.1, 0.92, objPath.c_str());
    
    // Lower pad
    pad2->cd();
    
    TH1* h_ratio = (TH1*)h1->Clone();
    h_ratio->SetName("h_ratio");
    h_ratio->SetTitle("");
    h_ratio->Divide(h2);
    
    h_ratio->SetLineColor(kBlack);
    h_ratio->SetMarkerStyle(21);
    h_ratio->GetYaxis()->SetTitle("Ratio");
    h_ratio->GetYaxis()->SetNdivisions(505);
    h_ratio->GetYaxis()->SetTitleSize(14);
    h_ratio->GetYaxis()->SetTitleFont(43);
    h_ratio->GetYaxis()->SetTitleOffset(1.55);
    h_ratio->GetYaxis()->SetLabelFont(43);
    h_ratio->GetYaxis()->SetLabelSize(13);
    
    h_ratio->GetXaxis()->SetTitleSize(14);
    h_ratio->GetXaxis()->SetTitleFont(43);
    h_ratio->GetXaxis()->SetTitleOffset(4.);
    h_ratio->GetXaxis()->SetLabelFont(43);
    h_ratio->GetXaxis()->SetLabelSize(13);
    
    h_ratio->Draw("ep");
    
    // Save the canvas to the PDF file
    c1->Print(pdfFileName);
    
    // Clean up
    delete h_ratio;
    delete leg;
    delete latex;
    // Note: do not delete pads, they are owned by the canvas
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: compareRootFiles file1.root file2.root" << std::endl;
        return 1;
    }
    gROOT->SetBatch(true); 
    compareRootFiles(argv[1], argv[2]);
    
    return 0;
}

