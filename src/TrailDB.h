#ifndef TRAILDB_H_INCLUDED
#define TRAILDB_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <sstream>
#include <string.h>
#include <stdint.h>
#include <fstream>

#include "PointerTypedefs.h"
#include "ByteManipulations.h"

#include "traildb.h"

static const uint32_t BUFFER_SIZE = 1000000;


class Event {

  public:

    Event(uint32_t timestamp=0xFFFFFFFFULL):
      timestamp_(timestamp) { }

    ~Event() { }

    uint32_t GetTimestamp() const {return timestamp_;}
    void SetTimestamp(const uint32_t timestamp) {timestamp_ = timestamp;}

    void ResetTrail(std::string item) {
      trail_[item] = "";
    }

    void AddTrail(std::string item, std::string value) {
      if (trail_.find(item) == trail_.end()) {
            ResetTrail(item);
          }
      trail_[item] = value;
    }

    std::string GetTrail(std::string item) {
      if (trail_.find(item) == trail_.end()) {
        std::cout <<"Requested item "<< item<<" not found acckk"<<std::endl;
        for(std::map<std::string,std::string>::iterator it = trail_.begin();
            it != trail_.end(); ++it){
          std::cout <<it->first<<"  "<<it->second<<std::endl;
        }
        ResetTrail(item);
      }
      return trail_[item];
    }

    bool IsEventType(std::string type){
      return GetTrail("type") == type;
    }

    // Define time sorting of events
    bool operator<(const Event& e) const {return timestamp_ < e.timestamp_;}
    bool operator>(const Event& e) const {return timestamp_ > e.timestamp_;}

  private:
    uint32_t timestamp_;
    std::map<std::string, std::string> trail_;

};

SHARED_POINTER_TYPEDEFS(Event);

class EventTypeMatch : public std::binary_function<Event, std::string, bool> {
  public:
    bool operator()(Event& e, const std::string type) const {
          return e.GetTrail("type") == type;
        }
};

class EventList {

  public:

    EventList() { }

    EventList(int nReserve) {
      events_.reserve(nReserve);
    }

    ~EventList() { }

    void AddEvent(const Event& e) {events_.push_back(e);}

    uint32_t GetSize() const {return (uint32_t)events_.size();}

    void Clear() {events_.clear();}

    typedef std::vector<Event>::iterator EventIterator;
    typedef std::vector<Event>::reverse_iterator EventRIterator;
    typedef std::vector<Event>::const_iterator ConstEventIterator;

    EventIterator EventsBegin() {return events_.begin();}
    EventIterator EventsEnd() {return events_.end();}

    EventRIterator EventsRBegin() {return events_.rbegin();}
    EventRIterator EventsREnd() {return events_.rend();}

    ConstEventIterator EventsBegin() const {return events_.begin();}
    ConstEventIterator EventsEnd() const {return events_.end();}

    bool HasEventType(std::string type) {
      return find_if(EventsBegin(), EventsEnd(), 
                     bind2nd(EventTypeMatch(), type)) != EventsEnd();
    }


    uint32_t GetCountEventType(std::string type) {
      uint32_t count = (uint32_t)count_if(EventsBegin(), EventsEnd(),
          bind2nd(EventTypeMatch(),type));
      return count;
    }


  private:

    std::vector<Event> events_;
};

SHARED_POINTER_TYPEDEFS(EventList);

class TrailDB {

  public:

    TrailDB(std::string dbPath_);

    virtual ~TrailDB();

    void LoadDimensionNames();

    uint64_t GetNumberOfCookies() const { return numCookies_; }

    uint64_t GetNumberOfEvents() const { return numEvents_; }

    uint32_t GetMaxTimestamp() const { return maxTimestamp_; }

    uint32_t GetMinTimestamp() const { return minTimestamp_; }

    uint32_t GetNumberOfDimensions() const { return numDimensions_; }

    std::vector<std::string> GetDimNames() const {
      return dimNames_;
    }

    bool HasCookieIndex() { return tdb_has_cookie_index(db_) == 1; }

    void GetCookieByInd(uint64_t ind, std::vector<uint8_t> &res);

    uint64_t GetIndexForCookie(const uint8_t * cookie_id);

    const std::string GetHexCookieByInd(uint64_t ind);

    uint32_t RawDecode(uint64_t cookie_index);

    uint32_t GetNumberOfEventsByType(uint64_t cookie_index, std::string type);

    const char* DecodeValue(uint8_t val);

    EventListPtr LoadEvents(uint64_t i);

    std::vector<uint32_t> GetTimestampVector(uint64_t cookie_index);

  //private:

    std::string dbPath_;
    tdb* db_;
    uint64_t numCookies_;
    uint64_t numEvents_;
    uint32_t numDimensions_;
    uint32_t minTimestamp_;
    uint32_t maxTimestamp_;
    uint32_t eventTypeInd_;

    std::vector<std::string> dimNames_;

    uint32_t buff_[BUFFER_SIZE];
    char res_[BUFFER_SIZE];

    static const unsigned TDB_KEY_SIZE_BYTES = 16;
};

