import os
import sys
import json
import subprocess

sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor/merge", ""))

from Inputs import *
Channels =  channelDetails

def merge_root_files_run2(Channels, Years, input_dir, output_base_dir):
    """
    Merges yearly histograms into a single 'Run2' ROOT file per channel and per sample.
    
    For channels where MC is not binned (e.g. ZmmJet), the sample name is determined by
    simply dropping the year part. For channels with binned MC (e.g. GamJet), the HT bins
    are grouped into a single sample (e.g. GJets or QCD). For data, we now group files by the
    trigger/dataset (e.g. EGamma, DoubleMuon) so that a single merged file is produced per trigger.
    """

    def parse_sample_name(key_parts):
        # Remove the year component (index 2) and return the remainder joined by underscores.
        return "_".join(key_parts[3:])

    for channel in Channels.keys():
        for year in Years:
            # For MC we use a dictionary: key = (possibly regrouped) sample name; value = list of file paths.
            mc_files_all_years = {}
            # For Data, we now group by the trigger/dataset (which is the fourth part in the key).
            data_files_all_years = {}

            json_file = os.path.join(input_dir, f"FilesHistMergedJobs_{channel}_{year}.json")
            if not os.path.exists(json_file):
                print(f"[Warning] JSON not found: {json_file}")
                continue

            with open(json_file, "r") as jf:
                filemap = json.load(jf)
                for key, file_path in filemap.items():
                    parts = key.split("_")  # e.g. ["MC", "GamJet", "2018", "GJetsHT100To200"]
                    category = parts[0]     # "MC" or "Data"
                    
                    if category == "MC":
                        sample_name = parse_sample_name(parts)
                        # --- New grouping for binned MC ---
                        # For GamJet, merge HT bins together into one sample.
                        if channel == "GamJet":
                            if sample_name.startswith("GJetsHT"):
                                sample_name = "GJets"
                            elif sample_name.startswith("QCDHT"):
                                sample_name = "QCD"
                        elif channel == "GamJetFake" or channel == "MultiJet":
                            if sample_name.startswith("QCDHT"):
                                sample_name = "QCD"
                        # For channels without binned MC, sample_name remains unchanged.
                        if sample_name not in mc_files_all_years:
                            mc_files_all_years[sample_name] = []
                        mc_files_all_years[sample_name].append(file_path)

                    elif category == "Data":
                        # --- New grouping for Data ---
                        # Assume the trigger/dataset is at index 3 (e.g., "EGamma" or "DoubleMuon")
                        data_sample = parts[3]
                        if data_sample not in data_files_all_years:
                            data_files_all_years[data_sample] = []
                        data_files_all_years[data_sample].append(file_path)

            # Create output directory for merged files (per channel and year)
            output_dir = os.path.join(output_base_dir, channel, "MergedEras", year)
            os.makedirs(output_dir, exist_ok=True)
            
            # This dictionary will map sample names to the merged ROOT file path.
            merged_json = {}

            # Merge the MC samples.
            for sample_name, file_list in mc_files_all_years.items():
                if not file_list:
                    continue
                output_file = os.path.join(output_dir, f"MC_{channel}_{year}_{sample_name}_Hist_Merged.root")
                print(f"[Merging MC] Merging {len(file_list)} files into {output_file}")
                subprocess.run(["hadd", "-f", output_file] + file_list, check=True)
                merged_json[f"MC_{channel}_{year}_{sample_name}"] = output_file
            
            # Merge the Data samples.
            for data_sample, file_list in data_files_all_years.items():
                if not file_list:
                    continue
                data_output_file = os.path.join(output_dir, f"Data_{channel}_{year}_{data_sample}_Hist_Merged.root")
                print(f"[Merging Data] Merging {len(file_list)} files for dataset '{data_sample}' into {data_output_file}")
                subprocess.run(["hadd", "-f", data_output_file] + file_list, check=True)
                merged_json[f"Data_{channel}_{year}_{data_sample}"] = data_output_file
            
            # Write out the merged JSON mapping.
            output_json_file = os.path.join(input_dir, f"FilesHistMergedEras_{channel}_{year}.json")
            with open(output_json_file, "w") as outj:
                json.dump(merged_json, outj, indent=4)
            print(f"[Done] Wrote {output_json_file}")

if __name__ == "__main__":
    input_dir = "merged_json"
    output_base_dir = eosHistDir
    merge_root_files_run2(Channels, Years, input_dir, output_base_dir)

