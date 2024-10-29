#include <TFile.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TROOT.h>

using namespace std;

//$ g++ -o /afs/cern.ch/user/r/rverma/public/scanTFile scanTFile.C `root-config --cflags --glibs`

//$ ./scanTFile file.root

void printInfo(TObject *obj) {
    if (TTree *tree = dynamic_cast<TTree*>(obj)) {
        std::cout<< setw(15) << "TTree: " << setw(35)<< tree->GetName() << setw(15) << tree->GetEntries() << std::endl;
    }
    else if (TH1F *h1f = dynamic_cast<TH1F*>(obj)) {
        std::cout<< setw(15) << "TH1F: " << setw(35)<< h1f->GetName() << setw(15) << h1f->GetEntries() << setw(15) << h1f->GetMean() << setw(15) << h1f->GetRMS() << std::endl;
    }
    else if (TH1D *h1f = dynamic_cast<TH1D*>(obj)) {
        std::cout<< setw(15) << "TH1D: " << setw(35)<< h1f->GetName() << setw(15) << h1f->GetEntries() << setw(15) << h1f->GetMean() << setw(15) << h1f->GetRMS() << std::endl;
    }
    else if (TH2D *h2d = dynamic_cast<TH2D*>(obj)) {
        std::cout<< setw(15) << "TH2D: " << setw(35)<< h2d->GetName() << setw(15) << h2d->GetEntries() << setw(15) << h2d->GetMean() << setw(15) << h2d->GetRMS() << std::endl;
    }
    else if (TProfile *prof = dynamic_cast<TProfile*>(obj)) {
        std::cout<< setw(15) << "TProfile: " << setw(35)<< prof->GetName() << setw(15) << prof->GetEntries() << setw(15) << prof->GetMean() << setw(15) << prof->GetRMS() << std::endl;
    }
    else if (TProfile2D *prof2d = dynamic_cast<TProfile2D*>(obj)) {
        std::cout<< setw(15) << "TProfile2D: " << setw(35)<< prof2d->GetName() << setw(15) << prof2d->GetEntries() << setw(15) << prof2d->GetMean() << setw(15) << prof2d->GetRMS() << std::endl;
    }
}

void scanDirectory(TDirectory *dir, const std::string &path = "") {
    std::string currentPath = path + dir->GetName() + "/";
    std::cout << "\nDirectory: " << currentPath << std::endl;

    TIter next(dir->GetListOfKeys());
    TKey *key;

    while ((key = (TKey*)next())) {
        TObject *obj = key->ReadObj();
        
        if (obj->InheritsFrom(TDirectory::Class())) {
            scanDirectory((TDirectory*)obj, currentPath); // Recursive call for subdirectories
        } else {
            printInfo(obj);
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: scanTFile file.root" << std::endl;
        return 1;
    }
    gROOT->SetBatch(true);    
    TFile *file = TFile::Open(argv[1]);
    if (!file || file->IsZombie()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }
    cout<<"\n-----------: Scan all directories and print Entries, Mean, RMS :------------\n"<<endl;
    scanDirectory(file);

    file->Close();
    delete file;
    return 0;
}