TrailDB::TrailDB(std::string db_path):
  dbPath_(db_path) {

    db_ = tdb_open(dbPath_.c_str());
    numCookies_ = tdb_num_cookies(db_);
    numDimensions_ = tdb_num_fields(db_);
    numEvents_ = tdb_num_events(db_);
    minTimestamp_ = tdb_min_timestamp(db_);
    maxTimestamp_ = tdb_max_timestamp(db_);

    LoadDimensionNames();

    // Figure out the index that stores type values
    for(int i = 0; i < dimNames_.size(); ++i) {
      if(!(dimNames_.at(i)).compare("type")) {
        eventTypeInd_ = i+1;
        break;
      }
    }

};

TrailDB::~TrailDB() {
  tdb_close(db_);
};

//Return vector of 16 bytes for cookie id
void TrailDB::GetCookieByInd(uint64_t ind, std::vector<uint8_t> &res) {
  const uint8_t *p_cookie = (uint8_t *)tdb_get_cookie(db_, ind);
  res.clear();
  res.resize(0);
  if (p_cookie != NULL) {
    for (int i = 0; i < TDB_KEY_SIZE_BYTES; ++i) {
      uint8_t cookie_byte;
      p_cookie = ParseValue(&(*p_cookie), cookie_byte, 1);
      res.push_back(cookie_byte);
    }
  }
};

//Return the Hex representation of the cookie in a string
const std::string TrailDB:: GetHexCookieByInd(uint64_t ind) {
  std::vector<uint8_t> res;
  GetCookieByInd(ind,res);
  std::stringstream ss;
  if (res.size() == TDB_KEY_SIZE_BYTES) {
    for (auto &i : res) {
      ss << std::hex << std::setfill('0') << std::setw(2) << (uint)i;
    }
  }
  const std::string s = ss.str();
  return s;
};

//Given cookie key (16 bytes) return corresponding index
uint64_t TrailDB::GetIndexForCookie(const uint8_t * cookie_id) {
  return tdb_get_cookie_id(db_, cookie_id);
}

//Given an index, return the number of events
//Also, load the events in buff_
uint32_t TrailDB::RawDecode(uint64_t cookie_index) {
  uint32_t count = tdb_decode_trail(db_, cookie_index, buff_, BUFFER_SIZE, 0);
  if (( count % (numDimensions_ + 1)) != 0) return 0;
  return count / (numDimensions_ + 1);
  //return count;
};

uint32_t TrailDB::GetNumberOfEventsByType(uint64_t cookie_index,
                                    std::string type) {

  uint32_t tot_evts = RawDecode(cookie_index);

  // if no type specified return sum of all event type counts
  if(!type.compare(""))
    return tot_evts;

  // event type encoded
  uint32_t evt_type_encoded = tdb_get_item(db_, eventTypeInd_, type.c_str());
  //std::cout <<"type encoded : "<<evt_type_encoded<<std::endl;

  uint32_t counts = 0;
  for (size_t i = 0; i < tot_evts; ++i) {
    uint32_t val = buff_[ i * (numDimensions_ + 1) + 1];
    //std::cout << i<<" type: "<<val<<std::endl;
    // only sum up events for the required event type
    if (val == evt_type_encoded) {
      ++counts;
    }
  }
  return counts;
};

const char* TrailDB::DecodeValue(uint8_t val) {
  const char* raw_val = tdb_get_item_value(db_, val);
  return raw_val;
};

std::vector<uint32_t> TrailDB::GetTimestampVector(uint64_t cookie_index) {
  uint32_t num_evts = RawDecode(cookie_index);
  std::vector<uint32_t> res;
  for (size_t i = 0; i < num_evts; ++i) {
    res.push_back(buff_[ i * (numDimensions_ + 1)]);
  }
  return res;
};

void TrailDB::LoadDimensionNames() {
  std::string line;
  std::string path =  dbPath_ + "/fields";
  std::ifstream fs(path);
  if(fs.is_open()) {
    while(std::getline(fs, line)) {
      dimNames_.push_back(line);
    }
    fs.close();
  }
  else {
    std::cerr << "Unable to open file:  " <<path<<std::endl;
    exit(0);
  }

  if (dimNames_.size()+1 != numDimensions_) {
    //for (auto &i : dimNames_) {
    //  std::cout << i <<std::endl;
    //}
    std::cout <<"Dimensions do not match... ! "<< dimNames_.size()
      << " vs. "<<numDimensions_<< std::endl;
  }
};

EventListPtr TrailDB::LoadEvents(uint64_t cookie_index) {
  //number of total items (events * fields)
  uint32_t count = tdb_decode_trail(db_, cookie_index, buff_, BUFFER_SIZE, 0);

  EventListPtr eventList = boost::make_shared<EventList>();

  //loop over all items
  uint32_t i = 0;
  while (i < count) {
    Event e;
    //timestamp
    e.SetTimestamp(buff_[i]);
    i += 1;
    std::vector<std::string>::const_iterator di = dimNames_.begin();
    while (i < count && buff_[i]) {
      std::stringstream ss;
      if(di == dimNames_.end()) {
        std::cerr <<"Reached premature end of dimension names"<<std::endl;
      }
      ss << tdb_get_value(db_, buff_[i] & 255, buff_[i] >> 8);
      e.AddTrail((*di), ss.str());
      ss.str(""); ss.clear();
      i += 1;
      ++di;
    }
    eventList->AddEvent(e);
    i +=1 ;
  }
  return eventList;
}

#endif  //TRAILDB_H_INCLUDED



