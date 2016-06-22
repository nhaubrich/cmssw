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

  emtfMatchQuality = ibooker.book2D("emtfMatchQuality","EMTF Match Quality", 16, 0, 16, 16, 0, 16);
  emtfMatchQuality->setAxisTitle("Data Quality",1);
  emtfMatchQuality->setAxisTitle("Emul Quality", 2);

  emtfMatchMode = ibooker.book2D("emtfMatchMode","EMTF Match Mode", 16, 0, 16, 16, 0, 16);
  emtfMatchMode->setAxisTitle("Data Mode",1);
  emtfMatchMode->setAxisTitle("Emul Mode", 2);
  for (int bin = 1; bin <= 16; ++bin) {
    emtfMatchQuality->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfMatchMode->setBinLabel(bin, std::to_string(bin - 1), 1);
    emtfMatchQuality->setBinLabel(bin, std::to_string(bin - 1), 2);
    emtfMatchMode->setBinLabel(bin, std::to_string(bin - 1), 2);
  }

//  emtfCollectionSizes = ibooker.book1D("emtfCollectionSizes","EMTF CollectionSizes", 3,0,3);
//  emtfCollectionSizes->setBinLabel(1,"Data Hits",1);
//  emtfCollectionSizes->setBinLabel(2,"Emul Hits",1);
//  emtfCollectionSizes->setBinLabel(1,"Data Tracks",1);
//  emtfCollectionSizes->setBinLabel(2,"Emul Tracks",1);
//  emtfCollectionSizes->setBinLabel(5,"Data Output" ,1);
//  emtfCollectionSizes->setBinLabel(6,"Emul output", 1);
}

void L1TdeStage2EMTF::analyze(const edm::Event& e, const edm::EventSetup& c) {

  if (verbose) edm::LogInfo("L1TdeStage2EMTF") << "L1TdeStage2EMTF: analyze..." << std::endl;

  //This module is only for direct by-event comparisons between data and emul
  //Ratio plots are made in the client L1TStage2EMTFDEClient.cc
  //Restrict BX to -1, 0, or 1
  edm::Handle<l1t::RegionalMuonCandBxCollection> dataMuons;
  e.getByToken(dataToken, dataMuons);

  edm::Handle<l1t::RegionalMuonCandBxCollection> emulMuons;
  e.getByToken(emulToken, emulMuons);

  emtfComparenMuonsEvent->Fill(dataMuons->size(), emulMuons->size());

  //data Hits
  edm::Handle<l1t::EMTFHitCollection> dataHitCollection;
  e.getByToken(datahitToken, dataHitCollection);
  //emul Hits
  edm::Handle<l1t::EMTFHitCollection> emulHitCollection;
  e.getByToken(emulhitToken, emulHitCollection);
 //data Tracks
    edm::Handle<l1t::EMTFTrackCollection> dataTrackCollection;
  e.getByToken(datatrackToken, dataTrackCollection);
  //emul Tracks
    edm::Handle<l1t::EMTFTrackCollection> emulTrackCollection;
  e.getByToken(emultrackToken, emulTrackCollection);

    //Best Match Track plots

  float mindR = 999;

  l1t::EMTFTrack closestData; 
  l1t::EMTFTrack closestEmul;

  //Find the pair of data/emulator tracks that minimizes d(phi)^2 + d(eta)^2 
  for (std::vector<l1t::EMTFTrack>::const_iterator dataTrack = dataTrackCollection->begin(); dataTrack != dataTrackCollection->end(); ++dataTrack){
    if (dataTrack->BX() > 1 || dataTrack->BX() < -1) continue;
    for (std::vector<l1t::EMTFTrack>::const_iterator emulTrack = emulTrackCollection->begin(); emulTrack !=emulTrackCollection->end(); ++emulTrack){
      if (emulTrack->BX() > 1 || emulTrack->BX() < -1) continue;
      float dPhi = dataTrack->Phi_glob_rad() - emulTrack->Phi_glob_rad();
      if (dPhi > 3.14159) dPhi -= 3.14159;//ensure dPhi falls between -pi and +pi
      if (dPhi < -3.14159) dPhi += 3.14159;
      float dEta = dataTrack->Eta() - emulTrack->Eta();
      if (dPhi*dPhi + dEta*dEta < mindR){
        mindR = dPhi*dPhi + dEta*dEta;
        closestData = *dataTrack;
        closestEmul = *emulTrack;
      }
    }
  }
  if (mindR != 999){
    emtfMatchEta->Fill(closestData.Eta(), closestEmul.Eta());
    emtfMatchPhi->Fill(closestData.Phi_glob_rad(), closestEmul.Phi_glob_rad());
    emtfMatchPt->Fill(closestData.Pt(), closestEmul.Pt());
    emtfMatchTrackBx->Fill(closestData.BX(), closestEmul.BX()); 
    emtfMatchMode->Fill(closestData.Mode(), closestEmul.Mode());
    emtfMatchQuality->Fill(closestData.Quality(), closestEmul.Quality());
  } 
} 
