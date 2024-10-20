import json
import glob
import os

# Function to reorganize the JSON data
def reorganize_json_data(files):
    merged_data = {}

    # Iterate over each JSON file
    for file in files:
        # Parse the filename to extract year, channel, and other details
        file_name = file.split('/')[-1]
        _, year_channel = file_name.split('FilesHistMerged_')
        year, channel = year_channel.split('_')
        channel = channel.replace('.json', '')

        # Load the content of each JSON file
        with open(file, 'r') as f:
            data = json.load(f)

        # Initialize the structure if channel is not present
        if channel not in merged_data:
            merged_data[channel] = {}

        # Initialize the structure if year is not present for the channel
        if year not in merged_data[channel]:
            merged_data[channel][year] = {'Data': {}, 'MC': {}}

        # Reorganize data for Data and MC categories
        for key, file_list in data.items():
            if key.startswith('Data_'):
                merged_data[channel][year]['Data'][key] = file_list
            elif key.startswith('MC_'):
                merged_data[channel][year]['MC'][key] = file_list

    merged_data["L3Res"] = {} # For L3Res: GamJet + ZeeJet + ZmmJet + MultiJet
    return merged_data

# Define the path where JSON files are stored (change this to your actual path)
json_files_path = 'json/FilesHist*.json'

# List all JSON files that match the pattern
json_files = glob.glob(json_files_path)

# Reorganize and merge the JSON data
merged_json_data = reorganize_json_data(json_files)

# Output the merged and reorganized data into a single JSON file
dirName = "json/rootFile"
os.system(f"mkdir -p {dirName}")
fileName = 'MergedHistFiles.json'
with open("{dirName}/{fileName}", 'w') as outfile:
    json.dump(merged_json_data, outfile, indent=4)

print(f'Merged and reorganized data saved to {output_file}')

