import os
import json
import itertools
from copy import deepcopy

isMorePlots = False

# Base template for ReadConfigBase
baseTemplate = {
    "histDir": "passAlpha",
    "trigDirs": [],
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
    "ConfigTime1Ds": deepcopy(baseTemplate),
    "ConfigEras1D": deepcopy(baseTemplate),
    "ConfigEra1Ds": deepcopy(baseTemplate),
    "ConfigYear1Ds": deepcopy(baseTemplate),
    "ConfigEra2D": deepcopy(baseTemplate),
    "ConfigErasXY": deepcopy(baseTemplate),
    "ConfigEraXYs": deepcopy(baseTemplate)
}

# Extend templates with specific fields for each configuration type
# ConfigTime1Ds inherits from ReadConfigBase and adds:
templates["ConfigTime1Ds"].update({
    "histNames": ["", ""],
})

templates["ConfigEra1Ds"].update({
    "histNames": ["", ""],
    "rTitle": "Data/MC",
    "rMin": 0.0,
    "rMax": 2.0
})

templates["ConfigYear1Ds"].update({
    "histNames": ["", ""],
    "rTitle": "Data/MC",
    "rMin": 0.0,
    "rMax": 2.0
})
# ConfigEras1D inherits from ReadConfigBase and adds:
templates["ConfigEras1D"].update({
    "histName": "p1ResponseInRefPt",
    "rTitle": "Data/MC",
    "rMin": 0.0,
    "rMax": 2.0
})

# ConfigEra2D inherits from ReadConfigBase and adds:
templates["ConfigEra2D"].update({
    "histName": "p1ResponseInRefPt",
    "zTitle": "Some Z Title",
    "zLog": False
})

# ConfigErasXY inherits from ConfigEras1D and adds:
templates["ConfigErasXY"].update({
    "histName": "p1ResponseInRefPt",
    "rTitle": "Data/MC",
    "rMin": 0.5,
    "rMax": 1.5,
    "varName": "Some Variable",
    "varMin": 0.0,
    "varMax": 100.0,
    "isVarOnX": True
})

# ConfigEraXYs inherits from ConfigEras1D and adds:
templates["ConfigEraXYs"].update({
    "histName": "p1ResponseInRefPt",
    "rTitle": "Data/MC",
    "rMin": 0.5,
    "rMax": 1.5,
    "varName": "Some Variable",
    "varBins": [0.0, 1.3, 2.5, 3.0, 3.5],
    "isVarOnX": True
})

# Function to create a new configuration by overriding specific fields
def createConfig(config_type, **overrides):
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
    
    # Return the config dictionary
    return config

# Initialize the main configuration dictionary
configs = {
    "ConfigTime1Ds": [],
    "ConfigEras1D": [],
    "ConfigEra1Ds": [],
    "ConfigYear1Ds": [],
    "ConfigEra2D": [],
    "ConfigErasXY": [],
    "ConfigEraXYs": []
}

# Example usage:
#-------------------------------------
# Adding elements to ConfigTime1Ds
#-------------------------------------

cutsForTime = [
    #"passAtleast1Jet",
    #"passRefBarrel",
    #"passAlpha",
    "passResponse"
]
for cut in cutsForTime:
    configs["ConfigTime1Ds"].append(
        createConfig(
            "ConfigTime1Ds",
            histDir=f"Base/{cut}/HistTime", 
            histNames = ["p1DbRespInRunForRefPt30", 
                        "p1MpfRespInRunForRefPt30", 
                        "p1Jet1ChHefInRunForRefPt30",
                        "p1Jet1NeEmefInRunForRefPt30",
                        "p1Jet1NeHefInRunForRefPt30",
                        "p1Jet1ChEmefInRunForRefPt30"],
            xTitle="Run Number",
            yTitle="Response and energy fractions",
            yMin=0.0001,
            yMax=1.5
        )
    )

#-------------------------------------
# Adding elements to ConfigEra1Ds
#-------------------------------------
configs["ConfigEra1Ds"].append(
    createConfig(
        "ConfigEra1Ds",
        histDir="Base/passAlpha/HistMain",
        histNames = [
                    "p1Jet1ChHefInRefPt",
                    "p1Jet1NeEmefInRefPt",
                    "p1Jet1NeHefInRefPt",
                    "p1Jet1ChEmefInRefPt",
                    #"p1Jet1MuefInRefPt",
                    ],
        xTitle="Reference p_{T} (GeV)",
        yTitle="PF Composition",
        yMin=0.001,
        yMax=1.0,
        rMin = 0.0,
        rMax = 2.0,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=False
    )
)

