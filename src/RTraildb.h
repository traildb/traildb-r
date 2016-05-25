#ifndef RTRAILDB_H_INCLUDED
#define RTRAILDB_H_INCLUDED

#include <Rcpp.h>
using namespace Rcpp;

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

#include "TrailDBcpp.h"
#include <PointerTypedefs.h>
#include <ByteManipulations.h>

class TrailDB;
SHARED_POINTER_TYPEDEFS(TrailDB);

class RTrailDB {

public:
  RTrailDB(std::string dbpath);
  virtual ~RTrailDB();
  static TrailDBPtr Create(Rcpp::List spec);
  std::string dbPath_;

};


TrailDBPtr RTrailDB::Create(Rcpp::List spec) {
  std::string filename = as<std::string>(spec["name"]);
  return TrailDBPtr(new TrailDB(filename));
}



#endif  //RTRAILDB_H_INCLUDED



