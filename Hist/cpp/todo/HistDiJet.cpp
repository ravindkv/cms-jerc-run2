#include "HistDiJet.h"

class DiJetHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;

  //2D hist in ALL probe eta
  TH2D *h2AllProbEtaAvgBisectorPt;
  TH2D *h2AllProbEtaAvgPt;
  TH2D *h2AllProbEtaTagPt;
  TH2D *h2AllProbEtaProbePt;
  
  //2D hist in Selected probe eta
  TH2D *h2SelProbEtaAvgBisectorPt;
  TH2D *h2SelProbEtaAvgPt;
  TH2D *h2SelProbEtaTagPt;
  TH2D *h2SelProbEtaProbePt;
  
  //JEC L2L3Res for undoing
  TProfile2D *p2ResidualInProbEtaAvgBisectorPt;
  TProfile2D *p2ResidualInProbEtaAvgPt; 
  TProfile2D *p2ResidualInProbEtaTagPt;
  TProfile2D *p2ResidualInProbeEtaProbePt; 

  // JER MPFX, DBX methods
  TProfile2D *p2M0bInProbeEtaAvgBisectorPt;
  TProfile2D *p2M0bxInProbeEtaAvgBisectorPt;
  TProfile2D *p2M2bInProbeEtaAvgBisectorPt;
  TProfile2D *p2M2bxInProbeEtaAvgBisectorPt;           

  // pT,avp (bisector)
  TProfile2D *p2M0aInProbeEtaAvgBisectorPt;
  TProfile2D *p2M2aInProbeEtaAvgBisectorPt;
  TProfile2D *p2MnaInProbeEtaAvgBisectorPt;
  TProfile2D *p2MuaInProbeEtaAvgBisectorPt;     
  
  // pT,ave (dijet axis)
  TProfile2D *p2M0dInProbeEtaAvgPt;
  TProfile2D *p2M2dInProbeEtaAvgPt;
  TProfile2D *p2M2nInProbeEtaAvgPt;
  TProfile2D *p2M2uInProbeEtaAvgPt;     

  // pT,tag (central)
  TProfile2D *p2M0cInProbeEtaTagPt;
  TProfile2D *p2M2cInProbeEtaTagPt;
  TProfile2D *p2MncInProbeEtaTagPt;
  TProfile2D *p2MucInProbeEtaTagPt;     
  
  // pt,probe (forward)
  TProfile2D *p2M0fInProbeEtaProbePt;
  TProfile2D *p2M2fInProbeEtaProbePt;
  TProfile2D *p2MnfInProbeEtaProbePt;
  TProfile2D *p2MufInProbeEtaProbePt;     

  // (Optional) composition plots
  // probe,avp
  TProfile2D *p2ProbePtInProbeEtaAvgBisectorPt;
  TProfile2D *p2RhoInProbeEtaAvgBisectorPt;
  TProfile2D *p2ChfInProbeEtaAvgBisectorPt;
  TProfile2D *p2NefInProbeEtaAvgBisectorPt;
  TProfile2D *p2NhfInProbeEtaAvgBisectorPt;
  TProfile2D *p2CefInProbeEtaAvgBisectorPt; 
  TProfile2D *p2MufInProbeEtaAvgBisectorPt;      
  // tag
  TProfile *ppt13;
  TProfile *prho13;
  TProfile *pchf13;
  TProfile *pnef13;
  TProfile *pnhf13;
  TProfile *pcef13; 
  TProfile *pmuf13; 
};


