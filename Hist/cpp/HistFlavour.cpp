#include "HistFlavour.h"

#include "SkimTree.h"      
#include "EventPick.h"     
#include "ObjectPick.h"    
#include "ObjectScale.h"   

int HistFlavour::Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout){
   
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

  TDirectory *curdir = gDirectory;
  assert(fout && !fout->IsZombie());

  curdir->mkdir("Flavour");
  curdir->cd("Flavour");


  // Main histograms 
  double binsPt[] = {10, 15, 21, 28, 37, 49, 59, 86, 110, 132, 170, 204, 236, 
    279, 302, 373, 460, 575, 638, 737, 846, 967, 1101, 1248, 1410, 1588, 
    1784, 2000, 2238, 2500, 2787, 3103, 3450, 4037, 5220};
  double nBinsPt = sizeof(binsPt) / sizeof(binsPt[0]) - 1;

  double binsEta[] = {0., 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 
    2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191};
  const int nBinsEta = sizeof(binsEta) / sizeof(binsEta[0]) - 1;


  TH2D *h2JetEtaGenJetPt = new TH2D("h2JetEtaGenJetPt", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                   "N_{events}",
                        nBinsEta, binsEta, nBinsPt, binsPt);
  TH2D *h2GenJetEtaGenJetPt = new TH2D("h2GenJetEtaGenJetPt", ";|#eta_{gen}|;p_{T,gen} (GeV);"
                                           "N_{events}",
                            nBinsEta, binsEta, nBinsPt, binsPt);
  TH2D *h2JetEtaJetPt = new TH2D("h2JetEtaJetPt", ";|#eta_{jet}|;p_{T,jet} (GeV);"
                                           "N_{events}",
                            nBinsEta, binsEta, nBinsPt, binsPt);
  TProfile2D *p2JesInJetEtaGenJetPt = new TProfile2D("p2JesInJetEtaGenJetPt", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                     "JES(jet)",
                            nBinsEta, binsEta, nBinsPt, binsPt);
  TProfile2D *p2ResponseInJetEtaGenJetPt = new TProfile2D("p2ResponseInJetEtaGenJetPt", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                 "p_{T,jet}/p_{T,gen}",
                          nBinsEta, binsEta, nBinsPt, binsPt);
  TProfile2D *p2HasMatchedVtxInGenJetEtaGenJetPt = new TProfile2D("p2HasMatchedVtxInGenJetEtaGenJetPt", ";|#eta_{gen}|;p_{T,gen} (GeV);"
                                       "Vertex efficiency",
                             nBinsEta, binsEta, nBinsPt, binsPt);
  TProfile2D *p2HasMatchedJetInGenJetEtaGenJetPt = new TProfile2D("p2HasMatchedJetInGenJetEtaGenJetPt", ";|#eta_{gen}|;p_{T,gen} (GeV);"
                                     "Efficiency",
                            nBinsEta, binsEta, nBinsPt, binsPt);
  TProfile2D *p2HasMatchedJetInJetEtaJetPt = new TProfile2D("p2HasMatchedJetInJetEtaJetPt", ";|#eta_{jet}|;p_{T,jet} (GeV);"
                                     "Purity",
                            nBinsEta, binsEta, nBinsPt, binsPt);
 
  map<string, double> mvar;
  map<string, map<string, TProfile*> > mp;
  string avar[] = {"p2ResponseInJetEtaGenJetPt"};
  string aflv[] = {"i","b","c","q","s","ud","g","n"};
  const int nvar = sizeof(avar)/sizeof(avar[0]);
  const int nflv = sizeof(aflv)/sizeof(aflv[0]);
  for (int ivar = 0; ivar != nvar; ++ivar) {
    for (int iflv = 0; iflv != nflv; ++iflv) {
      string& var = avar[ivar]; const char *cv = var.c_str();
      string& flv = aflv[iflv]; const char *cf = flv.c_str();
      mp[var][flv] = new TProfile(Form("%s_%s",cv,cf),"",nBinsPt,binsPt);
    } // for iflv
  } // for ivar

  // In different eta bins 
  std::vector<std::pair<float, float>> eta_bins;
  eta_bins = {{0.0, 1.3}, 
              {1.3, 2.4}, 
              {2.4, 2.7}, 
              {2.7, 3.0}, 
              {3.0, 5.0}};
  std::vector<TProfile*> pResponseInGenJetPt;
  std::vector<TH1F*> hResponse;
  for (size_t i = 0; i < eta_bins.size(); ++i) {
    std::string name = "pResponseInGenJetPt_JetEtaBin"+std::to_string(i);
    pResponseInGenJetPt.push_back(new TProfile(name.c_str(), name.c_str(), nBinsPt, binsPt));
    name = "hResponse_JetEtaBin"+std::to_string(i);
    hResponse.push_back(new TH1F(name.c_str(), name.c_str(), 30, 0, 3));
  }


  TLorentzVector p4Jet, p4GenJet; 
  Float_t drGenReco[100];
  //------------------------------------
  // Event for loop 
  //------------------------------------
  Long64_t nentries = tree->GetEntries(); 
  cout << "\nStarting loop over " << nentries << " entries" << endl;
  std::cout<<"---------------------------"<<std::endl;
  std::cout<<setw(10)<<"Progress"<<setw(10)<<"Time"<<std::endl;
  std::cout<<"---------------------------"<<std::endl;
  double totTime = 0.0;
	auto startClock = std::chrono::high_resolution_clock::now();
 
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //if (jentry>10000) break;
		if(nentries > 100  && jentry%(nentries/100) == 0){// print after every 1% of events
      totTime+= std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-startClock).count();
      int sec = (int)(totTime)%60;
      int min = (int)(totTime)/60;
	    std::cout<<setw(10)<<100*jentry/nentries<<" %"<<setw(10)<<min<<"m "<<sec<<"s"<<std::endl;
			startClock = std::chrono::high_resolution_clock::now();
		}
   
    // Weight
    double weight = (isMC ? tree->genWeight : 1);
    int countCut = 0;
    hCutflow->Fill(countCut++, weight);
    Long64_t ientry = tree->loadEntry(jentry);
    if (ientry < 0) break; 
    tree->fChain->GetTree()->GetEntry(ientry);

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

		//------------------------------------------------
    // Jet loop: apply JEC
		//------------------------------------------------
    for (int i = 0; i != tree->nJet; ++i) {
      //Undo the correction already applied in NanoAOD
      tree->Jet_pt[i]   = tree->Jet_pt[i] * (1.0 - tree->Jet_rawFactor[i]);
      tree->Jet_mass[i] = tree->Jet_mass[i] * (1.0 - tree->Jet_rawFactor[i]);

      //compute the L1FastJetRC correction and apply to the uncorrected jet;
      double corrL1FastJet = objS->getL1FastJetCorrection(tree->Jet_area[i], tree->Jet_eta[i], tree->Jet_pt[i], tree->Rho);
      tree->Jet_pt[i]   = tree->Jet_pt[i] * corrL1FastJet;
      tree->Jet_mass[i] = tree->Jet_mass[i] * corrL1FastJet;
      p4Jet.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);
    }
    
		//------------------------------------------------
		// Map reco->gen to quickly invert gen->reco, and reset dR
		//------------------------------------------------
		std::map<int, int> genToReco;

		for (int jetIndex = 0; jetIndex != tree->nJet; ++jetIndex) {
				if (tree->Jet_genJetIdx[jetIndex] >= 0) {
						genToReco[tree->Jet_genJetIdx[jetIndex]] = jetIndex;
				}
				
				drGenReco[jetIndex] = 999.0;  // Reset dR
				h2JetEtaJetPt->Fill(std::fabs(tree->Jet_eta[jetIndex]), tree->Jet_pt[jetIndex], weight);
		}

		//------------------------------------------------
		// Loop over genJets and update dR
		//------------------------------------------------
		for (UInt_t genJetIndex = 0; genJetIndex != tree->nGenJet; ++genJetIndex) {
		  // Set 4-momentum of the GenJet
		  p4GenJet.SetPtEtaPhiM(
		    tree->GenJet_pt[genJetIndex], 
		    tree->GenJet_eta[genJetIndex], 
		    tree->GenJet_phi[genJetIndex], 
		    tree->GenJet_mass[genJetIndex]
		  );
		  
		  double deltaR = 999.0;
		  int matchedJetIndex = -1;

		  // Check if there's a matching reco jet
		  auto recoMatch = genToReco.find(genJetIndex);
		  if (recoMatch != genToReco.end()) {
		    matchedJetIndex = recoMatch->second;
		    // Set 4-momentum of the matching reco Jet
		    p4Jet.SetPtEtaPhiM(
		      tree->Jet_pt[matchedJetIndex], 
		      tree->Jet_eta[matchedJetIndex], 
		      tree->Jet_phi[matchedJetIndex], 
		      tree->Jet_mass[matchedJetIndex]
		    );
		    deltaR = p4GenJet.DeltaR(p4Jet);
		    drGenReco[matchedJetIndex] = deltaR;  // Store dR value
		  } else {
		    p4Jet.SetPtEtaPhiM(0, 0, 0, 0);  // No matching jet
		  }

		  h2GenJetEtaGenJetPt->Fill(std::fabs(p4GenJet.Eta()), p4GenJet.Pt(), weight);

		  // Check for matching vertex (temporarily set to always true)
		  // //bool hasMatchVtx = (fabs(tree->PV_z - GenVtx_z) < 0.2);
		  bool hasMatchVtx = true; // FIXME
		  bool hasMatchJet = (deltaR < 0.2 && p4GenJet.Pt() > 0 && p4Jet.Pt() > 0);
      double resp = p4Jet.Pt() / p4GenJet.Pt();

		  if (hasMatchVtx && hasMatchJet) {
		    h2JetEtaGenJetPt->Fill(std::fabs(p4Jet.Eta()), p4GenJet.Pt(), weight);
		    p2JesInJetEtaGenJetPt->Fill(
		    		std::fabs(p4Jet.Eta()), p4GenJet.Pt(), 
		    		(1.0 - tree->Jet_rawFactor[matchedJetIndex]), weight
		    );
		    p2ResponseInJetEtaGenJetPt->Fill( std::fabs(p4Jet.Eta()), p4GenJet.Pt(), resp, weight);

		    // Fill response histograms for different eta bins
		    for (size_t etaBinIdx = 0; etaBinIdx < eta_bins.size(); ++etaBinIdx) {
		      if (std::fabs(p4Jet.Eta()) >= eta_bins[etaBinIdx].first && 
		        std::fabs(p4Jet.Eta()) < eta_bins[etaBinIdx].second) {
		        pResponseInGenJetPt[etaBinIdx]->Fill(p4GenJet.Pt(), resp, weight);
		        hResponse[etaBinIdx]->Fill(resp, weight);
		        break;
		      }
		    }
        //Per flavour
        int flv = (isMC ? tree->GenJet_partonFlavour[genJetIndex] : 99);
        for (int ivar = 0; ivar != nvar; ++ivar) {
          for (int iflv = 0; iflv != nflv; ++iflv) {
            string& svr = avar[ivar]; const char *cv = svr.c_str();
            string& sfl = aflv[iflv]; const char *cf = sfl.c_str();
            if(abs(flv)==5) mp[svr]["b"]->Fill(p4GenJet.Pt(), resp, weight);
            else if(abs(flv)==5) mp[svr]["b"]->Fill(p4GenJet.Pt(), resp, weight);
            else if(abs(flv)==4) mp[svr]["c"]->Fill(p4GenJet.Pt(), resp, weight);
            else if(abs(flv)==3) mp[svr]["s"]->Fill(p4GenJet.Pt(), resp, weight);
            else if(abs(flv)==21) mp[svr]["g"]->Fill(p4GenJet.Pt(), resp, weight);
            else if(abs(flv)<=3 && flv!=0 ) mp[svr]["q"]->Fill(p4GenJet.Pt(), resp, weight);
            else if(abs(flv)<=2 && flv!=0 ) mp[svr]["ud"]->Fill(p4GenJet.Pt(), resp, weight);
          } // for iflv
        } // for ivar
		  }//hasMatch

		  // Record whether a matched vertex and jet were found
		  p2HasMatchedVtxInGenJetEtaGenJetPt->Fill(
		  		std::fabs(p4GenJet.Eta()), p4GenJet.Pt(), 
		  		hasMatchVtx ? 1 : 0, weight
		  );
		  if (hasMatchVtx) {
		  		p2HasMatchedJetInGenJetEtaGenJetPt->Fill(
		  				std::fabs(p4GenJet.Eta()), p4GenJet.Pt(), 
		  				hasMatchJet ? 1 : 0, weight
		  		);
		  }
		}// loop GenJet

		//------------------------------------------------
		// Check fake rates for reco jets
		//------------------------------------------------
		for (int jetIndex = 0; jetIndex != tree->nJet; ++jetIndex) {
		  bool hasMatchVtx = true; // FIXME
		  bool hasMatchJet = (drGenReco[jetIndex] < 0.2);
		  if (hasMatchVtx) {
		    p2HasMatchedJetInJetEtaJetPt->Fill(
		    	std::fabs(tree->Jet_eta[jetIndex]), 
		    	tree->Jet_pt[jetIndex], 
		    	hasMatchJet ? 1 : 0
		    );
		  }
		}

  }//event for loop

  fout->Write();
  cout<<"--------: Cutlow summary :-------"<<endl;
  eventP->printBins(hCutflow);
  eventP->scanTFile(fout);
  cout<<"Output file: "<<fout->GetName()<<endl;
  fout->Close();
  return 0;
}
   
