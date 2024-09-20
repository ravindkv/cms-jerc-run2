#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC_Year_Channel_SampleName
# SampleName should not contain minus (-)
def getMC_ZeeJet():
    samps = {}
    samps['MC_2018_ZeeJet_DYJetsToLLM50']  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18NanoAODv9-20UL18JMENano_106X_upgrade2018_realistic_v16_L1v1-v1/NANOAODSIM'
    return samps

def getMC_ZmmJet():
    samps = {}
    samps['MC_2018_ZmmJet_DYJetsToLLM50']  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18NanoAODv9-20UL18JMENano_106X_upgrade2018_realistic_v16_L1v1-v1/NANOAODSIM' 
    return samps

def getData_ZeeJet():
    samps = {}
    samps['Data_2018A_ZeeJet_EGamma']    = '/EGamma/Run2018A-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2018B_ZeeJet_EGamma']    = '/EGamma/Run2018B-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2018C_ZeeJet_EGamma']    = '/EGamma/Run2018C-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2018D_ZeeJet_EGamma']    = '/EGamma/Run2018D-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    return samps

def getData_ZmmJet():
    samps = {}
    samps['Data_2018A_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2018A-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2018B_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2018B-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2018C_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2018C-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2018D_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2018D-UL2018_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
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

