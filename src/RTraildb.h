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
#include <memory>

#include <TrailDBcpp.h>
#include <ByteManipulations.h>

class TrailDB;

class RTrailDB {

public:
  RTrailDB(std::string dbpath);
  virtual ~RTrailDB();
  static std::shared_ptr<TrailDB> Create(Rcpp::List spec);
  std::string dbPath_;

};


std::shared_ptr<TrailDB> RTrailDB::Create(Rcpp::List spec) {
  std::string filename = as<std::string>(spec["name"]);
  return std::make_shared<TrailDB>(filename);
}



#endif  //RTRAILDB_H_INCLUDED



