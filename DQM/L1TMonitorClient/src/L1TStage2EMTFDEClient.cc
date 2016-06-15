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
  emtfMuonhwEtaComp_=ibooker.book1D("emtfMuonhwEtaComp","Data/Emul of Output Eta", 460, -230,230);
}

void L1TStage2EMTFDEClient::processHistograms(DQMStore::IGetter &igetter){
  
  MonitorElement* dataHist_;
  MonitorElement* emulHist_;

  //hwEta
  dataHist_ = igetter.get(input_dir_data_+"/MuonCand/"+"emtfMuonhwEta");
  emulHist_ = igetter.get(input_dir_emul_+"/MuonCand/"+"emtfMuonhwEta");

 if (dataHist_ && emulHist_){
    TH1F *hwEtaD = dataHist_->getTH1F();
    TH1F *hwEtaE = dataHist_->getTH1F();

    TH1F *hwEtaRatio = emtfMuonhwEtaComp_->getTH1F();
 
    hwEtaRatio->Divide(hwEtaD, hwEtaE);
 }
  
}
  
void L1TStage2EMTFDEClient::dqmEndJob(DQMStore::IBooker &ibooker, DQMStore::IGetter &igetter) {
  book(ibooker);
  processHistograms(igetter);
}


  
