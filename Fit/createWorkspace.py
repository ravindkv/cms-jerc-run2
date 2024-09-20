import ROOT
import pandas as pd

ROOT.gROOT.SetBatch(1)

df = pd.read_csv('data_calorimeter.csv')
df['ediff']=df['e_measured']-df['e_true']

bin_centres = range(10,55,5)
nb = len(bin_centres)


maxDiff =  0.5
minDiff = -0.5

def fillHistogram(arr):

  h = ROOT.TH1F("h","h",50,minDiff,maxDiff)
  h.Sumw2()
  for a in arr: h.Fill(a)
  return h


hists = []


ws = ROOT.RooWorkspace("wspace","Workspace")

x = ROOT.RooRealVar("ediff","e_{measured}-e{true}",0,minDiff,maxDiff)
s = ROOT.RooRealVar("s","parmeter s, #sigma=s #sqrt{E}",0.02,0.0001,0.1)

getattr(ws,'import')(x)
getattr(ws,'import')(s)

for i,E in enumerate(bin_centres):
  hist = fillHistogram(df[df.e_true==E]["ediff"].tolist())
  hist.SetName("hist_true_%g"%E)

  rdh = ROOT.RooDataHist("data_obs_E%d"%E,"Data for e_{true}=%g"%E,ROOT.RooArgSet(ws.var(x.GetName())),hist)
  getattr(ws,'import')(rdh)

  # now for the model - a simple Gaussian
  mu    = ROOT.RooRealVar("mu_E%d"%E,"My for E=%g"%E,0,minDiff,maxDiff)
  sigma = ROOT.RooFormulaVar("sigma_E%d"%E,"Sigma for E=%g"%E,"@0*TMath::Sqrt(%g)"%E,ROOT.RooArgList(ws.var(s.GetName())))

  model = ROOT.RooGaussian("pdf_E%d"%E,"PDF for E=%g"%E,ws.var(x.GetName()),mu,sigma)
  model.fitTo(rdh)
  model_norm = ROOT.RooRealVar("pdf_E%d_norm"%E,"Normalisation for bin E=%g"%E,rdh.sumEntries())
  print(rdh.sumEntries())
  getattr(ws,'import')(model)
  getattr(ws,'import')(model_norm)

ws.Print()
fout=ROOT.TFile.Open("fout.root","RECREATE")
fout.WriteTObject(ws)
