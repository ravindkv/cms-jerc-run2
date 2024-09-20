#include "HistIncJet.h"

#include "SkimTree.h"      
#include "EventPick.h"     
#include "ObjectPick.h"    
#include "ObjectScale.h"   


class IncJetHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptMin, ptMax, absEtaMin, absEtaMax;

  static const int ny = 10;
  TH2D *h2AllJetEtaJetPt;
  TH2D *h2SelJetEtaJetPt;
  TH2D *h2IdJetEtaJetPt;
  TH1D *hJetPtForJetEta1p3;
  TH1D *hJetPtForRapidity[ny];

  // (Optional) composition plots
  TProfile2D *p2JetPtInJetEtaJetPt; 
  TProfile2D *p2RhoInJetEtaJetPt;
  TProfile2D *p2ChfInJetEtaJetPt;
  TProfile2D *p2NefInJetEtaJetPt;
  TProfile2D *p2NhfInJetEtaJetPt;
  TProfile2D *p2CefInJetEtaJetPt;
  TProfile2D *p2MufInJetEtaJetPt;

  TProfile *pJetPtInJetPtForJetEta1p3;
  TProfile *pRhoInJetPtForJetEta1p3;
  TProfile *pChfInJetPtForJetEta1p3;
  TProfile *pNefInJetPtForJetEta1p3;
  TProfile *pNhfInJetPtForJetEta1p3;
  TProfile *pCefInJetPtForJetEta1p3; 
  TProfile *pMufInJetPtForJetEta1p3;
};


