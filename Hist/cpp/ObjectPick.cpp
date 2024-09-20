#include"ObjectPick.h"

void ObjectPick::setTree(SkimTree *skimT){
  tree = skimT;
}

void ObjectPick::clearObjects(){
  pickedElectrons.clear();
  pickedMuons.clear();
  pickedPhotons.clear();
  pickedRefs.clear();

  pickedGenElectrons.clear();
  pickedGenMuons.clear();
  pickedGenPhotons.clear();
  pickedGenRefs.clear();
}

//https://twiki.cern.ch/twiki/bin/view/CMS/MuonUL2016#High_pT_above_120_GeV
//https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#HighPt_Muon
//https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonSelection#HighPt_Tracker_Muon
//https://github.com/cms-sw/cmssw/blob/master/DataFormats/MuonReco/src/MuonObjectPicks.cc#L933-L960
void ObjectPick::pickMuons(){
  if (isDebug){
	  cout << "Found Event, Starting Muons" << endl;
	  cout << " nMu=" << tree->nMuon_ << endl;
  }
  for(UInt_t m = 0; m < tree->nMuon_; ++m){
    double eta = tree->muEta_[m];
    double pt = tree->muPt_[m];
    
    bool passPrompt   = false; 
    if(pt > 30){
      passPrompt = (TMath::Abs(eta) <= 2.4 
      && tree->muTightId_[m] 
      && tree->muDxy_[m]<0.2
      && tree->muDz_[m]<0.5);
    }
    if(passPrompt) pickedMuons.push_back(m);
    if (isDebug){
      cout << "-- " << m 
      << " passPrompt="<<passPrompt
      << " pt="<<pt
      << " eta="<<eta
      << endl;
    } 
  }//nMuon
  if(isDebug) cout<<"pickedMuons.size() = "<<pickedMuons.size()<<endl;
}

void ObjectPick::pickElectrons(){
  if(isDebug){
	  cout << "Found Event, Starting Electrons" << endl;
	  cout << " nEle=" << tree->nEle_ << endl;
  }
  for(int eleInd = 0; eleInd < tree->nEle_; ++eleInd){
    double eta = tree->eleEta_[eleInd];
    double absEta = TMath::Abs(eta);
    double SCeta = eta + tree->eleDeltaEtaSC_[eleInd];
    double absSCEta = TMath::Abs(SCeta);
    double pt = tree->elePt_[eleInd];
    // make sure it doesn't fall within the gap
    bool passEtaEBEEGap = (absSCEta < 1.4442) || (absSCEta > 1.566);
    //medium electron ID
    bool passTightID = tree->eleID_[eleInd];

    bool eleSel = (passEtaEBEEGap && absEta <= 2.4 && pt >= 40.0 && passTightID);
    if(eleSel) pickedElectrons.push_back(eleInd);
    if (isDebug){
      cout << "-- " << eleInd 
      << " eleSel=" <<  eleSel 
      << " pt="<<pt
      << " eta="<<eta
      << " phi="<<tree->elePhi_[eleInd]
      << " eleID="<<passTightID; 
    } 
  }//nEle
  if(isDebug) cout<<"pickedElectrons.size() = "<<pickedElectrons.size()<<endl;
}

//https://twiki.cern.ch/twiki/bin/view/CMS/MultivariatePhotonIdentificationRun2
void ObjectPick::pickPhotons(){
  if(isDebug){
	  cout << "Found Event Staring Photons" << endl;
	  cout << " nPho=" << tree->nPhoton << endl;
  }
  for(int phoInd = 0; phoInd < tree->nPhoton; ++phoInd){
    double et = tree->Photon_pt[phoInd];
    double eta = tree->Photon_eta[phoInd];
    double absEta = TMath::Abs(eta);
    double phi = tree->Photon_phi[phoInd];
    bool passPhoId  = tree->Photon_cutBased[phoInd];//tight 
    bool phoPresel = (et >= 20.0 && absEta <= 1.4442);
    if(phoPresel && passPhoId) pickedPhotons.push_back(phoInd);
    if (isDebug){
      cout << "-- " << phoInd 
      << " pt="<<et
      << " eta="<<eta
      << " phi="<<phi
      <<"presel="<< phoPresel
      << " medID="<<passPhoId<<endl;
    } 
  }//nPho
  if(isDebug) cout<<"pickedPhotons.size() = "<<pickedPhotons.size()<<endl;
}