#-------------------------------------
# Adding elements to ConfigYear1Ds
#-------------------------------------

cutsForResp = [
    #"passAtleast1Jet",
    #"passRefBarrel",
    "passAlpha",
]
for cut in cutsForResp:
    configs["ConfigYear1Ds"].append(
        createConfig(
            "ConfigYear1Ds",
            histDir=f"Base/{cut}/HistMain",
            histNames = [
                        "p1Jet1ChHefInRefPt",
                        "p1Jet1NeEmefInRefPt",
                        "p1Jet1NeHefInRefPt",
                        "p1Jet1ChEmefInRefPt",
                        #"p1Jet1MuefInRefPt",
                        ],
            xTitle="Reference p_{T} (GeV)",
            yTitle="Jet Energy Fractions",
            yMin=0.001,
            yMax=1.0,
            rMin = 0.5,
            rMax = 1.5,
            rTitle = "Data/MC",
            xLog=True,
            isNorm=False
        )
    )
for cut in cutsForResp:
    configs["ConfigYear1Ds"].append(
        createConfig(
            "ConfigYear1Ds",
            histDir=f"Base/{cut}/HistMain",
            histNames = [
                        "p1DbRespInRefPt",
                        "p1MpfRespInRefPt",
                        ],
            xTitle="Reference p_{T} (GeV)",
            yTitle="Response",
            yMin=0.5,
            yMax=1.5,
            rMin = 0.5,
            rMax = 1.5,
            rTitle = "Data/MC",
            xLog=True,
            isNorm=False
        )
    )

cutsForVar = [
    #"passGoodLumi",
    "passMetFilter",
    #"passExactly1Ref",
    #"passHltWithPt",
    #"passAtleast1Jet",
    #"passJetVetoMap",
    #"passDPhiRefJet1",
    #"passAlpha",
    #"passResponse",
    ]

if isMorePlots:
    for cut in cutsForVar:
        configs["ConfigYear1Ds"].append(
            createConfig(
                "ConfigYear1Ds",
                histDir=f"Base/{cut}/HistObjectVar/",
                histNames = [
                            "h1EventInJet1ChHef",
                            "h1EventInJet1NeEmef",
                            "h1EventInJet1NeHef",
                            #"h1EventInJet1ChEmef",
                            #"h1EventInJet1Muef",
                            ],
                xTitle="Jet Energy Fractions",
                yTitle="Events",
                yMin = 0.001,
                yMax = 1.5,
                rMin = 0.0,
                rMax = 2.0,
                rTitle = "Data/MC",
                yLog=True,
                isNorm=True
            )
        )

#-------------------------------------
# Adding elements to ConfigEras1D
#-------------------------------------
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/HistCutflow", 
        histName="h1EventInCutflow",
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

if isMorePlots:
    configs["ConfigEras1D"].append(
        createConfig(
            "ConfigEras1D",
            histDir="Base/passAlpha/HistRef",
            histName="h1EventInRefMass",
            yTitle="Events (normalized to 1)",
            xTitle="Reference mass (GeV)",
            xMin=60,
            xMax=120,
            yMin=0.001,
            yMax=0.4,
            isNorm=True
        )
    )

if isMorePlots:
    configs["ConfigEras1D"].append(
        createConfig(
            "ConfigEras1D",
            histDir="Base/passExactly1Ref/HistRef",
            histName="h1EventInRefPt",
            yTitle="Events (normalized to 1)",
            xTitle="Reference pT (GeV)",
            yMin=0.001,
            yMax=0.5,
            xLog=True,
            isNorm=True
        )
    )

configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/passAlpha/HistRef",
        histName="h1EventInRefPt",
        yTitle="Events (normalized to 1)",
        xTitle="Reference pT (GeV)",
        yMin=0.001,
        yMax=0.5,
        xLog=True,
        isNorm=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/passAlpha/HistRef",
        histName="p1RefMassInRefPt",
        yTitle="Mean of Z mass (GeV)",
        xTitle="Reference p_{T} (GeV)",
        yMin=70,
        yMax=120,
        xLog=True
    )
)

configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/passAlpha/HistMain",
        histName="p1DbRespInRefPt",
        yTitle="DB Response",
        xTitle="Reference p_{T} (GeV)",
        yMin=0.01,
        yMax=2.5,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/passAlpha/HistMain",
        histName="p1MpfRespInRefPt",
        yTitle="MPF Response",
        xTitle="Reference p_{T} (GeV)",
        yMin=0.01,
        yMax=2.5,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/passAlpha/HistMain",
        histName="p1Jet1ChHefInRefPt",
        yTitle="Mean of Charged Hadron Energy Fraction",
        xTitle="Reference p_{T} (GeV)",
        yMin=0.1,
        yMax=1.2,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/passAlpha/HistMain",
        histName="p1Jet1NeHefInRefPt",
        yTitle="Mean of Neutral Hadron Energy Fraction",
        xTitle="Reference p_{T} (GeV)",
        yMin=0.01,
        yMax=0.4,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/passAlpha/HistMain",
        histName="p1Jet1ChEmefInRefPt",
        yTitle="Mean of Charged EM Energy Fraction",
        xTitle="Reference p_{T} (GeV)",
        yMin=0.001,
        yMax=0.2,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/passAlpha/HistMain",
        histName="p1Jet1NeEmefInRefPt",
        yTitle="Mean of Neutral EM Energy Fraction",
        xTitle="Reference p_{T} (GeV)",
        yMin=0.01,
        yMax=0.6,
        xLog=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        histDir="Base/passAlpha/HistMain",
        histName="p1Jet1MuefInRefPt",
        yTitle="Mean of Muon Energy Fraction",
        xTitle="Reference p_{T} (GeV)",
        yMin=0.001,
        yMax=0.025,
        xLog=True
    )
)

if isMorePlots:
    #aTag = ["i", "b", "c", "q", "g", "n"]
    aTag = ["i", "b"]
    aFlv = ["i"]
    for tag, flv in itertools.product(aTag, aFlv):
        configs["ConfigEras1D"].append(
            createConfig(
                "ConfigEras1D",
                histDir="Base/passRefBarrel/HistTag",
                histName=f"p1MpfRespInRefPt_{tag}{flv}",
                yTitle="MPF Response for b-tagged jet",
                xTitle="Reference p_{T} (GeV)",
                yMin=0.01,
                yMax=3.0,
                xLog=True
            )
        )
        configs["ConfigEras1D"].append(
            createConfig(
                "ConfigEras1D",
                histDir="Base/passRefBarrel/HistTag",
                histName=f"p1Jet1ChHefInRefPt_{tag}{flv}",
                yTitle="Charged Hadron Energy Fractions",
                xTitle="Reference p_{T} (GeV)",
                yMin=0.1,
                yMax=1.2,
                xLog=True
            )
        )
        configs["ConfigEras1D"].append(
            createConfig(
                "ConfigEras1D",
                histDir="Base/passRefBarrel/HistTag",
                histName=f"p1Jet1NeHefInRefPt_{tag}{flv}",
                yTitle="Neutral Hadron Energy Fractions",
                xTitle="Reference p_{T} (GeV)",
                yMin=0.01,
                yMax=0.4,
                xLog=True
            )
        )
        configs["ConfigEras1D"].append(
            createConfig(
                "ConfigEras1D",
                histDir="Base/passRefBarrel/HistTag",
                histName=f"p1Jet1ChEmefInRefPt_{tag}{flv}",
                yTitle="Charged EM Energy Fractions",
                xTitle="Reference p_{T} (GeV)",
                yMin=0.001,
                yMax=0.2,
                xLog=True
            )
        )
        configs["ConfigEras1D"].append(
            createConfig(
                "ConfigEras1D",
                histDir="Base/passRefBarrel/HistTag",
                histName=f"p1Jet1NeEmefInRefPt_{tag}{flv}",
                yTitle="Neutral EM Energy Fractions",
                xTitle="Reference p_{T} (GeV)",
                yMin=0.01,
                yMax=0.6,
                xLog=True
            )
        )
        configs["ConfigEras1D"].append(
            createConfig(
                "ConfigEras1D",
                histDir="Base/passRefBarrel/HistTag",
                histName=f"p1Jet1MuefInRefPt_{tag}{flv}",
                yTitle="Muon Energy Fractions",
                xTitle="Reference p_{T} (GeV)",
                yMin=0.001,
                yMax=0.025,
                xLog=True
            )
        )


