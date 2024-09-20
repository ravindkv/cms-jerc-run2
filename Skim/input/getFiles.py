import os
import sys
import subprocess
import numpy as np
import itertools
import json
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("input",""))
from Inputs import *

def getFiles(dataset):
    dasquery = ["dasgoclient",  "-query=file dataset=%s" % dataset]
    files = subprocess.check_output(dasquery)
    files = files.splitlines()
    files_ = []
    for file in files:
        files_.append(file.decode('utf-8'))
    return files_

def getEvents(dataset):
    dasquery = ["dasgoclient",  "-query=summary dataset=%s" % dataset]
    summary  = subprocess.check_output(dasquery)
    summary  = summary.splitlines()
    summary  = json.loads(summary[0].decode('utf-8'))
    nevents  = summary[0]['nevents'] 
    return nevents

#function to convert number to million
def formatNum(num):
    suffixes = ['', 'K', 'M', 'B', 'T']
    magnitude = 0
    while abs(num) >= 1000 and magnitude < len(suffixes)-1:
        magnitude += 1
        num /= 1000.0
    return f"{round(num, 1)}{suffixes[magnitude]}"

if __name__=="__main__":
    allJobs = 0
    if not os.path.exists("json"):
        os.makedirs("json")
    for year, ch in itertools.product(Years, Channels):
        toNano = {}
        toSkim = {}
        toJobs = {}
        f1 = open(f"json/FilesNano_{year}_{ch}.json", "w")
        f2 = open(f"json/JobsSkim_{year}_{ch}.json", "w")
        f3 = open(f"json/FilesSkim_{year}_{ch}.json", "w")
        print('---------------------------------------')
        print(f"\t{year}: {ch}")
        print("nFiles\t  nJobs\t nEvents\t Samples")
        print('---------------------------------------')
        jobs = 0
        exec(f"from SamplesNano_{year} import sampleDict") 
        for sKey, sName in sampleDict().items():
            if not ch in sKey: continue
            fNano = getFiles(sName)
            if not fNano:
                print(f'PROBLEM: {sName}\n')
                continue
            toNano[sKey] = fNano
            nFiles = len(fNano)
            evt     = getEvents(sName)
            evtStr  = formatNum(evt) 
            if "Data" in sKey:
                nJob = int(np.ceil(evt/evtPerJobData))
            else:
                nJob = int(np.ceil(evt/evtPerJobMC))
            if nFiles<nJob: 
                nJob = nFiles
            toJobs[sKey] = [nJob, evtStr, evt, nFiles]
            jobs += nJob
            fSkim = []
            for i in range(nJob):
                fSkim.append("%s/%s/%s/%s_Skim_%sof%s.root"%(eosSkimDir, year, ch, sKey, i+1, nJob))
            toSkim[sKey] = fSkim
            print("%i\t %i\t %s\t %s"%(nFiles, nJob, evtStr, sKey))
        print("AllJobs_%s_%s = %i"%(year, ch, jobs))
        allJobs += jobs
        json.dump(toNano, f1, indent=4)
        json.dump(toJobs, f2, indent=4)
        json.dump(toSkim, f3, indent=4)
        f1.close()
        f2.close()
        f3.close()
    print('---------------------------------------')
    print("AllJobs_AllYears = %s"%str(allJobs))
    print('---------------------------------------')
 
