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
        histDir="Cutflow", 
        xTitle="Cutflow bins",
        yTitle="Events (normalized to 1)",
        yMin=0.0001,
        yMax=100,
        rMin = 0.0,
        rMax = 2.0,
        yLog=True,
        isNorm=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "h1NumJetsInEvents",
        histDir="Refs",
        yTitle="Events (normalized to 1)",
        xTitle="Reference Z mass (GeV)",
        xMin=0,
        xMax=40,
        yMin=0.001,
        yMax=1.1,
        isNorm=True
    )
)

configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1Pt13InPtAvp",
        histDir = "HLT_DiPFJetAve40/Multijet/PFcomposition",
        yTitle="DB Response",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.01,
        yMax=2.5,
        xLog=True
    )
)

configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1Chf13InPtAvp",
        histDir = "HLT_DiPFJetAve40/Multijet/PFcomposition",
        yTitle="Mean of Charged EM Energy Fraction",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.01,
        yMax=1.0,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1Nhf13InPtAvp",
        histDir = "HLT_DiPFJetAve40/Multijet/PFcomposition",
        yTitle="Mean of Neutral Hadron Energy Fraction",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.01,
        yMax=0.1,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1Muf13InPtAvp",
        histDir = "HLT_DiPFJetAve40/Multijet/PFcomposition",
        yTitle="Mean of Muon Energy Fraction",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.001,
        yMax=0.025,
        xLog=True
    )
)
'''
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1RefMassInRefPt",
        histDir="passAtleast1Ref",
        yTitle="Mean of Z mass (GeV)",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=70,
        yMax=120,
        xLog=True
    )
)

configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1DbRespInRefPt",
        yTitle="DB Response",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.01,
        yMax=2.5,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1MpfRespInRefPt",
        yTitle="MPF Response",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.01,
        yMax=2.5,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1JetChhefInRefPt",
        yTitle="Mean of Charged Hadron Energy Fraction",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.1,
        yMax=1.2,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1JetNeemefInRefPt",
        yTitle="Mean of Neutral EM Energy Fraction",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.01,
        yMax=0.6,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1JetChemefInRefPt",
        yTitle="Mean of Charged EM Energy Fraction",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.001,
        yMax=0.2,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1JetNehefInRefPt",
        yTitle="Mean of Neutral Hadron Energy Fraction",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.01,
        yMax=0.4,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1JetMuefInRefPt",
        yTitle="Mean of Muon Energy Fraction",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.001,
        yMax=0.025,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1MpfRespInRefPt_bi",
        histDir="passRefBarrel/flavourXTagY",
        yTitle="MPF Response for b-tagged jet",
        xTitle="Reference Z p_{T} (GeV)",
        yMin=0.01,
        yMax=3.0,
        xLog=True
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
        xTitle="Leading jet #eta",
        yTitle="Reference p_{T} (GeV)",
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
        varName="Z p_{T}",
        varMin=100,
        varMax=120,
        isVarOnX=True,
        xMin=-2.5,
        xMax=+2.5,
        yMin=0.5,
        yMax=1.7,
        xTitle="Leading jet #eta",
        yTitle="DB Response"
    )
)

configs["ConfigErasXY"].append(
    createConfig(
        "ConfigErasXY",
        "p2MpfRespInJet1EtaRefPt",
        histDir="passJet1EtaJet2Pt",
        varName="Z p_{T}",
        varMin=100,
        varMax=120,
        isVarOnX=True,
        xMin=-2.5,
        xMax=+2.5,
        yMin=0.5,
        yMax=1.7,
        xTitle="Leading jet #eta",
        yTitle="MPF Response"
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
        varName="J_{1} #eta",
        varBins=[0.0, 1.3, 2.5, 3.0, 3.5],
        isVarOnX=False,
        yMin=0.0,
        yMax=2.5,
        rMin=0.0,
        rMax=2.0,
        xMin=20.0,
        xMax=1000.0,
        xLog=True,
        xTitle="Reference pT (GeV)",
        yTitle="DB Response"
    )
)

configs["ConfigEraXY"].append(
    createConfig(
        "ConfigEraXY",
        "p2MpfRespInJet1EtaRefPt",
        histDir="passJet1EtaJet2Pt",
        varName="J_{1} #eta",
        varBins=[0.0, 1.3, 2.5, 3.0, 3.5],
        isVarOnX=False,
        yMin=0.0,
        yMax=2.5,
        rMin=0.0,
        rMax=2.0,
        xMin=20.0,
        xMax=1000.0,
        xLog=True,
        xTitle="Reference pT (GeV)",
        yTitle="MPF Response"
    )
)
'''
# Convert to JSON string with indentation for better readability
json_output = json.dumps(configs, indent=4)

# Write to a JSON file
dir_ = "json"
fileName = "FigConfigMultiJet.json"
os.makedirs(dir_, exist_ok=True)
with open(os.path.join(dir_, fileName), "w") as f:
    f.write(json_output)

# Print the output to check
print(f"{dir_}/{fileName}")

