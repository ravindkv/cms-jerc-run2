#!/usr/bin/env python3
import os
import sys
import json
import ROOT
from pathlib import Path

# Set up paths: assume this script is in a subdirectory (e.g. "input")
script_dir = Path(__file__).resolve().parent
project_root = script_dir.parent
sys.path.insert(0, str(project_root))

from Inputs import EOS_FIT_DIR, YEARS, CHANNELS_L3RES
from Config import update_fit_config_for_channel_year

class HistogramMerger:
    def __init__(self, json_dir, output_dir):
        self.json_dir = Path(json_dir)
        self.output_dir = Path(output_dir)
    
    @staticmethod
    def get_era_from_key(key):
        parts = key.split('_')
        if len(parts) < 3:
            raise ValueError(f"Key '{key}' does not have enough parts to extract era.")
        return parts[2]
    
    def merge_histograms_for_era(self, file_list, hist_path, hist_name):
        merged_hist = None
        for file_path in file_list:
            tf = ROOT.TFile.Open(file_path)
            if not tf or tf.IsZombie():
                continue
            h = tf.Get(hist_path)
            if not h:
                tf.Close()
                continue
            if merged_hist is None:
                merged_hist = h.Clone(hist_name)
                merged_hist.SetDirectory(0)
            else:
                merged_hist.Add(h)
            tf.Close()
        return merged_hist
    
    def process_channel_year(self, channel, year, channel_config):
        json_filename = self.json_dir / f"FilesHist_{channel}_{year}.json"
        if not json_filename.exists():
            print(f"{json_filename} does not exist. Skipping channel {channel} for year {year}.")
            return None
        
        with open(json_filename, "r") as jf:
            hist_files = json.load(jf)
        
        # Group files by era using a key convention (e.g. someprefix_<something>_<era>)
        era_to_files = {}
        for key, paths in hist_files.items():
            try:
                era = self.get_era_from_key(key)
            except ValueError as e:
                print(e)
                continue
            era_to_files.setdefault(era, []).extend(paths)
        
        merged_files = {}
        # Process each era
        for era, file_list in era_to_files.items():
            print(f"Processing channel {channel}, era {era} with {len(file_list)} files.")
            output_histograms = {}
            # Loop over each variation defined in the channel configuration.
            for var_name, hist_path in channel_config.get("paths", {}).items():
                merged_hist = self.merge_histograms_for_era(
                    file_list, hist_path, f"{var_name}_{channel}_{era}"
                )
                if merged_hist:
                    output_histograms[var_name] = merged_hist
                else:
                    print(f"Histogram for variation '{var_name}' not found in channel {channel}, era {era}.")
            
            # For the Base histogram, also create derived histograms.
            base_hist = output_histograms.get("Base")
            if base_hist:
                # Create derived histograms: "herr" and "norm"
                herr = base_hist.Clone(f"herr_{channel}_{era}")
                norm = base_hist.Clone(f"norm_{channel}_{era}")
                herr.Scale(1.05)
                norm.Scale(1.10)
                output_histograms["herr"] = herr
                output_histograms["norm"] = norm
                
                # Create scaled variations from the Base histogram based on scale factors.
                for scale_name, factor in channel_config.get("scales", {}).items():
                    scaled_hist = base_hist.Clone(f"{scale_name}_{channel}_{era}")
                    scaled_hist.Scale(factor)
                    output_histograms[scale_name] = scaled_hist
            else:
                print(f"No Base histogram found for channel {channel}, era {era}. Skipping derived histograms.")
            
            # Write output ROOT file containing all histograms.
            out_filename = self.output_dir / f"L3ResOutput_{channel}_{era}.root"
            out_file = ROOT.TFile(str(out_filename), "RECREATE")
            for hist in output_histograms.values():
                hist.Write()
            out_file.Close()
            merged_files[era] = str(out_filename)
        return merged_files

def main():
    ROOT.gROOT.SetBatch(True)
    
    # Define directories using pathlib
    json_dir = project_root / "Hist" / "input" / "json"
    output_dir = Path(EOS_FIT_DIR) / "L3ResOutput"
    json_config_dir = Path("json") / "fit_configs"
    
    json_config_dir.mkdir(parents=True, exist_ok=True)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    merger = HistogramMerger(json_dir, output_dir)
    
    for year in YEARS:
        for channel, channel_config in CHANNELS_L3RES.items():
            merged_files = merger.process_channel_year(channel, year, channel_config)
            if not merged_files:
                continue
            for era, root_filename in merged_files.items():
                # Extract the 4-digit year from the era (e.g. '2017' from '2017F')
                year_key = era[:4]
                # Combine extra systematic names from the channel configuration.
                extra_systs = []
                if "paths" in channel_config:
                    extra_systs.extend([k for k in channel_config["paths"].keys() if k != "Base"])
                if "scales" in channel_config:
                    extra_systs.extend(list(channel_config["scales"].keys()))
                if "herr" not in extra_systs:
                    extra_systs.append("herr")
                
                # Update the fit configuration for this channel and era.
                fit_config = update_fit_config_for_channel_year(
                    channel, year_key, input_var="Resp", extra_systs=extra_systs
                )
                fit_config["input_file"] = root_filename
                
                json_config_filename = json_config_dir / f"fit_config_{channel}_{era}.json"
                with open(json_config_filename, "w") as f:
                    json.dump(fit_config, f, indent=4)
                print(f"Wrote fit config {json_config_filename}")

if __name__ == "__main__":
    main()

