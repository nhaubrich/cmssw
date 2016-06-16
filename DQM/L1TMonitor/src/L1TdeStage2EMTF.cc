#include <string>
#include <vector>
#include <iostream>
#include "DQM/L1TMonitor/interface/L1TdeStage2EMTF.h"


L1TdeStage2EMTF::L1TdeStage2EMTF(const edm::ParameterSet& ps)
    : dataToken(consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("dataSource"))),
      emulToken(consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("emulSource"))),
      datahitToken(consumes<l1t::EMTFHitCollection>(ps.getParameter<edm::InputTag>("dataSource"))),
      emulhitToken(consumes<l1t::EMTFHitCollection>(ps.getParameter<edm::InputTag>("emulSource"))),
      datatrackToken(consumes<l1t::EMTFTrackCollection>(ps.getParameter<edm::InputTag>("dataSource"))),
      emultrackToken(consumes<l1t::EMTFTrackCollection>(ps.getParameter<edm::InputTag>("emulSource"))),
      monitorDir(ps.getUntrackedParameter<std::string>("monitorDir", "")),
      verbose(ps.getUntrackedParameter<bool>("verbose", false)) {}

L1TdeStage2EMTF::~L1TdeStage2EMTF() {}

void L1TdeStage2EMTF::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c) {}

void L1TdeStage2EMTF::beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) {}

