#-----------------------------------------------------------------
eosHistDir="/eos/user/r/rverma/www/public/cms-jerc-run2/Hist"
#-----------------------------------------------------------------
Years = []
Years.append('2016Pre')
Years.append('2016Post')
Years.append('2017')
Years.append('2018')

Channels = {}
#Channels["GamJet"]    = "GamJet"
Channels["ZeeJet"]    = "ZeeJet"
Channels["ZmmJet"]    = "ZmmJet"
#Channels["MCTruth"]   = "DiJet"
#Channels["Flavour"]   = "DiJet"
#Channels["VetoMap"]   = "DiJet"
#Channels["DiJet"]     = "DiJet"
#Channels["MultiJet"]  = "DiJet"
#Channels["IncJet"]    = "DiJet"

vomsProxy = "/afs/cern.ch/user/r/rverma/x509up_u93032"
#vomsProxy = "/tmp/x509up_u93032" # Does NOT work on condor node
reduceJobsMCBy     = 2
reduceJobsDataBy   = 5
