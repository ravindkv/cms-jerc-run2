
import re
import os
import json
from ROOT import TFile
from createJdlFiles import createJobs

#-------------------------------------------------
# Open each finished file and see if they are OK
#-------------------------------------------------
fHist_  = "../sample/FilesHist_cff.json"
fResub_ = "tmpSub/resub_FilesHist_cff.json"
fHist = open(fHist_, "r")
data = json.load(fHist)
unfinished =  {}
nEvents = 0
print("Checking corrupted files ...") 
for sKey, hists in data.items():
    print(sKey)
    corruptedList = []
    nEvents = 0
    for hist in hists :
        try:
            f = TFile.Open(hist, "READ")
        except Exception:
            corruptedList.append(hist)
            #print("Unable to open: %s"%hist)
            continue
        if not f:
            print("Null pointer: %s"%hist)
            corruptedList.append(hist)
            continue
        if f.IsZombie():
            print("Zombie: %s"%hist)
            corruptedList.append(hist)
            continue
        if f.GetSize() < 3000:
            print("Empty file: %s"%hist)
            corruptedList.append(hist)
            continue
        #h = f.Get("hEvents")
        #if not h:
        #    print("hEvents does not exist: %s"%hist)
        #    corruptedList.append(hist)
        #    continue
        #nEvents = nEvents + h.GetBinContent(2)
        f.Close()
    if len(corruptedList)>0:
        print("\n------ %s jobs to be resubmitted for %s ------\n"%(len(corruptedList),  sKey))
    unfinished[sKey] = corruptedList

fResub  = open(fResub_, "w")
json.dump(unfinished, fResub, indent=4)
fResub.close()

#-------------------------------------------------
# Create jdl files again for unfinished jobs 
#-------------------------------------------------
logDir = "log_resub"
jdlName = 'tmpSub/resub_submitJobs_cff.jdl'
createJobs(open(fResub_, "r"), open(jdlName, "w"), logDir)

