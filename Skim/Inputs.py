# Skim/Inputs.py

# Directory where skimmed files will be stored
eosSkimDir = "/eos/cms/store/group/phys_jetmet/rverma/cms-jerc-run2/Skim"

# Years and Channels to process
Years = [
    '2016Pre',
    '2016Post',
    '2017',
    '2018'
]

Channels = [
    'ZeeJet',
    'ZmmJet',
    'GamJet',
    'MultiJet',
]

# VOMS Proxy path (adjust as needed)
vomsProxy = "x509up_u93032"

# Events per job
eventsPerJobMC = 1e6  # Number of events per job for MC
eventsPerJobData = 5e6  # Number of events per job for Data

