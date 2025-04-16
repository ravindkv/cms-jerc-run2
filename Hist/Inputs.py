#-----------------------------------------------------------------
#eosHistDir="/eos/user/r/rverma/www/public/cms-jerc-run2/Hist"
eosHistDir="/eos/cms/store/group/phys_jetmet/rverma/cms-jerc-run2/Hist/"
#-----------------------------------------------------------------

Years = [
    '2016Pre',
    '2016Post',
    '2017',
    '2018',
]

# Example channels configuration without explicit extra systematics per year:
chBaseConfig = {
    #"ZeeJet":     {"Years": Years},
    #"ZmmJet":     {"Years": Years},
    #"Wqqe":     {"Years": Years},
    #"Wqqm":     {"Years": Years},
    "GamJet":   {"Years": Years},
}


vomsProxy = "x509up_u93032" # Does NOT work on condor node
reduceJobsMCBy     = 5
reduceJobsDataBy   = 10


# Base configuration template remains the same.
baseConfig = {
    "systOnMc": ["Base", "FsrUp", "FsrDown"],
    "systOnData": ["Base"],
}

def createChannelConfig(baseConfig, extraSystOnMc=None, extraSystOnData=None):
    config = baseConfig.copy()
    if extraSystOnMc:
        config["systOnMc"] = baseConfig.get("systOnMc", []) + extraSystOnMc
    if extraSystOnData:
        config["systOnData"] = baseConfig.get("systOnData", []) + extraSystOnData
    return config

def getExtraSystematics(channel, year):
    """Return extra systematics for MC and Data based on channel and year."""
    # Default empty lists if no extra systematics apply
    extraSyst = {"extraSystOnMc": [], "extraSystOnData": []}
    #if channel in ['Wqqe', 'Wqqm']:
    #    if year in ['2016Pre', '2016Post']:
    #        extraSyst["extraSystOnMc"] = ["FsrUp", "FsrDown"]
    return extraSyst

# Generate channelDetails programmatically
channelDetails = {}
for channel, conf in chBaseConfig.items():
    channelDetails[channel] = {}
    for year in conf["Years"]:
        syst = getExtraSystematics(channel, year)
        channelDetails[channel][year] = createChannelConfig(
            baseConfig,
            extraSystOnMc=syst.get("extraSystOnMc"),
            extraSystOnData=syst.get("extraSystOnData")
        )


if __name__=="__main__":
    from pprint import pprint
    pprint(channelDetails.keys())
    pprint(channelDetails)

