import os
import sys
import json
import itertools
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("input",""))
from Inputs import *

if __name__=="__main__":
    histDir = "../../Hist/input/json/"
    
    dirName = "json/rootFile"
    os.system(f"mkdir -p {dirName}")
    allJobs = 0
    for year, ch in itertools.product(Years, Channels.keys()):
        chHist = Channels[ch]
        print(f"\n{year} : {ch}\n")
        fHist = open(f"{histDir}/FilesHist_{year}_{chHist}.json", "r")
        jHist = json.load(fHist)

        dPlot = {}
        for sKey in jHist.keys():
            if year not in sKey:
                continue
            # Remove the last part of the path (the filename)
            print(f"\n===========> {sKey} <============\n")
            dirPath = os.path.dirname(jHist[sKey][0])
            hAddIn = ' '.join(jHist[sKey])
            hAddOut = f"{dirPath}/{sKey}_Hist_Merged.root"
            os.system(f"hadd -f -k {hAddOut} {hAddIn}")
            dPlot[sKey] = hAddOut
        fileName = f"FilesHistMerged_{year}_{ch}.json"
        fHistMerged = open(f"{dirName}/{fileName}", "w")
        json.dump(dPlot, fHistMerged, indent=4) 
        print(f"{dirName}/{fileName}")
    
