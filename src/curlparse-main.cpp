#include <Rcpp.h>

#include <curl/curl.h>

using namespace Rcpp;

String lc_url_get(CURLU *url, CURLUPart what, unsigned int flags = 0) {

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
DataFrame parse_curl(CharacterVector urls) {

  unsigned int input_size = urls.size();

  CharacterVector scheme_vec(input_size);
  CharacterVector user_vec(input_size);
  CharacterVector password_vec(input_size);
  CharacterVector host_vec(input_size);
  CharacterVector port_vec(input_size);
  CharacterVector path_vec(input_size);
  CharacterVector options_vec(input_size);
  CharacterVector query_vec(input_size);
  CharacterVector fragment_vec(input_size);

  CURLUcode rc;
  CURLU *url;

  for (unsigned int i = 0; i < input_size; i++) {

    url = curl_url();
    rc = curl_url_set(
      url, CURLUPART_URL, Rcpp::as<std::string>(urls[i]).c_str(), 0
    );

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

  out.attr("class") = CharacterVector::create("tbl_df", "tbl", "data.frame");

  return(out);

}

CharacterVector lc_part(CharacterVector urls, CURLUPart what, unsigned int flags = 0) {

  unsigned int input_size = urls.size();
  CharacterVector output(input_size);

  CURLUcode rc;
  CURLU *url;

  for (unsigned int i = 0; i < input_size; i++) {

    url = curl_url();
    rc = curl_url_set(
      url, CURLUPART_URL, Rcpp::as<std::string>(urls[i]).c_str(), 0
    );

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
CharacterVector scheme(CharacterVector urls) {
  return(lc_part(urls, CURLUPART_SCHEME, CURLU_DEFAULT_SCHEME));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
CharacterVector user(CharacterVector urls) {
  return(lc_part(urls, CURLUPART_USER));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
CharacterVector password(CharacterVector urls) {
  return(lc_part(urls, CURLUPART_PASSWORD));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
CharacterVector host(CharacterVector urls) {
  return(lc_part(urls, CURLUPART_HOST));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
CharacterVector port(CharacterVector urls) {
  return(lc_part(urls, CURLUPART_PORT, CURLU_DEFAULT_PORT));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
CharacterVector path(CharacterVector urls) {
  return(lc_part(urls, CURLUPART_PATH, CURLU_URLDECODE));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
CharacterVector url_options(CharacterVector urls) {
  return(lc_part(urls, CURLUPART_OPTIONS));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
CharacterVector query(CharacterVector urls) {
  return(lc_part(urls, CURLUPART_QUERY, CURLU_URLENCODE));
}

//' @rdname scheme
//' @export
// [[Rcpp::export]]
CharacterVector fragment(CharacterVector urls) {
  return(lc_part(urls, CURLUPART_FRAGMENT));
}
