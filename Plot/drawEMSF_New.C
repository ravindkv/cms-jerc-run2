// Purpose: Visualize (average) EM SF vs pT for |eta|<1.3
//          This is helpful to set photon scale in global fit
#include "TFile.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include "tdrstyle_mod15.C"


void drawPerGain(string gain){
  TFile *fp = new TFile("Data_2022C_GamJet_EGamma_Hist_PerGains.root","READ");
  assert(fp && !fp->IsZombie());

  TProfile *tPhoScaleSF     =(TProfile*)fp->Get(("control/tPhoScaleSF_Gain"+gain).c_str());
  TProfile *tPhoScaleSF_1   =(TProfile*)fp->Get(("control/tPhoScaleSF_etaBin1Gain"+gain).c_str());
  TProfile *tPhoScaleSF_2   =(TProfile*)fp->Get(("control/tPhoScaleSF_etaBin2Gain"+gain).c_str());
  TProfile *tPhoScaleSF_3   =(TProfile*)fp->Get(("control/tPhoScaleSF_etaBin3Gain"+gain).c_str());
  TProfile *tPhoScaleSF_4   =(TProfile*)fp->Get(("control/tPhoScaleSF_etaBin4Gain"+gain).c_str());
  TProfile *tPhoScaleSF_5   =(TProfile*)fp->Get(("control/tPhoScaleSF_etaBin5Gain"+gain).c_str());
  TProfile *tPhoScaleSF_6   =(TProfile*)fp->Get(("control/tPhoScaleSF_etaBin6Gain"+gain).c_str());

  lumi_13TeV = "2022C";
  TH1D *h = tdrHist("h","EM SF",0.99,1.02);
  TCanvas *c1 = tdrCanvas("c1",h,4,11,kSquare);
  //TCanvas *c1 = new TCanvas(); 
  c1->SetLogx();
  h->GetXaxis()->SetRangeUser(20,1500);//500);

  tdrDraw(tPhoScaleSF,"PL",kFullCircle);//kCyan+2);
  tPhoScaleSF->SetLineWidth(5);
  tdrDraw(tPhoScaleSF_1,"PL",kNone,kRed);
  tdrDraw(tPhoScaleSF_2,"PL",kNone,kOrange+2);
  tdrDraw(tPhoScaleSF_3,"PL",kNone,kGreen+2);
  tdrDraw(tPhoScaleSF_4,"PL",kNone,kBlue);
  tdrDraw(tPhoScaleSF_5,"PL",kNone,kCyan);
  tdrDraw(tPhoScaleSF_6,"PL",kNone,kMagenta);

  TF1 *f1 = new TF1("f1","[0]",100,180);
  f1->SetLineColor(kMagenta+1);
  f1->SetLineWidth(2);
  tPhoScaleSF->Fit(f1,"QRN");
  f1->SetRange(100,200);
  f1->Draw("SAME");

  TF1 *f2 = new TF1("f2","[0]",230,500);
  f2->SetLineColor(kMagenta+2);
  f2->SetLineWidth(3);
  tPhoScaleSF->Fit(f2,"QRN");
  f2->SetRange(200,500);
  f2->Draw("SAME");

  TLegend *leg = tdrLeg(0.60,0.56,0.85,0.92);
  leg->SetTextSize(0.035);
  leg->AddEntry(tPhoScaleSF," data for all #eta","PLE");
  leg->AddEntry(f2," fits of data","L");
  //leg->AddEntry(tPhoScaleSF,"|#eta|<1.5 tot.","LE");
  leg->AddEntry(tPhoScaleSF_1," -1.442 < #eta #leq -1.2","LE");
  leg->AddEntry(tPhoScaleSF_2," -1.2 < #eta #leq -1.0","LE");
  leg->AddEntry(tPhoScaleSF_3," -1.0 < #eta #leq 0.0","LE");
  leg->AddEntry(tPhoScaleSF_4," 0.0 < #eta  #leq 1.0","LE");
  leg->AddEntry(tPhoScaleSF_5," 1.0 < #eta  #leq 1.2","LE");
  leg->AddEntry(tPhoScaleSF_6," 1.2 < #eta  #leq 1.442","LE");

  TPaveText *pText = new TPaveText(0.40,0.80,0.50,0.90, "brNDC");
  pText->AddText(("Gain = "+gain).c_str());
  pText->Draw();

  c1->SaveAs(("pdf/drawEMSF_Gain"+gain+".pdf").c_str());
} // drawEMSF
  //
 
void drawEMSF_New() {

  setTDRStyle();
  TDirectory *curdir = gDirectory;

  curdir->cd();
  drawPerGain("1");
  drawPerGain("6");
  drawPerGain("12");
}

