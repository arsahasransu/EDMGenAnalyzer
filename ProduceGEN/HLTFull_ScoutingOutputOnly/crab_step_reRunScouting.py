from CRABClient.UserUtilities import config, getUsernameFromCRIC

config = config()

config.General.requestName = "JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8_221024ScoutingReRun200k"
config.General.workArea = "JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8_crab"
config.General.transferLogs = True
config.General.transferOutputs = True

config.JobType.pluginName = "Analysis"
config.JobType.psetName = "scoutingPF.py"

config.Data.inputDataset= "/JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8/asahasra-PrivateMC_CMSSW12410patch1_221019DIGIRAW-a74b34b0dcd2a29d9f812d56a0b763dd/USER"
config.Data.inputDBS = "phys03"
#config.Data.outLFNDirBase = "/store/user/asahasra/"
config.Data.outputDatasetTag = "PrivateMC_CMSSW12410patch1_221024ScoutingReRun200k"
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 10000
config.Data.totalUnits = 200000

config.Site.storageSite = "T2_BE_IIHE"
