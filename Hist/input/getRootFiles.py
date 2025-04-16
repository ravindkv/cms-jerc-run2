import os
import sys
import json
import itertools
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("input",""))
from Inputs import *

#Reduce number of condor jobs w.r.t Skim by a factor of rData and rMC
def reducedJob(nJob, sKey):
    rData   = reduceJobsDataBy
    rMC     = reduceJobsMCBy
    if "Data" in sKey:
        if nJob>rData:
            n = nJob/rData
        else:
            n = 1
    else:
        if nJob>rMC:
            n = nJob/rMC
        else:
            n = 1
    return int(n)

if __name__=="__main__":
    skimDir = "../../Skim/input/json/"
    os.system("mkdir -p json")
    allJobs = 0
    for ch, year in itertools.product(channelDetails.keys(), Years):
        fSkim = open(f"{skimDir}/FilesSkim_{ch}_{year}.json", "r")
        jSkim = json.load(fSkim)
        #Replace the keys (DiJet, IncJet, etc to use same Skims)
        keyMap = {}
        for oldSkimKey in jSkim.keys():
            newSkimKey = oldSkimKey.replace(f"_{ch}_", f"_{ch}_")
            keyMap[oldSkimKey] = newSkimKey
        for oldSkimKey, newSkimKey in keyMap.items():
            if oldSkimKey in jSkim:
                jSkim[newSkimKey] = jSkim.pop(oldSkimKey)
        
        fHist = open(f"json/FilesHist_{ch}_{year}.json", "w")
        dHist = {}
        print(f"{eosHistDir}/{ch}/{year}")
        yJobs = 0
        for sKey in jSkim.keys():
            if year not in sKey:
                continue
            lSkim = jSkim[sKey][1]
            nJob  = reducedJob(len(lSkim), sKey)
            yJobs = yJobs+nJob
            lHist = []
            for i in range(nJob):
                lHist.append("%s/%s/%s/%s_Hist_%sof%s.root"%(eosHistDir, ch, year, sKey, i+1, nJob))
            dHist[sKey] = lHist
            print(f"{ch}: {year}:  {sKey}: nJob = {nJob}")
        print(f"\n{ch}: {year} :  nJobs  = {yJobs}\n")
        allJobs = allJobs + yJobs
        fSkimNew = open(f"json/FilesSkim_{ch}_{year}.json", "w")
        json.dump(jSkim, fSkimNew, indent=4) 
        json.dump(dHist, fHist, indent=4) 
    print(f"All jobs =  {allJobs}")
    
