
from figConfigZeeJet import *

# Convert to JSON string with indentation for better readability
json_output = json.dumps(configs, indent=4)

# Write to a JSON file
dir_ = "json"
fileName = "FigConfigZmmJet.json"
os.makedirs(dir_, exist_ok=True)
with open(os.path.join(dir_, fileName), "w") as f:
    f.write(json_output)

# Print the output to check
print(f"{dir_}/{fileName}")
