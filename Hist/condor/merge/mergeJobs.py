import os
import sys
import json
import itertools
import multiprocessing
from functools import partial
import subprocess

sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor/merge",""))
from Inputs import *
Channels =  channelDetails

def merge_root_files(sKey, jHist, dirName, channel, year):
    dirPath = os.path.join(eosHistDir, channel, "MergedJobs", year)
    os.makedirs(dirPath, exist_ok=True)
    hAddIn = ' '.join(jHist[sKey])
    hAddOut = f"{dirPath}/{sKey}_Hist_Merged.root"
    print(f"Merging into {hAddOut}")
    os.system(f"hadd -f -v 0 -k {hAddOut} {hAddIn}")
    return sKey, hAddOut

if __name__=="__main__":
    histDir = "../../input/json/"
    dirName = "merged_json"
    os.makedirs(dirName, exist_ok=True)
    
    allJobs = 0
    tasks = []

    for ch, year in itertools.product(Channels.keys(), Years):
        print(f"\nProcessing Channel: {ch}, Year: {year}\n")
        fHist = open(f"{histDir}/FilesHist_{ch}_{year}.json", "r")
        jHist = json.load(fHist)
        fHist.close()  # Close the file after reading

        dMerged = {}
        sKeysToProcess = []

        for sKey in jHist.keys():
            sKeysToProcess.append(sKey)

        # Prepare the merge tasks
        with multiprocessing.Pool() as pool:
            # Use partial to fix the arguments that are the same for all tasks
            merge_func = partial(merge_root_files, channel=ch, year=year, jHist=jHist, dirName=dirName)
            # Map the function to the list of sKeys
            results = pool.map(merge_func, sKeysToProcess)

        # Collect the results
        for sKey, hAddOut in results:
            dMerged[sKey] = hAddOut

        fileName = f"FilesHistMergedJobs_{ch}_{year}.json"
        with open(f"{dirName}/{fileName}", "w") as fHistMerged:
            json.dump(dMerged, fHistMerged, indent=4)

        print(f"Merged histograms saved to {dirName}/{fileName}")

