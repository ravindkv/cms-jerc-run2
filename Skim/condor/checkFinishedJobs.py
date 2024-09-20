import re
import os
import sys
import json
import itertools
from ROOT import TFile
from createJobFiles import createJobs
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor",""))
from Inputs import *

#-------------------------------------------------
# Open each finished file and see if they are OK
#-------------------------------------------------
def checkJobs(jsonFile):
    unfinished =  {}
    nEvents = 0
    print("Checking corrupted files ...") 
    for sKey, skims in jsonFile.items():
        corruptedList = []
        nEvents = 0
        for skim in skims :
            try:
                f = TFile.Open(skim, "READ")
            except Exception:
                corruptedList.append(skim)
                #print("Unable to open: %s"%skim)
                continue
            if not f:
                print("Null pointer: %s"%skim)
                corruptedList.append(skim)
                continue
            if f.IsZombie():
                print("Zombie: %s"%skim)
                corruptedList.append(skim)
                continue
            if f.GetSize() < 3000:
                print("Empty file: %s"%skim)
                corruptedList.append(skim)
                continue
            h = f.Get("hEvents")
            if not h:
                print("hEvents does not exist: %s"%skim)
                corruptedList.append(skim)
                continue
            nEvents = nEvents + h.GetBinContent(2)
            f.Close()
        if len(corruptedList)>0:
            print("\n------ %s jobs to be resubmitted for %s ------\n"%(len(corruptedList),  sKey))
            unfinished[sKey] = corruptedList
    return unfinished

#-------------------------------------------------
# Open the skim files, check each of them and
# create new job file for the failed jobs 
#-------------------------------------------------
logDir = "resubLog"
dResubs = {}
fResub  = "tmpSub/resubFilesSkim.json"
jdlFile_ = open('tmpSub/resubJobs.jdl', 'w')
if os.path.exists(fResub):
    fResub_  = open(fResub, "r")
    jsonFile = json.load(fResub_)
    dResub = checkJobs(jsonFile)
    fResub_.close()
    dResubs.update(dResub)
    fResub__  = open(fResub, "w")
    json.dump(dResubs, fResub__, indent=4)
    fResub__.close()
    fResub___  = open(fResub, "r")
    createJobs(fResub___, jdlFile_, logDir)
else:
    for year, ch in itertools.product(Years, Channels):
        print(f"=======> {year}: {ch}")
        fSkim = open(f"../input/json/FilesSkim_{year}_{ch}.json", "r")
        jsonFile = json.load(fSkim)
        dResub = checkJobs(jsonFile)
        dResubs.update(dResub)
    fResub_  = open(fResub, "w")
    json.dump(dResubs, fResub_, indent=4)
    fResub_.close()
    fResub__  = open(fResub, "r")
    createJobs(fResub__, jdlFile_, logDir)

totResub = 0
for resub in list(dResubs.keys()):
    totResub = totResub + len(dResubs[resub])
print(f"\n=========> Total to be resubmitted = {totResub} <==========\n")
