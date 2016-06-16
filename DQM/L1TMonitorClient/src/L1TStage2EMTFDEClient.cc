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
  //RegionMuonCand Ratio Plots
  emtfMuonhwEtaComp_=ibooker.book1D("emtfMuonhwEtaComp","Data/Emul of Output Eta", 460, -230,230);
  emtfMuonhwPhiComp_=ibooker.book1D("emtfMuonhwPhiComp", "Data/Emul of Output Phi", 125, -20, 105);
  emtfMuonhwPtComp_=ibooker.book1D("emtfMuonhwPtComp", "Data/Emul of Output p_{T}", 512, 0, 512);
  emtfMuonhwQualComp_=ibooker.book1D("emtfMuonhwQualComp", "Data/Emul of Output Quality", 16, 0, 16);
  emtfMuonhwBXComp_=ibooker.book1D("emtfMuonhwBXComp", "Data/Emul of Output BX", 7, -3, 4); 
  for (int bin = 1, bin_label = -3; bin <= 7; ++bin, ++bin_label) {
    emtfMuonhwBXComp_->setBinLabel(bin, std::to_string(bin_label), 1);
  }

  //Track Ratio Plots
  emtfTrackEtaComp_=ibooker.book1D("emtfTrackEtaComp", "Data/Emul of Track Eta",100,-2.5,2.5);
  emtfTrackPhiComp_=ibooker.book1D("emtfTrackPhiComp", "Data/Emul of Track Phi", 126, -3.15, 3.15);
  emtfTrackPtComp_=ibooker.book1D("emtfTrackPtComp", "Data/Emul of Track p_{T}", 256, 1, 257);
  emtfTrackQualComp_=ibooker.book1D("emtfTrackQualComp", "Data/Emul of Track Quality", 16, 0, 16);
  emtfTrackModeComp_=ibooker.book1D("emtfTrackModeComp", "Data/Emul of Track Mode", 16, 0, 16);
  emtfTrackBXComp_=ibooker.book1D("emtfTrackBXComp", "Data/Emul of Track BX", 8, -3, 5);
}

void L1TStage2EMTFDEClient::processHistograms(DQMStore::IGetter &igetter){
  
  MonitorElement* dataHist_;
  MonitorElement* emulHist_;

  //RegionaMuonCand Ratio Plots
  //hwEta
  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonhwEta");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonhwEta");

 if (dataHist_ && emulHist_){
    TH1F *hwEtaD = dataHist_->getTH1F();
    TH1F *hwEtaE = emulHist_->getTH1F();

    TH1F *hwEtaRatio = emtfMuonhwEtaComp_->getTH1F();

    hwEtaD->Add(hwEtaD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    hwEtaE->Add(hwEtaD);
 
    hwEtaRatio->Divide(hwEtaD, hwEtaE);
 }
 
  //hwPhi
  
  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonhwPhi");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonhwPhi");

 if (dataHist_ && emulHist_){
    TH1F *hwPhiD = dataHist_->getTH1F();
    TH1F *hwPhiE = emulHist_->getTH1F();

    TH1F *hwPhiRatio = emtfMuonhwPhiComp_->getTH1F();

    hwPhiD->Add(hwPhiD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    hwPhiE->Add(hwPhiD);
 
    hwPhiRatio->Divide(hwPhiD, hwPhiE);
 }

  //hwPt

  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonhwPt");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonhwPt");

 if (dataHist_ && emulHist_){
    TH1F *hwPtD = dataHist_->getTH1F();
    TH1F *hwPtE = emulHist_->getTH1F();

    TH1F *hwPtRatio = emtfMuonhwPtComp_->getTH1F();
 
    hwPtD->Add(hwPtD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    hwPtE->Add(hwPtD);
 
    hwPtRatio->Divide(hwPtD, hwPtE);
 }
  //hwQual

  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonhwQual");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonhwQual");

 if (dataHist_ && emulHist_){
    TH1F *hwQualD = dataHist_->getTH1F();
    TH1F *hwQualE = emulHist_->getTH1F();

    TH1F *hwQualRatio = emtfMuonhwQualComp_->getTH1F();
    hwQualD->Add(hwQualD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    hwQualE->Add(hwQualD);
    hwQualRatio->Divide(hwQualD, hwQualE);
 }
  //hwBX
 
  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonBX");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonBX");

 if (dataHist_ && emulHist_){
    TH1F *hwBXD = dataHist_->getTH1F();
    TH1F *hwBXE = emulHist_->getTH1F();

    TH1F *hwBXRatio = emtfMuonhwBXComp_->getTH1F();
    hwBXD->Add(hwBXD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    hwBXE->Add(hwBXD);
    hwBXRatio->Divide(hwBXD, hwBXE);
 }

  //Track Ratio Plots

  //Eta
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackEta");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackEta");

 if (dataHist_ && emulHist_){
    TH1F *EtaD = dataHist_->getTH1F();
    TH1F *EtaE = emulHist_->getTH1F();

    TH1F *EtaRatio = emtfTrackEtaComp_->getTH1F();
    EtaD->Add(EtaD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    EtaE->Add(EtaD);
    EtaRatio->Divide(EtaD, EtaE);
 }

  //Phi
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackPhi");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackPhi");

 if (dataHist_ && emulHist_){
    TH1F *PhiD = dataHist_->getTH1F();
    TH1F *PhiE = emulHist_->getTH1F();

    TH1F *PhiRatio = emtfTrackPhiComp_->getTH1F();
    PhiD->Add(PhiD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    PhiE->Add(PhiD);
    PhiRatio->Divide(PhiD, PhiE);
 }


   //Pt
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackPt");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackPt");

 if (dataHist_ && emulHist_){
    TH1F *PtD = dataHist_->getTH1F();
    TH1F *PtE = emulHist_->getTH1F();

    TH1F *PtRatio = emtfTrackPtComp_->getTH1F();
    PtD->Add(PtD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    PtE->Add(PtD);
    PtRatio->Divide(PtD, PtE);
 }


   //Mode
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackMode");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackMode");

 if (dataHist_ && emulHist_){
    TH1F *ModeD = dataHist_->getTH1F();
    TH1F *ModeE = emulHist_->getTH1F();

    TH1F *ModeRatio = emtfTrackModeComp_->getTH1F();
    ModeD->Add(ModeD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    ModeE->Add(ModeD);
    ModeRatio->Divide(ModeD, ModeE);
 }


   //Quality
  dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackQuality");
  emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackQuality");

 if (dataHist_ && emulHist_){
    TH1F *QualD = dataHist_->getTH1F();
    TH1F *QualE = emulHist_->getTH1F();

    TH1F *QualRatio = emtfTrackQualComp_->getTH1F();
    QualD->Add(QualD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    QualE->Add(QualD);
    QualRatio->Divide(QualD, QualE);
 }


   //BX
   dataHist_ = igetter.get(input_dir_data_+"/"+"emtfTrackBX1D");
   emulHist_ = igetter.get(input_dir_emul_+"/"+"emtfTrackBX1D");

 if (dataHist_ && emulHist_){
    TH1F *BXD = dataHist_->getTH1F();
    TH1F *BXE = emulHist_->getTH1F();

    TH1F *BXRatio = emtfTrackBXComp_->getTH1F();
    BXD->Add(BXD);//Make ratio 2*Data/(data+emul) so 2=all data, 0=all emul
    BXE->Add(BXD);
    BXRatio->Divide(BXD, BXE);
 }


}
  
void L1TStage2EMTFDEClient::dqmEndJob(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {
  book(ibooker);
  processHistograms(igetter);
}


  
