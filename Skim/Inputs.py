#-----------------------------------------------------------------
eosSkimDir="/eos/user/r/rverma/www/public/cms-jerc-run2/Skim"
#-----------------------------------------------------------------
Years = []
Years.append('2016Pre')
Years.append('2016Post')
Years.append('2017')
Years.append('2018')

Channels = []
#Channels.append("GamJet")
Channels.append("ZeeJet")
Channels.append("ZmmJet")
#Channels.append("DiJet")

#vomsProxy = "/afs/cern.ch/user/r/rverma/x509up_u93032"
vomsProxy = "x509up_u93032"
#vomsProxy = "/tmp/x509up_u93032" # Does NOT work on condor node
evtPerJobMC   = 1e6 # n million events per job
evtPerJobData = 5e6 