int HistIncJet::Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout){
   
  TDirectory *curdir = gDirectory;
  assert(fout && !fout->IsZombie());
  string dir = (isMC ? "MC" : "DATA");

  // Cutflow histograms 
  vector<string> cuts;
  cuts.push_back("eventsInSkim");
  cuts.push_back("passHLT");
  cuts.push_back("passGoodLumi");
  cuts.push_back("passMetFilter");
  cuts.push_back("passJetVetoMap");
  cuts.push_back("passAtleast1Jet");
  TH1D *hCutflow = new TH1D("hCutflow",";hCutflow",cuts.size(),0.5,cuts.size()+0.5);
  for (int i = 0; i != cuts.size(); ++i) {
    hCutflow->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
  }

  bool isZB = false;
  double fwdeta = 3.139;  // was 2.853. 80% (100%) on negative (positive) side
  double fwdeta0 = 2.964; // 2.853; // 40 and 260 up
  map<string, pair<const Bool_t*, struct TrigRange>> trigsName_ValRange;
  trigsName_ValRange["HLT_MC"                ]  = make_pair(&tree->HLT_MC                 ,TrigRange{0, 10, 3000, 0, 5.2});
  trigsName_ValRange["HLT_ZeroBias"          ]  = make_pair(&tree->HLT_ZeroBias           ,TrigRange{0, 10, 3000, 0, 5.2});
  if (!isZB) {
  trigsName_ValRange["HLT_PFJet40"           ]  = make_pair(&tree->HLT_PFJet40            ,TrigRange{40, 40, 85, 0, 5.2   });
  trigsName_ValRange["HLT_PFJet60"           ]  = make_pair(&tree->HLT_PFJet60            ,TrigRange{60, 85, 100, 0, 5.2  });
  trigsName_ValRange["HLT_PFJet80"           ]  = make_pair(&tree->HLT_PFJet80            ,TrigRange{80, 100, 155, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet140"          ]  = make_pair(&tree->HLT_PFJet140           ,TrigRange{140, 155, 210, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet200"          ]  = make_pair(&tree->HLT_PFJet200           ,TrigRange{200, 210, 300, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet260"          ]  = make_pair(&tree->HLT_PFJet260           ,TrigRange{260, 300, 400, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet320"          ]  = make_pair(&tree->HLT_PFJet320           ,TrigRange{320, 400, 500, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet400"          ]  = make_pair(&tree->HLT_PFJet400           ,TrigRange{400, 500, 600, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet450"          ]  = make_pair(&tree->HLT_PFJet450           ,TrigRange{450, 500, 600, 0, 5.2 });
  trigsName_ValRange["HLT_PFJet500"          ]  = make_pair(&tree->HLT_PFJet500           ,TrigRange{500, 600, 6500, 0, 5.2});
  trigsName_ValRange["HLT_PFJet550"          ]  = make_pair(&tree->HLT_PFJet550           ,TrigRange{550, 700, 6500, 0, 5.2});
  trigsName_ValRange["HLT_DiPFJetAve40"      ]  = make_pair(&tree->HLT_DiPFJetAve40       ,TrigRange{40, 40, 85, 0, 5.2   });
  trigsName_ValRange["HLT_DiPFJetAve60"      ]  = make_pair(&tree->HLT_DiPFJetAve60       ,TrigRange{60, 85, 100, 0, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve80"      ]  = make_pair(&tree->HLT_DiPFJetAve80       ,TrigRange{80, 100, 155, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve140"     ]  = make_pair(&tree->HLT_DiPFJetAve140      ,TrigRange{140, 155, 210, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve200"     ]  = make_pair(&tree->HLT_DiPFJetAve200      ,TrigRange{200, 210, 300, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve260"     ]  = make_pair(&tree->HLT_DiPFJetAve260      ,TrigRange{260, 300, 400, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve320"     ]  = make_pair(&tree->HLT_DiPFJetAve320      ,TrigRange{320, 400, 500, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve400"     ]  = make_pair(&tree->HLT_DiPFJetAve400      ,TrigRange{400, 500, 600, 0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve500"     ]  = make_pair(&tree->HLT_DiPFJetAve500      ,TrigRange{500, 600, 6500, 0, 5.2});
  trigsName_ValRange["HLT_DiPFJetAve60_HFJEC"]  = make_pair(&tree->HLT_DiPFJetAve60_HFJEC ,TrigRange{60, 85, 100, fwdeta, 5.2   });
  trigsName_ValRange["HLT_DiPFJetAve80_HFJEC"]  = make_pair(&tree->HLT_DiPFJetAve80_HFJEC ,TrigRange{80, 100, 125, fwdeta, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve100_HFJEC"] = make_pair(&tree->HLT_DiPFJetAve100_HFJEC,TrigRange{100, 125, 180, fwdeta, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve160_HFJEC"] = make_pair(&tree->HLT_DiPFJetAve160_HFJEC,TrigRange{160, 180, 250, fwdeta, 5.2  });
  trigsName_ValRange["HLT_DiPFJetAve220_HFJEC"] = make_pair(&tree->HLT_DiPFJetAve220_HFJEC,TrigRange{220, 250, 350, fwdeta0, 5.2 });
  trigsName_ValRange["HLT_DiPFJetAve300_HFJEC"] = make_pair(&tree->HLT_DiPFJetAve300_HFJEC,TrigRange{300, 350, 6500, fwdeta0, 5.2});
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

  fout->cd();
 // Monitor trigger rates
  TH1D *hTrig = new TH1D("hTrig", "Triggers;Trigger;N_{events}",
                        trigsName_ValRange.size(), 0, trigsName_ValRange.size());

  fout->mkdir("Incjet");
  fout->cd("Incjet");
  int binN = 0;
  map<string, IncJetHistos *> mapStrHistIncJet;
  for (const auto& trigName_ValRange : trigsName_ValRange) {
    string trigName = trigName_ValRange.first;
    binN++;
    hTrig->GetXaxis()->SetBinLabel(binN, trigName.c_str());
    
    if (isDebug) cout << "Trigger " << trigName << endl;
    fout->mkdir(trigName.c_str());
    fout->cd(trigName.c_str());
    TDirectory *dout = gDirectory;

    const struct TrigRange &r = trigName_ValRange.second.second;
    IncJetHistos *h = new IncJetHistos();
    mapStrHistIncJet[trigName] = h;
    h->trg = trigName;
    h->trgpt = r.ptThresh;

    h->ptMin = r.ptMin;
    h->ptMax = r.ptMax;
    h->absEtaMin = r.absEtaMin;
    h->absEtaMax = r.absEtaMax;

    h->h2AllJetEtaJetPt = new TH2D("h2AllJetEtaJetPt", ";#eta;p_{T} (GeV);N_{jet}",
                              nx, vx, npti, vpti);
    h->h2SelJetEtaJetPt = new TH2D("h2SelJetEtaJetPt", ";#eta;p_{T} (GeV);N_{jet}",
                              nx, vx, npti, vpti);
    h->h2IdJetEtaJetPt = new TH2D("h2IdJetEtaJetPt", ";#eta;p_{T} (GeV);N_{jet}",
                          nx, vx, npti, vpti);

    h->hJetPtForJetEta1p3 = new TH1D("hJetPtForJetEta1p3", ";p_{T,jet} (GeV)", npti, vpti);
    for (int iy = 0; iy != h->ny; ++iy)
    {
      h->hJetPtForRapidity[iy] = new TH1D(Form("hJetPtForRapidity%02d", 5 * (iy + 1)), ";p_{T} (GeV);"
                                                           "N_{jet}",
                            npti, vpti);
    } // for iy

    //doPFComposition
    h->p2JetPtInJetEtaJetPt = new TProfile2D("p2JetPtInJetEtaJetPt", ";#eta;p_{T,jet} (GeV);"
                                     "p_{T,jet}",
                             nx, vx, npt, vpt);
    h->p2RhoInJetEtaJetPt = new TProfile2D("p2RhoInJetEtaJetPt", ";#eta;p_{T,jet} (GeV);"
                                       "#rho",
                              nx, vx, npt, vpt);
    h->p2ChfInJetEtaJetPt = new TProfile2D("p2ChfInJetEtaJetPt", ";#eta;p_{T,jet} (GeV);"
                                       "CHF",
                              nx, vx, npt, vpt);
    h->p2NhfInJetEtaJetPt = new TProfile2D("p2NhfInJetEtaJetPt", ";#eta;p_{T,jet} (GeV);"
                                       "NHF",
                              nx, vx, npt, vpt);
    h->p2NefInJetEtaJetPt = new TProfile2D("p2NefInJetEtaJetPt", ";#eta;p_{T,jet} (GeV);"
                                       "NEF",
                              nx, vx, npt, vpt);
    h->p2CefInJetEtaJetPt = new TProfile2D("p2CefInJetEtaJetPt", ";#eta;p_{T,jet} (GeV);"
                                       "CEF",
                              nx, vx, npt, vpt);
    h->p2MufInJetEtaJetPt = new TProfile2D("p2MufInJetEtaJetPt", ";#eta;p_{T,jet} (GeV);"
                                       "MUF",
                              nx, vx, npt, vpt);

    h->pJetPtInJetPtForJetEta1p3 = new TProfile("pJetPtInJetPtForJetEta1p3", ";#eta;p_{T,jet} (GeV);"
                                     "p_{T,jet}",
                            npt, vpt);
    h->pRhoInJetPtForJetEta1p3 = new TProfile("pRhoInJetPtForJetEta1p3", ";#eta;p_{T,jet} (GeV);"
                                       "#rho",
                             npt, vpt);
    h->pChfInJetPtForJetEta1p3 = new TProfile("pChfInJetPtForJetEta1p3", ";#eta;p_{T,jet} (GeV);"
                                       "CHF",
                             npt, vpt);
    h->pNhfInJetPtForJetEta1p3 = new TProfile("pNhfInJetPtForJetEta1p3", ";#eta;p_{T,jet} (GeV);"
                                       "NHF",
                             npt, vpt);
    h->pNefInJetPtForJetEta1p3 = new TProfile("pNefInJetPtForJetEta1p3", ";#eta;p_{T,jet} (GeV);"
                                       "NEF",
                             npt, vpt);
    h->pCefInJetPtForJetEta1p3 = new TProfile("pCefInJetPtForJetEta1p3", ";#eta;p_{T,jet} (GeV);"
                                       "CEF",
                             npt, vpt);
    h->pMufInJetPtForJetEta1p3 = new TProfile("pMufInJetPtForJetEta1p3", ";#eta;p_{T,jet} (GeV);"
                                       "MUF",
                             npt, vpt);
  } // for itrg
 
  //------------------------------------
  // Event for loop 
  //------------------------------------
  TLorentzVector p4Jet;

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
   
    Long64_t ientry = tree->loadEntry(jentry);
    if (ientry < 0) break; 
    tree->fChain->GetTree()->GetEntry(ientry);
    //if(isDebug) cout<<"Loaded jentry = "<<jentry<<endl;

    // Weight
    double weight = (isMC ? tree->genWeight : 1);
    int countCut = 0;
    hCutflow->Fill(countCut++, weight);

    //------------------------------------
    // Basic event selections 
    //------------------------------------
    //if(!eventP->passHLT(tree)) continue; 
    if(isDebug) cout<<"passHLT"<<endl;
    hCutflow->Fill(countCut++, weight);

    bool passGoodLumi = true; 
    if(isData){
      passGoodLumi = objS->checkGoodLumi(tree->run, tree->luminosityBlock);
    }
    if(!passGoodLumi) continue; 
    if(isDebug) cout<<"passGoodLumi"<<endl;
    hCutflow->Fill(countCut++, weight);

    if(!eventP->passFilter(tree)) continue; 
    if(isDebug) cout<<"passMetFilter"<<endl;
    hCutflow->Fill(countCut++, weight);

    if(objS->checkJetVetoMap(tree)) continue; 
    if(isDebug) cout<<"passJetVetoMap"<<endl;
    hCutflow->Fill(countCut++, weight);

    if(tree->nJet < 1) continue; 
    if(isDebug) cout<<"passAtleast1Jet"<<endl;
    hCutflow->Fill(countCut++, weight);

    int binN = 0;
    for (const auto& trigName_ValRange : trigsName_ValRange) {
      binN++;
      if ((*trigName_ValRange.second.first))
        hTrig->Fill(binN); 
    }

		//------------------------------------------------
    // Jet loop
		//------------------------------------------------
    for (int i = 0; i != tree->nJet; ++i) {
      //Data+MC: undo the correction already applied in NanoAOD
      tree->Jet_pt[i]   = tree->Jet_pt[i] * (1.0 - tree->Jet_rawFactor[i]);
      tree->Jet_mass[i] = tree->Jet_mass[i] * (1.0 - tree->Jet_rawFactor[i]);

      //Data+MC: L1RC correction is applied to the uncorrected jet;
      double corrL1FastJet = objS->getL1FastJetCorrection(tree->Jet_area[i], tree->Jet_eta[i], 
                                            tree->Jet_pt[i], tree->Rho);
      tree->Jet_pt[i]   = tree->Jet_pt[i] * corrL1FastJet;
      tree->Jet_mass[i] = tree->Jet_mass[i] * corrL1FastJet;

      //Data+MC: MCTruth correction is applied to the L1RC corrected jet;
      double corrL2Relative = objS->getL2RelativeCorrection(tree->Jet_eta[i], tree->Jet_phi[i],tree->Jet_pt[i]);
      tree->Jet_pt[i]   = tree->Jet_pt[i] * corrL2Relative;
      tree->Jet_mass[i] = tree->Jet_mass[i] * corrL2Relative;

      //Only Data: L2Residual+L3Residual correction is applied to the MCTruth corrected jet;
      if(isData){
        double corrL2L3Residual = objS->getL2L3ResidualCorrection(tree->Jet_eta[i], tree->Jet_pt[i]);
        tree->Jet_pt[i]   = tree->Jet_pt[i] * corrL2L3Residual;
        tree->Jet_mass[i] = tree->Jet_mass[i] * corrL2L3Residual;
      }
      //Only MC: JER Corrections
      if(isMC){
        //double corrJER = objS->getJERCorrection(tree, i, "nom");
        //tree->Jet_pt[i]   = tree->Jet_pt[i] * corrJER;
      }
      p4Jet.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);

      for (const auto& trigName_ValRange : trigsName_ValRange) {
        if (!(*trigName_ValRange.second.first)) continue;
        IncJetHistos *h = mapStrHistIncJet[trigName_ValRange.first];
      
        h->h2AllJetEtaJetPt->Fill(p4Jet.Eta(), p4Jet.Pt(), weight);
        if (tree->Jet_jetId[i] >= 4 )
        {
          h->h2IdJetEtaJetPt->Fill(p4Jet.Eta(), p4Jet.Pt(), weight);
      
          if (p4Jet.Pt() >= h->ptMin && p4Jet.Pt() < h->ptMax &&
              fabs(p4Jet.Rapidity()) > h->absEtaMin &&
              fabs(p4Jet.Rapidity()) < h->absEtaMax)
      
            h->h2SelJetEtaJetPt->Fill(p4Jet.Eta(), p4Jet.Pt(), weight);
      
          if (fabs(p4Jet.Rapidity()) < 1.3)
            h->hJetPtForJetEta1p3->Fill(p4Jet.Pt(), weight);
          int iy = int(fabs(p4Jet.Rapidity()) / 0.5);
          if (iy < h->ny)
            h->hJetPtForRapidity[iy]->Fill(p4Jet.Pt(), weight);
      
          //doPFComposition
          double eta = p4Jet.Eta();
          double pt = p4Jet.Pt();
          h->p2JetPtInJetEtaJetPt->Fill(eta, pt, tree->Jet_pt[i], weight);
          h->p2RhoInJetEtaJetPt->Fill(eta, pt, tree->Rho, weight);
          h->p2ChfInJetEtaJetPt->Fill(eta, pt, tree->Jet_chHEF[i], weight);
          h->p2NhfInJetEtaJetPt->Fill(eta, pt, tree->Jet_neHEF[i], weight);
          h->p2NefInJetEtaJetPt->Fill(eta, pt, tree->Jet_neEmEF[i], weight);
          h->p2CefInJetEtaJetPt->Fill(eta, pt, tree->Jet_chEmEF[i], weight);
          h->p2MufInJetEtaJetPt->Fill(eta, pt, tree->Jet_muEF[i], weight);
      
          if (fabs(eta) < 1.3)
          {
            h->pJetPtInJetPtForJetEta1p3->Fill(pt, tree->Jet_pt[i], weight);
            h->pRhoInJetPtForJetEta1p3->Fill(pt, tree->Rho, weight);
            h->pChfInJetPtForJetEta1p3->Fill(pt, tree->Jet_chHEF[i], weight);
            h->pNhfInJetPtForJetEta1p3->Fill(pt, tree->Jet_neHEF[i], weight);
            h->pNefInJetPtForJetEta1p3->Fill(pt, tree->Jet_neEmEF[i], weight);
            h->pCefInJetPtForJetEta1p3->Fill(pt, tree->Jet_chEmEF[i], weight);
            h->pMufInJetPtForJetEta1p3->Fill(pt, tree->Jet_muEF[i], weight);
          }
        }// JetID
      }// itrg
    }// nJet

  }//event for loop

  fout->Write();
  cout<<"--------: Cutlow summary :-------"<<endl;
  eventP->printBins(hCutflow);
  cout<<"--------: Trigger rate summary :-------"<<endl;
  eventP->printBins(hTrig);
  eventP->scanTFile(fout);
  cout<<"Output file: "<<fout->GetName()<<endl;
  fout->Close();
  return 0;
}
   
