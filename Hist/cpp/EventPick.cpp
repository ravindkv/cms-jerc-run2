#include "EventPick.h"

EventPick::~EventPick(){
  cout<<"Distructor of EventPick"<<endl;
}

//https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#Run_3_recommendations
bool EventPick::passHLT(SkimTree *tree){
  //We can also pick data or year-specific 
  //if(isData){}
  //if(is2022){}
  bool pass_HLT = false;
  if(isZeeJet){
    pass_HLT = tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
    if(isDebug){
    cout<<"tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = "<< tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
    }
  }

  if(isZmmJet){
    pass_HLT = tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
    if(isDebug){
    cout<<"tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8 = "<< tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8<<endl;
    }
  }

  if(isGamJet){
    pass_HLT = tree->HLT_Photon200 ||
    tree->HLT_Photon175 || 
    tree->HLT_Photon150 || 
    tree->HLT_Photon120 || 
    tree->HLT_Photon90 || 
    tree->HLT_Photon75 || 
    tree->HLT_Photon50 || 
    tree->HLT_Photon33 || 
    //tree->HLT_Photon20 ||
    //tree->HLT_Photon120EB_TightID_TightIso ||
    tree->HLT_Photon110EB_TightID_TightIso ||
    //tree->HLT_Photon100EB_TightID_TightIso ||
    tree->HLT_Photon100EBHE10 ||
    tree->HLT_Photon30EB_TightID_TightIso ||
    tree->HLT_Photon90_R9Id90_HE10_IsoM ||
    tree->HLT_Photon75_R9Id90_HE10_IsoM ||
    tree->HLT_Photon50_R9Id90_HE10_IsoM ||
    tree->HLT_Photon30_HoverELoose ||
    tree->HLT_Photon20_HoverELoose;
    if(isDebug){
      cout<<"tree->HLT_Photon200 = "                    << tree->HLT_Photon200 <<endl;
      cout<<"tree->HLT_Photon175 = "                    << tree->HLT_Photon175 <<endl; 
      cout<<"tree->HLT_Photon150 = "                    << tree->HLT_Photon150 <<endl; 
      cout<<"tree->HLT_Photon120 = "                    << tree->HLT_Photon120 <<endl; 
      cout<<"tree->HLT_Photon90 = "                     << tree->HLT_Photon90 <<endl; 
      cout<<"tree->HLT_Photon75 = "                     << tree->HLT_Photon75 <<endl; 
      cout<<"tree->HLT_Photon50 = "                     << tree->HLT_Photon50 <<endl; 
      cout<<"tree->HLT_Photon33 = "                     << tree->HLT_Photon33 <<endl; 
      cout<<"tree->HLT_Photon20 = "                     << tree->HLT_Photon20 <<endl;
      cout<<"tree->HLT_Photon120EB_TightID_TightIso = " << tree->HLT_Photon120EB_TightID_TightIso <<endl;
      cout<<"tree->HLT_Photon110EB_TightID_TightIso = " << tree->HLT_Photon110EB_TightID_TightIso <<endl;
      cout<<"tree->HLT_Photon100EB_TightID_TightIso = " << tree->HLT_Photon100EB_TightID_TightIso <<endl;
      cout<<"tree->HLT_Photon100EBHE10 = "              << tree->HLT_Photon100EBHE10 <<endl;
      cout<<"tree->HLT_Photon30EB_TightID_TightIso = "  << tree->HLT_Photon30EB_TightID_TightIso <<endl;
      cout<<"tree->HLT_Photon90_R9Id90_HE10_IsoM = "    << tree->HLT_Photon90_R9Id90_HE10_IsoM <<endl;
      cout<<"tree->HLT_Photon75_R9Id90_HE10_IsoM = "    << tree->HLT_Photon75_R9Id90_HE10_IsoM <<endl;
      cout<<"tree->HLT_Photon50_R9Id90_HE10_IsoM = "    << tree->HLT_Photon50_R9Id90_HE10_IsoM <<endl;
      cout<<"tree->HLT_Photon30_HoverELoose = "         << tree->HLT_Photon30_HoverELoose <<endl;
      cout<<"tree->HLT_Photon20_HoverELoose = "         << tree->HLT_Photon20_HoverELoose <<endl;
    }
  }//isGamJet
  if(isMCTruth || isVetoMap){
    pass_HLT = tree->HLT_MC;
    if(isDebug){
      cout<<"tree->HLT_MC = "                    << tree->HLT_MC <<endl;
    }
  }
  /*
  if(isDiJet){
    pass_HLT = tree->HLT_PFJet40     ||
        tree->HLT_PFJet60            ||
        tree->HLT_PFJet80            ||
        tree->HLT_PFJet140           ||
        tree->HLT_PFJet200           ||
        tree->HLT_PFJet260           ||
        tree->HLT_PFJet320           ||
        tree->HLT_PFJet400           ||
        tree->HLT_PFJet450           ||
        tree->HLT_PFJet500           ||
        tree->HLT_DiPFJetAve40       ||
        tree->HLT_DiPFJetAve60       ||
        tree->HLT_DiPFJetAve80       ||
        tree->HLT_DiPFJetAve140      ||
        tree->HLT_DiPFJetAve200      ||
        tree->HLT_DiPFJetAve260      ||
        tree->HLT_DiPFJetAve320      ||
        tree->HLT_DiPFJetAve400      ||
        tree->HLT_DiPFJetAve500      ||
        tree->HLT_DiPFJetAve60_HFJEC ||
        tree->HLT_DiPFJetAve80_HFJEC ||
        tree->HLT_DiPFJetAve100_HFJEC||
        tree->HLT_DiPFJetAve160_HFJEC||
        tree->HLT_DiPFJetAve220_HFJEC||
        tree->HLT_DiPFJetAve300_HFJEC;
    if(isDebug){
        cout<<"HLT_PFJet40 = "            << tree->HLT_PFJet40            ;
        cout<<"HLT_PFJet60 = "            << tree->HLT_PFJet60            ;
        cout<<"HLT_PFJet80 = "            << tree->HLT_PFJet80            ;
        cout<<"HLT_PFJet140 = "           << tree->HLT_PFJet140           ;
        cout<<"HLT_PFJet200 = "           << tree->HLT_PFJet200           ;
        cout<<"HLT_PFJet260 = "           << tree->HLT_PFJet260           ;
        cout<<"HLT_PFJet320 = "           << tree->HLT_PFJet320           ;
        cout<<"HLT_PFJet400 = "           << tree->HLT_PFJet400           ;
        cout<<"HLT_PFJet450 = "           << tree->HLT_PFJet450           ;
        cout<<"HLT_PFJet500 = "           << tree->HLT_PFJet500           ;
        cout<<"HLT_DiPFJetAve40 = "       << tree->HLT_DiPFJetAve40       ;
        cout<<"HLT_DiPFJetAve60 = "       << tree->HLT_DiPFJetAve60       ;
        cout<<"HLT_DiPFJetAve80 = "       << tree->HLT_DiPFJetAve80       ;
        cout<<"HLT_DiPFJetAve140 = "      << tree->HLT_DiPFJetAve140      ;
        cout<<"HLT_DiPFJetAve200 = "      << tree->HLT_DiPFJetAve200      ;
        cout<<"HLT_DiPFJetAve260 = "      << tree->HLT_DiPFJetAve260      ;
        cout<<"HLT_DiPFJetAve320 = "      << tree->HLT_DiPFJetAve320      ;
        cout<<"HLT_DiPFJetAve400 = "      << tree->HLT_DiPFJetAve400      ;
        cout<<"HLT_DiPFJetAve500 = "      << tree->HLT_DiPFJetAve500      ;
        cout<<"HLT_DiPFJetAve60_HFJEC = " << tree->HLT_DiPFJetAve60_HFJEC ;
        cout<<"HLT_DiPFJetAve80_HFJEC = " << tree->HLT_DiPFJetAve80_HFJEC ;
        cout<<"HLT_DiPFJetAve100_HFJEC = "<< tree->HLT_DiPFJetAve100_HFJEC;
        cout<<"HLT_DiPFJetAve160_HFJEC = "<< tree->HLT_DiPFJetAve160_HFJEC;
        cout<<"HLT_DiPFJetAve220_HFJEC = "<< tree->HLT_DiPFJetAve220_HFJEC;
        cout<<"HLT_DiPFJetAve300_HFJEC = "<< tree->HLT_DiPFJetAve300_HFJEC;
    }
  }//isDiJet
  */

  return pass_HLT;
}


