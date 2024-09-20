#include "HistMultiJet.h"

class MultiJetHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;

  TProfile *ptleada, *ptleadm, *ptleadl, *ptleadr;
  TProfile *pcrecoila, *pcrecoilm, *pcrecoill, *pcrecoilr;
  TH1D *hpta_all, *hptm_all, *hptl_all, *hptr_all;
  TH1D *hpta_sel, *hptm_sel, *hptl_sel, *hptr_sel;
  TProfile *presa, *presm, *presl, *presr;
  TProfile *pm0a, *pm2a, *pmna, *pmua; // *pmoa; // pT,avp3
  TProfile *pm0m, *pm2m, *pmnm, *pmum; // *pmom; // pT,ave
  TProfile *pm0l, *pm2l, *pmnl, *pmul; //*pmol; // pT,tag
  TProfile *pm0r, *pm2r, *pmnr, *pmur; // *pmor; // pT,probe

  // (Optional) 2D recoils
  TH2D *h2recoila, *h2recoilm, *h2recoill, *h2recoilr;

  // (Optional) composition plots
  TProfile *ppt13, *prho13, *pchf13, *pnef13, *pnhf13, *pcef13, *pmuf13; // lead pT,avp
  TProfile *ppt25, *prho25, *pchf25, *pnef25, *pnhf25, *pcef25, *pmuf25; // recoil,pT,avp

  // (Optional) Controls
  TH2D *h2m0a;
  TH2D *h2m2a;
  TH1D *hcosdphi;
};


