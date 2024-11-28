#-----------------------------------------------------------------
#eosHistDir="/eos/user/r/rverma/www/public/cms-jerc-run2/Hist"
eosHistDir="/eos/cms/store/group/phys_jetmet/rverma/cms-jerc-run2/Hist"
#-----------------------------------------------------------------
Years = []
Years.append('2016Pre')
Years.append('2016Post')
Years.append('2017')
Years.append('2018')

Channels = {}
Channels["ZeeJet"]    = "ZeeJet"
Channels["ZmmJet"]    = "ZmmJet"
Channels["GamJet"]    = "GamJet"
#Channels["MultiJet"]  = "MultiJet"
#Channels["MCTruth"]   = "MultiJet"
#Channels["Flavour"]   = "MultiJet"
#Channels["VetoMap"]   = "MultiJet"
#Channels["DiJet"]     = "MultiJet"
#Channels["MultiJet"]  = "MultiJet"
#Channels["IncJet"]    = "MultiJet"

vomsProxy = "x509up_u93032" # Does NOT work on condor node
reduceJobsMCBy     = 5
reduceJobsDataBy   = 10

