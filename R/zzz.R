#'@useDynLib RTraildb
#'@importClassesFrom Rcpp "C++Object"
#'@import digest
.onLoad <- function(libname, pkgname) { }

.onUnload <- function (libpath) {
  gc() # Force garbage collection of existing traildb files
  library.dynam.unload("RTraildb", libpath)
}
