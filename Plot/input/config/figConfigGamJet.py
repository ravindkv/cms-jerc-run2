
from figConfigZeeJet import *

configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "h1EventInRefMass",
        histDir="Base/passAtleast1Ref/HistBarrel",
        yTitle="Events (normalized to 1)",
        xTitle="Reference Gamma mass (GeV)",
        xMin=-1,
        xMax=1,
        yMin=0.001,
        yMax=1.1,
        isNorm=True
    )
)
configs["ConfigEras1D"].append(
    createConfig(
        "ConfigEras1D",
        "p1RefMassInRefPt",
        histDir="Base/passAtleast1Ref/HistBarrel",
        yTitle="Mean of Gamma mass (GeV)",
        xTitle="Reference Gamma p_{T} (GeV)",
        yMin=-1,
        yMax=1,
        xLog=True
    )
)

# Convert to JSON string with indentation for better readability
json_output = json.dumps(configs, indent=4)


# Write to a JSON file
dir_ = "json"
fileName = "FigConfigGamJet.json"
os.makedirs(dir_, exist_ok=True)
with open(os.path.join(dir_, fileName), "w") as f:
    f.write(json_output)

# Print the output to check
print(f"{dir_}/{fileName}")
