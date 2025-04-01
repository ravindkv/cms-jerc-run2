
import json
import os
from createConfigCommon import configs, createConfig, isMorePlots

# reset configs
for key in configs:
    configs[key] = []


configs["ConfigTime1Ds"].append(
    createConfig(
        "ConfigTime1Ds",
        histDir=f"Base/passExactly2bJet/HistTTbar",
        histNames = ["p1HadWMassInRun", 
                    "p1HadTMassInRun", 
                    "p1LepTMassInRun"],
        xTitle="Run Number",
        yTitle="Mean of Mass (GeV)",
        yMin=50,
        yMax=350
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passExactly2bJet/HistHadW",
        histNames = [
                    "h1EventInHadWMass",
                    ],
        xTitle="m_{W}^{reco} (GeV)",
        yTitle="Events (normalized to 1)",
        xMin=60.0,
        xMax=110.0,
        yMin=0.001,
        yMax=0.05,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=True
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passExactly2bJet/HistTTbar",
        histNames = [
                    "h1EventInHadWMass",
                    ],
        xTitle="m_{W}^{reco} (GeV)",
        yTitle="Events (normalized to 1)",
        xMin=60.0,
        xMax=110.0,
        yMin=0.001,
        yMax=0.05,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=True
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passExactly2bJet/HistHadW",
        histNames = [
                    "h1EventInChiSqrW",
                    ],
        xTitle="Chi2 of the fit",
        yTitle="Events (normalized to 1)",
        yMin=0.001,
        yMax=1.0,
        xMin=0.0,
        xMax=50.0,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        yLog=True,
        isNorm=True
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passExactly2bJet/HistTTbar",
        histNames = [
                    "h1EventInChiSqr",
                    ],
        xTitle="Chi2 of the fit",
        yTitle="Events (normalized to 1)",
        yMin=0.001,
        yMax=1.0,
        xMin=0.0,
        xMax=50.0,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        yLog=True,
        isNorm=True
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passExactly2bJet/HistTTbar",
        histNames = [
                    "h1EventInHadTMass",
                    "h1EventInLepTMass",
                    ],
        xTitle="m_{T} (GeV)",
        yTitle="Events (normalized to 1)",
        yMin=0.001,
        yMax=0.05,
        xMin=100.0,
        xMax=350.0,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        isNorm=True
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passExactly2bJet/HistTTbar",
        histNames = [
                    "p1HadWMassInHadWPt",
                    "p1HadWMassInAvgHadPt",
                    "p1HadTMassInHadTPt",
                    "p1LepTMassInLepTPt",
                    ],
        xTitle="p_{T} (GeV)",
        yTitle="Mean of Mass (GeV)",
        xMin=20,
        xMax=200,
        yMin=30,
        yMax=400,
        rMin = 0.9,
        rMax = 1.1,
        rTitle = "Data/MC",
        isNorm=False
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passExactly2bJet/HistTTbar",
        histNames = [
                    "h1EventInHadWPt",
                    "h1EventInAvgHadPt",
                    "h1EventInHadTPt",
                    "h1EventInLepTPt",
                    ],
        xTitle="p_{T} (GeV)",
        yTitle="Events (normalized to 1)",
        xMin=20,
        xMax=200,
        yMin=0.01,
        yMax=0.6,
        rMin = 0.9,
        rMax = 1.1,
        rTitle = "Data/MC",
        isNorm=True
    )
)

# Convert to JSON string with indentation for better readability
json_output = json.dumps(configs, indent=4)


# Write to a JSON file
dir_ = "json"
fileName = "ReadConfigWqqm.json"
os.makedirs(dir_, exist_ok=True)
with open(os.path.join(dir_, fileName), "w") as f:
    f.write(json_output)

# Print the output to check
print(f"{dir_}/{fileName}")
