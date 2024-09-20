#ifndef OBJECTPICK_H
#define OBJECTPICK_H

#include<vector>
#include<string>
#include<set>
#include<iostream>
#include<fstream>
#include<TH1F.h>
#include<TH1D.h>

#include"SkimTree.h"
#include"GlobalFlag.h"
#include "TLorentzVector.h"

class ObjectPick: public GlobalFlag{
public:
	ObjectPick(TString oName): GlobalFlag(oName){};
  //Reco objects
  void pickElectrons();
  void pickMuons();
  void pickPhotons();
  void pickRefs();

  vector<int> pickedElectrons;
  vector<int> pickedMuons;
  vector<int> pickedPhotons;
  vector<TLorentzVector> pickedRefs;

  //Gen objects
  void pickGenElectrons();
  void pickGenMuons();
  void pickGenPhotons();
  void pickGenRefs();

  vector<int> pickedGenElectrons;
  vector<int> pickedGenMuons;
  vector<int> pickedGenPhotons;
  vector<TLorentzVector> pickedGenRefs;

  void clearObjects();
  void setTree(SkimTree *skimT);
  
	~ObjectPick();

private:
  SkimTree * tree;
};
#endif