bool EventPick::passFilter(SkimTree *tree){
  //We can also pick year-specific filters
  //if(isData){}
  //if(is2022){}
  bool pass = 
  (tree->Flag_goodVertices &&
  tree->Flag_globalSuperTightHalo2016Filter &&
  tree->Flag_EcalDeadCellTriggerPrimitiveFilter &&
  tree->Flag_BadPFMuonFilter &&
  tree->Flag_BadPFMuonDzFilter &&
  tree->Flag_hfNoisyHitsFilter &&
  tree->Flag_eeBadScFilter &&
  tree->Flag_ecalBadCalibFilter);

  if(isDebug){
    cout<<"tree->Flag_goodVertices             = "<<tree->Flag_goodVertices <<endl;
    cout<<"tree->Flag_globalSuperTightHalo2016Filter   = "<<tree->Flag_globalSuperTightHalo2016Filter <<endl;
    cout<<"tree->Flag_EcalDeadCellTriggerPrimitiveFilter = "<<tree->Flag_EcalDeadCellTriggerPrimitiveFilter <<endl;
    cout<<"tree->Flag_BadPFMuonFilter          = "<<tree->Flag_BadPFMuonFilter <<endl;
    cout<<"tree->Flag_BadPFMuonDzFilter          = "<<tree->Flag_BadPFMuonDzFilter <<endl;
    cout<<"tree->Flag_hfNoisyHitsFilter          = "<<tree->Flag_hfNoisyHitsFilter <<endl;
    cout<<"tree->Flag_eeBadScFilter            = "<<tree->Flag_eeBadScFilter <<endl;
    cout<<"tree->Flag_ecalBadCalibFilter)        = "<<tree->Flag_ecalBadCalibFilter<<endl;
  }

  return pass;
}

