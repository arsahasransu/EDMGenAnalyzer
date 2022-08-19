import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

# Message Logger settings
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# run the input file through the end;
# for a limited number of events, replace -1 with the desired number 
#
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load( "SimGeneral.HepPDTESSource.pythiapdt_cfi" )

process.source = cms.Source( "PoolSource",
                             fileNames = cms.untracked.vstring(
			     'file:JPsiToEE_13p6_500k_GEN.root'
			     )
                           )
	      
# FileService is mandatory, as the following analyzer module 
# will want it, to create output histogram file
# 
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("TestJPsiToEE.root")
)

# the analyzer itself - empty parameter set 
#
process.TestHepMCEvt = cms.EDAnalyzer( "JPsiToEEGenExampleAnalyzer" )

process.p1 = cms.Path( process.TestHepMCEvt )