void L1TdeStage2EMTF::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {

  ibooker.setCurrentFolder(monitorDir);

  emtfComparenMuonsEvent = ibooker.book2D("emtfComparenMuonsEvent", "Number of EMTF Muon Cands per Event", 12, 0, 12, 12, 0, 12);
  for (int axis = 1; axis <= 2; ++axis) {
    std::string axisTitle = (axis == 1) ? "Data" : "Emulator";
    emtfComparenMuonsEvent->setAxisTitle(axisTitle, axis);
    for (int bin = 1; bin <= 12; ++bin) {
      std::string binLabel = (bin == 12) ? "Overflow" : std::to_string(bin - 1);
      emtfComparenMuonsEvent->setBinLabel(bin, binLabel, axis);
    }
  }

  emtfDataBX = ibooker.book1D("emtfDataBX", "EMTF Muon Cand BX", 7, -3, 4);
  emtfDataBX->setAxisTitle("BX", 1);

  emtfEmulBX = ibooker.book1D("emtfEmulBX", "EMTF Emulated Muon Cand BX", 7, -3, 4);
  emtfEmulBX->setAxisTitle("BX", 1);

  for (int bin = 1, bin_label = -3; bin <= 7; ++bin, ++bin_label) {
    emtfDataBX->setBinLabel(bin, std::to_string(bin_label), 1);
    emtfEmulBX->setBinLabel(bin, std::to_string(bin_label), 1);
  }

  emtfDatahwPt = ibooker.book1D("emtfDatahwPt", "EMTF Muon Cand p_{T}", 512, 0, 512);
  emtfDatahwPt->setAxisTitle("Hardware p_{T}", 1);

  emtfEmulhwPt = ibooker.book1D("emtfEmulhwPt", "EMTF Emulated Muon Cand p_{T}", 512, 0, 512);
  emtfEmulhwPt->setAxisTitle("Hardware p_{T}", 1);

  emtfDatahwEta = ibooker.book1D("emtfDatahwEta", "EMTF Muon Cand #eta", 460, -230, 230);
  emtfDatahwEta->setAxisTitle("Hardware #eta", 1);

  emtfEmulhwEta = ibooker.book1D("emtfEmulhwEta", "EMTF Emulated Muon Cand #eta", 460, -230, 230);
  emtfEmulhwEta->setAxisTitle("Hardware #eta", 1);

  emtfDatahwPhi = ibooker.book1D("emtfDatahwPhi", "EMTF Muon Cand #phi", 125, -20, 105);
  emtfDatahwPhi->setAxisTitle("Hardware #phi", 1);

  emtfEmulhwPhi = ibooker.book1D("emtfEmulhwPhi", "EMTF Emulated Muon Cand #phi", 125, -20, 105);
  emtfEmulhwPhi->setAxisTitle("Hardware #phi", 1);

  emtfDatahwQual = ibooker.book1D("emtfDatahwQual", "EMTF Muon Cand Quality", 16, 0, 16);
  emtfDatahwQual->setAxisTitle("Quality", 1);

  emtfEmulhwQual = ibooker.book1D("emtfEmulhwQual", "EMTF Emulated Muon Cand Quality", 16, 0, 16);
  emtfEmulhwQual->setAxisTitle("Quality", 1);

  for (int bin = 1; bin <= 16; ++bin) {
    emtfDatahwQual->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfEmulhwQual->setBinLabel(bin, std::to_string(bin - 1), 1);
  }

  // Comparison plots reserved for updated emulator.
  /*emtfComparehwPt = ibooker.book2D("emtfComparehwPt", "EMTF Muon Cand p_{T}", 512, 0, 512, 512, 0, 512);
  emtfComparehwPt->setAxisTitle("Hardware p_{T}", 1);
  emtfComparehwPt->setAxisTitle("Emulator Hardware p_{T}", 2);

  emtfComparehwEta = ibooker.book2D("emtfComparehwEta", "EMTF Muon Cand #eta", 460, -230, 230, 460, -230, 230);
  emtfComparehwEta->setAxisTitle("Hardware #eta", 1);
  emtfComparehwEta->setAxisTitle("Emulator Hardware #eta", 2);

  emtfComparehwPhi = ibooker.book2D("emtfComparehwPhi", "EMTF Muon Cand #phi", 125, -20, 105, 125, -20, 105);
  emtfComparehwPhi->setAxisTitle("Hardware #phi", 1);
  emtfComparehwPhi->setAxisTitle("Emulator Hardware #phi", 2);

  emtfComparehwQual = ibooker.book2D("emtfComparehwQual", "EMTF Muon Cand Quality", 16, 0, 16, 16, 0, 16);
  for (int axis = 1; axis <= 2; ++axis) {
    std::string axisTitle = (axis == 1) ? "Quality" : "Emulator Quality";
    emtfComparehwQual->setAxisTitle(axisTitle, axis);
    for (int bin = 1; bin <= 16; ++bin) {
      emtfComparehwQual->setBinLabel(bin, std::to_string(bin - 1), axis);
    }
  }*/

  //Track Emul and Data Plots
   emtfDataEta = ibooker.book1D("emtfDataEta", "EMTF Track Data #eta", 100, -2.5,2.5);
  emtfDataEta->setAxisTitle("Track #eta",1);

  emtfEmulEta = ibooker.book1D("emtfEmulEta", "EMTF Track Emul #eta", 100, -2.5,2.5);
  emtfEmulEta->setAxisTitle("Track #eta",1);

  emtfDataPhi = ibooker.book1D("emtfDataPhi", "EMTF Track Data #phi", 126,-3.15,3.15);
  emtfDataPhi->setAxisTitle("Track #phi",1);

  emtfEmulPhi = ibooker.book1D("emtfEmulPhi", "EMTF Track Emul #phi",126,-3.15,3.15);
  emtfEmulEta->setAxisTitle("Track #phi",1);

  emtfDataPt = ibooker.book1D("emtfDataPt", "EMTF Track Data p_{T}",256,1,257 );
  emtfDataPt->setAxisTitle("Track p_{T}",1);

  emtfEmulPt = ibooker.book1D("emtfEmulPt", "EMTF Track Emul p_{T}", 256,1,257);
  emtfEmulPt->setAxisTitle("Track p_{T}",1);


  emtfDataMode = ibooker.book1D("emtfDataMode", "EMTF Track Data Mode", 16,0,16);
  emtfDataMode->setAxisTitle("Track Mode",1);

  emtfEmulMode = ibooker.book1D("emtfEmulMode", "EMTF Track Emul Mode", 16,0,16);
  emtfEmulMode->setAxisTitle("Track Mode",1);


  emtfDataQuality = ibooker.book1D("emtfDataQuality", "EMTF Track Data Quality", 16, 0, 16);
  emtfDataQuality->setAxisTitle("Track Quality",1);

  emtfEmulQuality = ibooker.book1D("emtfEmulQuality", "EMTF Track Emul Quality", 16, 0, 16);
  emtfEmulQuality->setAxisTitle("Track Quality",1);

  emtfMatchWire = ibooker.book2D("emtfMatchWire","EMTF Match Wire", 128,0,128, 128, 0, 128);
  emtfMatchWire->setAxisTitle("Data Wire", 1);
  emtfMatchWire->setAxisTitle("Emul Wire", 2);

  emtfMatchStrip = ibooker.book2D("emtfMatchStrip","EMTF Match Strip", 256, 0, 256, 256, 0, 256);
  emtfMatchStrip->setAxisTitle("Data Strip", 1);
  emtfMatchStrip->setAxisTitle("Emul Strip", 2);

  emtfMatchHitBx = ibooker.book2D("emtfMatchHitBx","EMTF Match Hit Bx", 9, -4, 4, 9, -4 ,4);
  emtfMatchHitBx->setAxisTitle("Data Hit Bx", 1);
  emtfMatchHitBx->setAxisTitle("Emul Hit Bx", 2);

  emtfDataHitBx = ibooker.book1D("emtfDataHitBx","EMTF Data Hit Bx", 8, -4,4);
  emtfDataHitBx->setAxisTitle("BX Timing",1);

  emtfEmulHitBx = ibooker.book1D("emtfEmulHitBx","EMTF Emul Hit Bx", 8, -4,4);
  emtfEmulHitBx->setAxisTitle("BX Timing",1);

  emtfMatchEta = ibooker.book2D("emtfMatchEta","EMTF Match #eta", 100, -2.5, 2.5, 100, -2.5, 2.5);
  emtfMatchEta->setAxisTitle("Data #eta",1);
  emtfMatchEta->setAxisTitle("Emul #eta", 2);

  emtfMatchPhi = ibooker.book2D("emtfMatchPhi","EMTF Match #phi", 126, -3.15, 3.15, 126, -3.15, 3.15);
  emtfMatchPhi->setAxisTitle("Data #phi",1);
  emtfMatchPhi->setAxisTitle("Emul #phi", 2);

  emtfMatchPt = ibooker.book2D("emtfMatchPt","EMTF Match p_{T}", 256, 1, 257, 256, 1, 257);
  emtfMatchPt->setAxisTitle("Data p_{T}",1);
  emtfMatchPt->setAxisTitle("Emul p_{T}", 2);

  emtfMatchTrackBx = ibooker.book2D("emtfMatchTrackBx","EMTF Match Track Bx", 9, -4, 4, 9, -4 ,4);
  emtfMatchTrackBx->setAxisTitle("Data Track Bx", 1);
  emtfMatchTrackBx->setAxisTitle("Emul Track Bx", 2);


  emtfDifWire = ibooker.book1D("emtfDifWire", "EMTF Dif Wire", 127, 1, 128);
  emtfDifWire->setAxisTitle("Wire",1);
  emtfDifWire->setAxisTitle("Positive=data",2);

  emtfDifStrip = ibooker.book1D("emtfDifStrip", "EMTF Dif Strip", 255, 1, 256);
  emtfDifStrip->setAxisTitle("Wire",1);
  emtfDifStrip->setAxisTitle("Positive=data",2);

  emtfCollectionSizes = ibooker.book1D("emtfCollectionSizes","EMTF CollectionSizes", 6,0,6);
  emtfCollectionSizes->setBinLabel(1,"Data Hits",1);
  emtfCollectionSizes->setBinLabel(2,"Emul Hits",1);
  emtfCollectionSizes->setBinLabel(3,"Data Tracks",1);
  emtfCollectionSizes->setBinLabel(4,"Emul Tracks",1);
  emtfCollectionSizes->setBinLabel(5,"Data Output" ,1);
  emtfCollectionSizes->setBinLabel(6,"Emul output", 1);
}