void EventPick::printBins(TH1D *hist){
  cout<<setw(30)<<hist->GetXaxis()->GetBinLabel(1)<<" : "<<hist->GetBinContent(1)<<endl;
  for(int i = 2; i<hist->GetNbinsX()+1; i++){
    int previous_bin = hist->GetBinContent(i-1);
    int current_bin  = hist->GetBinContent(i);
    double change = ((previous_bin - current_bin) / static_cast<double>(previous_bin)) * 100;
    cout<<setw(30)<<hist->GetXaxis()->GetBinLabel(i)<<" : "<<hist->GetBinContent(i)<< " => "<<change<<" %" <<endl;
  }
}


void EventPick:: printInfo(TObject *obj) {
    if (TTree *tree = dynamic_cast<TTree*>(obj)) {
        cout<< setw(15) << "TTree: " << setw(35)<< tree->GetName() << setw(15) << tree->GetEntries() << endl;
    }
    else if (TH1F *h1f = dynamic_cast<TH1F*>(obj)) {
        cout<< setw(15) << "TH1F: " << setw(35)<< h1f->GetName() << setw(15) << h1f->GetEntries() << setw(15) << h1f->GetMean() << setw(15) << h1f->GetRMS() << endl;
    }
    else if (TH1D *h1f = dynamic_cast<TH1D*>(obj)) {
        cout<< setw(15) << "TH1D: " << setw(35)<< h1f->GetName() << setw(15) << h1f->GetEntries() << setw(15) << h1f->GetMean() << setw(15) << h1f->GetRMS() << endl;
    }
    else if (TH2D *h2d = dynamic_cast<TH2D*>(obj)) {
        cout<< setw(15) << "TH2D: " << setw(35)<< h2d->GetName() << setw(15) << h2d->GetEntries() << setw(15) << h2d->GetMean() << setw(15) << h2d->GetRMS() << endl;
    }
    else if (TProfile *prof = dynamic_cast<TProfile*>(obj)) {
        cout<< setw(15) << "TProfile: " << setw(35)<< prof->GetName() << setw(15) << prof->GetEntries() << setw(15) << prof->GetMean() << setw(15) << prof->GetRMS() << endl;
    }
    else if (TProfile2D *prof2d = dynamic_cast<TProfile2D*>(obj)) {
        cout<< setw(15) << "TProfile2D: " << setw(35)<< prof2d->GetName() << setw(15) << prof2d->GetEntries() << setw(15) << prof2d->GetMean() << setw(15) << prof2d->GetRMS() << endl;
    }
}

void EventPick::scanDirectory(TDirectory *dir, const string &path = "") {
    string currentPath = path + dir->GetName() + "/";
    cout << "\nDirectory: " << currentPath << endl;

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

void EventPick::scanTFile(TFile *file) {
		cout<<"\n-----------: Scan all directories and print Entries, Mean, RMS :------------\n"<<endl;
    scanDirectory(file);
}

