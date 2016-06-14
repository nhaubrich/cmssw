import FWCore.ParameterSet.Config as cms

l1tStage2EMTFDEClient = cms.EDAnalyzer("L1TStage2EMTFDEClient",
                  monitorDir = cms.untracked.string('L1T2016EMU/L1TdeStage2EMTF'),
                  inputDataDir = cms.untracked.string('L1T2016/L1TStage2EMTF'),
                  inputEmulDir = cms.untracked.string('L1T2016EMU/L1TStage2EMTFEMU')
)


