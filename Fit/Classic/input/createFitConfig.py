#!/usr/bin/env python3

import os
import sys
import json
import ROOT

sys.dont_write_bytecode = True
# make sure to import Inputs from the parent directory as before
sys.path.insert(0, os.getcwd().replace("input", ""))
from Inputs import eosFitDir, Years, ChannelsL3Res

# Directories for input and output fit files
strL3ResInput = "L3ResInput"
strL3ResOutput = "L3ResOutput"
dirL3ResInput = f"{eosFitDir}/{strL3ResInput}"
dirL3ResOutput = f"{eosFitDir}/{strL3ResOutput}"

def get_era_from_key(key):
    """
    E.g. Data_Wqqm_2017B_SingleMuon -> '2017B'
         MC_Wqqm_2017_something     -> '2017'
    """
    parts = key.split('_')
    return parts[2]

def process_year(year, channels, json_dir):
    for channel in channels:
        # Construct the JSON file name (assumes FilesHist_<channel>_<year>.json)
        json_filename = os.path.join(json_dir, f"FilesHist_{channel}_{year}.json")
        if not os.path.exists(json_filename):
            print(f"JSON file {json_filename} does not exist, skipping channel {channel} for year {year}.")
            continue
        
        print(f"Processing JSON file: {json_filename}")
        with open(json_filename, "r") as jf:
            hist_files = json.load(jf)
        
        # Group the ROOT file paths by era (as defined by the key)
        era_to_files = {}
        for key, paths in hist_files.items():
            era = get_era_from_key(key)
            if era not in era_to_files:
                era_to_files[era] = []
            era_to_files[era].extend(paths)
        
        # For each era, merge the corresponding histogram(s)
        for era, file_list in era_to_files.items():
            print(f"Processing channel {channel}, era {era} with {len(file_list)} files")
            # Get the histogram path from Inputs.py; we use the first element for "Resp"
            hist_path = ChannelsL3Res[channel][0]
            merged_resp = None

            # Loop over each ROOT file for this era
            for file_path in file_list:
                tf = ROOT.TFile.Open(file_path)
                if not tf or tf.IsZombie():
                    print(f"Error opening file {file_path}")
                    continue
                h = tf.Get(hist_path)
                if not h:
                    print(f"Histogram '{hist_path}' not found in {file_path}")
                    tf.Close()
                    continue
                if merged_resp is None:
                    merged_resp = h.Clone(f"Resp_{channel}_{era}")
                    merged_resp.SetDirectory(0)  # detach histogram from file
                else:
                    merged_resp.Add(h)
                tf.Close()
            
            if merged_resp is None:
                print(f"No valid histograms found for channel {channel} era {era}")
                continue

            # Create the two derived histograms:
            # herr is obtained by scaling Resp by 1.05
            # norm is obtained by scaling Resp by 1.10
            merged_resp.Scale(1/80)
            herr = merged_resp.Clone(f"herr_{channel}_{era}")
            norm = merged_resp.Clone(f"norm_{channel}_{era}")
            herr.Scale(1.05)
            norm.Scale(1.10)

            # Write the three histograms into an output ROOT file.
            # The file is named e.g. L3ResOutput_Wqqm_2017B.root
            out_filename = f"{dirL3ResOutput}/{strL3ResOutput}_{channel}_{era}.root"
            out_file = ROOT.TFile(out_filename, "RECREATE")
            merged_resp.Write("Resp")
            herr.Write("herr")
            norm.Write("norm")
            out_file.Close()
            print(f"Wrote output file: {out_filename}")

def main():
    ROOT.gROOT.SetBatch(True)
    channels = list(ChannelsL3Res.keys())
    # Define the directory where the JSON files are located.
    json_dir = os.path.join("..", "..", "..", "Hist", "input", "json")
    
    # Ensure that the input/output directories exist
    if not os.path.exists(dirL3ResInput):
        os.makedirs(dirL3ResInput, exist_ok=True)
    if not os.path.exists(dirL3ResOutput):
        os.makedirs(dirL3ResOutput, exist_ok=True)
    
    # Loop over the years and process each one.
    for year in Years:
        print(f"Processing year {year} for channels: {channels}")
        process_year(year, channels, json_dir)

if __name__ == "__main__":
    main()

