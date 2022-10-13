import FWCore.ParameterSet.Config as cms

# Set parameters externally
from FWCore.ParameterSet.VarParsing import VarParsing
params = VarParsing('analysis')

params.register(
    'inputFileName',
    'file:/pnfs/iihe/cms/store/user/asahasra/JPsiToEE_Pt2To30_13p6TeV_TuneCP5_pythia8/PrivateMC_CMSSW1245_220823GEN/220823_152058/0000/JPsiToEE_13p6_500_GEN_1.root',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    'Name of the input file'
)

params.register(
    'outputFileName',
    'TestJPsiToEE_1.root',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    'Name of the output file'
)

process = cms.Process("Analysis")

# Parse arguments
params.parseArguments()

# Message Logger settings
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# run the input file through the end;
# for a limited number of events, replace -1 with the desired number 
#
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load( "SimGeneral.HepPDTESSource.pythiapdt_cfi" )

process.source = cms.Source( "PoolSource",
                             fileNames = cms.untracked.vstring( params.inputFileName )
                           )
	      
# FileService is mandatory, as the following analyzer module 
# will want it, to create output histogram file
# 
process.TFileService = cms.Service("TFileService",
        fileName = cms.string( params.outputFileName )
)
print(params.inputFileName)
print(params.outputFileName)
# the analyzer itself - empty parameter set 
#
process.TestHepMCEvt = cms.EDAnalyzer( "JPsiToEEGenExampleAnalyzer" )

process.p1 = cms.Path( process.TestHepMCEvt )

