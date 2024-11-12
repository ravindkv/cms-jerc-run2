import os
import sys
import json
import itertools
import multiprocessing
from functools import partial

sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("input/root",""))
from Inputs import *

def merge_histograms(sKey, jHist, dirName):
    # Remove the last part of the path (the filename)
    dirPath = os.path.dirname(jHist[sKey][0])
    hAddIn = ' '.join(jHist[sKey])
    hAddOut = f"{dirPath}/{sKey}_Hist_Merged.root"
    print(f"Merging into {hAddOut}")
    os.system(f"hadd -f -v 0 -k {hAddOut} {hAddIn}")
    return sKey, hAddOut

if __name__=="__main__":
    histDir = "../../../Hist/input/json/"
    dirName = "json"
    os.makedirs(dirName, exist_ok=True)
    
    allJobs = 0
    tasks = []

    for ch, year in itertools.product(Channels.keys(), Years):
        chHist = Channels[ch]
        print(f"\nProcessing Channel: {ch}, Year: {year}\n")
        fHist = open(f"{histDir}/FilesHist_{chHist}_{year}.json", "r")
        jHist = json.load(fHist)
        fHist.close()  # Close the file after reading

        dPlot = {}
        sKeysToProcess = []

        for sKey in jHist.keys():
            sKeysToProcess.append(sKey)

        # Prepare the merge tasks
        with multiprocessing.Pool() as pool:
            # Use partial to fix the arguments that are the same for all tasks
            merge_func = partial(merge_histograms, jHist=jHist, dirName=dirName)
            # Map the function to the list of sKeys
            results = pool.map(merge_func, sKeysToProcess)

        # Collect the results
        for sKey, hAddOut in results:
            dPlot[sKey] = hAddOut

        fileName = f"FilesHistMerged_{ch}_{year}.json"
        with open(f"{dirName}/{fileName}", "w") as fHistMerged:
            json.dump(dPlot, fHistMerged, indent=4)

        print(f"Merged histograms saved to {dirName}/{fileName}")

