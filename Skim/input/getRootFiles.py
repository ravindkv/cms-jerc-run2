# Skim/input/getFiles.py

import os
import sys
import subprocess
import json
import math
from pathlib import Path
import itertools

# Add parent directory to sys.path to import Inputs.py
currentDir = Path(__file__).resolve().parent
parentDir = currentDir.parent
sys.path.insert(0, str(parentDir))

from Inputs import eosSkimDir, Years, Channels, eventsPerJobMC, eventsPerJobData

def getFiles(dataset):
    """
    Fetches the list of files for a given dataset using dasgoclient.
    """
    try:
        dasquery = ["dasgoclient", "-query=file dataset=%s" % dataset]
        output = subprocess.check_output(dasquery, stderr=subprocess.STDOUT)
        files = output.decode('utf-8').strip().splitlines()
        return files
    except subprocess.CalledProcessError as e:
        print(f"Error fetching files for dataset '{dataset}': {e.output.decode('utf-8')}")
        return []

def getEvents(dataset):
    """
    Fetches the number of events for a given dataset using dasgoclient.
    """
    try:
        dasquery = ["dasgoclient", "-query=summary dataset=%s" % dataset]
        output = subprocess.check_output(dasquery, stderr=subprocess.STDOUT)
        summary = json.loads(output.decode('utf-8').strip())
        nEvents = summary[0].get('nevents', 0)
        return nEvents
    except (subprocess.CalledProcessError, json.JSONDecodeError, IndexError) as e:
        print(f"Error fetching event count for dataset '{dataset}': {e}")
        return 0

def formatNum(num):
    """
    Formats a number into a human-readable string with suffixes.
    """
    suffixes = ['', 'K', 'M', 'B', 'T']
    magnitude = 0
    while abs(num) >= 1000 and magnitude < len(suffixes) - 1:
        magnitude += 1
        num /= 1000.0
    return f"{round(num, 1)}{suffixes[magnitude]}"

def main():
    allJobs = 0
    jsonDir = currentDir / "json"
    jsonDir.mkdir(exist_ok=True)

    # Iterate over each channel
    for channel in Channels:
        allJobsChannel = 0
        
        print(f"\n===========: Channel = {channel} :============\n")
        # Path to the channel-based JSON file
        samplesJsonPath = f"SamplesNano_{channel}.json"

        with open(samplesJsonPath, 'r') as f:
            samplesData = json.load(f)

        for year in Years:
            toNano = {}
            toSkim = {}
            toJobs = {}
            allJobsYear = 0
            print(f"========> {year}")
            if year not in samplesData:
                print(f"Year '{year}' not found in '{samplesJsonPath}'. Skipping.\n")
                continue

            yearData = samplesData[year]
            mcSamples = yearData.get("MC", {})
            dataSamples = yearData.get("Data", {})

            # Combine MC and Data samples
            allSamples = {}
            allSamples.update(mcSamples)
            allSamples.update(dataSamples)

            for sampleKey, dataset in allSamples.items():
                if(len(sampleKey.split("_"))!=4):
                    print(f"{sampleKey} has more than 3 underscores, follow DataOrMC_Channel_Year_SampleName convention")
                    print(f"Skipped this {sampleKey}")
                    continue
                xsecOrLumi = 1
                if "MC" in sampleKey: xsecOrLumi = dataset["xsecInPb"]
                else: xsecOrLumi = dataset["lumiInFb"]
                datasetName = dataset["nameOnDAS"]
                filesNano = getFiles(datasetName)
                if not filesNano:
                    print(f"PROBLEM: No files found for datasetName '{datasetName}'.\n")
                    continue

                toNano[sampleKey] = filesNano
                nFiles = len(filesNano)
                nEvents = getEvents(datasetName)
                evtStr = formatNum(nEvents)

                # Determine the number of jobs based on whether it's Data or MC
                if "Data" in sampleKey:
                    nJobs = math.ceil(nEvents / eventsPerJobData)
                else:
                    nJobs = math.ceil(nEvents / eventsPerJobMC)

                # Ensure the number of jobs does not exceed the number of files
                nJobs = min(nJobs, nFiles)
                jobDict = {}
                #Lumi per era:
                #2017, 2018: https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2018Analysis
                #2016Pre/Post: Provided by Fikri
                jobDict["xsecOrLumi"] = xsecOrLumi
                jobDict["nSkimJobs"] = nJobs
                jobDict["nEvents"] = nEvents
                jobDict["nEventsPretty"] = evtStr
                jobDict["nNanoFiles"] = nFiles
                toJobs[sampleKey] = [xsecOrLumi, nJobs, evtStr, nEvents, nFiles]
                allJobsYear += nJobs

                # Generate skim file paths
                skimFiles = [
                    f"{eosSkimDir}/{channel}/{year}/{sampleKey}_Skim_{i+1}of{nJobs}.root"
                    for i in range(nJobs)
                ]
                toSkim[sampleKey] = [jobDict, skimFiles]

                print(f"{nFiles}\t {nJobs}\t {evtStr}\t {sampleKey}")

            print(f"AllJobs for {year} = {allJobsYear}\n")
            allJobsChannel += allJobsYear
            # Define output JSON file paths
            filesNanoPath = jsonDir / f"FilesNano_{channel}_{year}.json"
            #jobsSkimPath = jsonDir / f"JobsSkim_{channel}_{year}.json"
            filesSkimPath = jsonDir / f"FilesSkim_{channel}_{year}.json"
            
            # Write JSON files
            with open(filesNanoPath, 'w') as f:
                json.dump(toNano, f, indent=4)
            #with open(jobsSkimPath, 'w') as f:
                #json.dump(toJobs, f, indent=4)
            with open(filesSkimPath, 'w') as f:
                json.dump(toSkim, f, indent=4)
            
        print(f"AllJobs for {channel} = {allJobsChannel}\n")
        allJobs += allJobsChannel
    print('---------------------------------------')
    print(f"AllJobs = {allJobs}")
    print('---------------------------------------')

if __name__ == "__main__":
    main()

