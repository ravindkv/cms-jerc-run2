import os
import sys
import json
import subprocess

sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor/merge", ""))

from Inputs import *

def merge_root_files_run2(Channels, Years, input_dir, output_base_dir):
    """
    Merges all yearly histograms into a single 'Run2' ROOT file per channel and per real sample.
    The updated logic parses out the year portion from the sample name (so '2016Pre_DYJetsToLLM50'
    becomes just 'DYJetsToLLM50'), ensuring all years truly merge into one file.
    """

    # Define a small helper to parse the sample name (remove year portion).
    def parse_sample_name(key_parts):
        """
        key_parts is something like ['MC', 'ZeeJet', '2016Pre', 'DYJetsToLLM50'].
        We want to skip index=2 if it matches a known year pattern,
        and then everything after that forms the 'true' sample name.

        e.g. ['MC', 'ZeeJet', '2016Post', 'DYJetsToLLM50'] -> 'DYJetsToLLM50'
        e.g. ['MC', 'ZeeJet', '2017', 'DYJetsToLLM50Herwig'] -> 'DYJetsToLLM50Herwig'
        """
        # Usually, index=2 is the year. We skip it.
        # The remainder is the sample name.
        # Example: key_parts[3:] = ['DYJetsToLLM50'] => "DYJetsToLLM50"
        # If your naming pattern is guaranteed to have year always in index=2, this is straightforward:
        return "_".join(key_parts[3:])

    for channel in Channels.keys():
        mc_files_all_years = {}
        data_files_all_years = []

        # Gather file paths across all years for this channel
        for year in Years:
            json_file = os.path.join(input_dir, f"FilesHistMergedEras_{channel}_{year}.json")
            if not os.path.exists(json_file):
                print(f"[Warning] JSON not found: {json_file}")
                continue

            with open(json_file, "r") as jf:
                filemap = json.load(jf)
                for key, file_path in filemap.items():
                    parts = key.split("_")  # e.g. ["MC","ZeeJet","2016Post","DYJetsToLLM50"]
                    category = parts[0]     # "MC" or "Data"
                    
                    if category == "MC":
                        # Parse out the real sample name (removing year portion)
                        sample_name = parse_sample_name(parts)
                        if sample_name not in mc_files_all_years:
                            mc_files_all_years[sample_name] = []
                        mc_files_all_years[sample_name].append(file_path)

                    elif category == "Data":
                        data_files_all_years.append(file_path)

        # Output directory for merged Run2 files
        output_dir = os.path.join(output_base_dir, channel, "MergedYears", "Run2")
        os.makedirs(output_dir, exist_ok=True)

        # We'll store the final merged JSON mappings here
        merged_json = {}

        # Merge all MC samples into one file per sample
        for sample_name, file_list in mc_files_all_years.items():
            if not file_list:
                continue

            output_file = os.path.join(output_dir, f"MC_{channel}_Run2_{sample_name}_Hist_Merged.root")
            print(f"[Merging MC] {len(file_list)} files into {output_file}")
            subprocess.run(["hadd", "-f", output_file] + file_list, check=True)
            merged_json[f"MC_{channel}_Run2_{sample_name}"] = output_file

        # Merge all Data files into one
        if data_files_all_years:
            data_output_file = os.path.join(output_dir, f"Data_{channel}_Run2_Hist_Merged.root")
            print(f"[Merging Data] {len(data_files_all_years)} files into {data_output_file}")
            subprocess.run(["hadd", "-f", data_output_file] + data_files_all_years, check=True)
            merged_json[f"Data_{channel}_Run2"] = data_output_file

        # Finally, write out JSON for this channel
        output_json_file = os.path.join(input_dir, f"FilesHistMergedYears_{channel}_Run2.json")
        with open(output_json_file, "w") as outj:
            json.dump(merged_json, outj, indent=4)
        print(f"[Done] Wrote {output_json_file}")

if __name__ == "__main__":
    input_dir = "merged_json"
    output_base_dir = eosHistDir
    merge_root_files_run2(Channels, Years, input_dir, output_base_dir)