if isMorePlots:
    configs["ConfigEras1D"].append(
        createConfig(
            "ConfigEras1D",
            histDir="Base/passAtleast1Jet/HistScale",
            histName="h1EventInJet1PtRaw",
            yTitle="Events (normalized to 1)",
            xTitle="Leading jet Raw pT (GeV)",
            yMin=0.001,
            yMax=0.5,
            xLog=True,
            isNorm=True
        )
    )

    configs["ConfigEras1D"].append(
        createConfig(
            "ConfigEras1D",
            histDir="Base/passAtleast1Jet/HistScale",
            histName="h1EventInJet1PtL2RelCorr",
            yTitle="Events (normalized to 1)",
            xTitle="Leading jet Corrected pT (GeV)",
            yMin=0.001,
            yMax=0.5,
            xLog=True,
            isNorm=True
        )
    )

#-------------------------------------
# Adding elements to ConfigEra2D
#-------------------------------------
configs["ConfigEra2D"].append(
    createConfig(
        "ConfigEra2D",
        histDir="Base/passAlpha/HistRef",
        histName="h2EventInRefPhiRefEta",
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
        histDir="Base/passAlpha/HistMain",
        histName="p2DbRespInRefPtJet1Eta",
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

if isMorePlots:
    configs["ConfigEra2D"].append(
        createConfig(
            "ConfigEra2D",
            histDir="Base/passAlpha/HistMain",
            histName="p2JetChHefInRefPtJet1Eta",
            xMin=-2.5,
            xMax=2.5,
            yMin=20,
            yMax=1000,
            yLog=True,
            xTitle="Leading jet #eta",
            yTitle="Reference p_{T} (GeV)",
            zTitle="CHF",
            zLog=False
        )
    )
    configs["ConfigEra2D"].append(
        createConfig(
            "ConfigEra2D",
            histDir="Base/passAlpha/HistMain",
            histName="p2JetChHefInJet1PhiJet1Eta",
            xMin=-2.5,
            xMax=2.5,
            yMin=-3.0,
            yMax=3.0,
            xTitle="Leading jet #eta",
            yTitle="Leading jet #phi",
            zTitle="CHF",
            zLog=False
        )
    )
    configs["ConfigEra2D"].append(
        createConfig(
            "ConfigEra2D",
            histDir="Base/passAlpha/HistMain",
            histName="p2JetChHefInJet1PhiJet1EtaForRefPt230",
            xMin=-2.5,
            xMax=2.5,
            yMin=-3.0,
            yMax=3.0,
            xTitle="Leading jet #eta",
            yTitle="Leading jet #phi",
            zTitle="CHF",
            zLog=False
        )
    )

#-------------------------------------
# Adding elements to ConfigErasXY
#-------------------------------------
configs["ConfigErasXY"].append(
    createConfig(
        "ConfigErasXY",
        histDir="Base/passAlpha/HistMain",
        histName="p2DbRespInRefPtJet1Eta",
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
        histDir="Base/passAlpha/HistMain",
        histName="p2MpfRespInRefPtJet1Eta",
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
# Adding elements to ConfigEraXYs
#-------------------------------------
configs["ConfigEraXYs"].append(
    createConfig(
        "ConfigEraXYs",
        histDir="Base/passAlpha/HistMain",
        histName="p2DbRespInRefPtJet1Eta",
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

configs["ConfigEraXYs"].append(
    createConfig(
        "ConfigEraXYs",
        histDir="Base/passAlpha/HistMain",
        histName="p2MpfRespInRefPtJet1Eta",
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
if __name__=="__main__":
    # Convert to JSON string with indentation for better readability
    json_output = json.dumps(configs, indent=4)

    # Write to a JSON file
    dir_ = "json"
    fileName = "ReadConfigBase.json"
    os.makedirs(dir_, exist_ok=True)
    with open(os.path.join(dir_, fileName), "w") as f:
        f.write(json_output)

    # Print the output to check
    print(f"{dir_}/{fileName}")

