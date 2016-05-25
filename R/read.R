#'@useDynLib RTraildb
#'@importClassesFrom Rcpp "C++Object"
NULL

#' Read a traildb file into a data frame
#'
#' \code{read_traildb} creates a dataframe from the traildb object
#'
#' @param file Path to the traildb object. If a tarball is provided, it will be unpacked
#' first
#' @param first The starting trail number. The default first is 0 implying it starts
#' from beginning of the trails but you can use any positive integer that is less than
#' the total number of trails in the traildb object. Use \code{describe_traildb} to
#' query the number of trails.
#' @param last The last trail number. The default is 100 implying it will only load 100
#' trails by default. You can make it any positive integer larger than first and less than
#' the total number of trails in the traildb object.
#' @param sample Uniformly sample the trails? By default, there are no sampling but you can
#' enable sampling by setting sample = \code{TRUE}.
#' @param p Sampling ratio. In case, \code{sample} is set to \code{TRUE}, you can specify
#' the fraction of trails that goes into the output data frame
#' @param startTime Filter all trails before startTime. It accepts unix a POSIXct object
#' By default, no trails are filtered by timestamp
#' @param stopTime Filter all trails after (including) stopTimestamp. It acceps POSIXct
#' object. By default, no trails are filtered by timestamp.
#' @param filter Only keep trails matching filter type. Default is none
#' @param verbose Produce debugging logs to the screen
#' @return A data frame.
#' @export
#' @examples
#' # Read from a path
#' read_traildb('/mnt/emails')
#'
#' # Or apply type filters
#' read_traildb('/mnt/emails', filter='personal')
#'
#' # Or apply filters on timestamp
#' read_traildb('/mnt/emails',
#'  startTime=as.POSIXct('2015-10-12',origin='1970-01-01'),
#'  stopTime=as.POSIXct('2015-10-22',origin='1970-01-01'))
read_traildb <- function(file,
                         first = 0, last = 0,
                         sample = FALSE, p = 1.0,
                         startTime=NA, stopTime=NA) {

  if(sample & abs(p) > 1) {
    stop('Sample fraction must be between 0 and 1', call.=FALSE)
  }

  startTime <- standardize_ts(startTime)
  stopTime <- standardize_ts(stopTime)

  file <- standardize_path(file)
  spec <- list(name=file)

  out <- read_trails(spec,
                     first_trail=first,
                     last_trail=last,
                     sample=sample,
                     frac=p,
                     start_timestamp=startTime, stop_timestamp=stopTime)
  out
}
