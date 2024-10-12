#include "HistVetoMap.h"

#include "SkimTree.h"      
#include "EventPick.h"     
#include "ObjectPick.h"    
#include "ObjectScale.h"   


class VetoMapHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptMin, ptMax, absEtaMin, absEtaMax;

  TH2D *h2AllJetEtaJetPt;
  TH2D *h2SelJetEtaJetPt;
  TH2D *h2SelJetEtaJetPhi;
  
  TProfile2D *p2ChfInSelJetEtaJetPhi;
  TProfile2D *p2NhfInSelJetEtaJetPhi;
  TProfile2D *p2NefInSelJetEtaJetPhi;

};


int HistVetoMap::Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout){
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
  TH1D *hCutflow = new TH1D("hCutflow",";hCutflow",cuts.size(),0.5,cuts.size()+0.5);
  for (int i = 0; i != cuts.size(); ++i) {
    hCutflow->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
  }


	TH1D* hEvents_Jet = new TH1D("hEvents_Jet", "Cutflow", 15, 0.5, 15.5);
  hEvents_Jet->GetXaxis()->SetBinLabel(1, "NanoAOD");

	hEvents_Jet->GetXaxis()->SetBinLabel(2, "jetPT15");
	hEvents_Jet->GetXaxis()->SetBinLabel(3, "jetPT15_FPIX");
	hEvents_Jet->GetXaxis()->SetBinLabel(4, "jetPT15_TightID");
	hEvents_Jet->GetXaxis()->SetBinLabel(5, "jetPT15_TightID_FPIX");
	hEvents_Jet->GetXaxis()->SetBinLabel(6, "jetPT30_TightID");
	hEvents_Jet->GetXaxis()->SetBinLabel(7, "jetPT30_TightID_FPIX");
	hEvents_Jet->GetXaxis()->SetBinLabel(8, "jetPT50_TightID");
	hEvents_Jet->GetXaxis()->SetBinLabel(9, "jetPT50_TightID_FPIX");
	hEvents_Jet->GetXaxis()->SetBinLabel(10, "jetPT100_TightID");
	hEvents_Jet->GetXaxis()->SetBinLabel(11, "jetPT100_TightID_FPIX");


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
  fout->mkdir("JetVeto");
  fout->cd("JetVeto");
  int binN = 0;
  map<string, VetoMapHistos *> mapStrHistVetoMap;
  for (const auto& trigName_ValRange : trigsName_ValRange) {
    string trigName = trigName_ValRange.first;
    binN++;
    hTrig->GetXaxis()->SetBinLabel(binN, trigName.c_str());
    
    if (isDebug) cout << "Trigger " << trigName << endl;
    fout->mkdir(trigName.c_str());
    fout->cd(trigName.c_str());
    TDirectory *dout = gDirectory;

    const struct TrigRange &r = trigName_ValRange.second.second;
    VetoMapHistos *h = new VetoMapHistos();
    mapStrHistVetoMap[trigName] = h;
    h->trg = trigName;
    h->trgpt = r.ptThresh;

    h->ptMin = r.ptMin;
    h->ptMax = r.ptMax;
    h->absEtaMin = r.absEtaMin;
    h->absEtaMax = r.absEtaMax;

    // Plots with inclusive jet selection
    h->h2AllJetEtaJetPt = new TH2D("h2AllJetEtaJetPt", ";#eta;p_{T} (GeV);N_{jet}",
                              nx, vx, npti, vpti);
    h->h2SelJetEtaJetPt = new TH2D("h2SelJetEtaJetPt", ";#eta;p_{T} (GeV);N_{jet}",
                              nx, vx, npti, vpti);
    h->h2SelJetEtaJetPhi = new TH2D("h2SelJetEtaJetPhi", ";#eta;#phi;N_{jet}",
                           nx, vx, 72, -TMath::Pi(), +TMath::Pi());

    //doPFComposition
    h->p2ChfInSelJetEtaJetPhi = new TProfile2D("p2ChfInSelJetEtaJetPhi", ";#eta;#phi;CHF (DM)",
                              nx, vx, 72, -TMath::Pi(), +TMath::Pi());
    h->p2NefInSelJetEtaJetPhi = new TProfile2D("p2NefInSelJetEtaJetPhi", ";#eta;#phi;NEF (DM)",
                              nx, vx, 72, -TMath::Pi(), +TMath::Pi());
    h->p2NhfInSelJetEtaJetPhi = new TProfile2D("p2NhfInSelJetEtaJetPhi", ";#eta;#phi;NHF (DM)",
                              nx, vx, 72, -TMath::Pi(), +TMath::Pi());

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

		hEvents_Jet->Fill(1);
		if(tree->nJet>0){
			double pt = tree->Jet_pt[0];
			double eta = tree->Jet_eta[0];
			double phi = tree->Jet_phi[0];
			bool id    = tree->Jet_jetId[0] & (1 << 1);
		  bool isFpix = !(eta > -2.5 && eta < -1.4 && phi > 2.5 && phi <3.0);
		
			if(pt > 15) hEvents_Jet->Fill(2);	
			if(pt > 15 && isFpix) hEvents_Jet->Fill(3);	
			if(pt > 15 && id) hEvents_Jet->Fill(4);	
			if(pt > 15 && id && isFpix) hEvents_Jet->Fill(5);	
			if(pt > 30 && id) hEvents_Jet->Fill(6);	
			if(pt > 30 && id && isFpix) hEvents_Jet->Fill(7);	
			if(pt > 50 && id) hEvents_Jet->Fill(8);	
			if(pt > 50 && id && isFpix) hEvents_Jet->Fill(9);	
			if(pt > 100 && id) hEvents_Jet->Fill(10);	
			if(pt > 100 && id && isFpix) hEvents_Jet->Fill(11);	
		}

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
      p4Jet.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);
      for (const auto& trigName_ValRange : trigsName_ValRange) {
        if (!(*trigName_ValRange.second.first)) continue;
        VetoMapHistos *h = mapStrHistVetoMap[trigName_ValRange.first];
       	double abseta = fabs(p4Jet.Eta());
        double pt = p4Jet.Pt();
      
        h->h2AllJetEtaJetPt->Fill(p4Jet.Eta(), p4Jet.Pt(), weight);
        if (tree->Jet_jetId[i] >= 4 &&
            pt >= h->ptMin && pt < h->ptMax &&
            abseta >= h->absEtaMin && abseta < h->absEtaMax)
        {
          h->h2SelJetEtaJetPt->Fill(p4Jet.Eta(), p4Jet.Pt(), weight);
          h->h2SelJetEtaJetPhi->Fill(p4Jet.Eta(), p4Jet.Phi(), weight);
          h->p2ChfInSelJetEtaJetPhi->Fill(p4Jet.Eta(), p4Jet.Phi(), tree->Jet_chHEF[i], weight);
          h->p2NefInSelJetEtaJetPhi->Fill(p4Jet.Eta(), p4Jet.Phi(), tree->Jet_neEmEF[i], weight);
          h->p2NhfInSelJetEtaJetPhi->Fill(p4Jet.Eta(), p4Jet.Phi(), tree->Jet_neHEF[i], weight);
      	}//jet sel
      } // itrg
    }// nJet

  }//event for loop

  fout->Write();
  cout<<"--------: Cutlow summary :-------"<<endl;
  eventP->printBins(hCutflow);
  cout<<"--------: hEvents_jet summary :-------"<<endl;
  eventP->printBins(hEvents_Jet);
  cout<<"--------: Trigger rate summary :-------"<<endl;
  eventP->printBins(hTrig);
  eventP->scanTFile(fout);
  cout<<"Output file: "<<fout->GetName()<<endl;
  fout->Close();
  return 0;
}
   
