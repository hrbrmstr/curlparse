#' Parse 'URLs' with 'libcurl'
#'
#' As of version 7.62.0 'libcurl' has exposed its 'URL' parser. Tools are
#' provided to parse 'URLs' using this new parser feature.
#'
#' @md
#' @name curlparse
#' @docType package
#' @author Bob Rudis (bob@@rud.is)
#' @importFrom stringi stri_detect_regex stri_opts_regex
#' @importFrom Rcpp sourceCpp
#' @useDynLib curlparse, .registration = TRUE
"_PACKAGE"