void L1TdeStage2EMTF::analyze(const edm::Event& e, const edm::EventSetup& c) {

  if (verbose) edm::LogInfo("L1TdeStage2EMTF") << "L1TdeStage2EMTF: analyze..." << std::endl;

  edm::Handle<l1t::RegionalMuonCandBxCollection> dataMuons;
  e.getByToken(dataToken, dataMuons);

  edm::Handle<l1t::RegionalMuonCandBxCollection> emulMuons;
  e.getByToken(emulToken, emulMuons);

  emtfComparenMuonsEvent->Fill(dataMuons->size(), emulMuons->size());

  for (int itBX = dataMuons->getFirstBX(); itBX <= dataMuons->getLastBX(); ++itBX) {
    for (l1t::RegionalMuonCandBxCollection::const_iterator dataMuon = dataMuons->begin(itBX); dataMuon != dataMuons->end(itBX); ++dataMuon) {
      if (itBX > -2 || itBX < 2){ //only compare -1,0,+1 BX tracks
        emtfDataBX->Fill(itBX);
        emtfDatahwPt->Fill(dataMuon->hwPt());
        emtfDatahwEta->Fill(dataMuon->hwEta());
        emtfDatahwPhi->Fill(dataMuon->hwPhi());
        emtfDatahwQual->Fill(dataMuon->hwQual());
        emtfCollectionSizes->Fill(4);
      } 
    }
  }

  for (int itBX = emulMuons->getFirstBX(); itBX <= emulMuons->getLastBX(); ++itBX) {
    for (l1t::RegionalMuonCandBxCollection::const_iterator emulMuon = emulMuons->begin(itBX); emulMuon != emulMuons->end(itBX); ++emulMuon) {
      if (itBX > -2 || itBX < 2){ //only compare -1,0,+1 BX tracks
        emtfEmulBX->Fill(itBX);
        emtfEmulhwPt->Fill(emulMuon->hwPt());
        emtfEmulhwEta->Fill(emulMuon->hwEta());
        emtfEmulhwPhi->Fill(emulMuon->hwPhi());
        emtfEmulhwQual->Fill(emulMuon->hwQual());
        emtfCollectionSizes->Fill(5);
        } 
    }
  }

  //data Hits
  edm::Handle<l1t::EMTFHitCollection> dataHitCollection;
  e.getByToken(datahitToken, dataHitCollection);
  //std::cout << "Data Hits:" << endl;
  for (std::vector<l1t::EMTFHit>::const_iterator Hit = dataHitCollection->begin(); Hit != dataHitCollection->end(); ++Hit){
    int Endcap = Hit->Endcap();
    int Station = Hit->Station();
    int Ring = Hit->Ring();
    int Sector = Hit->Sector();
    int Chamber = Hit->Chamber();
    emtfCollectionSizes->Fill(0);
    emtfDataHitBx->Fill(Hit->BX());
  //std::cout << Endcap << " " << Station << " " << Ring << " " << Sector << " " << Chamber << " " << Hit->Strip() << " " << Hit->Wire() << " " << Hit->BX() << " " << Hit->Quality() <<  endl;

   }

  //emul Hits
  edm::Handle<l1t::EMTFHitCollection> emulHitCollection;
  e.getByToken(emulhitToken, emulHitCollection);
  //std::cout << "Emul Hits:" << endl;
  for(std::vector<l1t::EMTFHit>::const_iterator Hit = emulHitCollection->begin(); Hit != emulHitCollection->end(); ++Hit){
    int Endcap = Hit->Endcap();
    int Station = Hit->Station();
    int Ring = Hit->Ring();
    int Sector = Hit->Sector();
    int Chamber = Hit->Chamber();
    emtfEmulHitBx->Fill(Hit->BX());
    emtfCollectionSizes->Fill(1);
    //std::cout << Endcap << " " << Station << " " << Ring << " " << Sector << " " << Chamber << " " << Hit->Strip() << " " << Hit->Wire() << " " << Hit->BX() << " " << Hit->Quality() << endl;


  }
  //data Tracks
    edm::Handle<l1t::EMTFTrackCollection> dataTrackCollection;
  e.getByToken(datatrackToken, dataTrackCollection);

  //std::cout << "Data Tracks:" << endl;
  for (std::vector<l1t::EMTFTrack>::const_iterator Track = dataTrackCollection->begin(); Track != dataTrackCollection->end(); ++Track){
    float eta = Track->Eta();
    float phi_glob_rad = Track->Phi_glob_rad();
    float pt = Track->Pt();
    int mode = Track->Mode();
    int quality = Track->Quality();
    //std::cout << eta << " " << phi_glob_rad << " " << pt << " " << mode << " " << quality << " " << Track->BX() << endl;
    
    emtfDataEta->Fill(eta);
    emtfDataPhi->Fill(phi_glob_rad);
    emtfDataPt->Fill(pt);
    emtfDataMode->Fill(mode);
    emtfDataQuality->Fill(quality);

    emtfCollectionSizes->Fill(2);
  }
  //std::cout << "Emul Tracks" << endl;
  //emul Tracks
    edm::Handle<l1t::EMTFTrackCollection> emulTrackCollection;
  e.getByToken(emultrackToken, emulTrackCollection);

  for (std::vector<l1t::EMTFTrack>::const_iterator Track = emulTrackCollection->begin(); Track !=emulTrackCollection->end(); ++Track){
    float eta = Track->Eta();
    float phi_glob_rad = Track->Phi_glob_rad();
    float pt = Track->Pt();
    int mode = Track->Mode();
    int quality = Track->Quality();

    //std::cout << eta << " " << phi_glob_rad << " " << pt << " " << mode << " " << quality << " " << Track->BX() << endl;
    emtfEmulEta->Fill(eta);
    emtfEmulPhi->Fill(phi_glob_rad);
    emtfEmulPt->Fill(pt);
    emtfEmulMode->Fill(mode);
    emtfEmulQuality->Fill(quality);
    emtfCollectionSizes->Fill(3);
  }
    
  //Best Match Hit plots
  int minWireDif = 999;
  int dataWire = -99;
  int emulWire = -99;

  int minStripDif = 999;
  int dataStrip = -99;
  int emulStrip = -99;

  int minHitBxDif = 999;
  int dataHitBx = -99;
  int emulHitBx = -99;

 for (std::vector<l1t::EMTFHit>::const_iterator Hit = dataHitCollection->begin(); Hit != dataHitCollection->end(); ++Hit){

    for(std::vector<l1t::EMTFHit>::const_iterator emulHit = emulHitCollection->begin(); emulHit != emulHitCollection->end(); ++emulHit){
      if(abs((Hit->Wire() - emulHit->Wire())) < minWireDif){
        minWireDif = abs((Hit->Wire() - emulHit->Wire()));
        dataWire = Hit->Wire();
        emulWire = emulHit->Wire();
      }

      if(abs((Hit->Strip() - emulHit->Strip())) < minStripDif){
        minStripDif = abs((Hit->Strip() - emulHit->Strip()));
        dataStrip = Hit->Strip();
        emulStrip = emulHit->Strip();
      }
      if(abs((Hit->BX() - emulHit->BX())) < minHitBxDif){
        minHitBxDif = abs((Hit->BX() - emulHit->BX()));
        dataHitBx = Hit->BX();
        emulHitBx = emulHit->BX();
      }
    }
  }

  emtfMatchWire->Fill(dataWire, emulWire);
  emtfDifWire->Fill(dataWire - emulWire);
  emtfMatchStrip->Fill(dataStrip, emulStrip);
  emtfDifStrip->Fill(dataStrip - emulStrip);
  emtfMatchHitBx->Fill(dataHitBx, emulHitBx);

  //Best Match Track plots
  float minEtaDif = 999;
  float dataEta = -99;
  float emulEta = -99;

  float minPhiDif = 999;
  float dataPhi = -99;
  float emulPhi = -99;

  float minPtDif = 999;
  float dataPt = -99;
  float emulPt = -99;

  float minTrackBxDif = 999;
  float dataTrackBx = -99;
  float emulTrackBx = -99;


for (std::vector<l1t::EMTFTrack>::const_iterator Track = dataTrackCollection->begin(); Track != dataTrackCollection->end(); ++Track){
    for (std::vector<l1t::EMTFTrack>::const_iterator emulTrack = emulTrackCollection->begin(); emulTrack !=emulTrackCollection->end(); ++emulTrack){
      //eta
      if(abs((Track->Eta() - emulTrack->Eta())) < minEtaDif){
        minEtaDif = abs((Track->Eta() - emulTrack->Eta()));
        dataEta = Track->Eta();
        emulEta = emulTrack->Eta();
      }
     //phi
     if(abs((Track->Phi_glob_rad() - emulTrack->Phi_glob_rad())) < minPhiDif){
        minPhiDif = abs((Track->Phi_glob_rad() - emulTrack->Phi_glob_rad()));
        dataPhi = Track->Phi_glob_rad();
        emulPhi = emulTrack->Phi_glob_rad();
      }
     //Pt
     if(abs((Track->Pt() - emulTrack->Pt())) < minPtDif){
        minPtDif = abs((Track->Pt() - emulTrack->Pt()));
        dataPt = Track->Pt();
        emulPt = emulTrack->Pt();
      }
     //Track Bx
     if(abs((Track->BX() - emulTrack->BX())) < minTrackBxDif){
        minTrackBxDif = abs((Track->BX() - emulTrack->BX()));
        dataTrackBx = Track->BX();
        emulTrackBx = emulTrack->BX();
      }
    }
  }
  emtfMatchEta->Fill(dataEta, emulEta);
  emtfMatchPhi->Fill(dataPhi, emulPhi);
  emtfMatchPt->Fill(dataPt, emulPt);
  emtfMatchTrackBx->Fill(dataTrackBx, emulTrackBx);
} 