int HistMultiJet::Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout){
  map<string, MultiJetHistos *> mapStrHistMultiJet;

  TDirectory *curdir = gDirectory;
  assert(fout && !fout->IsZombie());
  string dir = (isMC ? "MC" : "DATA");
  double weight = (isMC ? tree->genWeight : 1);

  //------------------------------------
  // Cutflow histograms 
  //------------------------------------
  TH1D *hCutflow = new TH1D("hCutflow",";hCutflow",15,0.5,15.5);
  TH1D *hCutflowWeight = new TH1D("hCutflowWeight",";hCutflowWeight",15,0.5,15.5);
  vector<string> cuts;
  cuts.push_back("passHLT");
  cuts.push_back("passGoodLumi");
  cuts.push_back("passMetFilter");
  cuts.push_back("passAtleast1Probe");
  cuts.push_back("passAtleast1Jet");
  cuts.push_back("passJetVetoMap");
  cuts.push_back("passDPhiProbeJet1");
  cuts.push_back("passProbeBarrel");
  cuts.push_back("passJet1EtaJet2Pt");
  cuts.push_back("passResponse");
  for (int i = 0; i != cuts.size(); ++i) {
    hCutflow->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
    hCutflowWeight->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
  }

  // JER smearing (JER SF)
  bool smearJets = true;
  bool useJERSFvsPt = true; // new file format
  int smearNMax = 3;
  std::uint32_t _seed;
	std::mt19937 _mersennetwister;

  bool isZB = false;
  double fwdeta = 3.139;  // was 2.853. 80% (100%) on negative (positive) side
  double fwdeta0 = 2.964; // 2.853; // 40 and 260 up
  map<string, pair<const Bool_t*, struct range>> trigsName_ValRange;
  trigsName_ValRange["HLT_MC"                ]  = make_pair(tree->HLT_MC                 ,range{10, 3000, 0, 5.2});
  trigsName_ValRange["HLT_ZeroBias"          ]  = make_pair(tree->HLT_ZeroBias           ,range{10, 3000, 0, 5.2});
  if (!isZB}) {
  trigsName_ValRange["HLT_PFJet40"           ]  = make_pair(tree->HLT_PFJet40            ,range{40, 85, 0, 5.2   });
  trigsName_ValRange["HLT_PFJet60"           ]  = make_pair(tree->HLT_PFJet60            ,range{85, 100, 0, 5.2  });
  trigsName_ValRange["HLT_PFJet80"           ]  = make_pair(tree->HLT_PFJet80            ,range{100, 155, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet140"          ]  = make_pair(tree->HLT_PFJet140           ,range{155, 210, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet200"          ]  = make_pair(tree->HLT_PFJet200           ,range{210, 300, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet260"          ]  = make_pair(tree->HLT_PFJet260           ,range{300, 400, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet320"          ]  = make_pair(tree->HLT_PFJet320           ,range{400, 500, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet400"          ]  = make_pair(tree->HLT_PFJet400           ,range{500, 600, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet450"          ]  = make_pair(tree->HLT_PFJet450           ,range{500, 600, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet500"          ]  = make_pair(tree->HLT_PFJet500           ,range{600, 6500, 0, 5.2});
  trigsName_ValRange["HLT_PFJet550"          ]  = make_pair(tree->HLT_PFJet550           ,range{700, 6500, 0, 5.2});
  trigsName_ValRange["HLT_DiPFJetAve40"      ]  = make_pair(tree->HLT_DiPFJetAve40       ,range{40, 85, 0, 5.2   });
  trigsName_ValRange["HLT_DiPFJetAve60"      ]  = make_pair(tree->HLT_DiPFJetAve60       ,range{85, 100, 0, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve80"      ]  = make_pair(tree->HLT_DiPFJetAve80       ,range{100, 155, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve140"     ]  = make_pair(tree->HLT_DiPFJetAve140      ,range{155, 210, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve200"     ]  = make_pair(tree->HLT_DiPFJetAve200      ,range{210, 300, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve260"     ]  = make_pair(tree->HLT_DiPFJetAve260      ,range{300, 400, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve320"     ]  = make_pair(tree->HLT_DiPFJetAve320      ,range{400, 500, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve400"     ]  = make_pair(tree->HLT_DiPFJetAve400      ,range{500, 600, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve500"     ]  = make_pair(tree->HLT_DiPFJetAve500      ,range{600, 6500, 0, 5.2});
  trigsName_ValRange["HLT_DiPFJetAve60_HFJEC"]  = make_pair(tree->HLT_DiPFJetAve60_HFJEC ,range{85, 100, fwdeta, 5.2   });
  trigsName_ValRange["HLT_DiPFJetAve80_HFJEC"]  = make_pair(tree->HLT_DiPFJetAve80_HFJEC ,range{100, 125, fwdeta, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve100_HFJEC"] = make_pair(tree->HLT_DiPFJetAve100_HFJEC,range{125, 180, fwdeta, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve160_HFJEC"] = make_pair(tree->HLT_DiPFJetAve160_HFJEC,range{180, 250, fwdeta, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve220_HFJEC"] = make_pair(tree->HLT_DiPFJetAve220_HFJEC,range{250, 350, fwdeta0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve300_HFJEC"] = make_pair(tree->HLT_DiPFJetAve300_HFJEC,range{350, 6500, fwdeta0, 5.2});
  }

  double JetL1rcFactor[tree->nJetMax];
  TLorentzVector p4rawmet, p4t1met, p4mht, p4l1rc, p4dj;
  // TLorentzVector p4, p4s, p4mht, p4mht2, p4mhtc, p4mhtc3, p4t, p4p;
  TLorentzVector p4, /*p4raw,*/ p4g, p4s, p4t, p4p;
  TLorentzVector p4lead, p4recoil; //, p4other;
  TLorentzVector p4leadRES, p4recoilRES;
  TLorentzVector p4b3, p4b3r, p4b3l, p4m;
  TLorentzVector p4b, p4bt, p4bp, p4bx, p4d, p4dx;
  TLorentzVector p4c, p4cx, p4f, p4fx, p4l, p4r;
  TLorentzVector p4m0, p4m2, p4mn, p4mu; //, p4mo;
  TLorentzVector p4m3, p4mn3, p4mu3;
  TLorentzVector p4corrjets, p4rcjets, p4rawjets;


 // Monitor trigger rates
  TH1D *htrg = new TH1D("htrg", "Triggers;Trigger;N_{events}",
                        trigNames.size(), 0, trigNames.size());
  for (int i = 1; i != htrg->GetNbinsX() + 1; ++i)
  {
    htrg->GetXaxis()->SetBinLabel(i, trigNames[i - 1].c_str());
  }

  // Inclusive jets pT binning
  double vpti[] =
      {1, 5, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84,
       97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468,
       507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248,
       1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500,
       2640, 2787, 2941, 3103, 3273, 3450, 3637, 3832, 4037, 4252, 4477, 4713,
       4961, 5220, 5492, 5777, 6076, 6389, 6717, 7000};
  double npti = sizeof(vpti) / sizeof(vpti[0]) - 1;

  // Regular L2Relative eta binning
  double vx[] =
      {-5.191,
       -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489,
       -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043,
       -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218,
       -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435,
       -0.348, -0.261, -0.174, -0.087, 0, 0.087, 0.174, 0.261, 0.348, 0.435,
       0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305,
       1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5,
       2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191,
       4.363, 4.538, 4.716, 4.889, 5.191};
  const int nx = sizeof(vx) / sizeof(vx[0]) - 1;

	TH2D *h2dphi = new TH2D("h2dphi", "#Delta#phi vs #eta;#eta;#Delta#phi",
                          nx, vx, 126, -TMath::TwoPi(), +TMath::TwoPi());

  fout->mkdir("Refs");
  fout->cd("Refs");
  TH1D *hnJet = new TH1D("hnJet", "hnJet", 500, 0, 500);
  fout->cd();

  for (const auto& trigName_ValRange : trigsName_ValRange) {
    string & trigName = trigName_ValRange.first;
    
    if (isDebug) cout << "Trigger " << trigName << endl;
    fout->mkdir(trigName.c_str());
    fout->cd(trigName.c_str());
    TDirectory *dout = gDirectory;

    dout->mkdir("Multijet");
    dout->cd("Multijet");

    MultiJetHistos *h = new MultiJetHistos();

    mapStrHistMultiJet[trigName] = h;
    h->trg = trigName;
    struct range &r = trigName_ValRange.second.second;
    h->trgpt = r.ptmin;//FIXME
    h->ptmin = r.ptmin;
    h->ptmax = r.ptmax;
    h->absetamin = r.absetamin;
    h->absetamax = r.absetamax;

    h->hpta_all = new TH1D("hpta_all", "", npti, vpti);
    h->hpta_sel = new TH1D("hpta_sel", "", npti, vpti);
    h->presa = new TProfile("presa", "", npti, vpti);
    h->ptleada = new TProfile("ptleada", "", npti, vpti);
    h->pcrecoila = new TProfile("pcrecoila", "", npti, vpti);

    h->pm0a = new TProfile("pm0a", "", npti, vpti);
    h->pm2a = new TProfile("pm2a", "", npti, vpti);
    h->pmna = new TProfile("pmna", "", npti, vpti);
    h->pmua = new TProfile("pmua", "", npti, vpti);

    h->hptm_all = new TH1D("hptm_all", "", npti, vpti);
    h->hptm_sel = new TH1D("hptm_sel", "", npti, vpti);
    h->presm = new TProfile("presm", "", npti, vpti);
    h->ptleadm = new TProfile("ptleadm", "", npti, vpti);
    h->pcrecoilm = new TProfile("pcrecoilm", "", npti, vpti);

    h->pm0m = new TProfile("pm0m", "", npti, vpti);
    h->pm2m = new TProfile("pm2m", "", npti, vpti);
    h->pmnm = new TProfile("pmnm", "", npti, vpti);
    h->pmum = new TProfile("pmum", "", npti, vpti);

    h->hptl_all = new TH1D("hptl_all", "", npti, vpti);
    h->hptl_sel = new TH1D("hptl_sel", "", npti, vpti);
    h->presl = new TProfile("presl", "", npti, vpti);
    h->ptleadl = new TProfile("ptleadl", "", npti, vpti);
    h->pcrecoill = new TProfile("pcrecoill", "", npti, vpti);

    h->pm0l = new TProfile("pm0l", "", npti, vpti);
    h->pm2l = new TProfile("pm2l", "", npti, vpti);
    h->pmnl = new TProfile("pmnl", "", npti, vpti);
    h->pmul = new TProfile("pmul", "", npti, vpti);

    h->hptr_all = new TH1D("hptr_all", "", npti, vpti);
    h->hptr_sel = new TH1D("hptr_sel", "", npti, vpti);
    h->presr = new TProfile("presr", "", npti, vpti);
    h->ptleadr = new TProfile("ptleadr", "", npti, vpti);
    h->pcrecoilr = new TProfile("pcrecoilr", "", npti, vpti);

    h->pm0r = new TProfile("pm0r", "", npti, vpti);
    h->pm2r = new TProfile("pm2r", "", npti, vpti);
    h->pmnr = new TProfile("pmnr", "", npti, vpti);
    h->pmur = new TProfile("pmur", "", npti, vpti);

    //doMultiJetControl
    h->h2m0a = new TH2D("h2m0a", "", npti, vpti, 200, -1, 3);
    h->h2m2a = new TH2D("h2m2a", "", npti, vpti, 200, -1, 3);
    h->hcosdphi = new TH1D("hcosdphi", "", 102, -1.01, 1.01);

    //doMultiJet2Drecoil
    dout->mkdir("Multijet/2Drecoil");
    dout->cd("Multijet/2Drecoil");
    h->h2recoila = new TH2D("h2recoila", "", npti, vpti, npti, vpti);
    h->h2recoilm = new TH2D("h2recoilm", "", npti, vpti, npti, vpti);
    h->h2recoill = new TH2D("h2recoill", "", npti, vpti, npti, vpti);
    h->h2recoilr = new TH2D("h2recoilr", "", npti, vpti, npti, vpti);
    
    //doPFComposition
    dout->mkdir("Multijet/PFcomposition");
    dout->cd("Multijet/PFcomposition");

    h->ppt13 = new TProfile("ppt13", ";#eta;p_{T,avp} (GeV);"
                                     "p_{T,lead}",
                            npti, vpti);
    h->prho13 = new TProfile("prho13", ";#eta;p_{T,avp} (GeV);"
                                       "#rho",
                             npti, vpti);
    h->pchf13 = new TProfile("pchf13", ";#eta;p_{T,avp} (GeV);"
                                       "CHF",
                             npti, vpti);
    h->pnhf13 = new TProfile("pnhf13", ";#eta;p_{T,avp} (GeV);"
                                       "NHF",
                             npti, vpti);
    h->pnef13 = new TProfile("pnef13", ";#eta;p_{T,avp} (GeV);"
                                       "NEF",
                             npti, vpti);
    h->pcef13 = new TProfile("pcef13", ";#eta;p_{T,avp} (GeV);"
                                       "CEF",
                             npti, vpti);
    h->pmuf13 = new TProfile("pmuf13", ";#eta;p_{T,avp} (GeV);"
                                       "MUF",
                             npti, vpti);

    h->ppt25 = new TProfile("ppt25", ";#eta;p_{T,avp} (GeV);"
                                     "p_{T,recoil}",
                            npti, vpti);
    h->prho25 = new TProfile("prho25", ";#eta;p_{T,avp} (GeV);"
                                       "#rho",
                             npti, vpti);
    h->pchf25 = new TProfile("pchf25", ";#eta;p_{T,avp} (GeV);"
                                       "CHF",
                             npti, vpti);
    h->pnhf25 = new TProfile("pnhf25", ";#eta;p_{T,avp} (GeV);"
                                       "NHF",
                             npti, vpti);
    h->pnef25 = new TProfile("pnef25", ";#eta;p_{T,avp} (GeV);"
                                       "NEF",
                             npti, vpti);
    h->pcef25 = new TProfile("pcef25", ";#eta;p_{T,avp} (GeV);"
                                       "CEF",
                             npti, vpti);
    h->pmuf25 = new TProfile("pmuf25", ";#eta;p_{T,avp} (GeV);"
                                       "MUF",
                             npti, vpti);
  } // for itrg
 
  //------------------------------------
  // Event for loop 
  //------------------------------------
  TLorentzVector p4Probe, p4GenProbe, p4Jet1, p4Jet2, p4Jetn;
  TLorentzVector p4Met, p4Met1, p4Metn, p4Metu, p4Metnu, p4RawMet, p4CorrMet, p4RawProbe;
  TLorentzVector p4Jeti, p4CorrJets, p4RawJet, p4RawJets, p4RcJets, p4RcOffsets;
  TLorentzVector p4GenJeti, p4GenJet1, p4GenJet2;
  TLorentzVector p4Probex; // for MPFX
   
  Long64_t nentries = tree->GetEntries(); 
  cout << "\nStarting loop over " << nentries << " entries" << endl;
 
  std::cout<<"---------------------------"<<std::endl;
  std::cout<<setw(10)<<"Progress"<<setw(10)<<"Time"<<std::endl;
  std::cout<<"---------------------------"<<std::endl;
  double totTime = 0.0;
	auto startClock = std::chrono::high_resolution_clock::now();
 
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //if (jentry>100) break;
		if(nentries > 100  && jentry%(nentries/100) == 0){// print after every 1% of events
      totTime+= std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-startClock).count();
      int sec = (int)(totTime)%60;
      int min = (int)(totTime)/60;
	    std::cout<<setw(10)<<100*jentry/nentries<<" %"<<setw(10)<<min<<"m "<<sec<<"s"<<std::endl;
			startClock = std::chrono::high_resolution_clock::now();
		}
   
    int count_passedCut = 0;
    Long64_t ientry = tree->loadEntry(jentry);
    if (ientry < 0) break; 
    tree->fChain->GetTree()->GetEntry(ientry);
    //if(isDebug) cout<<"Loaded jentry = "<<jentry<<endl;

    // Weight
    double weight = (isMC ? tree->genWeight : 1);
    //------------------------------------
    // trigger and golden lumi, MET filter selection 
    //------------------------------------
    //if(!eventP->passHLT(tree)) continue; 
    //if(isDebug) cout<<"passHLT"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    bool passGoodLumi = true; 
    if(isData){
      passGoodLumi = objS->checkGoodLumi(tree->run, tree->luminosityBlock);
    }
    if(!passGoodLumi) continue; 
    if(isDebug) cout<<"passLumi"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    //if(!eventP->passFilter(tree)) continue; 
    bool pass_METfilter = eventP->passFilter(tree); 
    if(isDebug) cout<<"passMetFilter"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    //------------------------------------------------
    // Jet loop: Apply JEC
    //------------------------------------------------
    // Select leading jets. Just exclude Probe, don't apply JetID yet
    bool allJetsGood(true);
    Float_t     JetResFactor[tree->nJetMax]; // Custom addition
    Float_t     JetDeltaJES[tree->nJetMax]; // Custom addition
    Float_t     JetCF[tree->nJetMax]; // Custom addition
    int iJet(-1), iJet2(-1), nJets(0);
    double djes(1), jes(1), res(1);
    p4Jet1.SetPtEtaPhiM(0,0,0,0);
    p4Jet2.SetPtEtaPhiM(0,0,0,0);
    p4Jetn.SetPtEtaPhiM(0,0,0,0);
    // Also calculate corrected type-I chsMET and HDM inputs
    p4CorrJets.SetPtEtaPhiM(0,0,0,0);
    p4RawJets.SetPtEtaPhiM(0,0,0,0);
    p4RcJets.SetPtEtaPhiM(0,0,0,0);
    p4RcOffsets.SetPtEtaPhiM(0,0,0,0);

    for (int i = 0; i != tree->nJet; ++i) {
      double rawJetPt = tree->Jet_pt[i] * (1.0 - tree->Jet_rawFactor[i]);
      double rawJetMass = tree->Jet_mass[i] * (1.0 - tree->Jet_rawFactor[i]);
      double corrs = 1.0;
      if(isDebug)cout<<"---: Jet correction :---"<<endl;
      for(auto l2l3Ref:objS->loadedJetL2L3Refs){
        try{ 
          //cout<<"rawJetPt = "<<rawJetPt<<", corr = "<<tree->Jet_eta[i]<<endl;
          //auto corr = l2l3Ref->evaluate({tree->Jet_eta[i],rawJetPt}); 
          double corr = 1.0;
          corrs  = corr*corrs;
          rawJetPt = corrs*rawJetPt;
          rawJetMass = corrs*rawJetMass;
          if(isDebug) cout<<"rawJetPt = "<<rawJetPt<<", corr = "<<corr<<", corrs = "<<corrs<<endl;
        } catch (const std::exception& e) {
          cout<<"\nEXCEPTION: in l2l3Ref: "<<e.what()<<endl;
          std::abort();
        }
      }
      //double res = (v.size()>1 ? v[v.size()-1]/v[v.size()-2] : 1.);
      //JetResFactor[i] = 1./res;
      res  = 1.0;
      JetDeltaJES[i] = (1./corrs) / (1.0 - tree->Jet_rawFactor[i]);
      tree->Jet_pt[i] = rawJetPt ;
      tree->Jet_mass[i] = rawJetMass; 
      tree->Jet_rawFactor[i] = (1.0 - 1.0/corrs);
      JetResFactor[i] = (1.0 - 1.0/res);
      // Smear jets
      if (smearJets) {
        //assert(false);
      }
     
      // Check that jet is not Probe and pTcorr>15 GeV
      bool pass_JetId = tree->Jet_jetId[i] & (1 << 2); // tightLepVeto
      if (tree->Jet_pt[i]>15 && pass_JetId) {
        p4Jeti.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);
        if (p4Probe.DeltaR(p4Jeti)<0.2) continue; // should not happen, but does?
        ++nJets;
      
        if (iJet==-1) { // Leading jet for balance
          iJet = i;
          p4Jet1 = p4Jeti;
          djes = JetDeltaJES[i];
          jes = (1.-tree->Jet_rawFactor[i]);
          res = (1.-JetResFactor[i]);
        }
        else { // Subleading jets 
          p4Jetn += p4Jeti;
          if (iJet2==-1) { // First subleading jet for alpha
          iJet2 = i;
          p4Jet2 = p4Jeti;
          }
        }
        // Calculate L1RC correction
        p4RawJet = (1-tree->Jet_rawFactor[i]) * p4Jeti;
        double corrl1rc(1.); // tree->isRun3
        p4RcJets = corrl1rc * p4RawJet;
        
        // Corrected type-I chsMET calculation
        p4CorrJets += p4Jeti;
        p4RawJets += p4RawJet;
        p4RcJets += p4RcJets;
        p4RcOffsets += (p4RawJet - p4RcJets);
      } // non-Probe jet
    } // for i in nJet

    for (int i = 0; i != tree->nJet; ++i){//FIXME
      JetCF[i] = 1.;
    } // reset JetCF
    
    if(nJets < 1) continue; 
    if(isDebug) cout<<"passAtleast1Jet"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    //------------------------------------------------
    //Event to be vetoed if leading jet falls in veto region
    //------------------------------------------------
    try{ 
      auto jvNumber= objS->loadedJetVetoRef->evaluate({objS->jetVetoKey, p4Jet1.Eta(), p4Jet1.Phi()});
    if(jvNumber>0) continue; 
    } catch (const std::exception& e) {
      cout<<"\nEXCEPTION: in objS->loadedJetVetoRef: "<<e.what()<<endl;
      std::abort();
    }
    if(isDebug) cout<<"passJetVetoMap"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);
     
    p4rawmet.SetPtEtaPhiM(tree->RawPuppiMET_pt, 0, tree->RawPuppiMET_phi, 0);
    p4t1met.SetPtEtaPhiM(tree->RawPuppiMET_pt, 0, tree->RawPuppiMET_phi, 0);
    p4m0.SetPtEtaPhiM(tree->RawPuppiMET_pt, 0, tree->RawPuppiMET_phi, 0);
    
    // Reset dijet vectors
    p4m2.SetPtEtaPhiM(0, 0, 0, 0);
    p4mn.SetPtEtaPhiM(0, 0, 0, 0);
    p4mu.SetPtEtaPhiM(0, 0, 0, 0);
    
    // Reset multiJet vectors
    // bool isMultiJet = (nJet>=3); // multiJet pre-setting
    p4lead.SetPtEtaPhiM(0, 0, 0, 0);
    p4recoil.SetPtEtaPhiM(0, 0, 0, 0);
    p4m3.SetPtEtaPhiM(0, 0, 0, 0);
    p4mn3.SetPtEtaPhiM(0, 0, 0, 0);
    p4mu3.SetPtEtaPhiM(0, 0, 0, 0);
    p4leadRES.SetPtEtaPhiM(0, 0, 0, 0);
    p4recoilRES.SetPtEtaPhiM(0, 0, 0, 0);
    int nlead(0);
    int nrecoil(0);
    bool multiJet_vetonear(false);
    bool multiJet_vetofwd(false);
    
    for (int i = 0; i != tree->nJet; ++i){
      // p4 is fully corrected and smeared
      p4.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);
      // p4raw.SetPtEtaPhiM(tree->Jet_pt[i]*(1.0-tree->Jet_rawFactor[i]), tree->Jet_eta[i],
      //		   tree->Jet_phi[i], tree->Jet_mass[i]*(1.0-tree->Jet_rawFactor[i]));
      // p4l1rc.SetPtEtaPhiM(tree->Jet_pt[i]*(1.0-JetL1rcFactor[i]), tree->Jet_eta[i],
      //		    tree->Jet_phi[i], tree->Jet_mass[i]*(1.0-JetL1rcFactor[i]));
      //  p4l1rc is before corrections and smearing
      p4l1rc.SetPtEtaPhiM(tree->Jet_pt[i] / (smearJets && JetCF[i] ? JetCF[i] : 1) * (1.0 - JetL1rcFactor[i]),
                          tree->Jet_eta[i], tree->Jet_phi[i],
                          tree->Jet_mass[i] / (smearJets && JetCF[i] ? JetCF[i] : 1) * (1.0 - JetL1rcFactor[i]));
      
      
      // Calculate type-I MET (L1L2L3-RC) and MHT
      if (p4.Pt() > 15.)
      {
        p4mht -= p4;
        p4t1met += p4l1rc - p4; // same as (+raw-rcoff) -corr,
        p4m0 += p4l1rc - p4;    // same as (+raw-rcoff) -corr
      }
      
      // L2Res HDM (dijet)
      if (i < 2 && p4.Pt() > 15.)
      { // two leading jets
        p4m2 -= p4;
      }
      else if (p4.Pt() > 15.)
      {
        p4mn -= p4;
      }
      
      // L3Res HDM (multiJet)
      if (i == 0 && p4.Pt() > 30.)
      { // leading jet
        p4lead += p4;
        p4m3 -= p4;
        p4leadRES += JetResFactor[i] * p4;
        ++nlead;
      }
      else if (i > 0 && p4.Pt() > 30. && fabs(p4.Eta()) < 2.5 &&
               p4.DeltaPhi(p4lead) > 1.0)
      { // recoil jets
        p4recoil += p4;
        p4m3 -= p4;
        p4recoilRES += JetResFactor[i] * p4;
        ++nrecoil;
      }
      else if (p4.Pt() > 15.)
      { // all other jets
        p4mn3 -= p4;
      }
      
      // Veto nearby jets for multiJet topology
      if (i > 0 && p4.Pt() > 30. && fabs(p4.Eta()) < 2.5 &&
          p4.DeltaPhi(p4lead) <= 1.0)
        multiJet_vetonear = true;
      
      // Veto forward jets for multiJet topology
      if (i > 0 && p4.Pt() > 30. && fabs(p4.Eta()) >= 2.5)
        multiJet_vetofwd = true;
      
    } // for i in tree->nJet
    if(isDebug) cout<<"passed nJet for loop"<<endl;

    // Calculate unclustered MET from the remainders
    // met = -j2 -jn -ju = m2 + mn + mu => mu = met -m2 -mn
    p4mu = p4m0 - p4m2 - p4mn;
    p4mu3 = p4m0 - p4m3 - p4mn3;
    
    // Also check recoil phi for multiJet selection
    double ptrecoil = p4recoil.Pt();
    double dphirecoil = p4.DeltaPhi(p4recoil);
    // Use tightLepVeto for JetID
    bool isMultiJet =
        (nlead == 1 && nrecoil >= 2 && !multiJet_vetonear && !multiJet_vetofwd &&
         fabs(dphirecoil - TMath::Pi()) < 0.3 && pass_METfilter > 0 &&
         tree->Jet_pt[0] > 30. && fabs(tree->Jet_eta[0]) < 1.3 && tree->Jet_jetId[0] >= 4 &&
         tree->Jet_pt[1] > 30. && fabs(tree->Jet_eta[1]) < 2.5 && tree->Jet_jetId[1] >= 4 &&
         tree->Jet_pt[2] > 30. && fabs(tree->Jet_eta[2]) < 2.5 && tree->Jet_jetId[2] >= 4 &&
         //!tree->Jet_jetveto[0] && !tree->Jet_jetveto[1] && !tree->Jet_jetveto[2] &&
         tree->Jet_pt[1] < 0.6 * ptrecoil && tree->Jet_pt[2] < 0.6 * ptrecoil);
    
    // Calculate Crecoil
    double logCrecoil(0);
    double ptavp3(0);
    // Proper bisector axis (equal angles to each jet)
    p4b3.SetPtEtaPhiM(0, 0, 0, 0);
    p4b3r.SetPtEtaPhiM(1, 0, p4recoil.Phi(), 0);
    p4b3l.SetPtEtaPhiM(1, 0, p4lead.Phi(), 0);
    p4b3 += p4b3r;
    p4b3 -= p4b3l;
    p4b3.SetPtEtaPhiM(p4b3.Pt(), 0., p4b3.Phi(), 0.);
    p4b3 *= 1. / p4b3.Pt();
    
    // Average projection pT to bisector axis, pT,avp
    // as explained in JME-21-001 (HDM method: bisector extension)
    ptavp3 = 0.5 * (p4recoil.Vect().Dot(p4b3.Vect()) -
                    p4lead.Vect().Dot(p4b3.Vect()));
    
    double ptlead = p4lead.Pt();
    double ptave = 0.5 * (ptlead + ptrecoil);
    for (int i = 0; i != tree->nJet; ++i){
      // Crecoil = exp(sum_i F_i log(f_i)), where
      // f_i = pT,i / pTrecoil, F_i = f_i cos(Delta phi(i,recoil))
      // To do this before calculating pTrecoil, we could do
      // sum_i pT,i * cos(Delta phi(i,-lead))* log(pT,i / pT,lead)
      // which should for practical purposes be the same
      // Maybe safer here as originally defined, just careful with selection
    
      // Make sure selection here matches the one above for p4recoil
      p4.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);
      if (i > 0 && p4.Pt() > 30. && fabs(p4.Eta()) < 2.5 && p4.DeltaPhi(p4lead) > 1.0){
        double pti = p4.Pt();
        double fi = pti / ptrecoil;
        double Fi = fi * cos(p4.DeltaPhi(p4recoil));
        logCrecoil += Fi * log(fi);
    
        //doMultiJet2Drecoili
        for (const auto& trigName_ValRange : trigsName_ValRange) {
          if (!(*trigName_ValRange.second.first)) continue;
          MultiJetHistos *h = mapStrHistMultiJet[trigName_ValRange.first];
    
          // Assumption is that sum_i F_i = 1, but should check?
          h->h2recoila->Fill(ptavp3, pti, weight * Fi);
          h->h2recoilm->Fill(ptave, pti, weight * Fi);
          h->h2recoill->Fill(ptlead, pti, weight * Fi);
          h->h2recoilr->Fill(ptrecoil, pti, weight * Fi);
    
          //doPFComposition
          h->ppt25->Fill(ptavp3, tree->Jet_pt[i], weight * Fi);
          h->prho25->Fill(ptavp3, tree->Rho, weight * Fi);
          h->pchf25->Fill(ptavp3, tree->Jet_chHEF[i], weight * Fi);
          h->pnhf25->Fill(ptavp3, tree->Jet_neHEF[i], weight * Fi);
          h->pnef25->Fill(ptavp3, tree->Jet_neEmEF[i], weight * Fi);
          h->pcef25->Fill(ptavp3, tree->Jet_chEmEF[i], weight * Fi);
          h->pmuf25->Fill(ptavp3, tree->Jet_muEF[i], weight * Fi);
        } // for itrg
      } // good recoil jet
    } // for i in inJet
    double Crecoil = exp(logCrecoil);

    hnJet->Fill(tree->nJet, weight);
    
    
    // Multijet selection
    if (isDebug) cout << "Analyze multiJet" << endl << flush;
    
    // pTave binning
    double ptlead = p4lead.Pt();
    double ptrecoil = p4recoil.Pt();
    double ptave = 0.5 * (ptlead + ptrecoil);
    // double ptavp3 defined earlier, as is p4b3
    
    // Projection to transverse plane (is this necessary?)
    p4m0.SetPtEtaPhiM(p4m0.Pt(), 0., p4m0.Phi(), 0.);
    p4m3.SetPtEtaPhiM(p4m3.Pt(), 0., p4m3.Phi(), 0.);
    p4mn3.SetPtEtaPhiM(p4mn3.Pt(), 0., p4mn3.Phi(), 0.);
    
    // Bisector axis p4b3 defined earlier (equal angles)
    double m0b = 1 + (p4m0.Vect().Dot(p4b3.Vect())) / ptave;
    double m3b = 1 + (p4m3.Vect().Dot(p4b3.Vect())) / ptave;
    double mnb = 0 + (p4mn3.Vect().Dot(p4b3.Vect())) / ptave;
    double mub = 0 + (p4mu3.Vect().Dot(p4b3.Vect())) / ptave;
    
    // Dijet axis (not equal angles)
    p4m.SetPtEtaPhiM(0, 0, 0, 0);
    p4m -= p4lead;
    p4m += p4recoil;
    p4m.SetPtEtaPhiM(p4m.Pt(), 0., p4m.Phi(), 0.);
    p4m *= 1. / p4m.Pt();
    
    double m0m = 1 + (p4m0.Vect().Dot(p4m.Vect())) / ptave;
    double m3m = 1 + (p4m3.Vect().Dot(p4m.Vect())) / ptave;
    double mnm = 0 + (p4mn3.Vect().Dot(p4m.Vect())) / ptave;
    double mum = 0 + (p4mu3.Vect().Dot(p4m.Vect())) / ptave;
    
    p4l.SetPtEtaPhiM(0, 0, 0, 0);
    p4l -= p4lead;
    p4l.SetPtEtaPhiM(p4l.Pt(), 0., p4l.Phi(), 0.);
    p4l *= 1. / p4l.Pt();
    
    double m0l = 1 + (p4m0.Vect().Dot(p4l.Vect())) / ptlead;
    double m3l = 1 + (p4m3.Vect().Dot(p4l.Vect())) / ptlead;
    double mnl = 0 + (p4mn3.Vect().Dot(p4l.Vect())) / ptlead;
    double mul = 0 + (p4mu3.Vect().Dot(p4l.Vect())) / ptlead;
    
    p4r.SetPtEtaPhiM(0, 0, 0, 0);
    p4r += p4recoil;
    p4r.SetPtEtaPhiM(p4r.Pt(), 0., p4r.Phi(), 0.);
    p4r *= 1. / p4r.Pt();
    
    double m0r = 1 + (p4m0.Vect().Dot(p4r.Vect())) / ptrecoil;
    double m3r = 1 + (p4m3.Vect().Dot(p4r.Vect())) / ptrecoil;
    double mnr = 0 + (p4mn3.Vect().Dot(p4r.Vect())) / ptrecoil;
    double mur = 0 + (p4mu3.Vect().Dot(p4r.Vect())) / ptrecoil;
    
    for (const auto& trigName_ValRange : trigsName_ValRange) {
      if (!(*trigName_ValRange.second.first)) continue;
      MultiJetHistos *h = mapStrHistMultiJet[trigName_ValRange.first];
    
      h->hpta_all->Fill(ptavp3, weight);
      h->hptm_all->Fill(ptave, weight);
      h->hptl_all->Fill(ptlead, weight);
      h->hptr_all->Fill(ptrecoil, weight);
    
      if (ptavp3 >= h->ptmin && ptavp3 < h->ptmax)
        h->hpta_sel->Fill(ptavp3, weight);
      if (ptave >= h->ptmin && ptave < h->ptmax)
        h->hptm_sel->Fill(ptave, weight);
      if (ptlead >= h->ptmin && ptlead < h->ptmax)
        h->hptl_sel->Fill(ptlead, weight);
      if (ptrecoil >= h->ptmin && ptrecoil < h->ptmax)
        h->hptr_sel->Fill(ptrecoil, weight);
    
      double res = (p4leadRES.Pt() / p4recoilRES.Pt()) /
                   (p4lead.Pt() / p4recoil.Pt());
      h->presa->Fill(ptavp3, res, weight);
      h->presm->Fill(ptave, res, weight);
      h->presl->Fill(ptlead, res, weight);
      h->presr->Fill(ptrecoil, res, weight);
    
      h->ptleada->Fill(ptavp3, ptlead, weight);
      h->ptleadm->Fill(ptave, ptlead, weight);
      h->ptleadl->Fill(ptlead, ptlead, weight);
      h->ptleadr->Fill(ptrecoil, ptlead, weight);
    
      h->pcrecoila->Fill(ptavp3, Crecoil, weight);
      h->pcrecoilm->Fill(ptave, Crecoil, weight);
      h->pcrecoill->Fill(ptlead, Crecoil, weight);
      h->pcrecoilr->Fill(ptrecoil, Crecoil, weight);
    
      h->pm0a->Fill(ptavp3, m0b, weight);
      h->pm2a->Fill(ptavp3, m3b, weight);
      h->pmna->Fill(ptavp3, mnb, weight);
      h->pmua->Fill(ptavp3, mub, weight);
    
      h->pm0m->Fill(ptave, m0m, weight);
      h->pm2m->Fill(ptave, m3m, weight);
      h->pmnm->Fill(ptave, mnm, weight);
      h->pmum->Fill(ptave, mum, weight);
    
      h->pm0l->Fill(ptlead, m0l, weight);
      h->pm2l->Fill(ptlead, m3l, weight);
      h->pmnl->Fill(ptlead, mnl, weight);
      h->pmul->Fill(ptlead, mul, weight);
    
      h->pm0r->Fill(ptrecoil, m0r, weight);
      h->pm2r->Fill(ptrecoil, m3r, weight);
      h->pmnr->Fill(ptrecoil, mnr, weight);
      h->pmur->Fill(ptrecoil, mur, weight);
    
      //doMultiJetControl
      h->h2m0a->Fill(ptavp3, m0b, weight);
      h->h2m2a->Fill(ptavp3, m3b, weight);
      if (ptave > 1.25 * h->trgpt)
        h->hcosdphi->Fill(cos(p4lead.DeltaPhi(p4recoil)), weight);
    
      //doPFComposition)
      h->ppt13->Fill(ptavp3, tree->Jet_pt[0], weight);
      h->prho13->Fill(ptavp3, tree->Rho, weight);
      h->pchf13->Fill(ptavp3, tree->Jet_chHEF[0], weight);
      h->pnhf13->Fill(ptavp3, tree->Jet_neHEF[0], weight);
      h->pnef13->Fill(ptavp3, tree->Jet_neEmEF[0], weight);
      h->pcef13->Fill(ptavp3, tree->Jet_chEmEF[0], weight);
      h->pmuf13->Fill(ptavp3, tree->Jet_muEF[0], weight);
    } // for itrg
  }//event for loop

  fout->Write();
  cout<<"Output file: "<<fout->GetName()<<endl;
  fout->Close();
  return 0;
}
   
