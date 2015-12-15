
#include <Rcpp.h>
#include <stdint.h>
#include <functional>
#include "TrailDB.h"

using namespace Rcpp;

CharacterVector get_dimensions(TrailDB* tdb) {
  return wrap(tdb->GetDimNames());
}

NumericVector get_ts_vector(TrailDB* tdb, uint64_t tdb_uuid) {
  return wrap(tdb->GetTimestampVector(tdb_uuid));
}

//Things to get frmo events
//GetTimestamp
//


//Unpack the entire traildb into a dataframe
void tdb_dataframe(TrailDB* tdb) {
  uint64_t tot_cookies = tdb->GetNumbeOfCookies();

  std::vector<std::string> vDimNames = tdb->GetDimNames();

  for (uint64_t num = 0; num < tot_cookies; ++num) {
    EventListPtr eventList = tdb->LoadEvents(num);

    EventList::EventIterator evtIter;

    //Number of trails
    uint32_t evtSize = eventList.GetSize();

    for (evtIter = eventList->EventsBegin();
        evtIter != eventList->EventEnd();
        ++evtIter) {
      for(std::vector<std::string> it = vDimNames.begin();
          it != vDimNames.end();
        ++it ) {

        Rcout << *it<< " " <<evtIter->GetTrail(*it)<<std::endl;

      }

    }


  }

}


//  .method("get_nevents", &TrailDB::GetNumberOfEvents,"Get number of events by event type for a particular cookie by index")

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

  ;
}
