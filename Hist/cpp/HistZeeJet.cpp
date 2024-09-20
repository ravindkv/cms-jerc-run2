#include "HistZeeJet.h"
   
int HistZeeJet::Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout){
   
  TDirectory *curdir = gDirectory;
  assert(fout && !fout->IsZombie());
  string dir = (isMC ? "MC" : "DATA");

  //pT binning
  double binsPt[] = {15, 20, 25, 30, 35, 40, 50, 60, 75, 90, 110, 130, 175, 230,
  		 300, 400, 500, 600, 700, 850, 1000, 1200, 1450, 1750,
   	  2100, 2500, 3000};
  const int nPt = sizeof(binsPt)/sizeof(binsPt[0])-1;
  //Eta binning
  double binsEta[] = {-5.191, -3.839, -3.489, -3.139, -2.964, -2.853, -2.650,
   	  -2.500, -2.322, -2.172, -1.930, -1.653, -1.479, -1.305,
   	  -1.044, -0.783, -0.522, -0.261, 0.000, 0.261, 0.522, 0.783,
   	  1.044, 1.305, 1.479, 1.653, 1.930, 2.172, 2.322, 2.500,
   	  2.650, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
  const int nEta = sizeof(binsEta)/sizeof(binsEta[0])-1;

  //------------------------------------
  // Cutflow histograms 
  //------------------------------------
  TH1D *hCutflow = new TH1D("hCutflow",";hCutflow",15,0.5,15.5);
  TH1D *hCutflowWeight = new TH1D("hCutflowWeight",";hCutflowWeight",15,0.5,15.5);
  vector<string> cuts;
  cuts.push_back("passHLT");
  cuts.push_back("passGoodLumi");
  cuts.push_back("passMetFilter");
  cuts.push_back("passAtleast1Ref");
  cuts.push_back("passAtleast1Jet");
  cuts.push_back("passJetVetoMap");
  cuts.push_back("passDPhiRefJet1");
  cuts.push_back("passRefBarrel");
  cuts.push_back("passJet1EtaJet2Pt");
  cuts.push_back("passResponse");
  for (int i = 0; i != cuts.size(); ++i) {
    hCutflow->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
    hCutflowWeight->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
  }
  
  //Ref = Z->ee, Z->mumu, Gamma, Jet
  //------------------------------------
  // Variables after at least 1 Ref 
  //------------------------------------
  fout->mkdir("passAtleast1Ref");
  fout->cd("passAtleast1Ref");
  TH1D *hRefPt = new TH1D("hRefPt","",197,15,1000);
  TH1D *hRefPtBarrel = new TH1D("hRefPtBarrel","",197,15,1000);
  TH2D *h2RefEtaRefPhi = new TH2D("h2RefEtaRefPhi","",30,-1.305,+1.305,
   	   	     72,-TMath::Pi(),TMath::Pi());
  // Match barrel edge to 1.305 with 3.132, even though EC edge should be 3.139
  TH2D *h2RefEtaRefPhiRebin = new TH2D("h2RefEtaRefPhiRebin","",360,-3.132,+3.132,
   	   		360,-TMath::Pi(),TMath::Pi());
  TH2D *h2RefEtaRefPhiRefPt110 = new TH2D("h2RefEtaRefPhiRefPt110","",30,-1.305,+1.305,
   	   	     72,-TMath::Pi(),TMath::Pi());
  TH2D *h2RefEtaRefPhiRefPt110Rebin = new TH2D("h2RefEtaRefPhiRefPt110Rebin","",360,-3.132,+3.132,
   	   		360,-TMath::Pi(),TMath::Pi());
  TH2D *h2RefPtCountRef = new TH2D("h2RefPtCountRef","",nPt,binsPt,5,0,5);

  TH1D *hGenRefPt = new TH1D("hGenRefPt","",nPt,binsPt);
  TProfile *pRefPtOgenRefPtInGenRefPt = new TProfile("pRefPtOgenRefPtInGenRefPt","",nPt,binsPt);// O = Over
  TProfile *pGenRefPtORefPtInRefPt  = new TProfile("pGenRefPtORefPtInRefPt","",nPt,binsPt);
   
  //------------------------------------
  // Variables after leading Ref in barrel (eta < 1.33) 
  //------------------------------------
  fout->mkdir("passRefBarrel");
  fout->cd("passRefBarrel");
  // Follow up on problematic cuts
  TH1D *hDphiJetRef = new TH1D("hDphiJetRef",";#Delta#phi;N_{events}",
   	   	 126,-TMath::TwoPi(),TMath::TwoPi());
  TH1D *hDrJetRef = new TH1D("hDrJetRef",";#DeltaR;N_{events}",100,0,10);
  TH2D *h2RefPtDbResp = new TH2D("h2RefPtDbResp","",nPt,binsPt,200,0,4);
  TH2D *h2RefPtMpfResp = new TH2D("h2RefPtMpfResp","",nPt,binsPt,300,-2,4);
  // Plots for jet properties
  TProfile *pGenJ1ptOprobePtInRefPt = new TProfile("pGenJ1ptOprobePtInRefPt","",nPt,binsPt);
  TProfile *pJ1ptOprobePtInRefPt = new TProfile("pJ1ptOprobePtInRefPt","",nPt,binsPt);
  TProfile *pJ1ptOgenJ1ptInGenJ1pt = new TProfile("pJ1ptOgenJ1ptInGenJ1pt","",nPt,binsPt);
   
  fout->mkdir("passRefBarrel/flavourXTaggedY");
  fout->cd("passRefBarrel/flavourXTaggedY");
  map<string, double> mvar;
  map<string, map<string, map<string, TH1*> > > mp;
  string avar[] = {"hRefPt",
                  "pMpfRespInRefPt",
                  "pDbRespInRefPt",
                  "pMpfResp1InRefPt",
                  "pMpfRespNInRefPt",
                  "pMpfRespUInRefPt",
                  "pRhoInRefPt",
                  "pJet1PtORefPtInRefPt",
                  "pGenJet1PtORefPtInRefPt",
                  "pJet1PtOGenJet1PtInGenJet1Pt"};
  string atag[] = {"i","b","c","q","g","n"};
  string aflv[] = {"i","b","c","q","s","ud","g","n"};
  const int nvar = sizeof(avar)/sizeof(avar[0]);
  const int ntag = sizeof(atag)/sizeof(atag[0]);
  const int nflv = sizeof(aflv)/sizeof(aflv[0]);
  for (int ivar = 0; ivar != nvar; ++ivar) {
    for (int itag = 0; itag != ntag; ++itag) {
      for (int iflv = 0; iflv != nflv; ++iflv) {
        string& var = avar[ivar]; const char *cv = var.c_str();
        string& tag = atag[itag]; const char *ct = tag.c_str();
        string& flv = aflv[iflv]; const char *cf = flv.c_str();
        if (var=="hGenJet1Pt")
          mp[var][tag][flv] = new TH1D(Form("%s_%s%s",cv,ct,cf),"",nPt,binsPt);
        else
          mp[var][tag][flv] = new TProfile(Form("%s_%s%s",cv,ct,cf),"",nPt,binsPt);
      } // for iflv
    } // for itag
  } // for ivar


  //------------------------------------
  // Variables after Jet1Eta, Jet2Pt cut 
  //------------------------------------
  fout->mkdir("passJet1EtaJet2Pt");
  fout->cd("passJet1EtaJet2Pt");
  TProfile *pJetEtaInRefPt = new TProfile("pJetEtaInRefPt","",nPt,binsPt);
  TH2D *h2JetEtaRefPt = new TH2D("h2JetEtaRefPt","",nPt,binsPt,nEta,binsEta);
  TProfile *pGenJ2ptOprobePtInRefPt = new TProfile("pGenJ2ptOprobePtInRefPt","",nPt,binsPt);
  TProfile *pJ2ptOprobePtInRefPt = new TProfile("pJ2ptOprobePtInRefPt","",nPt,binsPt);
  TProfile *pJ2ptOgenJ2ptInGenJ2pt = new TProfile("pJ2ptOgenJ2ptInGenJ2pt","",nPt,binsPt);

  // 1D composition and response
  // p = Profile
  // Db = Direct Balance, MPF = MET Projection Fraction
  // Resp = Response
  // Chhef = Charged Hadron Energy Fraction
  // Nehef = Neutral Hadron Energy Fraction
  // Neemef = Neutral ElectronMagnetic Energy Fraction
  // Chemef = Charged ElectronMagnetic Energy Fraction
  // Muef = Muon Energy Fraction
  // Chpv0ef = Charged PV=0 Energy Fraction
  TProfile *pDbRespInRefPt  = new TProfile("pDbRespInRefPt","",nPt,binsPt);
  TProfile *pMpfRespInRefPt = new TProfile("pMpfRespInRefPt","",nPt,binsPt);
  TProfile *pJetChhefInRefPt = new TProfile("pJetChhefInRefPt","",nPt,binsPt);
  TProfile *pJetNehefInRefPt = new TProfile("pJetNhefInRefPt","",nPt,binsPt);
  TProfile *pJetNeemefInRefPt  = new TProfile("pJetNeemefInRefPt","",nPt,binsPt);
  TProfile *pJetChemefInRefPt  = new TProfile("pJetChemefInRefPt","",nPt,binsPt);
  TProfile *pJetMuefInRefPt  = new TProfile("pJetMuefInRefPt","",nPt,binsPt);
  TProfile *pJetChpv0efInRefPt  = new TProfile("pJetChpv0efInRefPt","",nPt,binsPt);
  
  // for (int i = 0; i != 72; ++i) cout << Form("%1.3f, ",-TMath::Pi()+i*TMath::TwoPi()/72.); cout << endl;
  const int nPhi = 72;
  const double binsPhi[nPhi+1] = 
    {-3.142, -3.054, -2.967, -2.880, -2.793, -2.705, -2.618, -2.531, -2.443,
     -2.356, -2.269, -2.182, -2.094, -2.007, -1.920, -1.833, -1.745, -1.658,
     -1.571, -1.484, -1.396, -1.309, -1.222, -1.134, -1.047, -0.960, -0.873,
     -0.785, -0.698, -0.611, -0.524, -0.436, -0.349, -0.262, -0.175, -0.087,
     0.000, 0.087, 0.175, 0.262, 0.349, 0.436, 0.524, 0.611, 0.698, 0.785,
     0.873, 0.960, 1.047, 1.134, 1.222, 1.309, 1.396, 1.484, 1.571, 1.658,
     1.745, 1.833, 1.920, 2.007, 2.094, 2.182, 2.269, 2.356, 2.443, 2.531,
     2.618, 2.705, 2.793, 2.880, 2.967, 3.054,3.142};
  // 2D composition and response
  TProfile2D *p2DbRespInJetEtaJetPhiRefPt230  = new TProfile2D("p2DbRespInJetEtaJetPhiRefPt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2MpfRespInJetEtaJetPhiRefPt230 = new TProfile2D("p2MpfRespInJetEtaJetPhiRefPt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetChhefInJetEtaJetPhiRefPt230 = new TProfile2D("p2JetChhefInJetEtaJetPhiRefPt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetNehefInJetEtaJetPhiRefPt230 = new TProfile2D("p2JetNehefInJetEtaJetPhiRefPt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetNeemefInJetEtaJetPhiRefPt230 = new TProfile2D("p2JetNeemefInJetEtaJetPhiRefPt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetChemefInJetEtaJetPhiRefPt230 = new TProfile2D("p2JetChemefInJetEtaJetPhiRefPt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetMuefInJetEtaJetPhiRefPt230 = new TProfile2D("p2JetMuefInJetEtaJetPhiRefPt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetChfpv0efInJetEtaJetPhiRefPt230 = new TProfile2D("p2JetChfpv0efInJetEtaJetPhiRefPt230","",nEta,binsEta,nPhi,binsPhi);

  // Plots of npv, npvgood
  TProfile *pRhoInRefPt = new TProfile("pRhoInRefPt","",nPt,binsPt);
  TProfile *pNpvgoodInRefPt = new TProfile("pNpvgoodInRefPt","",nPt,binsPt);
  TProfile *pNpvInRefPt = new TProfile("pNpvInRefPt","",nPt,binsPt);

  TH2D *h2RefPtDbRespPassMpf = new TH2D("h2RefPtDbRespPassMpf","",nPt,binsPt,200,0,4);
  TH2D *h2RefPtMpfRespPassDb = new TH2D("h2RefPtMpfRespPassDb","",nPt,binsPt,300,-2,4);
  TH2D *h2RefPtDbRespPassBoth = new TH2D("h2RefPtDbRespPassBoth","",nPt,binsPt,200,0,4);
  TH2D *h2RefPtMpfRespPassBoth = new TH2D("h2RefPtMpfRespPassBoth","",nPt,binsPt,300,-2,4);

  //------------------------------------
  // Variables after response cuts 
  //------------------------------------
  fout->mkdir("passResponse");
  fout->cd("passResponse");
   
  Int_t runN = 20000;
  Double_t runMin = 370000.5;
  Double_t runMax = 390000.5;

  // Time stability of xsec
  TH1D *hRunRefPt30 = new TH1D("hRunRefPt30",";Run;N_{events};",runN, runMin, runMax);
  TH1D *hRunRefPt110 = new TH1D("hRunRefPt110",";Run;N_{events};",runN, runMin, runMax);
  TH1D *hRunRefPt230 = new TH1D("hRunRefPt230",";Run;N_{events};",runN, runMin, runMax);
  
  // Time stability of JEC
  TProfile *pDbRespInRunRefPt30 = new TProfile("pDbRespInRunRefPt30",";Run;BAL;",runN, runMin, runMax);
  TProfile *pDbRespInRunRefPt110 = new TProfile("pDbRespInRunRefPt110",";Run;BAL;",runN, runMin, runMax);
  TProfile *pDbRespInRunRefPt230 = new TProfile("pDbRespInRunRefPt230",";Run;BAL;",runN, runMin, runMax);
  TProfile *pMpfRespInRunRefPt30 = new TProfile("pMpfRespInRunRefPt30",";Run;MPF;",runN, runMin, runMax);
  TProfile *pMpfRespInRunRefPt110 = new TProfile("pMpfRespInRunRefPt110",";Run;MPF;",runN, runMin, runMax);
  TProfile *pMpfRespInRunRefPt230 = new TProfile("pMpfRespInRunRefPt230",";Run;MPF;",runN, runMin, runMax);
   
  // Time stability of PF composition
  TProfile *pJetChhefInRunRefPt30 = new TProfile("pJetChhefInRunRefPt30",";Run;CHF;",runN, runMin, runMax);
  TProfile *pJetChhefInRunRefPt110 = new TProfile("pJetChhefInRunRefPt110",";Run;CHF;",runN, runMin, runMax);
  TProfile *pJetChhefInRunRefPt230 = new TProfile("pJetChhefInRunRefPt230",";Run;CHF;",runN, runMin, runMax);

  TProfile *pJetNehefInRunRefPt30 = new TProfile("pJetNehefInRunRefPt30",";Run;NHF;",runN, runMin, runMax);
  TProfile *pJetNehefInRunRefPt110 = new TProfile("pJetNehefInRunRefPt110",";Run;NHF;",runN, runMin, runMax);
  TProfile *pJetNehefInRunRefPt230 = new TProfile("pJetNehefInRunRefPt230",";Run;NHF;",runN, runMin, runMax);

  TProfile *pJetNeemefInRunRefPt30 = new TProfile("pJetNeemefInRunRefPt30",";Run;NHF;",runN, runMin, runMax);
  TProfile *pJetNeemefInRunRefPt110 = new TProfile("pJetNeemefInRunRefPt110",";Run;NHF;",runN, runMin, runMax);
  TProfile *pJetNeemefInRunRefPt230 = new TProfile("pJetNeemefInRunRefPt230",";Run;NHF;",runN, runMin, runMax);
   
  curdir->cd();
  
  //------------------------------------
  // Event for loop 
  //------------------------------------
  TLorentzVector p4Ref, p4GenRef, p4Jet1, p4Jet2, p4Jetn;
  TLorentzVector p4Met, p4Met1, p4Metn, p4Metu, p4Metnu, p4RawMet, p4CorrMet, p4RawRef;
  TLorentzVector p4Jeti, p4CorrJets, p4RawJet, p4RawJets, p4RcJets, p4RcOffsets;
  TLorentzVector p4GenJeti, p4GenJet1, p4GenJet2;
  TLorentzVector p4Refx; // for MPFX
   
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
    if(isDebug) cout<<"Loaded jentry = "<<jentry<<endl;

    // Weight
    double weight = (isMC ? tree->genWeight : 1);
    //------------------------------------
    // trigger and golden lumi, MET filter selection 
    //------------------------------------
    if(!eventP->passHLT(tree)) continue; 
    if(isDebug) cout<<"passHLT"<<endl;
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

    if(!eventP->passFilter(tree)) continue; 
    if(isDebug) cout<<"passMetFilter"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    //------------------------------------------
    // Select objects
    //------------------------------------------
    // Reco objects
    objP->clearObjects();
    if(isZeeJet) objP->pickElectrons();
    objP->pickRefs();
    vector<TLorentzVector> p4Refs = objP->pickedRefs;

    int nRef(0);
    nRef = p4Refs.size();
    if(nRef<1) continue; 
    if(isDebug) cout<<"passAtleast1Ref"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);
    p4Ref = p4Refs.at(0);
    p4RawRef = p4Refs.at(0);
    double ptRef = p4Ref.Pt();
    if(!ptRef>0.0) continue;

    //Fill histograms
    h2RefPtCountRef->Fill(ptRef, nRef, weight);
    hRefPt ->Fill(ptRef, weight);
    h2RefEtaRefPhi->Fill(p4RawRef.Eta(), p4RawRef.Phi(), weight);
    h2RefEtaRefPhiRebin->Fill(p4RawRef.Eta(), p4RawRef.Phi(), weight);
    if (ptRef>=110){
      h2RefEtaRefPhiRefPt110->Fill(p4RawRef.Eta(), p4RawRef.Phi(), weight);
      h2RefEtaRefPhiRefPt110Rebin->Fill(p4RawRef.Eta(), p4RawRef.Phi(), weight);
    }

    // Gen objects
    p4GenRef.SetPtEtaPhiM(0,0,0,0);
    if(isMC){
      if(isZeeJet) objP->pickGenElectrons();
      objP->pickGenRefs();
      vector<TLorentzVector> p4GenRefs = objP->pickedGenRefs;
      if(p4GenRefs.size()<1) continue;
      p4GenRef = p4GenRefs.at(0);
      if (p4GenRef.Pt()>0 && fabs(p4GenRef.Eta()) < 1.3) {
        hGenRefPt->Fill(p4GenRef.Pt(), weight);
      }
      if (fabs(p4Ref.Eta()) < 1.3) {
        if (p4GenRef.Pt()>0) {
          hRefPtBarrel->Fill(ptRef, weight);
          pRefPtOgenRefPtInGenRefPt->Fill(p4GenRef.Pt(), ptRef / p4GenRef.Pt(), weight);
          pGenRefPtORefPtInRefPt->Fill(ptRef, p4GenRef.Pt() / ptRef, weight);
        }
      }
    }//isMC

    //------------------------------------------------
    // Jet loop: Apply JEC
    //------------------------------------------------
    // Select leading jets. Just exclude Ref, don't apply JetID yet
    Float_t     Jet_resFactor[tree->nJetMax]; // Custom addition
    Float_t     Jet_deltaJES[tree->nJetMax]; // Custom addition
    Float_t     Jet_CF[tree->nJetMax]; // Custom addition
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
      //Jet_RES[i] = 1./res;
      res  = 1.0;
      Jet_deltaJES[i] = (1./corrs) / (1.0 - tree->Jet_rawFactor[i]);
      tree->Jet_pt[i] = rawJetPt ;
      tree->Jet_mass[i] = rawJetMass; 
      tree->Jet_rawFactor[i] = (1.0 - 1.0/corrs);
      Jet_resFactor[i] = (1.0 - 1.0/res);
      // Smear jets
      if (smearJets) {
        //assert(false);
      }
     
      // Check that jet is not Ref and pTcorr>15 GeV
      bool pass_JetId = tree->Jet_jetId[i] & (1 << 2); // tightLepVeto
      if (tree->Jet_pt[i]>15 && pass_JetId) {
        p4Jeti.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);
        if (p4Ref.DeltaR(p4Jeti)<0.2) continue; // should not happen, but does?
        ++nJets;
      
        if (iJet==-1) { // Leading jet for balance
          iJet = i;
          p4Jet1 = p4Jeti;
          djes = Jet_deltaJES[i];
          jes = (1.-tree->Jet_rawFactor[i]);
          res = (1.-Jet_resFactor[i]);
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
      } // non-Ref jet
    } // for i in nJet
    
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
    if(jvNumber>0) continue; // passJetVetoMap
    } catch (const std::exception& e) {
      cout<<"\nEXCEPTION: in objS->loadedJetVetoRef: "<<e.what()<<endl;
      std::abort();
    }
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);
     
    //------------------------------------------------
    // GenJet loop
    //------------------------------------------------
    // Select p4GenJet1 matching leading and subleading reco jet
    int iGenJet(-1), iGenJet2(-1);
    p4GenJet1.SetPtEtaPhiM(0,0,0,0);
    p4GenJet2.SetPtEtaPhiM(0,0,0,0);
    if (isMC) {
      for (Int_t i = 0; i != tree->nGenJet; ++i) {
   	      p4GenJeti.SetPtEtaPhiM(tree->GenJet_pt[i],tree->GenJet_eta[i],
                  tree->GenJet_phi[i], tree->GenJet_mass[i]);
        if (iJet!=-1 && p4GenJeti.DeltaR(p4Jet1)<0.4 && iGenJet==-1) {
          iGenJet = i;
          p4GenJet1 = p4GenJeti;
        }
        else if (iJet2!=-1 && p4GenJeti.DeltaR(p4Jet2)<0.4 && iGenJet2==-1) {
          iGenJet2 = i;
          p4GenJet2 = p4GenJeti;
        }
      } // for i in tree->nGenJet
    } // isMC
    bool pass_gen = (iGenJet!=-1);
   
    //------------------------------------------------
    // Set MET vectors
    //------------------------------------------------
    p4RawMet.SetPtEtaPhiM(tree->RawPuppiMET_pt, 0, tree->RawPuppiMET_phi, 0);
    p4RawMet += p4RawRef - p4Ref; // replace PF Ref with Reco Ref
    p4Met1 = -p4Jet1 - p4Ref;
    p4Metn = -p4Jetn;
    //p4CorrMet = p4RawMet +p4RawJets -p4CorrJets -p4RcOffsets;
    p4CorrMet = p4RawMet +p4RcJets -p4CorrJets; // same as above
    // Unclustered MET from rawMET by taking out all the hard stuff
    // p4Metu = p4RawMet +Ref +p4RawJets -p4RcOffsets;
    // p4Metu = p4RawMet +Ref +p4RcJets;
    // Or equally well, from corrMET (modulo rounding errors)
    p4Metu = p4CorrMet + p4Ref +p4CorrJets;
    p4Metnu = p4Metn + 1.1*p4Metu;
    p4Met = p4CorrMet;
    
    // Make MET transverse
    p4CorrMet.SetPz(0);
    p4Met.SetPz(0);
    p4Metn.SetPz(0);
    p4Met1.SetPz(0);
    p4Metu.SetPz(0);
    
    //------------------------------------------------
    // Calculate basic variables
    //------------------------------------------------
    double ptjet = p4Jet1.Pt();
    double abseta = fabs(p4Jet1.Eta());
    double ptJet2 = p4Jet2.Pt();
    double ptJet2Min = 30;
    double bal(0), mpf(0), mpf1(0), mpfn(0), mpfu(0), mpfnu(0);
    double mpfx(0), mpf1x(0), mpfnPt(0), mpfux(0), mpfnux(0);
    bal = ptjet / ptRef;
    mpf = 1 + p4Met.Vect().Dot(p4Ref.Vect()) / (ptRef*ptRef);
    mpf1 = 1 + p4Met1.Vect().Dot(p4Ref.Vect()) / (ptRef*ptRef);
    mpfn = p4Metn.Vect().Dot(p4Ref.Vect()) / (ptRef*ptRef);
    mpfu = p4Metu.Vect().Dot(p4Ref.Vect()) / (ptRef*ptRef);
    mpfnu = p4Metnu.Vect().Dot(p4Ref.Vect()) / (ptRef*ptRef);
    //
    p4Refx.SetPtEtaPhiM(p4Ref.Pt(),p4Ref.Eta(),p4Ref.Phi()+0.5*TMath::Pi(),0.);
    mpfx = 1 + p4Met.Vect().Dot(p4Refx.Vect()) / (ptRef*ptRef);
    mpf1x = 1 + p4Met1.Vect().Dot(p4Refx.Vect()) / (ptRef*ptRef);
    mpfnPt = p4Metn.Vect().Dot(p4Refx.Vect()) / (ptRef*ptRef);
    mpfux = p4Metu.Vect().Dot(p4Refx.Vect()) / (ptRef*ptRef);
    mpfnux = p4Metnu.Vect().Dot(p4Refx.Vect()) / (ptRef*ptRef);
     
    // Sanity checks for HDM inputs
    if (!(fabs(mpf1+mpfn+mpfu-mpf)<1e-4)) {
      cout << "\nHDM input error: mpf=" << mpf << " mpf1=" << mpf1
        << " mpfn=" << mpfn << " mpfu=" << mpfu << endl;
      cout << "Difference = " << mpf1+mpfn+mpfu-mpf << endl << flush;
      //assert(false);
      cout << "Skip entry " << jentry
        << " ("<<tree->run<<","<<tree->luminosityBlock<<","<<tree->event<<")"
        << " in file " << fout->GetName() << endl << flush;
      continue;
    }
   
    bool pass_dPhiProbJet1 = (fabs(p4Ref.DeltaPhi(p4Jet1)) > 2.7); // pi-0.44 as in KIT Ref+j
    if(!pass_dPhiProbJet1) continue; 
    if(isDebug) cout<<"passDPhiRefJet1"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    if(fabs(p4Ref.Eta()) > 1.3) continue; 
    if(isDebug) cout<<"passRefBarrel"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);
   	hDphiJetRef->Fill(p4Ref.DeltaPhi(p4Jet1), weight);
   	hDrJetRef->Fill(p4Ref.DeltaR(p4Jet1), weight);

    if (pass_gen || isData) {
      pJ1ptOprobePtInRefPt->Fill(ptRef, p4Jet1.Pt() / ptRef, weight);
    }
    if (pass_gen) {
      pGenJ1ptOprobePtInRefPt->Fill(ptRef, p4GenJet1.Pt() / ptRef, weight);
      pJ1ptOgenJ1ptInGenJ1pt->Fill(p4GenJet1.Pt(), p4Jet1.Pt() / p4GenJet1.Pt(), weight);
    }
    int flv = (isMC ? tree->GenJet_partonFlavour[iGenJet] : 99);
    mvar["hRefPt"] = 1;
    mvar["pMpfRespInRefPt"] = mpf;
    mvar["pDbRespInRefPt"] = bal;
    mvar["pMpfResp1InRefPt"] = mpf1;
    mvar["pMpfRespNInRefPt"] = mpfn;
    mvar["pMpfRespUInRefPt"] = mpfu;
    mvar["pRhoInRefPt"] = tree->Rho;
    mvar["pJet1PtORefPtInRefPt"] = (p4Jet1.Pt() / ptRef);
    mvar["pGenJet1PtORefPtInRefPt"] = (p4GenJet1.Pt() / ptRef);
    mvar["pJet1PtOGenJet1PtInGenJet1Pt"] = (p4GenJet1.Pt()!=0 ? p4Jet1.Pt() / p4GenJet1.Pt() : 0);
    
    tree->Jet_btagDeepB[iJet] = tree->Jet_btagDeepFlavB[iJet];
    tree->Jet_btagDeepC[iJet] = 0.5*(tree->Jet_btagDeepFlavCvB[iJet] +
    		     tree->Jet_btagDeepFlavCvL[iJet]);
    tree->Jet_qgl[iJet] = tree->Jet_btagDeepFlavQG[iJet];
    bool isb = (tree->Jet_btagDeepB[iJet] > objS->bThresh);
    bool isc = (tree->Jet_btagDeepC[iJet] > objS->cThresh && !isb);
    bool isq = (tree->Jet_qgl[iJet]>=0.5 && tree->Jet_qgl[iJet] && !isb && !isc);
    bool isg = (tree->Jet_qgl[iJet]>=0 && tree->Jet_qgl[iJet]<0.5 && !isb && !isc);
    bool isn = (!isb && !isc && !isq && !isg);
    
    for (int ivar = 0; ivar != nvar; ++ivar) {
      for (int itag = 0; itag != ntag; ++itag) {
        for (int iflv = 0; iflv != nflv; ++iflv) {
          string& svr = avar[ivar]; const char *cv = svr.c_str();
          string& stg = atag[itag]; const char *ct = stg.c_str();
          string& sfl = aflv[iflv]; const char *cf = sfl.c_str();
          
          if (((sfl=="i") ||
             (sfl=="b" && abs(flv)==5) ||
             (sfl=="c" && abs(flv)==4) ||
             (sfl=="q" && abs(flv)<=3 && flv!=0) ||
             (sfl=="s" && abs(flv)==3) ||
             (sfl=="ud" && abs(flv)<=2 && flv!=0) ||
             (sfl=="g" && flv==21) ||
             (sfl=="n" && flv==0)) &&
            ((stg=="i") ||
             (stg=="b" && isb) ||
             (stg=="c" && isc) ||
             (stg=="q" && isq) ||
             (stg=="g" && isg) ||
             (stg=="n" && isn))) {
             double x = ptRef;
             if (svr=="pJet1PtOGenJet1PtInGenJet1Pt") x = p4GenJet1.Pt();
             if ((svr=="pJet1PtORefPtInRefPt" || svr=="pGenJet1PtORefPtInRefPt") && iGenJet==-1) x = 0;
             double var = mvar[svr];
             TH1* h = mp[svr][stg][sfl];
             if (!h) {
               cout << "Missing "<<svr<<stg<<sfl<<endl<<flush;
               assert(h);
             }
          
             if (svr=="hRefPt")
               ((TH1D*)h)->Fill(x, weight);
             else
               ((TProfile*)h)->Fill(x, var, weight);
          }
        } // for iflv
      } // for itag
    } // for ivar
  

    bool pass_Jet1Eta = (abseta < 1.3);
    bool pass_Jet2Pt = (ptJet2 < ptRef || ptJet2 < ptJet2Min);    
    if(!(pass_Jet1Eta &&  pass_Jet2Pt)) continue; 
    if(isDebug) cout<<"passJet1EtaJet2Pt"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);
    h2RefPtDbResp->Fill(ptRef, bal, weight);
    h2RefPtMpfResp->Fill(ptRef, mpf, weight);
    if (p4Jet2.Pt()>0) {
      if (iGenJet2!=-1 || !isMC) {
        pJ2ptOprobePtInRefPt->Fill(ptRef, p4Jet2.Pt() / ptRef, weight);
      }
      if (iGenJet2!=-1) {
        pGenJ2ptOprobePtInRefPt->Fill(ptRef, p4GenJet2.Pt() / ptRef, weight);
        pJ2ptOgenJ2ptInGenJ2pt->Fill(p4GenJet2.Pt(), p4Jet2.Pt() / p4GenJet2.Pt(), weight);
      }
    }
    // Control plots for Ref-jet 
    // PF composition plots
    h2JetEtaRefPt->Fill(ptRef, tree->Jet_eta[iJet], weight);
    pJetEtaInRefPt->Fill(ptRef, fabs(tree->Jet_eta[iJet]), weight);
    
    // 1D composition and response
    pDbRespInRefPt->Fill(ptRef, bal, weight);
    pMpfRespInRefPt->Fill(ptRef, mpf, weight);
    pJetChhefInRefPt->Fill(ptRef, tree->Jet_chHEF[iJet], weight);
    pJetNehefInRefPt->Fill(ptRef, tree->Jet_neHEF[iJet], weight);
    pJetNeemefInRefPt->Fill(ptRef, tree->Jet_neEmEF[iJet], weight);
    pJetChemefInRefPt->Fill(ptRef, tree->Jet_chEmEF[iJet], weight);
    pJetMuefInRefPt->Fill(ptRef, tree->Jet_muEF[iJet], weight);
    //pJetChpv0efInRefPt->Fill(ptRef, tree->Jet_chFPV0EF[iJet], weight);
    
    pRhoInRefPt->Fill(ptRef, tree->Rho, weight);
    pNpvgoodInRefPt->Fill(ptRef, tree->PV_npvsGood, weight);
    pNpvInRefPt->Fill(ptRef, tree->PV_npvs, weight);
    // 2D composition and response
    if (ptRef>230) {
      double eta = tree->Jet_eta[iJet];
      double phi = tree->Jet_phi[iJet];
      p2DbRespInJetEtaJetPhiRefPt230->Fill(eta, phi, bal, weight);
      p2MpfRespInJetEtaJetPhiRefPt230->Fill(eta, phi, mpf, weight);
      p2JetChhefInJetEtaJetPhiRefPt230->Fill(eta, phi, tree->Jet_chHEF[iJet], weight);
      p2JetNehefInJetEtaJetPhiRefPt230->Fill(eta, phi, tree->Jet_neHEF[iJet], weight);
      p2JetNeemefInJetEtaJetPhiRefPt230->Fill(eta, phi, tree->Jet_neEmEF[iJet], weight);
      p2JetChemefInJetEtaJetPhiRefPt230->Fill(eta, phi, tree->Jet_chEmEF[iJet], weight);
      p2JetMuefInJetEtaJetPhiRefPt230->Fill(eta, phi, tree->Jet_muEF[iJet], weight);
      //p2JetChfpv0efInJetEtaJetPhiRefPt230->Fill(eta, phi, tree->Jet_chFPV0EF[iJet], weight);
    } // high pT range
    

    bool pass_DbResp = (fabs(1-bal)<0.7);
    bool pass_MpfResp = (fabs(1-mpf)<0.7);
    if (pass_MpfResp) h2RefPtDbRespPassMpf->Fill(ptRef, bal, weight);
    if (pass_DbResp) h2RefPtMpfRespPassDb->Fill(ptRef, mpf, weight);
    if (pass_MpfResp && pass_DbResp) h2RefPtDbRespPassBoth->Fill(ptRef, bal, weight);
    if (pass_MpfResp && pass_DbResp) h2RefPtMpfRespPassBoth->Fill(ptRef, mpf, weight);

    if(!(pass_DbResp && pass_MpfResp)) continue; 
    if(isDebug) cout<<"passResponse"<<endl;
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    if (ptRef>30) {
      hRunRefPt30->Fill(tree->run, weight); 
      pDbRespInRunRefPt30->Fill(tree->run, bal, weight); 
      pMpfRespInRunRefPt30->Fill(tree->run, mpf, weight);
      pJetChhefInRunRefPt30->Fill(tree->run, tree->Jet_chHEF[iJet], weight);
      pJetNehefInRunRefPt30->Fill(tree->run, tree->Jet_neHEF[iJet], weight);
      pJetNeemefInRunRefPt30->Fill(tree->run, tree->Jet_neEmEF[iJet], weight);
    }
    if (ptRef>110) {
      hRunRefPt110->Fill(tree->run, weight);
      pDbRespInRunRefPt110->Fill(tree->run, bal, weight); 
      pMpfRespInRunRefPt110->Fill(tree->run, mpf, weight);
      pJetChhefInRunRefPt110->Fill(tree->run, tree->Jet_chHEF[iJet], weight);
      pJetNehefInRunRefPt110->Fill(tree->run, tree->Jet_neHEF[iJet], weight);
      pJetNeemefInRunRefPt110->Fill(tree->run, tree->Jet_neEmEF[iJet], weight);
    }
    if (ptRef>230) {
      hRunRefPt230->Fill(tree->run, weight);
      pDbRespInRunRefPt230->Fill(tree->run, bal, weight); 
      pMpfRespInRunRefPt230->Fill(tree->run, mpf, weight);
      pJetChhefInRunRefPt230->Fill(tree->run, tree->Jet_chHEF[iJet], weight);
      pJetNehefInRunRefPt230->Fill(tree->run, tree->Jet_neHEF[iJet], weight);
      pJetNeemefInRunRefPt230->Fill(tree->run, tree->Jet_neEmEF[iJet], weight);
    }
  }//event for loop
  cout << "\n ===========> Processed " << nentries << " events <=============\n";
  cout<<setw(20)<<"CUT"<< setw(10)<<"ENTRIES" << setw(10)<< "WEIGHT"<< setw(10)<<"CHANGE"<<endl;
  for (int i=1; i<hCutflow->GetNbinsX(); i++){
    int previous = 0;
    if(i==1) previous = nentries; 
    else previous = hCutflow->GetBinContent(i-1);
    int current   = hCutflow->GetBinContent(i);
    int currentWithWeight   = hCutflowWeight->GetBinContent(i);
    double change = ((previous - current) / static_cast<double>(previous)) * 100;
    double weight_ = currentWithWeight/static_cast<double>(current);
    cout<<setw(20)<<hCutflow->GetXaxis()->GetBinLabel(i)<<
    setw(10)<<current << setw(10)<< weight_<< setw(5)<<" - "<<change<<" %" <<endl;
  }

  // Add extra plot for jet response vs Ref pT
  if (isMC) {
    fout->cd("passRefBarrel");
    TH1D *hrgenvgen = pJ1ptOgenJ1ptInGenJ1pt->ProjectionX("hrgenvgen");
    TH1D *hrgenvRef = pJ1ptOprobePtInRefPt->ProjectionX("hrgenvRef");
    hrgenvRef->Divide(pGenJ1ptOprobePtInRefPt);
    fout->cd("passJet1EtaJet2Pt");
    TH1D *hrgen2vgen = pJ2ptOgenJ2ptInGenJ2pt->ProjectionX("hrgen2vgen");
    TH1D *hrgen2vRef = pJ2ptOprobePtInRefPt->ProjectionX("hrgen2vRef");
    hrgen2vRef->Divide(pGenJ2ptOprobePtInRefPt);
    curdir->cd();
  }
  fout->Write();
  cout<<"Output file: "<<fout->GetName()<<endl;
  fout->Close();
  return 0;
}
   
