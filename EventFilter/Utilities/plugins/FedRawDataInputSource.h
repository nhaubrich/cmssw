#ifndef EventFilter_Utilities_FedRawDataInputSource_h
#define EventFilter_Utilities_FedRawDataInputSource_h

#include <memory>
#include <stdio.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_vector.h"

#include "boost/filesystem.hpp"

#include "DataFormats/Provenance/interface/Timestamp.h"
#include "EventFilter/Utilities/plugins/EvFDaqDirector.h"
#include "FWCore/Sources/interface/RawInputSource.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Sources/interface/DaqProvenanceHelper.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "IOPool/Streamer/interface/FRDEventMessage.h"

class FEDRawDataCollection;
class InputSourceDescription;
class ParameterSet;

namespace evf {
class FastMonitoringService;
}

namespace jsoncollector {
class DataPointDefinition;
}

class FedRawDataInputSource: public edm::RawInputSource {

public:
  explicit FedRawDataInputSource(edm::ParameterSet const&,edm::InputSourceDescription const&);
  virtual ~FedRawDataInputSource();

protected:
  virtual bool checkNextEvent() override;
  virtual void read(edm::EventPrincipal& eventPrincipal) override;

private:
  virtual void preForkReleaseResources() override;
  virtual void postForkReacquireResources(boost::shared_ptr<edm::multicore::MessageReceiverForSource>) override;
  virtual void rewind_() override;

  void maybeOpenNewLumiSection(const uint32_t lumiSection);
  evf::EvFDaqDirector::FileStatus cacheNextEvent();
  edm::Timestamp fillFEDRawDataCollection(std::auto_ptr<FEDRawDataCollection>&) const;
  void deleteFile(std::string const&);
  int grabNextJsonFile(boost::filesystem::path const&);
  void renameToNextFree(std::string const& fileName) const;
  //void purgeOldFiles(bool checkAll);

  //thread functions
  void readSupervisor();
  void readWorker(unsigned int tid, std::atomic<bool> *started);
  void threadError();
  bool exceptionState() {return setExceptionState_;}

  std::string defPath_;

  unsigned int eventChunkSize_; // for buffered read-ahead
  unsigned int eventChunkBlock_; // how much read(2) asks at the time
  unsigned int readBlocks_;
  unsigned int numConcurrentReads_;
  bool deleteFileAfterRead_;

  // get LS from filename instead of event header
  const bool getLSFromFilename_;
  const bool verifyAdler32_;
  const bool testModeNoBuilderUnit_;

  const edm::RunNumber_t runNumber_;

  const std::string buInputDir_;
  const std::string fuOutputDir_;

  const edm::DaqProvenanceHelper daqProvenanceHelper_;

  std::unique_ptr<FRDEventMsgView> event_;

  boost::filesystem::path openFile_;
  edm::EventID eventID_;

  unsigned int currentLumiSection_;
  unsigned int eventsThisLumi_;

  jsoncollector::DataPointDefinition *dpd_;

  evf::FastMonitoringService *fms_ = nullptr;

  /*
   *
   * Multithreaded file reader
   *
   **/

  struct InputChunk {
    unsigned char * buf_;
    InputChunk *next_ = nullptr;
    uint32_t size_;
    uint32_t usedSize_ = 0;
    unsigned int index_;
    unsigned int offset_;
    unsigned int fileIndex_;
    std::atomic<bool> readComplete_;

    InputChunk(unsigned int index, uint32_t size): size_(size),index_(index) {
	    buf_ = new unsigned char[size_];
	    reset(0,0,0);
    }
    void reset(unsigned int newOffset, unsigned int toRead, unsigned int fileIndex) {
	    offset_=newOffset;
	    usedSize_=toRead;
	    fileIndex_=fileIndex;
	    readComplete_=false;
    }

    ~InputChunk() {delete buf_;}
  };

  struct InputFile {
//    public:
      FedRawDataInputSource *parent_;
      evf::EvFDaqDirector::FileStatus status_;
      unsigned int lumi_;
      std::string fileName_;
      uint32_t fileSize_;
      uint32_t nChunks_;
      unsigned int nEvents_;
      unsigned int nProcessed_;
      //CompletitionCounter cc_;

      tbb::concurrent_vector<InputChunk*> chunks_;

      uint32_t  bufferPosition_ = 0;
      uint32_t  chunkPosition_ = 0;
      unsigned int currentChunk_ = 0;

      bool fileExists_ = false;
      //bool deleted_ = false;

      //maybe make a few constructors
      InputFile(evf::EvFDaqDirector::FileStatus status, unsigned int lumi = 0, std::string const& name = std::string(), 
	  uint32_t fileSize =0, uint32_t nChunks=0, unsigned int nEvents=0, FedRawDataInputSource *parent = nullptr): // CompletitionCounter * cc = nullptr):
	parent_(parent),
	status_(status),
	lumi_(lumi),
	fileName_(name),
	fileSize_(fileSize),
	nChunks_(nChunks),
	nEvents_(nEvents),
	nProcessed_(0)
	//cc_(cc)
      {
	for (unsigned int i=0;i<nChunks;i++)
	  chunks_.push_back(nullptr);
      }

      bool waitForChunk(unsigned int chunkid) {
	//some atomics to make sure everything is cache synchronized for the main thread
        return chunks_[chunkid]!=nullptr && chunks_[chunkid]->readComplete_;
      }
      bool advance(unsigned char* & dataPosition, const size_t size);
      void moveToPreviousChunk(const size_t size, const size_t offset);
      void rewindChunk(const size_t size);
  };
/*
  struct CompletitionCounter {
	  std::string name_;
	  unsigned int nChunks_;
	  std::atomic<unsigned int> chunksComplete_; 
	  CompletitionCounter(std::string & name, unsigned int nChunks):
		  name_(name),
		  nChunks_(nChunks),
		  chunksComplete_(0)
	  {}
  }
*/
  typedef std::pair<InputFile*,InputChunk*> ReaderInfo;

  InputFile *currentFile_ = nullptr;
  bool chunkIsFree_=false;

  std::unique_ptr<std::thread> readSupervisorThread_;
  std::vector<std::thread*> workerThreads_;

  tbb::concurrent_queue<unsigned int> workerPool_;
  std::vector<ReaderInfo> workerJob_;

  tbb::concurrent_queue<InputChunk*> freeChunks_;
  tbb::concurrent_queue<InputFile*> fileQueue_;

  std::mutex mReader_;
  std::vector<std::condition_variable*> cvReader_;

//  std::queue<CompletitionCounter*> openFileTracker_;

  //communication with threads
  bool quit_threads_=false;
  bool setExceptionState_ = false;

};

#endif // EventFilter_Utilities_FedRawDataInputSource_h

/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
