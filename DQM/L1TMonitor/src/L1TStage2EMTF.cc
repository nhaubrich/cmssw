#include <string>
#include <vector>
#include <iostream>
#include "DQM/L1TMonitor/interface/L1TStage2EMTF.h"
using namespace std;

L1TStage2EMTF::L1TStage2EMTF(const edm::ParameterSet& ps) 
    : emtfToken(consumes<l1t::EMTFOutputCollection>(ps.getParameter<edm::InputTag>("emtfSource"))),
      monitorDir(ps.getUntrackedParameter<std::string>("monitorDir", "")),
      verbose(ps.getUntrackedParameter<bool>("verbose", false)) {}

L1TStage2EMTF::~L1TStage2EMTF() {}

void L1TStage2EMTF::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c) {}

void L1TStage2EMTF::beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) {}

void L1TStage2EMTF::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {

  int n_xbins;
  int xbin_shift = 1;
  std::vector<std::string> name = {"42", "41", "32", "31", "22", "21", "13", "12", "11"};
  std::vector<std::string> label = {"4/2", "4/1", "3/2", "3/1", "2/2", "2/1", "1/3", "1/2", "1/1"};

  ibooker.setCurrentFolder(monitorDir);
  emtfChamber = ibooker.book1D("chambers", "chambers", 20, -1.5,18.5);
  // ME (LCTs) Monitor Elements
  emtfErrors = ibooker.book1D("emtfErrors", "EMTF Errors", 6, 0, 6);
  emtfErrors->setAxisTitle("Error Type (Corruptions not implemented)", 1);
  emtfErrors->setAxisTitle("Number of Errors", 2);
  emtfErrors->setBinLabel(1, "Corruptions", 1);
  emtfErrors->setBinLabel(2, "Synch. Err.", 1);
  emtfErrors->setBinLabel(3, "Synch. Mod.", 1);
  emtfErrors->setBinLabel(4, "BX Mismatch", 1);
  emtfErrors->setBinLabel(5, "Time Misalign.", 1);
  emtfErrors->setBinLabel(6, "FMM != Ready", 1);

  emtfLCTBX = ibooker.book2D("emtfLCTBX", "EMTF LCT BX", 9, -1, 8, 18, -9, 9);
  emtfLCTBX->setAxisTitle("BX", 1);
  for (int bin = 1, j = -4; bin <= 9; ++bin, ++j) {
    emtfLCTBX->setBinLabel(bin, std::to_string(j), 1);
  }

  for (int i = 0; i < 9; ++i) {
    emtfLCTBX->setBinLabel(i + 1, "ME-" + label[i], 2);
    emtfLCTBX->setBinLabel(18 - i, "ME+" + label[i], 2);

    emtfLCTStrip[i] = ibooker.book1D("emtfLCTStripMENeg" + name[i], "EMTF Halfstrip ME-" + label[i], 256, 0, 256);
    emtfLCTStrip[i]->setAxisTitle("Cathode Halfstrip, ME-" + label[i], 1);

    emtfLCTStrip[17 - i] = ibooker.book1D("emtfLCTStripMEPos" + name[i], "EMTF Halfstrip ME+" + label[i], 256, 0, 256);
    emtfLCTStrip[17 - i]->setAxisTitle("Cathode Halfstrip, ME+" + label[i], 1);

    emtfLCTWire[i] = ibooker.book1D("emtfLCTWireMENeg" + name[i], "EMTF Wiregroup ME-" + label[i], 128, 0, 128);
    emtfLCTWire[i]->setAxisTitle("Anode Wiregroup, ME-" + label[i], 1);

    emtfLCTWire[17 - i] = ibooker.book1D("emtfLCTWireMEPos" + name[i], "EMTF Wiregroup ME+" + label[i], 128, 0, 128);
    emtfLCTWire[17 - i]->setAxisTitle("Anode Wiregroup, ME+" + label[i], 1);

    if (i < 6) {
      n_xbins = (i % 2) ? 18 : 36;
      
      emtfChamberStrip[i] = ibooker.book2D("emtfChamberStripMENeg" + name[i], "EMTF Halfstrip ME-" + label[i], n_xbins, xbin_shift, n_xbins + xbin_shift, 256, 0, 256);
      emtfChamberStrip[i]->setAxisTitle("Chamber, ME-" + label[i], 1);
      emtfChamberStrip[i]->setAxisTitle("Cathode Halfstrip", 2);
    
      emtfChamberWire[i] = ibooker.book2D("emtfChamberWireMENeg" + name[i], "EMTF Wiregroup ME-" + label[i], n_xbins, xbin_shift, n_xbins+xbin_shift, 128, 0, 128);
      emtfChamberWire[i]->setAxisTitle("Chamber, ME-" + label[i], 1);
      emtfChamberWire[i]->setAxisTitle("Anode Wiregroup", 2);

      emtfChamberStrip[17 - i] = ibooker.book2D("emtfChamberStripMEPos" + name[i], "EMTF Halfstrip ME+" + label[i], n_xbins, xbin_shift, n_xbins+xbin_shift, 256, 0, 256);
      emtfChamberStrip[17 - i]->setAxisTitle("Chamber, ME+" + label[i], 1);
      emtfChamberStrip[17 - i]->setAxisTitle("Cathode Halfstrip", 2);
    
      emtfChamberWire[17 - i] = ibooker.book2D("emtfChamberWireMEPos" + name[i], "EMTF Wiregroup ME+" + label[i], n_xbins, xbin_shift, n_xbins+xbin_shift, 128, 0, 128);
      emtfChamberWire[17 - i]->setAxisTitle("Chamber, ME+" + label[i], 1);
      emtfChamberWire[17 - i]->setAxisTitle("Anode Wiregroup", 2);
    } else {
      emtfChamberStrip[i] = ibooker.book2D("emtfChamberStripMENeg" + name[i], "EMTF Halfstrip ME-" + label[i], 36, xbin_shift, 36+xbin_shift, 256, 0, 256);
      emtfChamberStrip[i]->setAxisTitle("Chamber, ME-" + label[i], 1);
      emtfChamberStrip[i]->setAxisTitle("Cathode Halfstrip", 2);
   
      emtfChamberWire[i] = ibooker.book2D("emtfChamberWireMENeg" + name[i], "EMTF Wiregroup ME-" + label[i], 36, xbin_shift, 36+xbin_shift, 128, 0, 128);
      emtfChamberWire[i]->setAxisTitle("Chamber, ME-" + label[i], 1);
      emtfChamberWire[i]->setAxisTitle("Anode Wiregroup", 2);

      emtfChamberStrip[17 - i] = ibooker.book2D("emtfChamberStripMEPos" + name[i], "EMTF Halfstrip ME+" + label[i], 36, xbin_shift, 36+xbin_shift, 256, 0, 256);
      emtfChamberStrip[17 - i]->setAxisTitle("Chamber, ME+" + label[i], 1);
      emtfChamberStrip[17 - i]->setAxisTitle("Cathode Halfstrip", 2);
  
      emtfChamberWire[17 - i] = ibooker.book2D("emtfChamberWireMEPos" + name[i], "EMTF Wiregroup ME+" + label[i], 36, xbin_shift, 36+xbin_shift, 128, 0, 128);
      emtfChamberWire[17 - i]->setAxisTitle("Chamber, ME+" + label[i], 1);
      emtfChamberWire[17 - i]->setAxisTitle("Anode Wiregroup", 2);
    }
  }

  emtfChamberOccupancy = ibooker.book2D("emtfChamberOccupancy", "EMTF Chamber Occupancy", 55, 1, 56, 10, -5, 5);
  emtfChamberOccupancy->setAxisTitle("Sector (CSCID 1-9 Unlabelled)", 1);
  for (int bin = 1; bin <= 46; bin += 9) {
    emtfChamberOccupancy->setBinLabel(bin, std::to_string(bin % 8), 1);
  }
  emtfChamberOccupancy->setBinLabel(1, "ME-4", 2);
  emtfChamberOccupancy->setBinLabel(2, "ME-3", 2);
  emtfChamberOccupancy->setBinLabel(3, "ME-2", 2);
  emtfChamberOccupancy->setBinLabel(4, "ME-1b", 2);
  emtfChamberOccupancy->setBinLabel(5, "ME-1a", 2);
  emtfChamberOccupancy->setBinLabel(6, "ME+1a", 2);
  emtfChamberOccupancy->setBinLabel(7, "ME+1b", 2);
  emtfChamberOccupancy->setBinLabel(8, "ME+2", 2);
  emtfChamberOccupancy->setBinLabel(9, "ME+3", 2);
  emtfChamberOccupancy->setBinLabel(10, "ME+4", 2);

  // SP (Tracks) Monitor Elements
  emtfnTracks = ibooker.book1D("emtfnTracks", "Number of EMTF Tracks per Event", 4, 0, 4);
  for (int bin = 1; bin <= 4; ++bin) {
    emtfnTracks->setBinLabel(bin, std::to_string(bin - 1), 1);
  }

  emtfnLCTs = ibooker.book1D("emtfnLCTs", "Number of LCTs per EMTF Track", 5, 0, 5);
  for (int bin = 1; bin <= 5; ++bin) {
    emtfnLCTs->setBinLabel(bin, std::to_string(bin - 1), 1);
  }

  emtfTrackBX = ibooker.book2D("emtfTrackBX", "EMTF Track Bunch Crossings", 12, -6, 6, 7, 0, 7);
  emtfTrackBX->setAxisTitle("Sector (Endcap)", 1);
  for (int i = 0; i < 6; ++i) {
    emtfTrackBX->setBinLabel(i + 1, std::to_string(6 - i) + " (-)", 1);
    emtfTrackBX->setBinLabel(12 - i, std::to_string(6 - i) + " (+)", 1);
  }
  emtfTrackBX->setAxisTitle("Track BX", 2);
  for (int bin = 1, i = -3; bin <= 7; ++bin, ++i) {
    emtfTrackBX->setBinLabel(bin, std::to_string(i), 2);
  }
  
  emtfTrackPt = ibooker.book1D("emtfTrackPt", "EMTF Track p_{T}", 256, 1, 257);
  emtfTrackPt->setAxisTitle("Track p_{T} [GeV]", 1);

  emtfTrackEta = ibooker.book1D("emtfTrackEta", "EMTF Track #eta", 100, -2.5, 2.5);
  emtfTrackEta->setAxisTitle("Track #eta", 1);

  emtfTrackPhi = ibooker.book1D("emtfTrackPhi", "EMTF Track #phi", 126, -3.15, 3.15);
  emtfTrackPhi->setAxisTitle("Track #phi", 1);

  emtfTrackOccupancy = ibooker.book2D("emtfTrackOccupancy", "EMTF Track Occupancy", 100, -2.5, 2.5, 126, -3.15, 3.15);
  emtfTrackOccupancy->setAxisTitle("#eta", 1);
  emtfTrackOccupancy->setAxisTitle("#phi", 2);
  
  emtfMode = ibooker.book1D("emtfMode", "EMTF Track Mode", 17, -0.5, 16.5);
  emtfMode->setAxisTitle("Mode", 1);

  emtfQuality = ibooker.book1D("emtfQuality", "EMTF Track Quality", 17, -0.5, 16.5);
  emtfQuality->setAxisTitle("Quality", 1);

  emtfModeQuality = ibooker.book2D("emtfModeQuality", "EMTF Track Mode vs Quality", 17, -0.5, 16.5, 17, -0.5, 16.5);
  emtfModeQuality->setAxisTitle("Mode", 1);
  emtfModeQuality->setAxisTitle("Qualtiy", 2);
}

