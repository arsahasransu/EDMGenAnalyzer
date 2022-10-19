from CRABClient.UserUtilities import config, getUsernameFromCRIC

config = config()

config.General.requestName = "JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8_221019DIGIRAW"
config.General.workArea = "JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8_crab"
config.General.transferLogs = True
config.General.transferOutputs = True

config.JobType.pluginName = "Analysis"
config.JobType.psetName = "JPsiToEE_13p6_100_DIGIRAW_cfg.py"
config.JobType.maxMemoryMB = 4000

config.Data.inputDataset= "/JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8/asahasra-PrivateMC_CMSSW1245_220823GEN-f81fd232652d72612e99f39f9c293b6e/USER"
config.Data.inputDBS = "phys03"
#config.Data.outLFNDirBase = "/store/user/asahasra/"
config.Data.outputDatasetTag = "PrivateMC_CMSSW12410patch1_221019DIGIRAW"
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 100

config.Data.publication = True

config.Site.storageSite = "T2_BE_IIHE"
