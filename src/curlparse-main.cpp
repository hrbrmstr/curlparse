#include <Rcpp.h>

#include <curl/curl.h>
#include <curl/urlapi.h>

using namespace Rcpp;

static inline String lc_url_get(CURLU *url, CURLUPart what, unsigned int flags = 0) {

  char *thing;
  CURLUcode rc = curl_url_get(url, what, &thing, flags);

  if (!rc) {
    String out(thing);
    curl_free(thing);
    return(out);
  } else {
    return(NA_STRING);
  }

}

//' Parse a character vector of URLs into component parts
//'
//' @md
//' @param urls character vector of URLs
//' @return data frame (tibble)
//' @export
// [[Rcpp::export]]
DataFrame parse_curl(StringVector urls) {

  R_xlen_t input_size = urls.size();

  StringVector scheme_vec(input_size);
  StringVector user_vec(input_size);
  StringVector password_vec(input_size);
  StringVector host_vec(input_size);
  StringVector port_vec(input_size);
  StringVector path_vec(input_size);
  StringVector options_vec(input_size);
  StringVector query_vec(input_size);
  StringVector fragment_vec(input_size);

  CURLUcode rc;
  CURLU *url;

  for (R_xlen_t i = 0; i < input_size; i++) {

    url = curl_url();
    rc = curl_url_set(url, CURLUPART_URL, urls[i], 0);

    if (!rc) {

      scheme_vec[i] = lc_url_get(url, CURLUPART_SCHEME, CURLU_DEFAULT_SCHEME);
      user_vec[i] = lc_url_get(url, CURLUPART_USER);
      password_vec[i] = lc_url_get(url, CURLUPART_PASSWORD);
      host_vec[i] = lc_url_get(url, CURLUPART_HOST);
      port_vec[i] = lc_url_get(url, CURLUPART_PORT, CURLU_DEFAULT_PORT);
      path_vec[i] = lc_url_get(url, CURLUPART_PATH, CURLU_URLDECODE);
      options_vec[i] = lc_url_get(url, CURLUPART_OPTIONS);
      query_vec[i] = lc_url_get(url, CURLUPART_QUERY, CURLU_URLDECODE);
      fragment_vec[i] = lc_url_get(url, CURLUPART_FRAGMENT);

    } else {

      scheme_vec[i] = NA_STRING;
      user_vec[i] = NA_STRING;
      password_vec[i] = NA_STRING;
      host_vec[i] = NA_STRING;
      port_vec[i] = NA_STRING;
      path_vec[i] = NA_STRING;
      options_vec[i] = NA_STRING;
      query_vec[i] = NA_STRING;
      fragment_vec[i] = NA_STRING;

    }

    curl_url_cleanup(url);

  }

  DataFrame out = DataFrame::create(
    _["scheme"] = scheme_vec,
    _["user"] = user_vec,
    _["password"] = password_vec,
    _["host"] = host_vec,
    _["port"] = port_vec,
    _["path"] = path_vec,
    _["options"] = options_vec,
    _["query"] = query_vec,
    _["fragment"] = fragment_vec,
    _["stringsAsFactors"] = false
  );

  out.attr("class") = StringVector::create("tbl_df", "tbl", "data.frame");

  return(out);

}


//' Parse a character vector of URLs into component parts (`urltools` compatibility function)
//'
//' @md
//' @param urls character vector of URLs
//' @return data frame (not a tibble)
//' @export
// [[Rcpp::export]]
DataFrame url_parse(StringVector urls) {

  R_xlen_t input_size = urls.size();

  StringVector scheme_vec(input_size);
  StringVector host_vec(input_size);
  StringVector port_vec(input_size);
  StringVector path_vec(input_size);
  StringVector query_vec(input_size);
  StringVector fragment_vec(input_size);

  CURLUcode rc;
  CURLU *url;

  for (R_xlen_t i = 0; i < input_size; i++) {

    url = curl_url();
    rc = curl_url_set(url, CURLUPART_URL, urls[i], 0);

    if (!rc) {

      scheme_vec[i] = lc_url_get(url, CURLUPART_SCHEME, CURLU_DEFAULT_SCHEME);
      host_vec[i] = lc_url_get(url, CURLUPART_HOST);
      port_vec[i] = lc_url_get(url, CURLUPART_PORT, CURLU_DEFAULT_PORT);
      path_vec[i] = lc_url_get(url, CURLUPART_PATH, CURLU_URLDECODE);
      query_vec[i] = lc_url_get(url, CURLUPART_QUERY, CURLU_URLDECODE);
      fragment_vec[i] = lc_url_get(url, CURLUPART_FRAGMENT);

    } else {

      scheme_vec[i] = NA_STRING;
      host_vec[i] = NA_STRING;
      port_vec[i] = NA_STRING;
      path_vec[i] = NA_STRING;
      query_vec[i] = NA_STRING;
      fragment_vec[i] = NA_STRING;

    }

    curl_url_cleanup(url);

  }

  DataFrame out = DataFrame::create(
    _["scheme"] = scheme_vec,
    _["domain"] = host_vec,
    _["port"] = port_vec,
    _["path"] = path_vec,
    _["query"] = query_vec,
    _["fragment"] = fragment_vec,
    _["stringsAsFactors"] = false
  );

  return(out);

}


StringVector lc_part(StringVector urls, CURLUPart what, unsigned int flags = 0) {

  R_xlen_t input_size = urls.size();
  StringVector output(input_size);

  CURLUcode rc;
  CURLU *url;

  for (R_xlen_t i = 0; i < input_size; i++) {

    url = curl_url();
    rc = curl_url_set(url, CURLUPART_URL, urls[i], 0);

    output[i] = (!rc) ? lc_url_get(url, what, flags) : NA_STRING;

    curl_url_cleanup(url);

  }

  return(output);

}

//' Extract member components from a URL string
//'
//' @md
//' @param urls character vector of URLs
//' @return character vector of the extracted URL component
//' @export
// [[Rcpp::export]]
StringVector scheme(StringVector urls) {
  return(lc_part(urls, CURLUPART_SCHEME, CURLU_DEFAULT_SCHEME));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
StringVector user(StringVector urls) {
  return(lc_part(urls, CURLUPART_USER));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
StringVector password(StringVector urls) {
  return(lc_part(urls, CURLUPART_PASSWORD));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
StringVector host(StringVector urls) {
  return(lc_part(urls, CURLUPART_HOST));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
StringVector port(StringVector urls) {
  return(lc_part(urls, CURLUPART_PORT, CURLU_DEFAULT_PORT));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
StringVector path(StringVector urls) {
  return(lc_part(urls, CURLUPART_PATH, CURLU_URLDECODE));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
StringVector url_options(StringVector urls) {
  return(lc_part(urls, CURLUPART_OPTIONS));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
StringVector query(StringVector urls) {
  return(lc_part(urls, CURLUPART_QUERY, CURLU_URLENCODE));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
StringVector fragment(StringVector urls) {
  return(lc_part(urls, CURLUPART_FRAGMENT));
}
