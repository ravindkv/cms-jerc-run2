#-----------------------------------------------------------------
eosFitDir="/eos/cms/store/group/phys_jetmet/rverma/cms-jerc-run2/Fit"
#-----------------------------------------------------------------

Years = [
    #'2016Pre',
    #'2016Post',
    '2017',
    #'2018'
    ]

# -------------- USER-SPECIFIC --------------
ChannelsL3Res = {
    "Wqqm": [
        "Base/passExactly2bJet/HistTTbar/p1HadWMassInAvgHadPt",
    ],
    "Wqqe": [
        "Base/passExactly2bJet/HistTTbar/p1HadWMassInAvgHadPt",
    ]
}

vomsProxy = "x509up_u93032" # Does NOT work on condor node
reduceJobsMCBy     = 5
reduceJobsDataBy   = 10