void ObjectPick::pickRefs(){
  // Z->ee + jets channel
  if(isZeeJet && pickedElectrons.size()>1){
    for (int j=0; j<pickedElectrons.size(); j++){
    	for (int k=j+1; k<pickedElectrons.size(); k++){
    		if(j==k) 
    			continue;
    		if((tree->eleCharge_[j])*(tree->eleCharge_[k]) == 1)
    			continue;
    		TLorentzVector p4Lep1;
    		TLorentzVector p4Lep2;
    		p4Lep1.SetPtEtaPhiM(tree->elePt_[j], tree->eleEta_[j], tree->elePhi_[j], tree->eleMass_[j]);
    		p4Lep2.SetPtEtaPhiM(tree->elePt_[k], tree->eleEta_[k], tree->elePhi_[k], tree->eleMass_[k]);
    		TLorentzVector p4Probe = p4Lep1 + p4Lep2;
    		if ( abs(p4Probe.M() - 91.1876) > 10 ) 
    			continue;
    		pickedRefs.push_back(p4Probe);
    	}
    }
  }

  // Z->mumu + jets channel
  if(isZeeJet && pickedMuons.size()>1){
    for (int j=0; j<pickedMuons.size(); j++){
    	for (int k=j+1; k<pickedMuons.size(); k++){
    		if(j==k) 
    			continue;
    		if((tree->muCharge_[j])*(tree->muCharge_[k]) == 1)
    			continue;
    		TLorentzVector p4Lep1;
    		TLorentzVector p4Lep2;
    		p4Lep1.SetPtEtaPhiM(tree->muPt_[j], tree->muEta_[j], tree->muPhi_[j], tree->muMass_[j]);
    		p4Lep2.SetPtEtaPhiM(tree->muPt_[k], tree->muEta_[k], tree->muPhi_[k], tree->muMass_[k]);
    		TLorentzVector p4Probe = p4Lep1 + p4Lep2;
    		if ( abs(p4Probe.M() - 91.1876) > 10 ) 
    			continue;
    		pickedRefs.push_back(p4Probe);
    	}
    }
  }

  // Gamma + jets channel
  if(isGamJet && pickedPhotons.size()>1){
    for (int j=0; j<pickedPhotons.size(); j++){
    		TLorentzVector p4Pho;
    		p4Pho.SetPtEtaPhiM(tree->Photon_pt[j], tree->Photon_eta[j], tree->Photon_phi[j], tree->Photon_mass[j]);
        pickedRefs.push_back(p4Pho); 
    }
  }
  if(isDebug) cout<<"pickedRefs.size() = "<<pickedRefs.size()<<endl;
}//pickRefs


//--------------------------------------------
// Gen objects
//--------------------------------------------
void ObjectPick::pickGenMuons(){
  for(int i = 0; i < tree->nGenDressedLepton; ++i){
  	if(fabs(tree->GenDressedLepton_pdgId[i])==13) 
      pickedGenMuons.push_back(i);
  }
  if(isDebug) cout<<"pickedGenMuons.size() = "<<pickedGenMuons.size()<<endl;
}

void ObjectPick::pickGenElectrons(){
  for(int i = 0; i < tree->nGenDressedLepton; ++i){
  	if(fabs(tree->GenDressedLepton_pdgId[i])==11) 
      pickedGenElectrons.push_back(i);
  }
  if(isDebug) cout<<"pickedGenElectrons.size() = "<<pickedGenElectrons.size()<<endl;
}

void ObjectPick::pickGenPhotons(){
  for(int i = 0; i < tree->nGenIsolatedPhoton; ++i){
    pickedGenPhotons.push_back(i);
  }
  if(isDebug) cout<<"pickedGenPhotons.size() = "<<pickedGenPhotons.size()<<endl;
}

void ObjectPick::pickGenRefs(){
  // Z->ee + jets channel
  if(isZeeJet && pickedGenElectrons.size()>1){
    for (int j=0; j<pickedGenElectrons.size(); j++){
    	for (int k=j+1; k<pickedGenElectrons.size(); k++){
    		if(j==k) continue;
    		TLorentzVector p4Lep1;
    		TLorentzVector p4Lep2;
    		p4Lep1.SetPtEtaPhiM(tree->GenDressedLepton_pt[j], 
                            tree->GenDressedLepton_eta[j], 
                            tree->GenDressedLepton_phi[j], 
                            tree->GenDressedLepton_mass[j]);
    		p4Lep2.SetPtEtaPhiM(tree->GenDressedLepton_pt[k], 
                            tree->GenDressedLepton_eta[k], 
                            tree->GenDressedLepton_phi[k], 
                            tree->GenDressedLepton_mass[k]);
    		TLorentzVector p4GenProbe = p4Lep1 + p4Lep2;
    		pickedGenRefs.push_back(p4GenProbe);
    	}
    }
  }

  // Z->mumu + jets channel
  if(isZeeJet && pickedGenMuons.size()>1){
    for (int j=0; j<pickedGenMuons.size(); j++){
    	for (int k=j+1; k<pickedGenMuons.size(); k++){
    		if(j==k) continue;
    		TLorentzVector p4Lep1;
    		TLorentzVector p4Lep2;
    		p4Lep1.SetPtEtaPhiM(tree->GenDressedLepton_pt[j], 
                            tree->GenDressedLepton_eta[j], 
                            tree->GenDressedLepton_phi[j], 
                            tree->GenDressedLepton_mass[j]);
    		p4Lep2.SetPtEtaPhiM(tree->GenDressedLepton_pt[k], 
                            tree->GenDressedLepton_eta[k], 
                            tree->GenDressedLepton_phi[k], 
                            tree->GenDressedLepton_mass[k]);
    		TLorentzVector p4GenProbe = p4Lep1 + p4Lep2;
    		pickedGenRefs.push_back(p4GenProbe);
    	}
    }
  }

  // Gamma + jets channel
  if(isGamJet && pickedGenPhotons.size()>0){
    for (int j=0; j<pickedPhotons.size(); j++){
      for (int k=j+1; k<pickedGenPhotons.size(); k++){
    		if(j==k) continue;
    		TLorentzVector p4Pho;
    		TLorentzVector p4GenPho;
    		p4Pho.SetPtEtaPhiM(tree->Photon_pt[j], tree->Photon_eta[j], tree->Photon_phi[j], tree->Photon_mass[j]);
        p4GenPho.SetPtEtaPhiM(tree->GenIsolatedPhoton_pt[k],tree->GenIsolatedPhoton_eta[k],
   			  tree->GenIsolatedPhoton_phi[k],tree->GenIsolatedPhoton_mass[k]);
        if(p4GenPho.DeltaR(p4Pho) < 0.2) pickedRefs.push_back(p4Pho); 
      }
    }
  }
  if(isDebug) cout<<"pickedGenRefs.size() = "<<pickedGenRefs.size()<<endl;
}//pickGenRefs


ObjectPick::~ObjectPick(){
}
