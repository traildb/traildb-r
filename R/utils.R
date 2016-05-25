#'@useDynLib RTraildb
#'@importClassesFrom Rcpp "C++Object"
#'@import digest
printf <- function(...) invisible(cat(sprintf(...),"\n",sep=""))

standardize_ts <- function(ts) {
  if(!is.na(ts)) {
    if(!inherits(ts, "POSIXct")) {
      stop("time must be POSIXct or NA", call. = FALSE)
    }
    else{return(as.numeric(ts))}
  }
  else{return(0)}
}

#https://github.com/hadley/readr/blob/master/R/source.R
standardize_path <- function(path) {
  path <- check_path(path)
  switch(tools::file_ext(path),
         tar = tarfile(path),
         path
  )
}

check_path <- function(path) {
  if (file.exists(path))
    return(normalizePath(path, "/", mustWork = FALSE))

  stop("'", path, "' does not exist",
       if (!is_absolute_path(path))
         paste0(" in current working directory ('", getwd(), "')"),
       ".",
       call. = FALSE
  )
}

is_absolute_path <- function(path) {
  grepl("^(/|[A-Za-z]:|\\\\|~)", path)
}

tarfile <- function(path) {
  message("untar ", path)
  files <- utils::untar(path, list=TRUE)
  return(tools::file_path_sans_ext(path)) #likely a brittle implementation
}
