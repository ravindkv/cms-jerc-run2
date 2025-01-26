
from figConfigBase import *

cutsForResp = [
    #"passAtleast1Jet",
    #"passRefBarrel",
    "passAlpha",
]
for cut in cutsForResp:
    configs["ConfigYear1Ds"].append(
        createConfig(
            "ConfigYear1Ds",
            histDir=f"HistMultijet/PFcomposition",
            trigDirs = [
                        "HLT_PFJet40",
                        "HLT_PFJet60",
                        ],
            histNames = [
                        "p1Jet1CefInAvgProjPtForLeadEta1p3",
                        "p1Jet1NefInAvgProjPtForLeadEta1p3",
                        "p1Jet1ChfInAvgProjPtForLeadEta1p3",
                        "p1Jet1NhfInAvgProjPtForLeadEta1p3",
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
            histDir=f"HistMultijet",
            trigDirs = [
                        "HLT_PFJet40",
                        "HLT_PFJet60",
                        ],
            histNames = [
                        "p1MpfResp0InAvgProjPt",
                        "p1MpfResp3InAvgProjPt",
                        "p1MpfRespnInAvgProjPt",
                        "p1MpfRespuInAvgProjPt",
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

if isMorePlots:
    configs["ConfigEras1D"].append(
        createConfig(
            "ConfigEras1D",
            histDir="Base/passRefBarrel/HistBarrel",
            histName="h1EventInDeltaPhiJetRef",
            yTitle="Events",
            xTitle="Delta Phi (Jet1, Ref)",
            xMin=2.0,
            xMax=3.5,
            yMin=0.02,
            yMax=0.3,
            isNorm=True
        )
    )

    configs["ConfigEras1D"].append(
        createConfig(
            "ConfigEras1D",
            histDir="Base/passRefBarrel/HistBarrel",
            histName="h1EventInDeltaRadiusJetRef",
            yTitle="Events",
            xTitle="Delta Phi (Jet1, Ref)",
            xMin=1.5,
            xMax=4.5,
            yMin=0.01,
            yMax=0.7,
            isNorm=True
        )
    )

    configs["ConfigEra2D"].append(
        createConfig(
            "ConfigEra2D",
            histDir="Base/passMetFilter/HistObjectId",
            histName="h2EventInPhoEtaPhoPt",
            yMin=-2.5,
            yMax=2.5,
            xTitle="Reference #eta",
            yTitle="Reference p_{T}",
            zTitle="Events",
            zLog=False
        )
    )

    configs["ConfigEra2D"].append(
        createConfig(
            "ConfigEra2D",
            histDir="Base/passMetFilter/HistObjectId",
            histName="h2EventInPhoCountPhoPt",
            yMin=0.0,
            yMax=5.0,
            xTitle="Reference count",
            yTitle="Reference p_{T}",
            zTitle="Events",
            zLog=False
        )
    )

    configs["ConfigEra2D"].append(
        createConfig(
            "ConfigEra2D",
            histDir="Base/passMetFilter/HistObjectId",
            histName="h2EventInPhoR9PhoPt",
            yMin=0.0,
            yMax=2.0,
            xTitle="Reference R9",
            yTitle="Reference p_{T}",
            zTitle="Events",
            zLog=False
        )
    )

    configs["ConfigEra2D"].append(
        createConfig(
            "ConfigEra2D",
            histDir="Base/passMetFilter/HistObjectId",
            histName="h2EventInPhoHoePhoPt",
            yMin=0.0,
            yMax=2.0,
            xTitle="Reference H/E",
            yTitle="Reference p_{T}",
            zTitle="Events",
            zLog=False
        )
    )

    configs["ConfigEra2D"].append(
        createConfig(
            "ConfigEra2D",
            histDir="Base/passMetFilter/HistObjectId",
            histName="h2EventInPhoIdPhoPt",
            yMin=0.0,
            yMax=5.0,
            xTitle="Reference ID",
            yTitle="Reference p_{T}",
            zTitle="Events",
            zLog=False
        )
    )
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
