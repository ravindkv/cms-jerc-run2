import os
import json
from copy import deepcopy

# Base template for FigConfigBase
baseTemplate = {
    "histDir": "passJet1EtaJet2Pt",
    "xTitle": "Reference Jet p_{T} (GeV)",
    "yTitle": "Response",
    "xMin": -9999,
    "xMax": -9999,
    "yMin": -9999,
    "yMax": -9999,
    "xLog": False,
    "yLog": False,
    "isNorm": False
}

# Templates for each configuration type
templates = {
    "ConfigEras1D": deepcopy(baseTemplate),
    "ConfigEra2D": deepcopy(baseTemplate),
    "ConfigErasXY": deepcopy(baseTemplate),
    "ConfigEraXY": deepcopy(baseTemplate)
}

# Extend templates with specific fields for each configuration type

# ConfigEras1D inherits from FigConfigBase and adds:
templates["ConfigEras1D"].update({
    "rTitle": "Data/MC",
    "rMin": 0.5,
    "rMax": 1.5
})

# ConfigEra2D inherits from FigConfigBase and adds:
templates["ConfigEra2D"].update({
    "zTitle": "Some Z Title",
    "zLog": False
})

# ConfigErasXY inherits from ConfigEras1D and adds:
templates["ConfigErasXY"].update({
    "rTitle": "Data/MC",
    "rMin": 0.5,
    "rMax": 1.5,
    "varName": "Some Variable",
    "varMin": 0.0,
    "varMax": 100.0,
    "isVarOnX": True
})

# ConfigEraXY inherits from ConfigEras1D and adds:
templates["ConfigEraXY"].update({
    "rTitle": "Data/MC",
    "rMin": 0.5,
    "rMax": 1.5,
    "varName": "Some Variable",
    "varBins": [0.0, 1.3, 2.5, 3.0, 3.5],
    "isVarOnX": True
})

# Function to create a new configuration by overriding specific fields
def createConfig(config_type, hist_name, **overrides):
    # Check if the configuration type exists
    if config_type not in templates:
        raise ValueError(f"Unknown configuration type: {config_type}")
    
    # Create a copy of the template for the given configuration type
    config = deepcopy(templates[config_type])
    
    # Override specific fields if provided
    for key, value in overrides.items():
        if key in config:
            config[key] = value
        else:
            print(f"Warning: '{key}' is not a recognized parameter for {config_type}")
            config[key] = value  # Add it anyway if needed
    
    # Add the histogram name as part of the values
    config['histName'] = hist_name
    
    # Return the config dictionary
    return config

# Initialize the main configuration dictionary
configs = {
    "ConfigEras1D": [],
    "ConfigEra2D": [],
    "ConfigErasXY": [],
    "ConfigEraXY": []
}

# Example usage:

#-------------------------------------
# Adding elements to ConfigEras1D
#-------------------------------------
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "h1EventInCutflow",
        histDir="", 
        xTitle="Cutflow",
        yTitle="Events (normalized to 1)",
        yMin=0.0001,
        yMax=10,
        yLog=True,
        isNorm=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1DbRespInRefPt",
        yTitle="DB Response",
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1MpfRespInRefPt",
        yTitle="MPF Response",
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "h1EventInRefPt",
        histDir="passAtleast1Ref",
        yTitle="Events (normalized to 1)",
        yMin=0.0001,
        yMax=10,
        xLog=True,
        yLog=True,
        isNorm=True
    )
)

#-------------------------------------
# Adding elements to ConfigEra2D
#-------------------------------------
configs["ConfigEra2D"].append(
    createConfig(
        "ConfigEra2D",
        "h2EventInRefEtaRefPhi",
        histDir="passAtleast1Ref",
        yMin=-2.5,
        yMax=2.5,
        xTitle="Reference #eta",
        yTitle="Reference #phi",
        zTitle="Events",
        zLog=False
    )
)
configs["ConfigEra2D"].append(
    createConfig(
        "ConfigEra2D",
        "p2DbRespInJet1EtaRefPt",
        histDir="passJet1EtaJet2Pt",
        xMin=-2.5,
        xMax=2.5,
        yMin=20,
        yMax=1000,
        yLog=True,
        xTitle="Reference #eta",
        yTitle="Reference #phi",
        zTitle="DB Response",
        zLog=False
    )
)

#-------------------------------------
# Adding elements to ConfigErasXY
#-------------------------------------
configs["ConfigErasXY"].append(
    createConfig(
        "ConfigErasXY",
        "p2DbRespInJet1EtaRefPt",
        histDir="passJet1EtaJet2Pt",
        varName="Jet pT",
        varMin=100,
        varMax=120,
        isVarOnX=True,
        xMin=-3.5,
        xMax=+3.5,
        yMin=0.5,
        yMax=1.5,
        xTitle="Reference pT (GeV)"
    )
)

#-------------------------------------
# Adding elements to ConfigEraXY
#-------------------------------------
configs["ConfigEraXY"].append(
    createConfig(
        "ConfigEraXY",
        "p2DbRespInJet1EtaRefPt",
        histDir="passJet1EtaJet2Pt",
        varName="Jet Eta",
        varBins=[0.0, 1.3, 2.5, 3.0, 3.5],
        isVarOnX=False,
        yMin=0.5,
        yMax=1.5,
        xTitle="Reference pT (GeV)"
    )
)

# Convert to JSON string with indentation for better readability
json_output = json.dumps(configs, indent=4)

# Write to a JSON file
dir_ = "json/figConfig"
fileName = "FigConfigZeeJet.json"
os.makedirs(dir_, exist_ok=True)
with open(os.path.join(dir_, fileName), "w") as f:
    f.write(json_output)

# Print the output to check
print(f"{dir_}/{fileName}")

