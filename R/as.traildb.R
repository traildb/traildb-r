#' Create traildb objects
#'
#' @export
#' @param X An object to be converted to a traildb object.
as.traildb <- function(X, ...) {
  Xname <- deparse(substitute(X))
  if (is.traildb(X)) {
    message(paste0(Xname, " is a valid traildb object"))
    return(X)
  } else {
  }
}

#' @rdname as.shinystan
is.traildb <- function(object) inherits(object, "traildb")