void L1TStage2EMTF::analyze(const edm::Event& e, const edm::EventSetup& c) {

  if (verbose) edm::LogInfo("L1TStage2EMTF") << "L1TStage2EMTF: analyze..." << std::endl;

  edm::Handle<l1t::EMTFOutputCollection> EMTFOutputCollection;
  e.getByToken(emtfToken, EMTFOutputCollection);

  int nTracks = 0;
 
  for (std::vector<l1t::EMTFOutput>::const_iterator EMTFOutput = EMTFOutputCollection->begin(); EMTFOutput != EMTFOutputCollection->end(); ++EMTFOutput) {

    // Event Record Header
    const l1t::emtf::EventHeader* EventHeader = EMTFOutput->PtrEventHeader();
    int Endcap = EventHeader->Endcap(); //-1 or +1
    int Sector = EventHeader->Sector()-1;// (1-6)-1
   
    
    // Check if FMM Signal was good
    if (EventHeader->Rdy() == 0) emtfErrors->Fill(5);

    // ME (LCTs) Data Record
    const l1t::emtf::MECollection* MECollection = EMTFOutput->PtrMECollection();

    for (std::vector<l1t::emtf::ME>::const_iterator ME = MECollection->begin(); ME != MECollection->end(); ++ME) {
      int Station = ME->Station(); //1,2,3,or 4
      int CSCID = ME->CSC_ID(); //1-9
      int half_strip = ME->Strip(); 
      int wire_group = ME->Wire();
      float bin_offset;
      int histogram_index;
      int Ring = ME->Ring();
      
      if (Ring==4) {
        Station = 0;//make station ME1a = station 0
        Ring = 1;
      }
      if (ME->SE()) emtfErrors->Fill(1);
      if (ME->SM()) emtfErrors->Fill(2);
      if (ME->BXE()) emtfErrors->Fill(3);
      if (ME->AF()) emtfErrors->Fill(4);

      /*
      if ( Station == 0 || Station == 1) {
        if (CSCID < 3) {
          bin_offset = 0.5;//1/1 goes to .5
        } else if (CSCID > 2 && CSCID < 6) {
          bin_offset = 1.5;// 1/2 goes to 1.5
        } else {
          bin_offset = 2.5;// 1/3 goes to 2.5
        }
      } else {
        if (CSCID < 3) {
          if (Station == 2) {
            bin_offset = 3.5;// 2/1 3.5
          } else if (Station == 3) {
            bin_offset = 5.5;// 3/1 5.5
          } else {
            bin_offset = 7.5;// 4/1 7.5
          }
        } else {
          if (Station == 2) {
            bin_offset = 4.5;// 2/2 4.5
          } else if (Station == 3) {
            bin_offset = 6.5;// 3/2 6.5
          } else {
            bin_offset = 8.5;// 4/2 8.5
          }
        }
      }
    */

    if (Station == 0 || Station == 1){
      bin_offset = Ring - 0.5;
    }else{
      bin_offset = Station + Ring + (Station - 1.5);
    }
   /* 
    cout << Station << endl;
    cout << Sector << endl;
    cout << Ring << endl;
    cout << CSCID << endl;
    cout << endl;
       //cout << bin_offset << endl;
    */
    if (bin_offset > 8.5) bin_offset = 8.5;


    histogram_index = int(8.5 + Endcap * bin_offset);

      emtfLCTBX->Fill(ME->Tbin_num(), Endcap * bin_offset);
      emtfLCTStrip[histogram_index]->Fill(half_strip);
      emtfLCTWire[histogram_index]->Fill(wire_group);

      if (bin_offset == 0.5) {
          emtfChamberStrip[histogram_index]->Fill((Sector * 6) + (CSCID + Station *3 ), half_strip);
	  emtfChamberWire[histogram_index]->Fill((Sector * 6) + (CSCID + Station *3 ), wire_group);
      } else if (bin_offset == 1.5) {
          emtfChamberStrip[histogram_index]->Fill((Sector * 6) + (CSCID + Station * 3 - 3), half_strip);
	  emtfChamberWire[histogram_index]->Fill((Sector * 6) + (CSCID + Station * 3 - 3), wire_group);
      } else if (bin_offset == 2.5) {
          emtfChamberStrip[histogram_index]->Fill((Sector * 6) + (CSCID + Station * 3 - 6), half_strip);
	  emtfChamberWire[histogram_index]->Fill((Sector * 6) + (CSCID + Station * 3 - 6), wire_group);
      } else {
        if(int(bin_offset) % 2) {
          emtfChamberStrip[histogram_index]->Fill((Sector * 3) + CSCID, half_strip);
	  emtfChamberWire[histogram_index]->Fill((Sector * 3) + CSCID, wire_group);
        } else {
          emtfChamberStrip[histogram_index]->Fill((Sector * 6) + (CSCID - 3), half_strip);
	  emtfChamberWire[histogram_index]->Fill((Sector * 6) + (CSCID - 3), wire_group);
        }
      }

      emtfChamberOccupancy->Fill((Sector * 9) + CSCID-1, Endcap * (Station + 0.5));
    }

    // SP (Tracks) Data Record
    const l1t::emtf::SPCollection* SPCollection = EMTFOutput->PtrSPCollection();

    for (std::vector<l1t::emtf::SP>::const_iterator SP = SPCollection->begin(); SP != SPCollection->end(); ++SP) {
      float Pt = SP->Pt();
      float Eta = SP->Eta_GMT();
      float Phi_GMT_global_rad = SP->Phi_GMT_global() * (M_PI/180);
      if (Phi_GMT_global_rad > M_PI) Phi_GMT_global_rad -= 2*M_PI;
      int Quality = SP->Quality();
      int Mode = SP->Mode();
      
      emtfChamber->Fill(Mode);

      if (Quality == 0) {
        emtfnLCTs->Fill(0);
      } else if (Quality == 1 || Quality == 2 || Quality == 4 || Quality == 8) {
        emtfnLCTs->Fill(1);
      } else if (Quality == 3 || Quality == 5 || Quality == 9 || Quality == 10 || Quality == 12) {
        emtfnLCTs->Fill(2);
      } else if (Quality == 7 || Quality == 11 || Quality == 13 || Quality == 14) {
        emtfnLCTs->Fill(3);
      } else {
        emtfnLCTs->Fill(4);
      }

      emtfTrackBX->Fill(Endcap * (Sector + 0.5), SP->TBIN_num());
      emtfTrackPt->Fill(Pt);
      emtfTrackEta->Fill(Eta);
      emtfTrackPhi->Fill(Phi_GMT_global_rad);
      emtfTrackOccupancy->Fill(Eta, Phi_GMT_global_rad);
      emtfMode->Fill(Mode);
      emtfQuality->Fill(Quality);
      emtfModeQuality->Fill(Mode, Quality);      

      nTracks++;
    }
  }
  emtfnTracks->Fill(nTracks);
}
