
import json
import os
from createConfigCommon import configs, createConfig, isMorePlots

# reset configs
for key in configs:
    configs[key] = []


configs["ConfigTime1Ds"].append(
    createConfig(
        "ConfigTime1Ds",
        histDir=f"Base/passMultiJet/HistTime", 
        histNames = ["p1DbRespInRunForRefPt30", 
                    "p1MpfRespInRunForRefPt30", 
                    "p1Jet1ChHefInRunForRefPt30",
                    "p1Jet1NeEmefInRunForRefPt30",
                    "p1Jet1NeHefInRunForRefPt30",
                    "p1Jet1ChEmefInRunForRefPt30"],
        xTitle="Run Number",
        yTitle="Mean of Response and energy fractions",
        yMin=0.0001,
        yMax=1.5
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passMultiJet/HistMultiJet",
        histNames = [
                    "p1CrecoilInAvgProjPt",
                    "p1CrecoilInAvgPt",
                    "p1CrecoilInLeadPt",
                    "p1CrecoilInRecoilPt",
                    ],
        xTitle="p_{T} (GeV)",
        yTitle="Mean of C_{recoil}",
        yMin=0.4,
        yMax=1.6,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=False
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passMultiJet/HistMultiJet",
        histNames = [
                    "h1EventInAvgProjPt",
                    "h1EventInAvgPt",
                    "h1EventInLeadPt",
                    "h1EventInRecoilPt",
                    ],
        xTitle="p_{T} (GeV)",
        yTitle="Events (normalized to 1)",
        yMin=0.001,
        yMax=1.0,
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
        histDir=f"Base/passMultiJet/HistMultiJet",
        histNames = [
                    "p1LeadPtInAvgProjPt",
                    "p1LeadPtInAvgPt",
                    "p1LeadPtInLeadPt",
                    "p1LeadPtInRecoilPt",
                    ],
        xTitle="p_{T} (GeV)",
        yTitle="Mean of Leading jet p_{T} (GeV)",
        yMin=10.0,
        yMax=1000,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=False
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passMultiJet/HistMultiJet",
        histNames = [
                    "p1Jet1ChfInAvgProjPtForLeadEta1p3",
                    "p1Jet1NefInAvgProjPtForLeadEta1p3",
                    "p1Jet1NhfInAvgProjPtForLeadEta1p3",
                    "p1Jet1CefInAvgProjPtForLeadEta1p3"
                    ],
        xTitle="Average Projected p_{T} (GeV)",
        yTitle="Mean of Jet Energy Fractions",
        yMin=0.001,
        yMax=1.0,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=False
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passMultiJet/HistMultiJet",
        histNames = [
                    "p1MpfResp0InAvgProjPt",
                    "p1MpfResp0InAvgPt",
                    "p1MpfResp0InLeadPt",
                    "p1MpfResp0InRecoilPt",
                    ],
        xTitle="p_{T} (GeV)",
        yTitle="Mean of Mpf0 Response",
        yMin=0.5,
        yMax=1.5,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=False
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passMultiJet/HistMultiJet",
        histNames = [
                    "p1MpfResp3InAvgProjPt",
                    "p1MpfResp3InAvgPt",
                    "p1MpfResp3InLeadPt",
                    "p1MpfResp3InRecoilPt",
                    ],
        xTitle="p_{T} (GeV)",
        yTitle="Mean of Mpf3 Response",
        yMin=0.5,
        yMax=1.5,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=False
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passMultiJet/HistMultiJet",
        histNames = [
                    "p1MpfRespnInAvgProjPt",
                    "p1MpfRespnInAvgPt",
                    "p1MpfRespnInLeadPt",
                    "p1MpfRespnInRecoilPt",
                    ],
        xTitle="p_{T} (GeV)",
        yTitle="Mean of Mpfn Response",
        yMin=0.5,
        yMax=1.5,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=False
    )
)

configs["ConfigYear1Ds"].append(
    createConfig(
        "ConfigYear1Ds",
        histDir=f"Base/passMultiJet/HistMultiJet",
        histNames = [
                    "p1MpfRespuInAvgProjPt",
                    "p1MpfRespuInAvgPt",
                    "p1MpfRespuInLeadPt",
                    "p1MpfRespuInRecoilPt",
                    ],
        xTitle="p_{T} (GeV)",
        yTitle="Mean of Mpfu Response",
        yMin=0.5,
        yMax=1.5,
        rMin = 0.5,
        rMax = 1.5,
        rTitle = "Data/MC",
        xLog=True,
        isNorm=False
    )
)
# Convert to JSON string with indentation for better readability
json_output = json.dumps(configs, indent=4)


# Write to a JSON file
dir_ = "json"
fileName = "ReadConfigMultiJet.json"
os.makedirs(dir_, exist_ok=True)
with open(os.path.join(dir_, fileName), "w") as f:
    f.write(json_output)

# Print the output to check
print(f"{dir_}/{fileName}")
