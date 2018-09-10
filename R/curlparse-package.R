#' Parse 'URLs' with 'libcurl'
#'
#' As of version 7.62.0 'libcurl' has exposed its 'URL' parser. Tools are
#' provided to parse 'URLs' using this new parser feature.
#'
#' **UNTIL `curl`/`libcurl` general release at the end of October you _must_
#' use the development version which can be cloned and built from
#' <https://github.com/curl/curl>.
#'
#' - URL: <https://gitlab.com/hrbrmstr/curlparse>
#' - BugReports: <https://gitlab.com/hrbrmstr/curlparse/issues>
#'
#' @md
#' @name curlparse
#' @docType package
#' @author Bob Rudis (bob@@rud.is)
#' @importFrom stringi stri_detect_regex stri_opts_regex
#' @importFrom Rcpp sourceCpp
#' @useDynLib curlparse
NULL