#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC_Year_Channel_SampleName
# SampleName should not contain minus (-)
def getMC_ZeeJet():
    samps = {}
    samps['MC_2017_ZeeJet_DYJetsToLLM50']  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17NanoAODv9-20UL17JMENano_106X_mc2017_realistic_v9-v1/NANOAODSIM'
    return samps

def getMC_ZmmJet():
    samps = {}
    samps['MC_2017_ZmmJet_DYJetsToLLM50']  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17NanoAODv9-20UL17JMENano_106X_mc2017_realistic_v9-v1/NANOAODSIM'
    return samps

def getData_ZeeJet():
    samps = {}
    samps['Data_2017B_ZeeJet_DoubleEG']    = '/DoubleEG/Run2017B-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2017C_ZeeJet_DoubleEG']    = '/DoubleEG/Run2017C-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2017D_ZeeJet_DoubleEG']    = '/DoubleEG/Run2017D-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2017E_ZeeJet_DoubleEG']    = '/DoubleEG/Run2017E-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2017F_ZeeJet_DoubleEG']    = '/DoubleEG/Run2017F-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    return samps

def getData_ZmmJet():
    samps = {}
    samps['Data_2017B_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2017B-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2017C_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2017C-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2017D_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2017D-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2017E_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2017E-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2017F_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2017F-UL2017_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    return samps

def sampleDict():
    allSamples = {}
    allSamples.update(getMC_ZeeJet())
    allSamples.update(getMC_ZmmJet())

    allSamples.update(getData_ZeeJet())
    allSamples.update(getData_ZmmJet())
    return allSamples

if __name__=="__main__":
    samp = sampleDict()
    print(samp)

