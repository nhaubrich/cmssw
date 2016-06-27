import FWCore.ParameterSet.Config as cms

source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/00606170-1927-E611-8C59-02163E01459E.root",
#
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F626F77F-2027-E611-A1D1-02163E014437.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F62A06C8-0B27-E611-853F-02163E011A89.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F6A06E2A-1B27-E611-8F4B-02163E01427F.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F6C15C4B-0E27-E611-B088-02163E0118CA.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F6C70DA0-1127-E611-881C-02163E0145D3.root"


"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/00C11D92-B131-E611-A63B-02163E013975.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/00CD9BA0-B431-E611-81DE-02163E014151.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/0216ECF0-B431-E611-BEAB-02163E01434F.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/022F6BCB-A631-E611-9F30-02163E013744.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/023E14A7-CC31-E611-95B9-02163E013451.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/02779357-BE31-E611-AF5F-02163E0133E2.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/02887E5F-CE31-E611-A1E2-02163E0145D1.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/028E513A-CF31-E611-AE48-02163E01416B.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/029F2D03-AB31-E611-AFC9-02163E013754.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/02CB579F-9A31-E611-B966-02163E011B6E.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/04342C18-C931-E611-8347-02163E013527.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/04777BC9-C631-E611-944E-02163E014589.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/047A630D-A231-E611-8B87-02163E01443E.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/04BB454B-BE31-E611-99B0-02163E01453A.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/06223868-BE31-E611-8649-02163E013483.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/062BBBBD-B431-E611-BCA3-02163E0142B5.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/06525B81-B131-E611-9CA7-02163E0127A7.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/06BA5911-A731-E611-A8D3-02163E012691.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/083D6CBD-C231-E611-B52F-02163E01191C.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/0842A856-B131-E611-87FE-02163E011EC0.root",
"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/0852D62D-AB31-E611-BC5F-02163E01451E.root"
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/0A0B9D7A-C831-E611-ABD0-02163E013975.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/0A1DC598-A731-E611-A624-02163E0143AB.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/0A4DE96C-CF31-E611-970C-02163E014568.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/001/00000/0A5C6B59-B131-E611-8021-02163E01344F.root"

#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/00606170-1927-E611-8C59-02163E01459E.root",
#
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F626F77F-2027-E611-A1D1-02163E014437.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F62A06C8-0B27-E611-853F-02163E011A89.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F6A06E2A-1B27-E611-8F4B-02163E01427F.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F6C15C4B-0E27-E611-B088-02163E0118CA.root",
#"/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/274/250/00000/F6C70DA0-1127-E611-881C-02163E0145D3.root"

    )
)

maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
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
