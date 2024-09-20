#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC_Year_Channel_SampleName
# SampleName should not contain minus (-)
def getMC_ZeeJet():
    samps = {}
    samps['MC_2016Pre_ZeeJet_DYJetsToLLM50']  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16NanoAODAPVv9-20UL16APVJMENano_106X_mcRun2_asymptotic_preVFP_v11-v1/NANOAODSIM'
    return samps

def getMC_ZmmJet():
    samps = {}
    samps['MC_2016Pre_ZmmJet_DYJetsToLLM50']  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16NanoAODAPVv9-20UL16APVJMENano_106X_mcRun2_asymptotic_preVFP_v11-v1/NANOAODSIM'
    return samps

def getData_ZeeJet():
    samps = {}
    samps['Data_2016PreBv1_ZeeJet_DoubleEG']  = '/DoubleEG/Run2016B-ver1_HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PreBv2_ZeeJet_DoubleEG']  = '/DoubleEG/Run2016B-ver2_HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD' 
    samps['Data_2016PreC_ZeeJet_DoubleEG']    = '/DoubleEG/Run2016C-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PreD_ZeeJet_DoubleEG']    = '/DoubleEG/Run2016D-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PreE_ZeeJet_DoubleEG']    = '/DoubleEG/Run2016E-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PreF_ZeeJet_DoubleEG']    = '/DoubleEG/Run2016F-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v2/NANOAOD'
    return samps

def getData_ZmmJet():
    samps = {}
    samps['Data_2016PreBv1_ZmmJet_DoubleMuon']  = '/DoubleMuon/Run2016B-ver1_HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PreBv2_ZmmJet_DoubleMuon']  = '/DoubleMuon/Run2016B-ver2_HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD' 
    samps['Data_2016PreC_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2016C-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PreD_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2016D-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PreE_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2016E-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v1/NANOAOD'
    samps['Data_2016PreF_ZmmJet_DoubleMuon']    = '/DoubleMuon/Run2016F-HIPM_UL2016_MiniAODv2_JMENanoAODv9-v2/NANOAOD'
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

