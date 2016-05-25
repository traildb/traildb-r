#'@useDynLib RTraildb
#'@importClassesFrom Rcpp "C++Object"
NULL

#' Provide a summary of the traildb object
#'
#' \code{describe_traildb} returns a summary of the traildb object
#'
#' @param file Path to the traildb object (tarball will be automatically unpacked)
#' @return Summary of the traildb object
#' @export
describe_traildb <- function(file) {
  file <- standardize_path(file)
  spec <- list(name=file)
  out <- read_trails_meta(spec)

  printf("")
  printf(" - Path: %s",file)
  printf(' # uuids:                     %12d',out$number_of_uuids)
  printf(' # events:                    %12d',out$number_of_events)
  printf(' # fields:                    %12d',(out$number_of_dimensions))
  for(i in 1:length(out$dimensions)) {
    printf(' # -%-25s %12d',out$dimensions[i],out$dimensions_uniques[i]+1)
  }
  printf(' \u2265 time:               %s',format(as.POSIXct(out$min_timestamp,origin='1970-01-01'),'%Y-%m-%d %H:%M:%S'))
  printf(' \u2264 time:               %s',format(as.POSIXct(out$max_timestamp,origin='1970-01-01'), '%Y-%m-%d %H:%M:%S'))
}

#' get_dim_traildb returns the list of dimensions
#'
#' \code{get_dim_traildb} returns a vector of the dimensions
#'
#' @param file Path to the traildb object.
#' @return Vector of dimensions
#' @export
get_dim_traildb <- function(file) {
  file <- standardize_path(file)
  spec <- list(name=file)
  out <- read_trails_meta(spec)

  return(c(out$dimensions,'uuid','timestamp'))
}
