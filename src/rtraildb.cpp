#include <Rcpp.h>
#include <stdint.h>
#include <functional>
#include "RTraildb.h"

using namespace Rcpp;

// [[Rcpp::export]]
List read_trails_meta(List tdbSpec, bool verbose = false) {
  auto tdb = RTrailDB::Create(tdbSpec);
  std::vector<std::string> fnames = tdb->GetDimNames();
  std::vector<uint32_t> fsize;
  for(auto it = fnames.begin(); it != fnames.end(); ++it) {
    fsize.push_back(tdb->GetFieldSize(*it));
  }

  return List::create(
    _["number_of_uuids"] = tdb->GetNumberOfUUIDs(),
    _["number_of_events"] = tdb->GetNumberOfEvents(),
    _["number_of_dimensions"] = tdb->GetNumberOfFields(),
    _["dimensions"] = tdb->GetDimNames(),
    _["dimensions_uniques"] = fsize,
    _["min_timestamp"] = tdb->GetMinTimestamp(),
    _["max_timestamp"] = tdb->GetMaxTimestamp()
  );
}

// The following will return a data frame object
// [[Rcpp::export]]
RObject read_trails(List tdbSpec,
                    uint32_t first_trail = 0,
                    uint32_t last_trail = 0,
                    bool sample = false,
                    double fraction = 1.0,
                    uint32_t start_timestamp = 0,
                    uint32_t stop_timestamp = 0,
                    std::string filter = "",
                    bool verbose=false) {

  auto tdb = RTrailDB::Create(tdbSpec);

  //sanity checks
  if(sample && (fraction < 0  || fraction > 1)) {
    Rcpp::stop("fraction must be within 0 and 1");
  }

  if(first_trail > last_trail) {
    Rcpp::stop("last_trail must be equal to or larger than first_trail");
  }

  if (start_timestamp > stop_timestamp) {
    Rcpp::stop("stop_timestamp must be equal to or larger than start_timestamp");
  }

  bool doFilterTrail = false;
  if (first_trail != 0 | last_trail != 0) {
    doFilterTrail = true;
  }

  bool doFilterTimestamp = false;
  if (start_timestamp != 0 | stop_timestamp != 0) {
    doFilterTimestamp = true;
  }

  bool doFilterType = false;
  if (filter.compare("")) {
    doFilterType = true;
  }


  // Strategy is to fill columns into corresponding vectors
  std::vector<std::string> vDimNames = tdb->GetDimNames();
  std::vector<std::vector<std::string> > colVector(vDimNames.size());
  std::vector<std::string> vuuids;
  std::vector<std::uint32_t> vts;

  uint64_t tot_uuids = tdb->GetNumberOfUUIDs();

  // Process all trails if not stated otherwise
  if (!doFilterTrail) {
    last_trail = tot_uuids;
  }

  for (uint64_t num = first_trail; num < last_trail; ++num) {
    auto eventList = tdb->LoadEvents(num);

    if (num % 5000 == 0)
      Rcpp::checkUserInterrupt();

    if (sample && R::runif(0,1) >= fraction) continue;

    EventList::EventIterator evtIter;

    //Number of trails for current uuid
    uint32_t evtSize = eventList->GetSize();

    for (evtIter = eventList->EventsBegin();
         evtIter != eventList->EventsEnd();
         ++evtIter) {

      uint32_t ts = evtIter->GetTimestamp();

      if (doFilterTimestamp &&
          (ts < start_timestamp || ts > stop_timestamp )) continue;

      if (doFilterType && (filter.compare(evtIter->GetEventType())) ) continue;

      vuuids.push_back(tdb->GetHexUUIDByInd(num));
      vts.push_back(ts);

      int nd = 0;
      for(std::vector<std::string>::iterator it = vDimNames.begin();
          it != vDimNames.end();
          ++it ) {
        if((*it).compare("time")) {
          colVector[nd].push_back(evtIter->GetTrail(*it));
        }
        else{
          colVector[nd].push_back("");
        }
        ++nd;
      }
    }
  }

  //Set the column names
  std::vector<std::string> colNames(2 + vDimNames.size());
  List out =  List(2 + vDimNames.size());
  out[0] = vuuids; colNames[0] = "uuid";
  out[1] = vts; colNames[1] = "timestamp";
  for (int k = 0; k < vDimNames.size(); ++k) {
    out[k+2] = colVector[k];
    colNames[k+2] = vDimNames[k];
  }

  out.attr("row.names") = IntegerVector::create(NA_INTEGER, -(vuuids.size()));
  out.attr("names") = colNames;
  out.attr("class") = CharacterVector::create("tbl_df", "tbl", "data.frame");

  return out;

}
