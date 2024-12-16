import ROOT
import os
import json

# Path to your JSON file
json_file_path = '../Plot/input/root/json/MergedHistFiles.json'

# Load the JSON file
with open(json_file_path, 'r') as f:
    merged_hist_files = json.load(f)

def copy_histograms(input_file_path, output_dir):
    """Copy histograms and TProfiles from the input file to the output directory."""
    input_file = ROOT.TFile.Open(input_file_path)
    if not input_file or input_file.IsZombie():
        print(f"Error: Cannot open file {input_file_path}")
        return

    # Recursively copy objects
    # Recursively copy objects while excluding specific 2D histogram classes
    def copy_objects(input_dir, output_dir):
        excluded_classes = ["TH2D", "TH2F", "TProfile2D"]  # Add more classes as needed
        for key in input_dir.GetListOfKeys():
            obj = key.ReadObj()
            cls = obj.ClassName()
            
            # Check if the object is a TH1 or TProfile but not in the excluded list
            if (obj.InheritsFrom("TH1") or obj.InheritsFrom("TProfile")) and cls not in excluded_classes:
                output_dir.cd()
                obj.Write()
            elif obj.InheritsFrom("TDirectory"):
                sub_input_dir = obj
                output_dir.cd()
                # Create the subdirectory if it doesn't exist
                sub_output_dir = output_dir.GetDirectory(obj.GetName()) or output_dir.mkdir(obj.GetName())
                copy_objects(sub_input_dir, sub_output_dir)

    # Usage
    copy_objects(input_file, output_dir)

    input_file.Close()

def process_dict(d, parent_dir):
    """Recursively process the dictionary and create directories in the output file."""
    for key, value in d.items():
        parent_dir.cd()
        new_dir = parent_dir.mkdir(key)
        if isinstance(value, dict):
            process_dict(value, new_dir)
        else:
            copy_histograms(value, new_dir)

# Loop over channels and years
for channel, years in merged_hist_files.items():
    yToY = {}
    yToY["2016Pre"] = "Summer20UL16APV"
    yToY["2016Post"] = "Summer20UL16"
    yToY["2017"] = "Summer20UL17"
    yToY["2018"] = "Summer20UL18"

    if "GamJet" in channel: continue
    chToCh = {}
    chToCh["ZeeJet"] = "L3Residual_Z/JEC_Combination_Zee"
    chToCh["ZmmJet"] = "L3Residual_Z/JEC_Combination_Zmm"
    chToCh["GamJet"] = "L3Residual_gamma/JEC_Combination_Gamma"
    for year, data_mc in years.items():
        # Create the output ROOT file for each channel and year
        dirN = "%s/%s"%(yToY[year], chToCh[channel])
        os.system(f"mkdir -p {dirN}")
        output_file_name = f"{dirN}/commit_9ba540f_{channel}_MiniAODv2_%s_JECV7_L1L2L3.root"%yToY[year]
        output_file = ROOT.TFile(output_file_name, "RECREATE")
        
        # Process the Data and MC for this channel and year
        process_dict(data_mc, output_file)
        
        # Close the output file
        output_file.Close()
        print(f"Created {output_file_name}")

