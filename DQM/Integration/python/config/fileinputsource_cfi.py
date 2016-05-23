import FWCore.ParameterSet.Config as cms

source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(

 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/494/00000/0088D9BD-731B-E611-B349-02163E011DB6.root",
 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/494/00000/00A8EF77-6F1B-E611-A9E4-02163E014732.root",
 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/494/00000/00B34FD6-731B-E611-85BE-02163E014327.root",
 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/494/00000/020B579C-6F1B-E611-AFB9-02163E01455C.root",
 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/494/00000/023314C9-6C1B-E611-B5C8-02163E0138D8.root",
 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/494/00000/02BE3F35-6D1B-E611-9CC2-02163E0133A4.root",
 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/494/00000/02C461F1-6F1B-E611-AB2D-02163E013583.root",
 "/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/273/494/00000/02DB5AFB-731B-E611-9725-02163E014575.root"






        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/001AF1D0-6BE9-E511-9A8D-02163E0143FE.root", 
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/005BCAC3-72E9-E511-B002-02163E014310.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/00E3C803-BBE9-E511-A99C-02163E01465A.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/00EFE5C2-6EE9-E511-ABB3-02163E013417.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/04159D15-95E9-E511-B73F-02163E014176.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/04E4A62F-93E9-E511-83A3-02163E0134CD.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/04F7F2D7-72E9-E511-9534-02163E014310.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/06077D63-78E9-E511-9D8A-02163E014310.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/061EFCD5-97E9-E511-8764-02163E0143FE.root",
        #"root://eoscms//store/express/Commissioning2016/ExpressCosmics/FEVT/Express-v1/000/266/681/00000/063C23DD-82E9-E511-9020-02163E011FCE.root",        
        #"file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/DQMTest/MinimumBias__RAW__v1__165633__1CC420EE-B686-E011-A788-0030487CD6E8.root",
        #"/store/data/Commissioning2014/Cosmics/RAW//v3/000/224/380/00000/E05943D1-1227-E411-BB8E-02163E00F0C4.root"
        #"/store/data/Commissioning2014/Cosmics/RAW/v3/000/224/380/00000/68FDADE5-1227-E411-8AA6-02163E00A10C.root"
    )
)

maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Parameters for runType
import FWCore.ParameterSet.VarParsing as VarParsing
import sys
from dqmPythonTypes import *

options = VarParsing.VarParsing("analysis")

options.register(
    "runkey",
    "cosmic_run",
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.string,
    "Run Keys of CMS"
)

options.parseArguments()

# Fix to allow scram to compile
#if len(sys.argv) > 1:
#  options.parseArguments()

runType = RunType()
if not options.runkey.strip():
    options.runkey = "pp_run"

runType.setRunType(options.runkey.strip())
