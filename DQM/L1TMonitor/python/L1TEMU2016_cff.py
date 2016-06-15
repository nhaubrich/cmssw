import FWCore.ParameterSet.Config as cms

from DQM.L1TMonitor.L1TStage2CaloLayer2Emul_cfi import *

from DQM.L1TMonitor.L1TStage2EMTFEmul_cfi import *

l1tEmul2016Online = cms.Sequence(
            l1tStage2CaloLayer2Emul +
            l1tStage2EmtfEmul
            )
