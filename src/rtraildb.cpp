#include <Rcpp.h>
#include <stdint.h>
#include <functional>
#include "CPPTrailDB.h"

using namespace Rcpp;

CharacterVector get_dimensions(TrailDB* tdb) {
  return wrap(tdb->GetDimNames());
}

NumericVector get_ts_vector(TrailDB* tdb, uint64_t tdb_uuid) {
  return wrap(tdb->GetTimestampVector(tdb_uuid));
}

//Things to get frmo events
//GetTimestamp

//Unpack the entire traildb into a dataframe
List tdb_dataframe(TrailDB* tdb,
    uint32_t first_trail = 0,
    uint32_t last_trail = 0,
    bool sample = false,
    double fraction = 1.0,
    uint32_t start_timestamp = 0,
    uint32_t stop_timestamp = 0,
    std::string filter = ""
    ) {

  //sanity checks
  if(sample && (fraction < 0  || fraction > 1)) {
    Rcpp::stop("fraction must be within 0 and 1");
  }
  

  //
  if(first_trail > last_trail)
    Rcpp::stop("last_trail must be equal to or larger than first_trail");

  if (start_timestamp > stop_timestamp)
    Rcpp::stop("stop_timestamp must be equal to or larger than start_timestamp");

  bool doFilterTrail = false;
  if (first_trail != 0| last_trail != 0) {
    doFilterTrail = true;
  }

  bool doFilterTimestamp = false;
  if (start_timestamp != 0| stop_timestamp != 0) {
    doFilterTimestamp = true;
  }

  bool doFilterType = false;
  if (filter.compare("")) {
    doFilterType = true;
  }

  uint64_t tot_cookies = tdb->GetNumberOfCookies();

  std::vector<std::string> vDimNames = tdb->GetDimNames();

  //create list per events
  //append to list of events

  std::vector<std::vector<std::string> > colVector(vDimNames.size());
  std::vector<std::string> vcookies;
  std::vector<std::uint32_t> vts;


  for (uint64_t num = 0; num < tot_cookies; ++num) {
    EventListPtr eventList = tdb->LoadEvents(num);

    if (sample && R::runif(0,1) >= fraction) continue;
    if (doFilterTrail && num < first_trail) continue;
    if (doFilterTrail && num> last_trail) break;

    EventList::EventIterator evtIter;

    //Number of trails
    uint32_t evtSize = eventList->GetSize();

    List rowList(evtSize);

    for (evtIter = eventList->EventsBegin();
        evtIter != eventList->EventsEnd();
        ++evtIter) {

      uint32_t ts = evtIter->GetTimestamp();

      if (doFilterTimestamp && 
          (ts < start_timestamp || ts > stop_timestamp )) continue;

      if (doFilterType && (filter.compare(evtIter->GetEventType())) ) continue;

      vcookies.push_back(tdb->GetHexCookieByInd(num));
      vts.push_back(ts);

      int nd = 0; 
      for(std::vector<std::string>::iterator it = vDimNames.begin();
          it != vDimNames.end();
        ++it ) {

        colVector[nd].push_back(evtIter->GetTrail(*it));
        ++nd;
      }
    }
  }

  std::vector<std::string> col_names(2 + vDimNames.size());
  List finalList =  List(1 + 1 + vDimNames.size());
  finalList[0] = vcookies; col_names[0] = "uuid";
  finalList[1] = vts; col_names[1] = "timestamp";
  for (int k = 0; k < vDimNames.size(); ++k) {
    finalList[k+2] = colVector[k];
    col_names[k+2] = vDimNames[k];
  }

  //odd hack to convert list into a dataframe
  // http://gallery.rcpp.org/articles/faster-data-frame-creation
  List returned_frame = clone(finalList);
  GenericVector sample_row = returned_frame(0);

  StringVector row_names(sample_row.length());
  for (int i = 0; i < sample_row.length(); ++i ) {
    char names[5];
    sprintf(&(names[0]),"%d",i);
    row_names(i) = names;
  }
  returned_frame.attr("row.names") = row_names;

  returned_frame.attr("names") = col_names;
  returned_frame.attr("class") = "data.frame";

  return returned_frame;
}


RCPP_MODULE(traildb) {

  class_<TrailDB>( "TrailDB" )

  .constructor<std::string>()

  .method("get_ncookies", &TrailDB::GetNumberOfCookies, "Get Number of cookies")

  .method("get_max_timestamp", &TrailDB::GetMaxTimestamp, "Get Maximum Timestamp in DB")

  .method("get_min_timestamp", &TrailDB::GetMinTimestamp, "Get Minimum Timestamp in DB")

  .method("get_ndimension", &TrailDB::GetNumberOfDimensions,"Get Number of Dimensions")

  .method("has_tdb_uuid_index", &TrailDB::HasCookieIndex, "tdb_uuid exists at index")

  .method("get_nevents", &TrailDB::GetNumberOfEventsByType,
      "Get number of events by event type for a particular cookie by index")

  .method("get_hext_tdb_uuid", &TrailDB::GetHexCookieByInd, "Get Hex value for tdb_uuid at index")

  .method("get_ts_vector", &get_ts_vector)

  .method("get_dim_names", &get_dimensions)

  .method("tdb_dataframe", &tdb_dataframe)

  ;
}
