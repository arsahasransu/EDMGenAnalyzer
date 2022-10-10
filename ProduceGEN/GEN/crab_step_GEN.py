from CRABClient.UserUtilities import config, getUsernameFromCRIC

config = config()

config.General.requestName = "JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8_220823GEN"
config.General.workArea = "JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8_crab"
config.General.transferLogs = True
config.General.transferOutputs = True

config.JobType.pluginName = "PrivateMC"
config.JobType.psetName = "JPsiToEE_13p6TeV_pythia8_cfg.py"

config.Data.outputPrimaryDataset = "JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8"
#config.Data.outLFNDirBase = "/store/user/asahasra/"
config.Data.outputDatasetTag = "PrivateMC_CMSSW1245_220823GEN"
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 100000
NJOBS = 500
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.publication = True

config.Site.storageSite = "T2_BE_IIHE"
