#include "HistZeeJet.h"
   
// Constructor implementation
HistZeeJet::HistZeeJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}


int HistZeeJet::Run(std::shared_ptr<SkimTree>& skimT, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout){
   
  TDirectory *curdir = gDirectory;
  assert(fout && !fout->IsZombie());
  std::string dir = (globalFlags_.isMC() ? "MC" : "DATA");
  
  // pT binning
  std::vector<double> binsPt = {15, 20, 25, 30, 35, 40, 50, 60, 75, 90, 110, 130, 175, 230,
  															300, 400, 500, 600, 700, 850, 1000, 1200, 1500};
  const int nPt = binsPt.size() - 1;
  
  // Eta binning
  std::vector<double> binsEta = {-5.191, -3.839, -3.489, -3.139, -2.964, -2.853, -2.650,
  															 -2.500, -2.322, -2.172, -1.930, -1.653, -1.479, -1.305,
  															 -1.044, -0.783, -0.522, -0.261, 0.000, 0.261, 0.522, 0.783,
  															 1.044, 1.305, 1.479, 1.653, 1.930, 2.172, 2.322, 2.500,
  															 2.650, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
  const int nEta = binsEta.size() - 1;
  
  //------------------------------------
  // Cutflow histograms
  //------------------------------------
  std::vector<std::string> cuts = {
  		"passSkim", "passHLT", "passGoodLumi", "passMetFilter", "passAtleast1Ref",
  		"passAtleast1Jet", "passJetVetoMap", "passDPhiRefJet1", "passRefBarrel",
  		"passJet1EtaJet2Pt", "passResponse"
  };
  
  // Create the histogram with the number of bins equal to the number of cuts
  auto h1Cutflow = std::make_unique<TH1D>("h1Cutflow", ";Cutflow;Events", cuts.size(), 0.5, cuts.size() + 0.5);
  
  // Map each cut name to a bin number (1-based bin number for ROOT)
  std::map<std::string, int> cutToBinMap;
  for (size_t i = 0; i < cuts.size(); ++i) {
  		cutToBinMap[cuts[i]] = i + 1;  // Bin numbers are 1-based in ROOT
  		h1Cutflow->GetXaxis()->SetBinLabel(i + 1, cuts[i].c_str());  // Set bin labels
  }
  
  //------------------------------------
  // Variables after at least 1 Ref
  //------------------------------------
  fout->mkdir("passAtleast1Ref");
  fout->cd("passAtleast1Ref");
  
  auto h1RefPt = std::make_unique<TH1D>("h1RefPt", ";Reference #it{p}_{T} (GeV);Events", 197, 15, 1000);
  auto h1RefPtBarrel = std::make_unique<TH1D>("h1RefPtBarrel", ";Barrel Reference #it{p}_{T} (GeV);Events", 197, 15, 1000);
  
  auto h2RefEtaRefPhi = std::make_unique<TH2D>("h2RefEtaRefPhi", ";Reference #eta;Reference #phi", 30, -1.305, 1.305,
  																						 72, -TMath::Pi(), TMath::Pi());
  
  // Match barrel edge to 1.305 with 3.132, even though EC edge should be 3.139
  auto h2RefEtaRefPhiRebin = std::make_unique<TH2D>("h2RefEtaRefPhiRebin", ";Reference #eta;Reference #phi",
  																									360, -3.132, 3.132, 360, -TMath::Pi(), TMath::Pi());
  
  auto h2RefEtaRefPhiRefPt110 = std::make_unique<TH2D>("h2RefEtaRefPhiRefPt110", ";Reference #eta;Reference #phi",
  																										 30, -1.305, 1.305, 72, -TMath::Pi(), TMath::Pi());
  
  auto h2RefEtaRefPhiRefPt110Rebin = std::make_unique<TH2D>("h2RefEtaRefPhiRefPt110Rebin", ";Reference #eta;Reference #phi",
  																													360, -3.132, 3.132, 360, -TMath::Pi(), TMath::Pi());
  
  auto h2RefPtCountRef = std::make_unique<TH2D>("h2RefPtCountRef", ";Reference #it{p}_{T} (GeV);Counts", nPt, binsPt.data(), 5, 0, 5);
  
  auto h1GenRefPt = std::make_unique<TH1D>("h1GenRefPt", ";Generated Reference #it{p}_{T} (GeV);Events", nPt, binsPt.data());
  
  auto p1RefPtOverGenRefPtInGenRefPt = std::make_unique<TProfile>("p1RefPtOverGenRefPtInGenRefPt", ";Generated Reference #it{p}_{T} (GeV);RefPt/GenRefPt", nPt, binsPt.data());
  auto p1GenRefPtOverRefPtInRefPt = std::make_unique<TProfile>("p1GenRefPtOverRefPtInRefPt", ";Reference #it{p}_{T} (GeV);GenRefPt/RefPt", nPt, binsPt.data());
  
  //------------------------------------
  // Variables after leading Ref in barrel (eta < 1.33)
  //------------------------------------
  fout->mkdir("passRefBarrel");
  fout->cd("passRefBarrel");
  
  // Follow up on problematic cuts
  auto h1DphiJetRef = std::make_unique<TH1D>("h1DphiJetRef", ";#Delta#phi;Events", 126, -TMath::TwoPi(), TMath::TwoPi());
  auto h1DrJetRef = std::make_unique<TH1D>("h1DrJetRef", ";#DeltaR;Events", 100, 0, 10);
  auto h2RefPtDbResp = std::make_unique<TH2D>("h2RefPtDbResp", ";Reference #it{p}_{T} (GeV);Direct Balance Response", nPt, binsPt.data(), 200, 0, 4);
  auto h2RefPtMpfResp = std::make_unique<TH2D>("h2RefPtMpfResp", ";Reference #it{p}_{T} (GeV);MPF Response", nPt, binsPt.data(), 300, -2, 4);
  
  // Plots for jet properties
  auto p1GenJ1PtOverProbePtInRefPt = std::make_unique<TProfile>("p1GenJ1PtOverProbePtInRefPt", ";Reference #it{p}_{T} (GeV);GenJet1Pt/ProbePt", nPt, binsPt.data());
  auto p1J1PtOverProbePtInRefPt = std::make_unique<TProfile>("p1J1PtOverProbePtInRefPt", ";Reference #it{p}_{T} (GeV);Jet1Pt/ProbePt", nPt, binsPt.data());
  auto p1J1PtOverGenJ1PtInGenJ1Pt = std::make_unique<TProfile>("p1J1PtOverGenJ1PtInGenJ1Pt", ";Generated Jet1 #it{p}_{T} (GeV);Jet1Pt/GenJet1Pt", nPt, binsPt.data());
  
  //------------------------------------
  // Variables in 'flavourXTaggedY' directory
  //------------------------------------
  fout->mkdir("passRefBarrel/flavourXTaggedY");
  fout->cd("passRefBarrel/flavourXTaggedY");
  
  std::map<std::string, double> mvar;
  std::map<std::string, std::map<std::string, std::map<std::string, std::unique_ptr<TH1>>>> mp;
  
  std::vector<std::string> avar = {
  		"h1RefPt",
  		"h1GenJet1Pt",
  		"p1MpfRespInRefPt",
  		"p1DbRespInRefPt",
  		"p1MpfResp1InRefPt",
  		"p1MpfRespNInRefPt",
  		"p1MpfRespUInRefPt",
  		"p1RhoInRefPt",
  		"p1Jet1PtORefPtInRefPt",
  		"p1GenJet1PtORefPtInRefPt",
  		"p1Jet1PtOGenJet1PtInGenJet1Pt"
  };
  
  std::vector<std::string> atag = {"i", "b", "c", "q", "g", "n"};
  std::vector<std::string> aflv = {"i", "b", "c", "q", "s", "ud", "g", "n"};
  
  for (const auto& var : avar) {
  		for (const auto& tag : atag) {
  				for (const auto& flv : aflv) {
  						std::string histName = var + "_" + tag + flv;
  						if (var == "h1GenJet1Pt") {
  								mp[var][tag][flv] = std::make_unique<TH1D>(histName.c_str(), ";#it{p}_{T} (GeV);Events", nPt, binsPt.data());
  						} else {
  								mp[var][tag][flv] = std::make_unique<TProfile>(histName.c_str(), ";#it{p}_{T} (GeV);Value", nPt, binsPt.data());
  						}
  				}
  		}
  }
  
  //------------------------------------
  // Variables after Jet1Eta, Jet2Pt cut
  //------------------------------------
  fout->mkdir("passJet1EtaJet2Pt");
  fout->cd("passJet1EtaJet2Pt");
  
  auto p1JetEtaInRefPt = std::make_unique<TProfile>("p1JetEtaInRefPt", ";Reference #it{p}_{T} (GeV);Jet #eta", nPt, binsPt.data());
  auto h2JetEtaRefPt = std::make_unique<TH2D>("h2JetEtaRefPt", ";Reference #it{p}_{T} (GeV);Jet #eta", nPt, binsPt.data(), nEta, binsEta.data());
  
  auto p1GenJ2PtOverProbePtInRefPt = std::make_unique<TProfile>("p1GenJ2PtOverProbePtInRefPt", ";Reference #it{p}_{T} (GeV);GenJet2Pt/ProbePt", nPt, binsPt.data());
  auto p1J2PtOverProbePtInRefPt = std::make_unique<TProfile>("p1J2PtOverProbePtInRefPt", ";Reference #it{p}_{T} (GeV);Jet2Pt/ProbePt", nPt, binsPt.data());
  auto p1J2PtOverGenJ2PtInGenJ2Pt = std::make_unique<TProfile>("p1J2PtOverGenJ2PtInGenJ2Pt", ";Generated Jet2 #it{p}_{T} (GeV);Jet2Pt/GenJet2Pt", nPt, binsPt.data());
  
  // 1D composition and response
  auto p1DbRespInRefPt = std::make_unique<TProfile>("p1DbRespInRefPt", ";Reference #it{p}_{T} (GeV);Direct Balance Response", nPt, binsPt.data());
  auto p1MpfRespInRefPt = std::make_unique<TProfile>("p1MpfRespInRefPt", ";Reference #it{p}_{T} (GeV);MPF Response", nPt, binsPt.data());
  auto p1JetChhefInRefPt = std::make_unique<TProfile>("p1JetChhefInRefPt", ";Reference #it{p}_{T} (GeV);Charged Hadron Energy Fraction", nPt, binsPt.data());
  auto p1JetNehefInRefPt = std::make_unique<TProfile>("p1JetNehefInRefPt", ";Reference #it{p}_{T} (GeV);Neutral Hadron Energy Fraction", nPt, binsPt.data());
  auto p1JetNeemefInRefPt = std::make_unique<TProfile>("p1JetNeemefInRefPt", ";Reference #it{p}_{T} (GeV);Neutral EM Energy Fraction", nPt, binsPt.data());
  auto p1JetChemefInRefPt = std::make_unique<TProfile>("p1JetChemefInRefPt", ";Reference #it{p}_{T} (GeV);Charged EM Energy Fraction", nPt, binsPt.data());
  auto p1JetMuefInRefPt = std::make_unique<TProfile>("p1JetMuefInRefPt", ";Reference #it{p}_{T} (GeV);Muon Energy Fraction", nPt, binsPt.data());
  auto p1JetChpv0efInRefPt = std::make_unique<TProfile>("p1JetChpv0efInRefPt", ";Reference #it{p}_{T} (GeV);Charged PV=0 Energy Fraction", nPt, binsPt.data());
  
  // Phi binning
  const int nPhi = 72;
  const double binsPhi[nPhi + 1] = {
  		-3.142, -3.054, -2.967, -2.880, -2.793, -2.705, -2.618, -2.531, -2.443,
  		-2.356, -2.269, -2.182, -2.094, -2.007, -1.920, -1.833, -1.745, -1.658,
  		-1.571, -1.484, -1.396, -1.309, -1.222, -1.134, -1.047, -0.960, -0.873,
  		-0.785, -0.698, -0.611, -0.524, -0.436, -0.349, -0.262, -0.175, -0.087,
  		0.000, 0.087, 0.175, 0.262, 0.349, 0.436, 0.524, 0.611, 0.698, 0.785,
  		0.873, 0.960, 1.047, 1.134, 1.222, 1.309, 1.396, 1.484, 1.571, 1.658,
  		1.745, 1.833, 1.920, 2.007, 2.094, 2.182, 2.269, 2.356, 2.443, 2.531,
  		2.618, 2.705, 2.793, 2.880, 2.967, 3.054, 3.142
  };
  
  // 2D composition and response
  auto p2DbRespInJetEtaJetPhiRefPt230 = std::make_unique<TProfile2D>("p2DbRespInJetEtaJetPhiRefPt230", ";Jet #eta;Jet #phi;Direct Balance Response", nEta, binsEta.data(), nPhi, binsPhi);
  auto p2MpfRespInJetEtaJetPhiRefPt230 = std::make_unique<TProfile2D>("p2MpfRespInJetEtaJetPhiRefPt230", ";Jet #eta;Jet #phi;MPF Response", nEta, binsEta.data(), nPhi, binsPhi);
  auto p2JetChhefInJetEtaJetPhiRefPt230 = std::make_unique<TProfile2D>("p2JetChhefInJetEtaJetPhiRefPt230", ";Jet #eta;Jet #phi;Charged Hadron Energy Fraction", nEta, binsEta.data(), nPhi, binsPhi);
  auto p2JetNehefInJetEtaJetPhiRefPt230 = std::make_unique<TProfile2D>("p2JetNehefInJetEtaJetPhiRefPt230", ";Jet #eta;Jet #phi;Neutral Hadron Energy Fraction", nEta, binsEta.data(), nPhi, binsPhi);
  auto p2JetNeemefInJetEtaJetPhiRefPt230 = std::make_unique<TProfile2D>("p2JetNeemefInJetEtaJetPhiRefPt230", ";Jet #eta;Jet #phi;Neutral EM Energy Fraction", nEta, binsEta.data(), nPhi, binsPhi);
  auto p2JetChemefInJetEtaJetPhiRefPt230 = std::make_unique<TProfile2D>("p2JetChemefInJetEtaJetPhiRefPt230", ";Jet #eta;Jet #phi;Charged EM Energy Fraction", nEta, binsEta.data(), nPhi, binsPhi);
  auto p2JetMuefInJetEtaJetPhiRefPt230 = std::make_unique<TProfile2D>("p2JetMuefInJetEtaJetPhiRefPt230", ";Jet #eta;Jet #phi;Muon Energy Fraction", nEta, binsEta.data(), nPhi, binsPhi);
  auto p2JetChfpv0efInJetEtaJetPhiRefPt230 = std::make_unique<TProfile2D>("p2JetChfpv0efInJetEtaJetPhiRefPt230", ";Jet #eta;Jet #phi;Charged PV=0 Energy Fraction", nEta, binsEta.data(), nPhi, binsPhi);
  
  // Plots of npv, npvgood
  auto p1RhoInRefPt = std::make_unique<TProfile>("p1RhoInRefPt", ";Reference #it{p}_{T} (GeV);#rho", nPt, binsPt.data());
  auto p1NpvgoodInRefPt = std::make_unique<TProfile>("p1NpvgoodInRefPt", ";Reference #it{p}_{T} (GeV);N_{PV}^{good}", nPt, binsPt.data());
  auto p1NpvInRefPt = std::make_unique<TProfile>("p1NpvInRefPt", ";Reference #it{p}_{T} (GeV);N_{PV}", nPt, binsPt.data());
  
  auto h2RefPtDbRespPassMpf = std::make_unique<TH2D>("h2RefPtDbRespPassMpf", ";Reference #it{p}_{T} (GeV);Direct Balance Response", nPt, binsPt.data(), 200, 0, 4);
  auto h2RefPtMpfRespPassDb = std::make_unique<TH2D>("h2RefPtMpfRespPassDb", ";Reference #it{p}_{T} (GeV);MPF Response", nPt, binsPt.data(), 300, -2, 4);
  auto h2RefPtDbRespPassBoth = std::make_unique<TH2D>("h2RefPtDbRespPassBoth", ";Reference #it{p}_{T} (GeV);Direct Balance Response", nPt, binsPt.data(), 200, 0, 4);
  auto h2RefPtMpfRespPassBoth = std::make_unique<TH2D>("h2RefPtMpfRespPassBoth", ";Reference #it{p}_{T} (GeV);MPF Response", nPt, binsPt.data(), 300, -2, 4);
  
  //------------------------------------
  // Variables after response cuts
  //------------------------------------
  fout->mkdir("passResponse");
  fout->cd("passResponse");
  
  Int_t runN = 20000;
  Double_t runMin = 370000.5;
  Double_t runMax = 390000.5;
  
  // Time stability of cross-section
  auto h1RunRefPt30 = std::make_unique<TH1D>("h1RunRefPt30", ";Run;Events", runN, runMin, runMax);
  auto h1RunRefPt110 = std::make_unique<TH1D>("h1RunRefPt110", ";Run;Events", runN, runMin, runMax);
  auto h1RunRefPt230 = std::make_unique<TH1D>("h1RunRefPt230", ";Run;Events", runN, runMin, runMax);
  
  // Time stability of JEC
  auto p1DbRespInRunRefPt30 = std::make_unique<TProfile>("p1DbRespInRunRefPt30", ";Run;Direct Balance Response", runN, runMin, runMax);
  auto p1DbRespInRunRefPt110 = std::make_unique<TProfile>("p1DbRespInRunRefPt110", ";Run;Direct Balance Response", runN, runMin, runMax);
  auto p1DbRespInRunRefPt230 = std::make_unique<TProfile>("p1DbRespInRunRefPt230", ";Run;Direct Balance Response", runN, runMin, runMax);
  
  auto p1MpfRespInRunRefPt30 = std::make_unique<TProfile>("p1MpfRespInRunRefPt30", ";Run;MPF Response", runN, runMin, runMax);
  auto p1MpfRespInRunRefPt110 = std::make_unique<TProfile>("p1MpfRespInRunRefPt110", ";Run;MPF Response", runN, runMin, runMax);
  auto p1MpfRespInRunRefPt230 = std::make_unique<TProfile>("p1MpfRespInRunRefPt230", ";Run;MPF Response", runN, runMin, runMax);
  
  // Time stability of PF composition
  auto p1JetChhefInRunRefPt30 = std::make_unique<TProfile>("p1JetChhefInRunRefPt30", ";Run;Charged Hadron Energy Fraction", runN, runMin, runMax);
  auto p1JetChhefInRunRefPt110 = std::make_unique<TProfile>("p1JetChhefInRunRefPt110", ";Run;Charged Hadron Energy Fraction", runN, runMin, runMax);
  auto p1JetChhefInRunRefPt230 = std::make_unique<TProfile>("p1JetChhefInRunRefPt230", ";Run;Charged Hadron Energy Fraction", runN, runMin, runMax);
  
  auto p1JetNehefInRunRefPt30 = std::make_unique<TProfile>("p1JetNehefInRunRefPt30", ";Run;Neutral Hadron Energy Fraction", runN, runMin, runMax);
  auto p1JetNehefInRunRefPt110 = std::make_unique<TProfile>("p1JetNehefInRunRefPt110", ";Run;Neutral Hadron Energy Fraction", runN, runMin, runMax);
  auto p1JetNehefInRunRefPt230 = std::make_unique<TProfile>("p1JetNehefInRunRefPt230", ";Run;Neutral Hadron Energy Fraction", runN, runMin, runMax);
  
  auto p1JetNeemefInRunRefPt30 = std::make_unique<TProfile>("p1JetNeemefInRunRefPt30", ";Run;Neutral EM Energy Fraction", runN, runMin, runMax);
  auto p1JetNeemefInRunRefPt110 = std::make_unique<TProfile>("p1JetNeemefInRunRefPt110", ";Run;Neutral EM Energy Fraction", runN, runMin, runMax);
  auto p1JetNeemefInRunRefPt230 = std::make_unique<TProfile>("p1JetNeemefInRunRefPt230", ";Run;Neutral EM Energy Fraction", runN, runMin, runMax);
  
  curdir->cd();


  
//------------------------------------
// Event loop
//------------------------------------

// Initialize TLorentzVectors
TLorentzVector p4Ref, p4GenRef, p4Jet1, p4Jet2, p4Jetn;
TLorentzVector p4Met, p4Met1, p4Metn, p4Metu, p4Metnu, p4RawMet, p4CorrMet, p4RawRef;
TLorentzVector p4Jeti, p4CorrJets, p4RawJet, p4RawJets, p4RcJet, p4RcJets, p4RcOffsets;
TLorentzVector p4GenJeti, p4GenJet1, p4GenJet2;
TLorentzVector p4Refx; // for MPFX

Long64_t nentries = skimT->getEntries(); 
std::cout << "\nStarting loop over " << nentries << " entries" << std::endl;

std::cout << "---------------------------" << std::endl;
std::cout << std::setw(10) << "Progress" << std::setw(10) << "Time" << std::endl;
std::cout << "---------------------------" << std::endl;
double totTime = 0.0;
auto startClock = std::chrono::high_resolution_clock::now();

for (Long64_t jentry = 0; jentry < nentries; ++jentry) {
    if (globalFlags_.isDebug() && jentry > globalFlags_.getNDebug()) break;
    eventP->printProgress(jentry, nentries, startClock, totTime);
   
    Long64_t ientry = skimT->loadEntry(jentry);
    if (ientry < 0) break; 
    skimT->getChain()->GetTree()->GetEntry(ientry);
    h1Cutflow->Fill(cutToBinMap["passSkim"]);

    //------------------------------------
    // Trigger and golden lumi, MET filter selection 
    //------------------------------------
    if (!eventP->passHLT(skimT)) continue; 
    h1Cutflow->Fill(cutToBinMap["passHLT"]);

    bool passGoodLumi = true; 
    if (globalFlags_.isData()){
        passGoodLumi = objS->checkGoodLumi(skimT->run, skimT->luminosityBlock);
    }
    if (!passGoodLumi) continue; 
    h1Cutflow->Fill(cutToBinMap["passGoodLumi"]);

    if (!eventP->passFilter(skimT)) continue; 
    h1Cutflow->Fill(cutToBinMap["passMetFilter"]);

    //------------------------------------------
    // Select objects
    //------------------------------------------
    // Reco objects
    objP->clearObjects();
    objP->pickElectrons();
    if (objP->getPickedElectrons().size() < 2) continue;
    if (objP->getPickedElectrons().size() > 3) continue;
    objP->pickRefs();
    std::vector<TLorentzVector> p4Refs = objP->getPickedRefs();

    if (p4Refs.empty()) continue; 
    h1Cutflow->Fill(cutToBinMap["passAtleast1Ref"]);

    // Weight
    double weight = (globalFlags_.isMC() ? skimT->genWeight : 1.0);
    if (globalFlags_.isMC()) weight *= objS->getPuCorrection(skimT->Pileup_nTrueInt, "nominal");

    p4Ref = p4Refs.at(0);
    p4RawRef = p4Refs.at(0);
    double ptRef = p4Ref.Pt();
    // Fill histograms
    h2RefPtCountRef->Fill(ptRef, p4Refs.size(), weight);
    h1RefPt->Fill(ptRef, weight);
    h2RefEtaRefPhi->Fill(p4RawRef.Eta(), p4RawRef.Phi(), weight);
    h2RefEtaRefPhiRebin->Fill(p4RawRef.Eta(), p4RawRef.Phi(), weight);
    if (ptRef >= 110) {
        h2RefEtaRefPhiRefPt110->Fill(p4RawRef.Eta(), p4RawRef.Phi(), weight);
        h2RefEtaRefPhiRefPt110Rebin->Fill(p4RawRef.Eta(), p4RawRef.Phi(), weight);
    }

    // Gen objects
    p4GenRef.SetPtEtaPhiM(0, 0, 0, 0);
    if (globalFlags_.isMC()) {
        objP->pickGenElectrons();
        objP->pickGenRefs();
        std::vector<TLorentzVector> p4GenRefs = objP->getPickedGenRefs();
        if (p4GenRefs.empty()) continue;
        p4GenRef = p4GenRefs.at(0);
        if (p4GenRef.Pt() > 0 && fabs(p4GenRef.Eta()) < 1.3) {
            h1GenRefPt->Fill(p4GenRef.Pt(), weight);
        }
        if (fabs(p4Ref.Eta()) < 1.3) {
            if (p4GenRef.Pt() > 0) {
                h1RefPtBarrel->Fill(ptRef, weight);
                p1RefPtOverGenRefPtInGenRefPt->Fill(p4GenRef.Pt(), ptRef / p4GenRef.Pt(), weight);
                p1GenRefPtOverRefPtInRefPt->Fill(ptRef, p4GenRef.Pt() / ptRef, weight);
            }
        }
    }

    //------------------------------------------------
    // Jet loop: Apply JEC
    //------------------------------------------------
    // Select leading jets. Just exclude Ref, don't apply JetID yet
    int iJet1(-1), iJet2(-1), nJets(0);
    p4Jet1.SetPtEtaPhiM(0, 0, 0, 0);
    p4Jet2.SetPtEtaPhiM(0, 0, 0, 0);
    p4Jetn.SetPtEtaPhiM(0, 0, 0, 0);
    // Also calculate corrected type-I chsMET and HDM inputs
    p4RawJets.SetPtEtaPhiM(0, 0, 0, 0);
    p4RcJets.SetPtEtaPhiM(0, 0, 0, 0);
    p4RcOffsets.SetPtEtaPhiM(0, 0, 0, 0);
    p4CorrJets.SetPtEtaPhiM(0, 0, 0, 0);

    for (int i = 0; i != skimT->nJet; ++i) {
        if (!(skimT->Jet_jetId[i] & (1 << 2))) continue; // tightLepVeto

        // Data+MC: undo the correction already applied in NanoAOD
        skimT->Jet_pt[i] *= (1.0 - skimT->Jet_rawFactor[i]);
        skimT->Jet_mass[i] *= (1.0 - skimT->Jet_rawFactor[i]);
        p4RawJet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i], skimT->Jet_phi[i], skimT->Jet_mass[i]);
        p4RawJets += p4RawJet;

        // Data+MC: L1RC correction is applied to the uncorrected jet
        double corrL1FastJet = objS->getL1FastJetCorrection(skimT->Jet_area[i], skimT->Jet_eta[i], 
                                                            skimT->Jet_pt[i], skimT->Rho);
        skimT->Jet_pt[i] *= corrL1FastJet;
        skimT->Jet_mass[i] *= corrL1FastJet;
        p4RcJet.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i], skimT->Jet_phi[i], skimT->Jet_mass[i]);
        p4RcJets += p4RcJet;
        p4RcOffsets += (p4RawJet - p4RcJet);

        // Data+MC: MCTruth correction is applied to the L1RC corrected jet
        double corrL2Relative = objS->getL2RelativeCorrection(skimT->Jet_eta[i], skimT->Jet_pt[i]);
        skimT->Jet_pt[i] *= corrL2Relative;
        skimT->Jet_mass[i] *= corrL2Relative;

        // Only Data: L2Residual+L3Residual correction is applied to the MCTruth corrected jet
        if (globalFlags_.isData()) {
            double corrL2L3Residual = objS->getL2L3ResidualCorrection(skimT->Jet_eta[i], skimT->Jet_pt[i]);
            skimT->Jet_pt[i] *= corrL2L3Residual;
            skimT->Jet_mass[i] *= corrL2L3Residual;
        }

        p4Jeti.SetPtEtaPhiM(skimT->Jet_pt[i], skimT->Jet_eta[i], skimT->Jet_phi[i], skimT->Jet_mass[i]);
        if (smearJets_) {
            // Apply JER correction if necessary
            // double corrJER = objS->getJERCorrection(skimT, i, "nom");
            // skimT->Jet_pt[i] *= corrJER;
        }
       
        if (skimT->Jet_pt[i] < 12) continue;
        if (p4Ref.DeltaR(p4Jeti) < 0.2) continue; // should not happen, but does?
        ++nJets;
        if (iJet1 == -1) { // Leading jet for balance
            iJet1 = i;
            p4Jet1 = p4Jeti;
        } else { // Subleading jets 
            p4Jetn += p4Jeti;
            if (iJet2 == -1) { // First subleading jet for alpha
                iJet2 = i;
                p4Jet2 = p4Jeti;
            }
        }
        p4CorrJets += p4Jeti;
    }//nJet

    if (nJets < 1) continue; 
    h1Cutflow->Fill(cutToBinMap["passAtleast1Jet"]);

    if (objS->checkJetVetoMap()) continue; // expensive function
    h1Cutflow->Fill(cutToBinMap["passJetVetoMap"]);
    
    //------------------------------------------------
    // Set MET vectors
    //------------------------------------------------
    p4RawMet.SetPtEtaPhiM(skimT->ChsMET_pt, 0, skimT->ChsMET_phi, 0);
    p4RawMet += p4RawRef - p4Ref; // replace PF Ref with Reco Ref
    p4Met1 = -p4Jet1 - p4Ref;
    p4Metn = -p4Jetn;
    p4CorrMet = p4RawMet + p4RcJets - p4CorrJets; // Corrected MET
    // Unclustered MET from corrMET
    p4Metu = p4CorrMet + p4Ref + p4CorrJets;
    p4Metnu = p4Metn + 1.1 * p4Metu;
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
    double bal = p4Jet1.Pt() / ptRef;
    double mpf = 1 + p4Met.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);
    double mpf1 = 1 + p4Met1.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);
    double mpfn = p4Metn.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);
    double mpfu = p4Metu.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);
    double mpfnu = p4Metnu.Vect().Dot(p4Ref.Vect()) / (ptRef * ptRef);

    // For MPFX
    p4Refx.SetPtEtaPhiM(p4Ref.Pt(), p4Ref.Eta(), p4Ref.Phi() + 0.5 * TMath::Pi(), 0.0);
    double mpfx = 1 + p4Met.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
    double mpf1x = 1 + p4Met1.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
    double mpfnPt = p4Metn.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
    double mpfux = p4Metu.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
    double mpfnux = p4Metnu.Vect().Dot(p4Refx.Vect()) / (ptRef * ptRef);
     
    // Sanity checks for HDM inputs
    if (!(fabs(mpf1 + mpfn + mpfu - mpf) < 1e-4)) {
        std::cout << "\nHDM input error: mpf=" << mpf << " mpf1=" << mpf1
                  << " mpfn=" << mpfn << " mpfu=" << mpfu << std::endl;
        std::cout << "Difference = " << mpf1 + mpfn + mpfu - mpf << std::endl << std::flush;
        std::cout << "Skip entry " << jentry
                  << " (" << skimT->run << "," << skimT->luminosityBlock << "," << skimT->event << ")"
                  << " in file " << fout->GetName() << std::endl << std::flush;
        continue;
    }
   
    if (fabs(p4Ref.DeltaPhi(p4Jet1) - TMath::Pi()) >= 0.44) continue; 
    h1Cutflow->Fill(cutToBinMap["passDPhiRefJet1"]);

    if (fabs(p4Ref.Eta()) > 1.3) continue; 
    h1Cutflow->Fill(cutToBinMap["passRefBarrel"]);

    h1DphiJetRef->Fill(p4Ref.DeltaPhi(p4Jet1), weight);
    h1DrJetRef->Fill(p4Ref.DeltaR(p4Jet1), weight);

    //------------------------------------------------
    // GenJet loop
    //------------------------------------------------
    // Select p4GenJet1 matching leading and subleading reco jet
    int iGenJet(-1), iGenJet2(-1);
    p4GenJet1.SetPtEtaPhiM(0, 0, 0, 0);
    p4GenJet2.SetPtEtaPhiM(0, 0, 0, 0);
    if (globalFlags_.isMC()) {
        for (Int_t i = 0; i != skimT->nGenJet; ++i) {
            p4GenJeti.SetPtEtaPhiM(skimT->GenJet_pt[i], skimT->GenJet_eta[i],
                                   skimT->GenJet_phi[i], skimT->GenJet_mass[i]);
            if (iJet1 != -1 && p4GenJeti.DeltaR(p4Jet1) < 0.4 && iGenJet == -1) {
                iGenJet = i;
                p4GenJet1 = p4GenJeti;
            } else if (iJet2 != -1 && p4GenJeti.DeltaR(p4Jet2) < 0.4 && iGenJet2 == -1) {
                iGenJet2 = i;
                p4GenJet2 = p4GenJeti;
            }
        }
    }
    bool pass_gen = (iGenJet != -1);
   
    if (pass_gen || globalFlags_.isData()) {
        p1J1PtOverProbePtInRefPt->Fill(ptRef, p4Jet1.Pt() / ptRef, weight);
    }
    if (pass_gen) {
        p1GenJ1PtOverProbePtInRefPt->Fill(ptRef, p4GenJet1.Pt() / ptRef, weight);
        p1J1PtOverGenJ1PtInGenJ1Pt->Fill(p4GenJet1.Pt(), p4Jet1.Pt() / p4GenJet1.Pt(), weight);
    }
    /*
    int flv = (globalFlags_.isMC() ? skimT->GenJet_partonFlavour[iGenJet] : 99);
    mvar["h1RefPt"] = 1.0;
    mvar["p1MpfRespInRefPt"] = mpf;
    mvar["p1DbRespInRefPt"] = bal;
    mvar["p1MpfResp1InRefPt"] = mpf1;
    mvar["p1MpfRespNInRefPt"] = mpfn;
    mvar["p1MpfRespUInRefPt"] = mpfu;
    mvar["p1RhoInRefPt"] = skimT->Rho;
    mvar["p1Jet1PtORefPtInRefPt"] = (p4Jet1.Pt() / ptRef);
    mvar["p1GenJet1PtORefPtInRefPt"] = (p4GenJet1.Pt() / ptRef);
    mvar["p1Jet1PtOGenJet1PtInGenJet1Pt"] = (p4GenJet1.Pt() != 0 ? p4Jet1.Pt() / p4GenJet1.Pt() : 0.0);
    
    bool isb = (skimT->Jet_btagDeepB[iJet1] > objS->bThresh);
    bool isc = (skimT->Jet_btagDeepC[iJet1] > objS->cThresh && !isb);
    bool isq = (skimT->Jet_qgl[iJet1] >= 0.5 && skimT->Jet_qgl[iJet1] && !isb && !isc);
    bool isg = (skimT->Jet_qgl[iJet1] >= 0.0 && skimT->Jet_qgl[iJet1] < 0.5 && !isb && !isc);
    bool isn = (!isb && !isc && !isq && !isg);
    
    for (const auto& var : avar) {
        for (const auto& tag : atag) {
            for (const auto& flv_str : aflv) {
                if (((flv_str == "i") ||
                    (flv_str == "b" && abs(flv) == 5) ||
                    (flv_str == "c" && abs(flv) == 4) ||
                    (flv_str == "q" && abs(flv) <= 3 && flv != 0) ||
                    (flv_str == "s" && abs(flv) == 3) ||
                    (flv_str == "ud" && abs(flv) <= 2 && flv != 0) ||
                    (flv_str == "g" && flv == 21) ||
                    (flv_str == "n" && flv == 0)) &&
                   ((tag == "i") ||
                    (tag == "b" && isb) ||
                    (tag == "c" && isc) ||
                    (tag == "q" && isq) ||
                    (tag == "g" && isg) ||
                    (tag == "n" && isn))) {
                    
                    double x = ptRef;
                    if (var == "p1Jet1PtOGenJet1PtInGenJet1Pt") x = p4GenJet1.Pt();
                    if ((var == "p1Jet1PtORefPtInRefPt" || var == "p1GenJet1PtORefPtInRefPt") && iGenJet == -1) x = 0.0;
                    double val = mvar[var];
                    TH1* h = mp[var][tag][flv_str].get();
                    if (!h) {
                        std::cout << "Missing " << var << tag << flv_str << std::endl << std::flush;
                        assert(h);
                    }
                    if (var == "h1RefPt") {
                        static_cast<TH1D*>(h)->Fill(x, weight);
                    } else {
                        static_cast<TProfile*>(h)->Fill(x, val, weight);
                    }
                }
            }
        }
    }
  
    if (!((fabs(p4Jet1.Eta()) < 1.3) && (p4Jet2.Pt() < ptRef || p4Jet2.Pt() < 30))) continue; 
    h1Cutflow->Fill(cutToBinMap["passJet1EtaJet2Pt"]);

    h2RefPtDbResp->Fill(ptRef, bal, weight);
    h2RefPtMpfResp->Fill(ptRef, mpf, weight);
    if (p4Jet2.Pt() > 0) {
        if (iGenJet2 != -1 || !globalFlags_.isMC()) {
            p1J2PtOverProbePtInRefPt->Fill(ptRef, p4Jet2.Pt() / ptRef, weight);
        }
        if (iGenJet2 != -1) {
            p1GenJ2PtOverProbePtInRefPt->Fill(ptRef, p4GenJet2.Pt() / ptRef, weight);
            p1J2PtOverGenJ2PtInGenJ2Pt->Fill(p4GenJet2.Pt(), p4Jet2.Pt() / p4GenJet2.Pt(), weight);
        }
    }
    // Control plots for Ref-jet 
    // PF composition plots
    h2JetEtaRefPt->Fill(ptRef, skimT->Jet_eta[iJet1], weight);
    p1JetEtaInRefPt->Fill(ptRef, fabs(skimT->Jet_eta[iJet1]), weight);
    
    // 1D composition and response
    p1DbRespInRefPt->Fill(ptRef, bal, weight);
    p1MpfRespInRefPt->Fill(ptRef, mpf, weight);
    p1JetChhefInRefPt->Fill(ptRef, skimT->Jet_chHEF[iJet1], weight);
   


    // Control plots for Ref-jet
    // PF composition plots
    h2JetEtaRefPt->Fill(ptRef, skimT->Jet_eta[iJet1], weight);
    p1JetEtaInRefPt->Fill(ptRef, fabs(skimT->Jet_eta[iJet1]), weight);

    // 1D composition and response
    p1DbRespInRefPt->Fill(ptRef, bal, weight);
    p1MpfRespInRefPt->Fill(ptRef, mpf, weight);
    p1JetChhefInRefPt->Fill(ptRef, skimT->Jet_chHEF[iJet1], weight);
    p1JetNehefInRefPt->Fill(ptRef, skimT->Jet_neHEF[iJet1], weight);
    p1JetNeemefInRefPt->Fill(ptRef, skimT->Jet_neEmEF[iJet1], weight);
    p1JetChemefInRefPt->Fill(ptRef, skimT->Jet_chEmEF[iJet1], weight);
    p1JetMuefInRefPt->Fill(ptRef, skimT->Jet_muEF[iJet1], weight);
    // p1JetChpv0efInRefPt->Fill(ptRef, skimT->Jet_chFPV0EF[iJet1], weight);

    p1RhoInRefPt->Fill(ptRef, skimT->Rho, weight);
    p1NpvgoodInRefPt->Fill(ptRef, skimT->PV_npvsGood, weight);
    p1NpvInRefPt->Fill(ptRef, skimT->PV_npvs, weight);

    // 2D composition and response
    if (ptRef > 230) {
        double eta = skimT->Jet_eta[iJet1];
        double phi = skimT->Jet_phi[iJet1];
        p2DbRespInJetEtaJetPhiRefPt230->Fill(eta, phi, bal, weight);
        p2MpfRespInJetEtaJetPhiRefPt230->Fill(eta, phi, mpf, weight);
        p2JetChhefInJetEtaJetPhiRefPt230->Fill(eta, phi, skimT->Jet_chHEF[iJet1], weight);
        p2JetNehefInJetEtaJetPhiRefPt230->Fill(eta, phi, skimT->Jet_neHEF[iJet1], weight);
        p2JetNeemefInJetEtaJetPhiRefPt230->Fill(eta, phi, skimT->Jet_neEmEF[iJet1], weight);
        p2JetChemefInJetEtaJetPhiRefPt230->Fill(eta, phi, skimT->Jet_chEmEF[iJet1], weight);
        p2JetMuefInJetEtaJetPhiRefPt230->Fill(eta, phi, skimT->Jet_muEF[iJet1], weight);
        // p2JetChfpv0efInJetEtaJetPhiRefPt230->Fill(eta, phi, skimT->Jet_chFPV0EF[iJet1], weight);
    }  // high pT range

    bool pass_DbResp = (fabs(1 - bal) < 0.7);
    bool pass_MpfResp = (fabs(1 - mpf) < 0.7);

    if (pass_MpfResp) h2RefPtDbRespPassMpf->Fill(ptRef, bal, weight);
    if (pass_DbResp) h2RefPtMpfRespPassDb->Fill(ptRef, mpf, weight);
    if (pass_MpfResp && pass_DbResp) {
        h2RefPtDbRespPassBoth->Fill(ptRef, bal, weight);
        h2RefPtMpfRespPassBoth->Fill(ptRef, mpf, weight);
    }

    if (!(pass_DbResp && pass_MpfResp)) continue;
    h1Cutflow->Fill(cutToBinMap["passResponse"]);

    if (ptRef > 30) {
        h1RunRefPt30->Fill(skimT->run, weight);
        p1DbRespInRunRefPt30->Fill(skimT->run, bal, weight);
        p1MpfRespInRunRefPt30->Fill(skimT->run, mpf, weight);
        p1JetChhefInRunRefPt30->Fill(skimT->run, skimT->Jet_chHEF[iJet1], weight);
        p1JetNehefInRunRefPt30->Fill(skimT->run, skimT->Jet_neHEF[iJet1], weight);
        p1JetNeemefInRunRefPt30->Fill(skimT->run, skimT->Jet_neEmEF[iJet1], weight);
    }
    if (ptRef > 110) {
        h1RunRefPt110->Fill(skimT->run, weight);
        p1DbRespInRunRefPt110->Fill(skimT->run, bal, weight);
        p1MpfRespInRunRefPt110->Fill(skimT->run, mpf, weight);
        p1JetChhefInRunRefPt110->Fill(skimT->run, skimT->Jet_chHEF[iJet1], weight);
        p1JetNehefInRunRefPt110->Fill(skimT->run, skimT->Jet_neHEF[iJet1], weight);
        p1JetNeemefInRunRefPt110->Fill(skimT->run, skimT->Jet_neEmEF[iJet1], weight);
    }
    if (ptRef > 230) {
        h1RunRefPt230->Fill(skimT->run, weight);
        p1DbRespInRunRefPt230->Fill(skimT->run, bal, weight);
        p1MpfRespInRunRefPt230->Fill(skimT->run, mpf, weight);
        p1JetChhefInRunRefPt230->Fill(skimT->run, skimT->Jet_chHEF[iJet1], weight);
        p1JetNehefInRunRefPt230->Fill(skimT->run, skimT->Jet_neHEF[iJet1], weight);
        p1JetNeemefInRunRefPt230->Fill(skimT->run, skimT->Jet_neEmEF[iJet1], weight);
    }
    */
}  // end of event loop

// Add extra plot for jet response vs Ref pT
if (globalFlags_.isMC()) {
    fout->cd("passRefBarrel");
    auto hrgenvgen = std::unique_ptr<TH1D>(p1J1PtOverGenJ1PtInGenJ1Pt->ProjectionX("hrgenvgen"));
    auto hrgenvRef = std::unique_ptr<TH1D>(p1J1PtOverProbePtInRefPt->ProjectionX("hrgenvRef"));
    hrgenvRef->Divide(p1GenJ1PtOverProbePtInRefPt.get());

    fout->cd("passJet1EtaJet2Pt");
    auto hrgen2vgen = std::unique_ptr<TH1D>(p1J2PtOverGenJ2PtInGenJ2Pt->ProjectionX("hrgen2vgen"));
    auto hrgen2vRef = std::unique_ptr<TH1D>(p1J2PtOverProbePtInRefPt->ProjectionX("hrgen2vRef"));
    hrgen2vRef->Divide(p1GenJ2PtOverProbePtInRefPt.get());

    curdir->cd();
}

fout->Write();
eventP->printBins(h1Cutflow.get());
// eventP->scanTFile(fout);
std::cout << "Output file: " << fout->GetName() << std::endl;
fout->Close();
return 0;

}
   
