#include "DQM/L1TMonitorClient/interface/L1TStage2EMTFDEClient.h"

L1TStage2EMTFDEClient::L1TStage2EMTFDEClient(const edm::ParameterSet& ps):
  monitor_dir_(ps.getUntrackedParameter<std::string>("monitorDir","")),
  input_dir_data_(ps.getUntrackedParameter<std::string>("inputDataDir","")),
  input_dir_emul_(ps.getUntrackedParameter<std::string>("inputEmulDir",""))
{}

L1TStage2EMTFDEClient::~L1TStage2EMTFDEClient(){}

void L1TStage2EMTFDEClient::dqmEndLuminosityBlock(DQMStore::IBooker &ibooker,DQMStore::IGetter &igetter,const edm::LuminosityBlock& lumiSeg, const edm::EventSetup& c) {
  book(ibooker);
  processHistograms(igetter);
}

void L1TStage2EMTFDEClient::book(DQMStore::IBooker &ibooker){
  
  ibooker.setCurrentFolder(monitor_dir_);
  //RegionMuonCand Plots
  emtfMuonhwEtaComp_=ibooker.book1D("emtfMuonhwEtaComp","Data/Emul of Output Eta", 460, -230,230);
  emtfMuonhwPhiComp_=ibooker.book1D("emtfMuonhwPhiComp", "Data/Emul of Output Phi", 125, -20, 105);
  emtfMuonhwPtComp_=ibooker.book1D("emtfMuonhwPtComp", "Data/Emul of Output p_{T}", 512, 0, 512);
  emtfMuonhwQualComp_=ibooker.book1D("emtfMuonhwQualComp", "Data/Emul of Output Quality", 16, 0, 16);
  emtfMuonhwBXComp_=ibooker.book1D("emtfMuonhwBXComp", "Data/Emul of Output BX", 7, -3, 4); 
  for (int bin = 1, bin_label = -3; bin <= 7; ++bin, ++bin_label) {
    emtfMuonhwBXComp_->setBinLabel(bin, std::to_string(bin_label), 1);
  }

  emtfMuonhwEtaDif_ = ibooker.book1D("emtfMuonhwEtaDif", "Data - Emul of Output Eta", 460, -230, 230);
  emtfMuonhwPhiDif_ = ibooker.book1D("emtfMuonhwPhiDif", "Data - Emul of Output Phi", 125, -20, 105);
  emtfMuonhwPtDif_ = ibooker.book1D("emtfMuonhwPtDif", "Data - Emul of Output Pt", 512, 0, 512);
  emtfMuonhwQualDif_ = ibooker.book1D("emtfMuonhwQualDif", "Data - Emul of Output Quality", 16, 0, 16);
  emtfMuonhwBXDif_ = ibooker.book1D("emtfMuonhwBXDif", "Data - Emul of Output BX", 7, -3, 4);

  //Track Ratio Plots
  emtfTrackEtaComp_=ibooker.book1D("emtfTrackEtaComp", "Data/Emul of Track Eta",100,-2.5,2.5);
  emtfTrackPhiComp_=ibooker.book1D("emtfTrackPhiComp", "Data/Emul of Track Phi", 126, -3.15, 3.15);
  emtfTrackPtComp_=ibooker.book1D("emtfTrackPtComp", "Data/Emul of Track p_{T}", 256, 1, 257);
  emtfTrackQualComp_=ibooker.book1D("emtfTrackQualComp", "Data/Emul of Track Quality", 16, 0, 16);
  emtfTrackModeComp_=ibooker.book1D("emtfTrackModeComp", "Data/Emul of Track Mode", 16, 0, 16);
  emtfTrackBXComp_=ibooker.book1D("emtfTrackBXComp", "Data/Emul of Track BX", 8, -3, 5);
  emtfTrackSectorIndexComp_=ibooker.book1D("emtfTrackSectorIndexComp","Data/Emul of Track Sector Index", 13, -6.5, 6.5);


  emtfTrackEtaDif_=ibooker.book1D("emtfTrackEtaDif", "Data - Emul of Track Eta",100, -2.5, 2.5);
  emtfTrackPhiDif_=ibooker.book1D("emtfTrackPhiDif", "Data - Emul of Track Phi", 126, -3.15, 3.15);
  emtfTrackPtDif_=ibooker.book1D("emtfTrackPtDif", "Data - Emul of Track Pt", 256, 1, 257);
  emtfTrackQualDif_=ibooker.book1D("emtfTrackQualDif", "Data - Emul of Track Qual", 16, 0, 16);
  emtfTrackModeDif_=ibooker.book1D("emtfTrackModeDif", "Data - Emul of Track Mode", 16, 0, 16);
  emtfTrackBXDif_=ibooker.book1D("emtfTrackBXDif", "Data - Emul of Track BX", 8, -3, 5);
  emtfTrackSectorIndexDif_=ibooker.book1D("emtfTrackSectorIndexDif", "Data - Emul of Track Sector Index", 13, -6.5, 6.5);
}

