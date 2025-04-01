import re
import os
import sys
import json
import itertools
import multiprocessing

# from ROOT import TFile  # Uncomment in actual environment with ROOT

sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor",""))

from Inputs import *
from createJobFiles import createJobs  # Adjust import path if needed

#-------------------------------------------------
# Function to check a single ROOT file
#-------------------------------------------------
def check_file(args):
    """
    Checks a single file (skim) for corruption.
    Returns tuple: (sKey, skim, is_corrupted)
    """
    from ROOT import TFile  # Import ROOT inside the worker
    sKey, skim = args
    f = None
    try:
        f = TFile.Open(skim, "READ")
        
        if not f or f.IsZombie() or f.GetSize() < 3000:
            print(f"[Corrupt/Empty] {skim}")
            return (sKey, skim, True)
        
        # Check for the specific histogram
        h = f.Get("h1EventInCutflow")
        if not h:
            print(f"[Missing Cutflow] {skim}")
            return (sKey, skim, True)

        # Check for the specific TTree
        tree = f.Get("Events")
        if not tree:
            print(f"[Missing Events TTree] {skim}")
            return (sKey, skim, True)

        # If we got here, the file is considered good
        return (sKey, skim, False)
        
    except Exception as e:
        print(f"[Exception] while opening {skim}: {e}")
        return (sKey, skim, True)
    finally:
        if f:
            f.Close()

#-------------------------------------------------
# Check each file in the new-style JSON
#-------------------------------------------------
def checkJobs(jsonFile):
    """
    Where the input `jsonFile` is of the form:
    {
        "SampleName": [
            {  "xssOrLumi": ..., "nSkimJobs": ..., ... },  # metadata
            [ "file1.root", "file2.root", ... ]            # all files
        ],
        ...
    }
    """
    print("Checking for corrupted files using multiprocessing...")
    unfinished = {}

    # Build list of (key, file) pairs
    file_list = []
    for sKey, val in jsonFile.items():
        # Expect a list: [metadata_dict, [file1, file2, ...]]
        if (isinstance(val, list) and len(val) == 2 and 
            isinstance(val[0], dict) and isinstance(val[1], list)):
            metadata, files = val[0], val[1]
            for skim in files:
                file_list.append((sKey, skim))
        else:
            print(f"WARNING: '{sKey}' does not conform to new JSON structure. Skipping.")
            continue

    # Check in parallel
    with multiprocessing.Pool(processes=5) as pool:
        results = pool.map(check_file, file_list)

    # Build the "unfinished" (corrupted) dictionary
    corrupted_map = {}
    for sKey, skim, is_corrupted in results:
        if is_corrupted:
            # Accumulate corrupted files under each key
            if sKey not in corrupted_map:
                corrupted_map[sKey] = []
            corrupted_map[sKey].append(skim)

    # Build new dictionary in the SAME structure:
    #   "SampleName": [
    #       original_metadata,
    #       [only corrupted_files]
    #   ]
    for sKey, val in jsonFile.items():
        if sKey in corrupted_map and corrupted_map[sKey]:
            # Keep the original metadata
            metadata = val[0]
            corrupted_files = corrupted_map[sKey]
            unfinished[sKey] = [metadata, corrupted_files]

    return unfinished

#-------------------------------------------------
# Main execution block
#-------------------------------------------------
if __name__ == "__main__":
    logDir = "resubLog"
    dResubs = {}
    fResub = "tmpSub/resubFilesSkim.json"

    # Ensure the 'tmpSub' directory exists
    os.makedirs("tmpSub", exist_ok=True)

    with open('tmpSub/resubJobs.jdl', 'w') as jdlFile_:
        if os.path.exists(fResub):
            # If resubmission file exists, let's check those
            with open(fResub, "r") as fResub_:
                jsonFile = json.load(fResub_)
            dResub = checkJobs(jsonFile)

            # Merge into dResubs
            # If a sample re-appears, we overwrite with the newly corrupted list.
            # This means if a file is "fixed," it disappears from the new dictionary.
            for k, v in dResub.items():
                dResubs[k] = v

            # Write updated list of files to resubmit (still in new format)
            with open(fResub, "w") as fResub__:
                json.dump(dResubs, fResub__, indent=4)

            # Create jobs for resubmission
            with open(fResub, "r") as fResub___:
                createJobs(fResub___, jdlFile_, logDir)

        else:
            for year, ch in itertools.product(Years, Channels):
                print(f"\nProcessing Year: {year}, Channel: {ch}")
                json_file_path = f"../input/json/FilesSkim_{ch}_{year}.json"
                with open(json_file_path, "r") as fSkim:
                    jsonFile = json.load(fSkim)

                dResub = checkJobs(jsonFile)
                # Merge each newly corrupted set into dResubs
                for k, v in dResub.items():
                    dResubs[k] = v

            # Write the list of files to resubmit
            with open(fResub, "w") as fResub_:
                json.dump(dResubs, fResub_, indent=4)

            # Create jobs for resubmission
            with open(fResub, "r") as fResub__:
                createJobs(fResub__, jdlFile_, logDir)

    # Calculate total # of files to resubmit
    totResub = sum(len(v[1]) for v in dResubs.values())  # v[1] is the list of corrupted files
    print(f"\n=========> Total files to be resubmitted: {totResub} <==========\n")
    print(f"{fResub}\n")

