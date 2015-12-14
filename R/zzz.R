.onUnload <- function (libpath) {
  library.dynam.unload("RTraildb", libpath)
}