void L1TStage2EMTFDEClient::processHistograms(DQMStore::IGetter &igetter){
  
  MonitorElement* dataHist_;
  MonitorElement* emulHist_;

  //Ratio Plots are 2*data/(data + emul) so plots go from 0 to 2, where 0=no data, 1=perfect match, 2=no emul
  //RegionaMuonCand Ratio Plots
  //hwEta
  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonhwEta");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonhwEta");

 if (dataHist_ && emulHist_){
    TH1F *hwEtaD = dataHist_->getTH1F();
    TH1F *hwEtaE = emulHist_->getTH1F();

    TH1F *hwEtaRatio = emtfMuonhwEtaComp_->getTH1F();
    TH1F *hwEtaDif = emtfMuonhwEtaDif_->getTH1F();

    hwEtaDif->Add(hwEtaD, hwEtaE);

    hwEtaRatio->Divide(hwEtaD, hwEtaE, 2, 1);
 
    hwEtaDif->Add(hwEtaD, hwEtaE, 1, -1);
}
 
  //hwPhi
  
  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonhwPhi");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonhwPhi");

 if (dataHist_ && emulHist_){
    TH1F *hwPhiD = dataHist_->getTH1F();
    TH1F *hwPhiE = emulHist_->getTH1F();

    TH1F *hwPhiRatio = emtfMuonhwPhiComp_->getTH1F();
    TH1F *hwPhiDif = emtfMuonhwPhiDif_->getTH1F();
    hwPhiDif->Add(hwPhiD, hwPhiE);

    hwPhiE->Add(hwPhiD); 
    hwPhiRatio->Divide(hwPhiD, hwPhiE, 2, 1);
    hwPhiDif->Add(hwPhiD, hwPhiE, 1, -1);
 }

  //hwPt

  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonhwPt");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonhwPt");

 if (dataHist_ && emulHist_){
    TH1F *hwPtD = dataHist_->getTH1F();
    TH1F *hwPtE = emulHist_->getTH1F();

    TH1F *hwPtRatio = emtfMuonhwPtComp_->getTH1F();
    TH1F *hwPtDif = emtfMuonhwPtDif_->getTH1F();
    hwPtDif->Add(hwPtD, hwPtE);

    hwPtRatio->Divide(hwPtD, hwPtE, 2, 1);
    hwPtDif->Add(hwPtD, hwPtE, 1, -1);
 }
  //hwQual

  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonhwQual");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonhwQual");

 if (dataHist_ && emulHist_){
    TH1F *hwQualD = dataHist_->getTH1F();
    TH1F *hwQualE = emulHist_->getTH1F();

    TH1F *hwQualRatio = emtfMuonhwQualComp_->getTH1F();
    TH1F *hwQualDif = emtfMuonhwQualDif_->getTH1F();
    hwQualDif->Add(hwQualD, hwQualE);
    hwQualRatio->Divide(hwQualD, hwQualE, 2, 1);
    hwQualDif->Add(hwQualD, hwQualE, 1, -1);
 }
  //hwBX
 
  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonBX");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonBX");

 if (dataHist_ && emulHist_){
    TH1F *hwBXD = dataHist_->getTH1F();
    TH1F *hwBXE = emulHist_->getTH1F();

    TH1F *hwBXRatio = emtfMuonhwBXComp_->getTH1F();
    TH1F *hwBXDif = emtfMuonhwBXDif_->getTH1F();

    hwBXDif->Add(hwBXD,hwBXE);
    //hwBXE->Add(hwBXD);
    hwBXRatio->Divide(hwBXD, hwBXE);
    hwBXDif->Add(hwBXD, hwBXE, 1, -1);
  }


  //Track Ratio Plots

  //Eta
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackEta");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackEta");

 if (dataHist_ && emulHist_){
    TH1F *EtaD = dataHist_->getTH1F();
    TH1F *EtaE = emulHist_->getTH1F();

    TH1F *EtaRatio = emtfTrackEtaComp_->getTH1F();
    TH1F *EtaDif = emtfTrackEtaDif_->getTH1F();

    EtaDif->Add(EtaD, EtaE);
   
    EtaRatio->Divide(EtaD, EtaE, 2, 1); 
    EtaDif->Add(EtaD, EtaE, 1, -1);
}

  //Phi
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackPhi");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackPhi");

 if (dataHist_ && emulHist_){
    TH1F *PhiD = dataHist_->getTH1F();
    TH1F *PhiE = emulHist_->getTH1F();

    TH1F *PhiRatio = emtfTrackPhiComp_->getTH1F();
    TH1F *PhiDif = emtfTrackPhiDif_->getTH1F();

    PhiDif->Add(PhiD, PhiE);
    PhiRatio->Divide(PhiD, PhiE, 2, 1);
    PhiDif->Add(PhiD, PhiE, 1, -1);
 }


   //Pt
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackPt");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackPt");

 if (dataHist_ && emulHist_){
    TH1F *PtD = dataHist_->getTH1F();
    TH1F *PtE = emulHist_->getTH1F();

    TH1F *PtRatio = emtfTrackPtComp_->getTH1F(); 
    TH1F *PtDif = emtfTrackPtDif_->getTH1F();

    PtDif->Add(PtD, PtE);
    PtRatio->Divide(PtD, PtE, 2, 1);
    PtDif->Add(PtD, PtE, 1, -1);
 }


   //Mode
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackMode");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackMode");

 if (dataHist_ && emulHist_){
    TH1F *ModeD = dataHist_->getTH1F();
    TH1F *ModeE = emulHist_->getTH1F();

    TH1F *ModeRatio = emtfTrackModeComp_->getTH1F();
    TH1F *ModeDif = emtfTrackModeDif_->getTH1F();

    ModeDif->Add(ModeD, ModeE);
    ModeRatio->Divide(ModeD, ModeE, 2, 1);
    ModeDif->Add(ModeD, ModeE, 1, -1);
 }


   //Quality
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackQuality");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackQuality");

 if (dataHist_ && emulHist_){
    TH1F *QualD = dataHist_->getTH1F();
    TH1F *QualE = emulHist_->getTH1F();

    TH1F *QualRatio = emtfTrackQualComp_->getTH1F();
    TH1F *QualDif = emtfTrackQualDif_->getTH1F();

    QualDif->Add(QualD, QualE);

    QualRatio->Divide(QualD, QualE, 2, 1);
    QualDif->Add(QualD, QualE, 1, -1);
 }


   //BX
   dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackBX1D");
   emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackBX1D");

 if (dataHist_ && emulHist_){
    TH1F *BXD = dataHist_->getTH1F();
    TH1F *BXE = emulHist_->getTH1F();

    TH1F *BXRatio = emtfTrackBXComp_->getTH1F();
    TH1F *BXDif = emtfTrackBXDif_->getTH1F();
    
    BXDif->Add(BXD, BXE, 1, 1);
   
    BXRatio->Divide(BXD,BXDif, 2, 1);
    BXDif->Add(BXD, BXE, 1, -1);
 }
  //Sector Index (Sector*Endcap)
   dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackSectorIndex");
   emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackSectorIndex");

 if (dataHist_ && emulHist_){
    TH1F *SID = dataHist_->getTH1F();
    TH1F *SIE = emulHist_->getTH1F();

    TH1F *SIRatio = emtfTrackSectorIndexComp_->getTH1F();
    TH1F *SIDif = emtfTrackSectorIndexDif_->getTH1F();
    
    SIDif->Add(SID, SIE);  
    SIRatio->Divide(SID, SIE, 2, 1);
    SIDif->Add(SID, SIE, 1, -1);
 }

}
  
void L1TStage2EMTFDEClient::dqmEndJob(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {
  book(ibooker);
  processHistograms(igetter);
} 
