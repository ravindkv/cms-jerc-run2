#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC_Year_Channel_SampleName
# SampleName should not contain minus (-)
def getMC_ZeeJet():
    samps = {}
    samps['MC_2016Post_ZeeJet_DYJetsToLLM50']  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16NanoAODv9-20UL16JMENano_106X_mcRun2_asymptotic_v17-v1/NANOAODSIM'
    return samps

def getMC_ZmmJet():
    samps = {}
    samps['MC_2016Post_ZmmJet_DYJetsToLLM50']  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16NanoAODv9-20UL16JMENano_106X_mcRun2_asymptotic_v17-v1/NANOAODSIM'
    return samps

def getData_ZeeJet():
    samps = {}
    #EGamma0
    samps['Data_2016PostF_ZeeJet_DoubleEG']    = '/DoubleEG/Run2016F-UL2016_MiniAODv2_JMENanoAODv9-v2/NANOAOD'
    samps['Data_2016PostG_ZeeJet_DoubleEG']    = '/DoubleEG/Run2016G-UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PostH_ZeeJet_DoubleEG']    = '/DoubleEG/Run2016H-UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    return samps

def getData_ZmmJet():
    samps = {}
    samps['Data_2016PostF_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2016F-UL2016_MiniAODv2_JMENanoAODv9-v2/NANOAOD'
    samps['Data_2016PostG_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2016G-UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PostH_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2016H-UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
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

