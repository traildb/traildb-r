TrailDB R bindings
========================

![TrailDB logo](traildb_logo_512.png?raw=true)

A minimal R binding for traildb C API [TrailDB](http://traildb.io/). The initial
goal for this project was to allow interactive querying of traildb data in a
more R-friendly manner (think dataframes!).

It has a dependency on the [traildb-cpp](https://github.com/traildb/traildb-cpp)
project.

How to build
------------

You need to install traildb and traildb-cpp (install the headers in a system-wide location). 



### Build from source

    >> library(devtools)
    >> devtools::install_github("traildb/traildb-r")


### Try it out. Use the minimal testfile   

    >> library(RTraildb)
    >> read_traildb('tdbtest.tdb')
