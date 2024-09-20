import os
import json

#-------------------------------------------------
# hadd (=merge) output root files
#-------------------------------------------------
fHist_  = "../sample/FilesHist_cff.json"
fHist = open(fHist_, "r")
data = json.load(fHist)
unfinished =  {}
nEvents = 0
for sKey, hists in data.items():
    print(sKey)
    outRawDir = hists[0].split(sKey)[0]
    outMergeDir = outRawDir.replace("Raw", "Merged") 
    os.system(f"mkdir -p {outMergeDir}")

    haddIn  = " ".join(hists)
    haddOut = f"{outMergeDir}/{sKey}.root"
    #print(f"hadd -f  {haddOut} {haddIn}") 
    os.system(f"hadd -f -v 0 {haddOut} {haddIn}") 