int HistDiJet::Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout){
   
  map<string, DiJetHistos *> mapStrHistDiJet;

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

  const double vpt[] = {10, 15, 20, 25, 30, 35,
                        40, 50, 60, 70, 85, 100, 125, 155, 180, 210, 250, 300,
                        350, 400, 500, 600, 800, 1000, 1200, 1500,
                        1800, 2100, 2400, 2700, 3000, 3500, 4000, 5000};
  const int npt = sizeof(vpt) / sizeof(vpt[0]) - 1;

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
  // Current L2Res |eta| binning from Jindrich
  // https://indico.cern.ch/event/1263476/contributions/5311425/attachments/2612023/4513129/L2Res+HDM-March15.pdf
  double vxd[] =
      //{0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
      // Newer L2Res |eta| binning from Mikel
      // https://indico.cern.ch/event/1335203/#7-update-on-l2res-for-2022-rer
      {0., 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191};
  const int nxd = sizeof(vxd) / sizeof(vxd[0]) - 1;


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
    dout->mkdir("Dijet");
    dout->cd("Dijet");

    DiJetHistos *h = new DiJetHistos();

    string &t = trigNames[itrg];
    mapStrHistDiJet[t] = h;
    h->trg = t;
    h->trgpt = trgpt;

    struct range &r = trigRanges[t];
    h->ptmin = r.ptmin;
    h->ptmax = r.ptmax;
    h->absetamin = r.absetamin;
    h->absetamax = r.absetamax;

    // Counting of events, and JEC L2L3Res for undoing
    h->h2AllProbEtaAvgBisectorPt = new TH2D("h2AllProbEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                                 "N_{events}",
                                nx, vx, npt, vpt);
    h->h2SelProbEtaAvgBisectorPt = new TH2D("h2SelProbEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                                 "N_{events}",
                                nx, vx, npt, vpt);
    h->p2ResidualInProbEtaAvgBisectorPt = new TProfile2D("p2ResidualInProbEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                           "JES(probe)/JES(tag)",
                                nx, vx, npt, vpt);

    // MPF decomposition for HDM method
    h->p2M0aInProbeEtaAvgBisectorPt = new TProfile2D("p2M0aInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);MPF0",
                               nx, vx, npt, vpt);
    h->p2M2aInProbeEtaAvgBisectorPt = new TProfile2D("p2M2aInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);MPF2",
                               nx, vx, npt, vpt);
    h->p2MnaInProbeEtaAvgBisectorPt = new TProfile2D("p2MnaInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);MPFn",
                               nx, vx, npt, vpt);
    h->p2MuaInProbeEtaAvgBisectorPt = new TProfile2D("p2MuaInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);MPFu",
                               nx, vx, npt, vpt);

    // Variants with different binnings and with error on the mean
    h->h2AllProbEtaAvgPt = new TH2D("h2AllProbEtaAvgPt", ";#eta;p_{T,ave} (GeV);"
                                                 "N_{events}",
                                nx, vx, npt, vpt);
    h->h2SelProbEtaAvgPt = new TH2D("h2SelProbEtaAvgPt", ";#eta;p_{T,ave} (GeV);"
                                                 "N_{events}",
                                nx, vx, npt, vpt);
    h->p2ResidualInProbEtaAvgPt = new TProfile2D("p2ResidualInProbEtaAvgPt", ";#eta;p_{T,ave} (GeV);"
                                           "JES(probe)/JES(tag)",
                                nx, vx, npt, vpt);

    // MPF decomposition for HDM method
    h->p2M0dInProbeEtaAvgPt = new TProfile2D("p2M0dInProbeEtaAvgPt", ";#eta;p_{T,ave} (GeV);MPF0",
                               nx, vx, npt, vpt);
    h->p2M2dInProbeEtaAvgPt = new TProfile2D("p2M2dInProbeEtaAvgPt", ";#eta;p_{T,ave} (GeV);MPF2",
                               nx, vx, npt, vpt);
    h->p2M2nInProbeEtaAvgPt = new TProfile2D("p2M2nInProbeEtaAvgPt", ";#eta;p_{T,ave} (GeV);MPFn",
                               nx, vx, npt, vpt);
    h->p2M2uInProbeEtaAvgPt = new TProfile2D("p2M2uInProbeEtaAvgPt", ";#eta;p_{T,ave} (GeV);MPFu",
                               nx, vx, npt, vpt);

    h->h2AllProbEtaTagPt = new TH2D("h2AllProbEtaTagPt", ";#eta;p_{T,ave} (GeV);"
                                                 "N_{events}",
                                nx, vx, npt, vpt);
    h->h2SelProbEtaTagPt = new TH2D("h2SelProbEtaTagPt", ";#eta;p_{T,ave} (GeV);"
                                                 "N_{events}",
                                nx, vx, npt, vpt);
    h->p2ResidualInProbEtaTagPt = new TProfile2D("p2ResidualInProbEtaTagPt", ";#eta;p_{T,ave} (GeV);"
                                           "JES(probe)/JES(tag)",
                                nx, vx, npt, vpt);

    h->p2M0cInProbeEtaTagPt = new TProfile2D("p2M0cInProbeEtaTagPt", ";#eta;p_{T,tag} (GeV);MPF0",
                               nx, vx, npt, vpt);
    h->p2M2cInProbeEtaTagPt = new TProfile2D("p2M2cInProbeEtaTagPt", ";#eta;p_{T,tag} (GeV);MPF2",
                               nx, vx, npt, vpt);
    h->p2MncInProbeEtaTagPt = new TProfile2D("p2MncInProbeEtaTagPt", ";#eta;p_{T,tag} (GeV);MPFn",
                               nx, vx, npt, vpt);
    h->p2MucInProbeEtaTagPt = new TProfile2D("p2MucInProbeEtaTagPt", ";#eta;p_{T,tag} (GeV);MPFu",
                               nx, vx, npt, vpt);

    h->h2AllProbEtaProbePt = new TH2D("h2AllProbEtaProbePt", ";#eta;p_{T,ave} (GeV);"
                                                 "N_{events}",
                                nx, vx, npt, vpt);
    h->h2SelProbEtaProbePt = new TH2D("h2SelProbEtaProbePt", ";#eta;p_{T,ave} (GeV);"
                                                 "N_{events}",
                                nx, vx, npt, vpt);
    h->p2ResidualInProbeEtaProbePt = new TProfile2D("p2ResidualInProbeEtaProbePt", ";#eta;p_{T,ave} (GeV);"
                                           "JES(probe)/JES(tag)",
                                nx, vx, npt, vpt);

    h->p2M0fInProbeEtaProbePt = new TProfile2D("p2M0fInProbeEtaProbePt", ";#eta;p_{T,probe} (GeV);MPF0",
                               nx, vx, npt, vpt);
    h->p2M2fInProbeEtaProbePt = new TProfile2D("p2M2fInProbeEtaProbePt", ";#eta;p_{T,probe} (GeV);MPF2",
                               nx, vx, npt, vpt);
    h->p2MnfInProbeEtaProbePt = new TProfile2D("p2MnfInProbeEtaProbePt", ";#eta;p_{T,probe} (GeV);MPFn",
                               nx, vx, npt, vpt);
    h->p2MufInProbeEtaProbePt = new TProfile2D("p2MufInProbeEtaProbePt", ";#eta;p_{T,probe} (GeV);MPFu",
                               nx, vx, npt, vpt);

    //doDiJetJER)
    dout->mkdir("Dijet/JER");
    dout->cd("Dijet/JER");

    // Basic profiles with RMS as error ("S") for JER studies
    h->p2M0bInProbeEtaAvgBisectorPt = new TProfile2D("p2M0bInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                     "MPF0 (MPF)",
                             nx, vx, npt, vpt, "S");
    h->p2M0bxInProbeEtaAvgBisectorPt = new TProfile2D("p2M0bxInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                       "MPFX0 (MPFX)",
                              nx, vx, npt, vpt, "S");
    h->p2M2bInProbeEtaAvgBisectorPt = new TProfile2D("p2M2bInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                     "MPF2 (DB)",
                             nx, vx, npt, vpt, "S");
    h->p2M2bxInProbeEtaAvgBisectorPt = new TProfile2D("p2M2bxInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                       "MPF2 (DBX)",
                              nx, vx, npt, vpt, "S");
    
    //doPFComposition
    dout->mkdir("Dijet/PFcomposition");
    dout->cd("Dijet/PFcomposition");

    h->p2ProbePtInProbeEtaAvgBisectorPt = new TProfile2D("p2ProbePtInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                     "p_{T,probe}",
                             nx, vx, npt, vpt);
    h->p2RhoInProbeEtaAvgBisectorPt = new TProfile2D("p2RhoInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                       "#rho",
                              nx, vx, npt, vpt);
    h->p2ChfInProbeEtaAvgBisectorPt = new TProfile2D("p2ChfInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                       "CHF",
                              nx, vx, npt, vpt);
    h->p2NhfInProbeEtaAvgBisectorPt = new TProfile2D("p2NhfInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                       "NHF",
                              nx, vx, npt, vpt);
    h->p2NefInProbeEtaAvgBisectorPt = new TProfile2D("p2NefInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                       "NEF",
                              nx, vx, npt, vpt);
    h->p2CefInProbeEtaAvgBisectorPt = new TProfile2D("p2CefInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                       "CEF",
                              nx, vx, npt, vpt);
    h->p2MufInProbeEtaAvgBisectorPt = new TProfile2D("p2MufInProbeEtaAvgBisectorPt", ";#eta;p_{T,avp} (GeV);"
                                       "MUF",
                              nx, vx, npt, vpt);

    h->ppt13 = new TProfile("ppt13", ";#eta;p_{T,avp} (GeV);"
                                     "p_{T,tag}",
                            npt, vpt);
    h->prho13 = new TProfile("prho13", ";#eta;p_{T,avp} (GeV);"
                                       "#rho",
                             npt, vpt);
    h->pchf13 = new TProfile("pchf13", ";#eta;p_{T,avp} (GeV);"
                                       "CHF",
                             npt, vpt);
    h->pnhf13 = new TProfile("pnhf13", ";#eta;p_{T,avp} (GeV);"
                                       "NHF",
                             npt, vpt);
    h->pnef13 = new TProfile("pnef13", ";#eta;p_{T,avp} (GeV);"
                                       "NEF",
                             npt, vpt);
    h->pcef13 = new TProfile("pcef13", ";#eta;p_{T,avp} (GeV);"
                                       "CEF",
                             npt, vpt);
    h->pmuf13 = new TProfile("pmuf13", ";#eta;p_{T,avp} (GeV);"
                                       "MUF",
                             npt, vpt);
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
      
    } // for i in tree->nJet
    if(isDebug) cout<<"passed nJet for loop"<<endl;

    // Calculate unclustered MET from the remainders
    // met = -j2 -jn -ju = m2 + mn + mu => mu = met -m2 -mn
    p4mu = p4m0 - p4m2 - p4mn;
    p4mu3 = p4m0 - p4m3 - p4mn3;
    
    hnJet->Fill(tree->nJet, weight);

    // Dijet pre-selection
    if (tree->nJet < 2) continue;
    if (isDebug) cout << "Dijet analysis" << endl << flush;

    // Both leading jets act as tag and probe in turn
    for (int itag = 0; itag != 2; ++itag)
    {
      // Tag and probe jet selection
      int iprobe = (itag == 0 ? 1 : 0);
      p4t.SetPtEtaPhiM(tree->Jet_pt[itag], tree->Jet_eta[itag], tree->Jet_phi[itag],
                       tree->Jet_mass[itag]);
      p4p.SetPtEtaPhiM(tree->Jet_pt[iprobe], tree->Jet_eta[iprobe], tree->Jet_phi[iprobe],
                       tree->Jet_mass[iprobe]);

      // Dijet observables
      double eta = p4p.Eta();
      double pttag = p4t.Pt();
      double ptprobe = p4p.Pt();
      double ptave = 0.5 * (pttag + ptprobe);
      double asymm = (ptprobe - pttag) / ptave;

      double dphi = p4t.DeltaPhi(p4p);
      double dr = p4t.DeltaR(p4p);

      // Proper bisector axis (equal angles to each jet)
      p4b.SetPtEtaPhiM(0, 0, 0, 0);
      p4bt.SetPtEtaPhiM(1, 0, p4t.Phi(), 0);
      p4bp.SetPtEtaPhiM(1, 0, p4p.Phi(), 0);
      p4b += p4bt;
      p4b -= p4bp;
      p4b.SetPtEtaPhiM(p4b.Pt(), 0., p4b.Phi(), 0.);
      p4b *= 1. / p4b.Pt();
      p4bx.SetPtEtaPhiM(p4b.Pt(), 0., p4b.Phi() + 0.5 * TMath::Pi(), 0.);

      // Average projection pT to bisector axis, pT,avp
      // as explained in JME-21-001 (HDM method: bisector extension)
      double ptavp2 = 0.5 * (p4t.Vect().Dot(p4b.Vect()) -
                             p4p.Vect().Dot(p4b.Vect()));

      double m0b = 1 + (p4m0.Vect().Dot(p4b.Vect())) / ptavp2;
      double m2b = 1 + (p4m2.Vect().Dot(p4b.Vect())) / ptavp2;
      double mnb = 0 + (p4mn.Vect().Dot(p4b.Vect())) / ptavp2;
      double mub = 0 + (p4mu.Vect().Dot(p4b.Vect())) / ptavp2;
      // double mob = 0 + (p4mo.Vect().Dot(p4b.Vect()))/ptavp;

      double m0bx = 1 + (p4m0.Vect().Dot(p4bx.Vect())) / ptavp2;
      double m2bx = 1 + (p4m2.Vect().Dot(p4bx.Vect())) / ptavp2;

      // Extras
      double cu = 1. / 0.92;
      double mnub = 0 + ((p4mn + cu * p4mu).Vect().Dot(p4b.Vect())) / ptavp2;
      double mnbx = 0 + (p4mn.Vect().Dot(p4bx.Vect())) / ptavp2;
      double mubx = 0 + (p4mu.Vect().Dot(p4bx.Vect())) / ptavp2;
      double mnubx = 0 + ((p4mn + cu * p4mu).Vect().Dot(p4bx.Vect())) / ptavp2;

      // bisector axis => dijet axis really (not equal angles)
      p4d.SetPtEtaPhiM(0, 0, 0, 0);
      p4d += p4t;
      p4d -= p4p;
      p4d.SetPtEtaPhiM(p4d.Pt(), 0., p4d.Phi(), 0.);
      p4d *= 1. / p4d.Pt();
      p4dx.SetPtEtaPhiM(p4d.Pt(), 0., p4d.Phi() + 0.5 * TMath::Pi(), 0.);

      double m0d = 1 + (p4m0.Vect().Dot(p4d.Vect())) / ptave;
      double m2d = 1 + (p4m2.Vect().Dot(p4d.Vect())) / ptave;
      double mnd = 0 + (p4mn.Vect().Dot(p4d.Vect())) / ptave;
      double mud = 0 + (p4mu.Vect().Dot(p4d.Vect())) / ptave;
      // double mod = 0 + (p4mo.Vect().Dot(p4d.Vect()))/ptave;

      double m0dx = 1 + (p4m0.Vect().Dot(p4dx.Vect())) / ptave;
      double m2dx = 1 + (p4m2.Vect().Dot(p4dx.Vect())) / ptave;

      // central axis + pttag norm&binning
      p4c.SetPtEtaPhiM(0, 0, 0, 0);
      p4c += p4t;
      p4c.SetPtEtaPhiM(p4c.Pt(), 0., p4c.Phi(), 0.);
      p4c *= 1. / p4c.Pt();
      p4cx.SetPtEtaPhiM(p4c.Pt(), 0., p4c.Phi() + 0.5 * TMath::Pi(), 0.);

      double m0c = 1 + (p4m0.Vect().Dot(p4c.Vect())) / pttag;
      double m2c = 1 + (p4m2.Vect().Dot(p4c.Vect())) / pttag;
      double mnc = 0 + (p4mn.Vect().Dot(p4c.Vect())) / pttag;
      double muc = 0 + (p4mu.Vect().Dot(p4c.Vect())) / pttag;
      // double moc = 0 + (p4mo.Vect().Dot(p4c.Vect()))/pttag;

      double m0cx = 1 + (p4m0.Vect().Dot(p4cx.Vect())) / pttag;
      double m2cx = 1 + (p4m2.Vect().Dot(p4cx.Vect())) / pttag;

      // forward axis ("backwards" to tag hemisphere) + ptprobe norm&binning
      p4f.SetPtEtaPhiM(0, 0, 0, 0);
      p4f -= p4p;
      p4f.SetPtEtaPhiM(p4f.Pt(), 0., p4f.Phi(), 0.);
      p4f *= 1. / p4f.Pt();
      p4fx.SetPtEtaPhiM(p4f.Pt(), 0., p4f.Phi() + 0.5 * TMath::Pi(), 0.);

      double m0f = 1 + (p4m0.Vect().Dot(p4f.Vect())) / ptprobe;
      double m2f = 1 + (p4m2.Vect().Dot(p4f.Vect())) / ptprobe;
      double mnf = 0 + (p4mn.Vect().Dot(p4f.Vect())) / ptprobe;
      double muf = 0 + (p4mu.Vect().Dot(p4f.Vect())) / ptprobe;
      // double mof = 0 + (p4mo.Vect().Dot(p4f.Vect()))/ptprobe;

      double m0fx = 1 + (p4m0.Vect().Dot(p4fx.Vect())) / ptprobe;
      double m2fx = 1 + (p4m2.Vect().Dot(p4fx.Vect())) / ptprobe;

      // Dijet mass
      p4dj = p4t;
      p4dj += p4p;
      double mjj = p4dj.M();
      double deta = fabs(p4p.Eta() - p4t.Eta());
      
      bool isdijet = (fabs(p4t.Eta()) < 1.3 && dphi > 2.7 &&
                      fabs(asymm) < maxa && //!
                      p4t.Pt() > 15. && tree->Jet_jetId[itag] >= 4 &&
                      p4p.Pt() > 15. && tree->Jet_jetId[iprobe] >= 4 &&
                      //!tree->Jet_jetveto[itag] && !tree->Jet_jetveto[iprobe] && //!
                      pass_METfilter > 0);
      // DESY selection. Note tighter asymmetry cut and allJetsGood
      bool isdijet2 = (fabs(p4t.Eta()) < 1.3 && dphi > 2.7 &&
                       fabs((pttag - ptprobe) / (pttag + ptprobe)) < 0.7 && //!
                       // fabs(asymm)<maxa && //!
                       p4t.Pt() > 15. && tree->Jet_jetId[itag] >= 4 &&
                       p4p.Pt() > 15. && tree->Jet_jetId[iprobe] >= 4 &&
                       //! tree->Jet_jetveto[itag] && !tree->Jet_jetveto[iprobe] && //!
                       allJetsGood && //!
                       pass_METfilter > 0);

      for (const auto& trigName_ValRange : trigsName_ValRange) {
        if (!(*trigName_ValRange.second.first)) continue;
        DiJetHistos *h = mapStrHistDiJet[trigName_ValRange.first];
        double res = JetResFactor[iprobe] / JetResFactor[itag];

        h->h2AllProbEtaAvgBisectorPt->Fill(eta, ptavp2, weight);
        h->h2AllProbEtaAvgPt->Fill(eta, ptave, weight);
        h->h2AllProbEtaTagPt->Fill(eta, pttag, weight);
        h->h2AllProbEtaProbePt->Fill(eta, ptprobe, weight);

        // Bisector (proper)
        if (ptavp2 >= h->ptmin && ptavp2 < h->ptmax &&
            fabs(eta) >= h->absetamin && fabs(eta) < h->absetamax)
        {
          h->h2SelProbEtaAvgBisectorPt->Fill(eta, ptavp2, weight);
        }
        { // Bisector (proper)
          //doDiJetJER
          h->p2M0bInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, m0b, weight);
          h->p2M0bxInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, m0bx, weight);
          h->p2M2bInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, m2b, weight);
          h->p2M2bxInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, m2bx, weight);
          //doPFComposition
          h->p2ProbePtInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, tree->Jet_pt[iprobe], weight);
          h->p2RhoInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, tree->Rho, weight);
          h->p2ChfInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, tree->Jet_chHEF[iprobe], weight);
          h->p2NhfInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, tree->Jet_neHEF[iprobe], weight);
          h->p2NefInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, tree->Jet_neEmEF[iprobe], weight);
          h->p2CefInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, tree->Jet_chEmEF[iprobe], weight);
          h->p2MufInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, tree->Jet_muEF[iprobe], weight);

          h->ppt13->Fill(ptavp2, tree->Jet_pt[itag], weight);
          h->prho13->Fill(ptavp2, tree->Rho, weight);
          h->pchf13->Fill(ptavp2, tree->Jet_chHEF[itag], weight);
          h->pnhf13->Fill(ptavp2, tree->Jet_neHEF[itag], weight);
          h->pnef13->Fill(ptavp2, tree->Jet_neEmEF[itag], weight);
          h->pcef13->Fill(ptavp2, tree->Jet_chEmEF[itag], weight);
          h->pmuf13->Fill(ptavp2, tree->Jet_muEF[itag], weight);

          h->p2ResidualInProbEtaAvgBisectorPt->Fill(eta, ptavp2, res, weight);
          h->p2M0aInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, m0b, weight);
          h->p2M2aInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, m2b, weight);
          h->p2MnaInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, mnb, weight);
          h->p2MuaInProbeEtaAvgBisectorPt->Fill(eta, ptavp2, mub, weight);
        }
        // Dijet axis
        if (ptave >= h->ptmin && ptave < h->ptmax &&
            fabs(eta) >= h->absetamin && fabs(eta) < h->absetamax)
        {
          h->h2SelProbEtaAvgPt->Fill(eta, ptave, weight);
        }
        { // Dijet axis
          h->p2ResidualInProbEtaAvgPt->Fill(eta, ptave, res, weight);
          h->p2M0dInProbeEtaAvgPt->Fill(eta, ptave, m0d, weight);
          h->p2M2dInProbeEtaAvgPt->Fill(eta, ptave, m2d, weight);
          h->p2M2nInProbeEtaAvgPt->Fill(eta, ptave, mnd, weight);
          h->p2M2uInProbeEtaAvgPt->Fill(eta, ptave, mud, weight);
        }
        // Tag jet axis
        if (pttag >= h->ptmin && pttag < h->ptmax)
        {
          h->h2SelProbEtaTagPt->Fill(eta, pttag, weight);
        }
        // Tag jet axis
        {
          h->p2ResidualInProbEtaTagPt->Fill(eta, pttag, res, weight);
          h->p2M0cInProbeEtaTagPt->Fill(eta, pttag, m0c, weight);
          h->p2M2cInProbeEtaTagPt->Fill(eta, pttag, m2c, weight);
          h->p2MncInProbeEtaTagPt->Fill(eta, pttag, mnc, weight);
          h->p2MucInProbeEtaTagPt->Fill(eta, pttag, muc, weight);
        }
        // Probe jet axis
        if (ptprobe >= h->ptmin && ptprobe < h->ptmax)
        {
          h->h2SelProbEtaProbePt->Fill(eta, ptprobe, weight);
        }
        // Probe jet axis
        {
          h->p2ResidualInProbeEtaProbePt->Fill(eta, ptprobe, res, weight);
          h->p2M0fInProbeEtaProbePt->Fill(eta, ptprobe, m0f, weight);
          h->p2M2fInProbeEtaProbePt->Fill(eta, ptprobe, m2f, weight);
          h->p2MnfInProbeEtaProbePt->Fill(eta, ptprobe, mnf, weight);
          h->p2MufInProbeEtaProbePt->Fill(eta, ptprobe, muf, weight);
        }
      } // for itrg

      // Dijet without deltaphi cut
      if (fabs(p4t.Eta() < 1.3) && fabs(asymm) < maxa && ptave >=40)
      {
        h2dphi->Fill(p4p.Eta(), dphi, weight);
      }
    } // for itag
  }//event for loop

  fout->Write();
  cout<<"Output file: "<<fout->GetName()<<endl;
  fout->Close();
  return 0;
}
   
