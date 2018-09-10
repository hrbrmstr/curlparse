
# curlparse

Parse ‘URLs’ with ‘libcurl’

## Description

As of version 7.62.0 ‘libcurl’ has exposed its ‘URL’ parser. Tools are
provided to parse ‘URLs’ using this new parser feature.

\*\*UNTIL `curl`/`libcurl` general release at the end of October you
*must* use the development version which can be cloned and built from
<https://github.com/curl/curl>.

## What’s Inside The Tin

Core function to turn a vector of URLs into a named `list` of component
parts (which can easily be turned into a data frame)

  - `parse_curl`: Parse a character vector of URLs into component parts
    (deliberately named soas not to conflict with `httr::parse_url()`)

URL validation:

  - `is_valid_url`: Test if a URL is valid (Ref:
    <https://mathiasbynens.be/demo/url-regex>)

URL component extractors:

  - `scheme`: Extract member components from a URL string
  - `user`: Extract member components from a URL string
  - `password`: Extract member components from a URL string
  - `host`/`domain`: Extract member components from a URL string
  - `port`: Extract member components from a URL string
  - `path`: Extract member components from a URL string
  - `query`: Extract member components from a URL string
  - `url_options`: Extract member components from a URL string
    (deliberately named soas not to conflict with `base::options()`)
  - `fragment`: Extract member components from a URL string

## Installation

``` r
devtools::install_github("hrbrmstr/curlparse")
```

## Usage

``` r
library(curlparse)

# current verison
packageVersion("curlparse")
## [1] '0.1.0'
```

### Process Some URLs

``` r
library(rvest)
library(curlparse)
library(tidyverse)
```

``` r
read_html("https://www.r-bloggers.com/blogs-list/") %>% 
  html_nodes(xpath=".//li[contains(., 'Contributing Blogs')]/ul/li/a[contains(@href, 'http')]") %>% 
  html_attr("href") -> blog_urls
```

``` r
(parsed <- parse_curl(blog_urls))
## # A tibble: 794 x 9
##    scheme user  password host                    port  path            options query fragment
##    <chr>  <chr> <chr>    <chr>                   <chr> <chr>           <chr>   <chr> <chr>   
##  1 http   <NA>  <NA>     dmlc.ml                 80    /               <NA>    <NA>  <NA>    
##  2 https  <NA>  <NA>     lionel-.github.io       443   /               <NA>    <NA>  <NA>    
##  3 https  <NA>  <NA>     jean9208.github.io      443   /rss-R.xml      <NA>    <NA>  <NA>    
##  4 https  <NA>  <NA>     ryouready.wordpress.com 443   /               <NA>    <NA>  <NA>    
##  5 https  <NA>  <NA>     rveryday.wordpress.com  443   /               <NA>    <NA>  <NA>    
##  6 http   <NA>  <NA>     www.talyarkoni.org      80    /blog           <NA>    <NA>  <NA>    
##  7 https  <NA>  <NA>     rtricks.wordpress.com   443   /               <NA>    <NA>  <NA>    
##  8 http   <NA>  <NA>     mgritts.com             80    /feed.r.xml     <NA>    <NA>  <NA>    
##  9 http   <NA>  <NA>     blog.ambodi.com         80    /               <NA>    <NA>  <NA>    
## 10 https  <NA>  <NA>     xcafebabe.blogspot.com  443   /search/label/R <NA>    <NA>  <NA>    
## # ... with 784 more rows

count(parsed, scheme, sort=TRUE)
## # A tibble: 2 x 2
##   scheme     n
##   <chr>  <int>
## 1 https    467
## 2 http     327

filter(parsed, !is.na(query))
## # A tibble: 2 x 9
##   scheme user  password host             port  path                  options query                             fragment
##   <chr>  <chr> <chr>    <chr>            <chr> <chr>                 <chr>   <chr>                             <chr>   
## 1 https  <NA>  <NA>     blog.datazar.com 443   /tagged/r-language    <NA>    source=rss----e2c7e6e1c75--r_lan… <NA>    
## 2 https  <NA>  <NA>     kevinkuang.net   443   /tagged/r-programming <NA>    source=rss----a1ff9aea4bf1--r_pr… <NA>
```

### Stress Test

``` r
c(
  "", "foo", "foo;params?query#fragment", "http://foo.com/path", "http://foo.com",
  "//foo.com/path", "//user:pass@foo.com/", "http://user:pass@foo.com/", 
  "file:///tmp/junk.txt", "imap://mail.python.org/mbox1",
  "mms://wms.sys.hinet.net/cts/Drama/09006251100.asf", "nfs://server/path/to/file.txt",
  "svn+ssh://svn.zope.org/repos/main/ZConfig/trunk/",
  "git+ssh://git@github.com/user/project.git", "HTTP://WWW.PYTHON.ORG/doc/#frag",
  "http://www.python.org:080/", "http://www.python.org:/", "javascript:console.log('hello')",
  "javascript:console.log('hello');console.log('world')", "http://example.com/?", 
  "http://example.com/;", "tel:0108202201", "unknown:0108202201",
  "http://user@example.com:8080/path;param?query#fragment", 
  "http://www.python.org:65536/", "http://www.python.org:-20/",
  "http://www.python.org:8589934592/", "http://www.python.org:80hello/", 
  "http://:::cnn.com/", "http://./", "http://foo..com/", "http://foo../"
) -> ugly_urls

(u_parsed <- parse_curl(ugly_urls))
## # A tibble: 32 x 9
##    scheme user  password host            port  path          options query fragment
##    <chr>  <chr> <chr>    <chr>           <chr> <chr>         <chr>   <chr> <chr>   
##  1 <NA>   <NA>  <NA>     <NA>            <NA>  <NA>          <NA>    <NA>  <NA>    
##  2 <NA>   <NA>  <NA>     <NA>            <NA>  <NA>          <NA>    <NA>  <NA>    
##  3 <NA>   <NA>  <NA>     <NA>            <NA>  <NA>          <NA>    <NA>  <NA>    
##  4 http   <NA>  <NA>     foo.com         80    /path         <NA>    <NA>  <NA>    
##  5 http   <NA>  <NA>     foo.com         80    /             <NA>    <NA>  <NA>    
##  6 <NA>   <NA>  <NA>     <NA>            <NA>  <NA>          <NA>    <NA>  <NA>    
##  7 <NA>   <NA>  <NA>     <NA>            <NA>  <NA>          <NA>    <NA>  <NA>    
##  8 http   user  pass     foo.com         80    /             <NA>    <NA>  <NA>    
##  9 file   <NA>  <NA>     <NA>            0     /tmp/junk.txt <NA>    <NA>  <NA>    
## 10 imap   <NA>  <NA>     mail.python.org 143   /mbox1        <NA>    <NA>  <NA>    
## # ... with 22 more rows

filter(u_parsed, !is.na(scheme))
## # A tibble: 13 x 9
##    scheme user  password host            port  path          options query fragment
##    <chr>  <chr> <chr>    <chr>           <chr> <chr>         <chr>   <chr> <chr>   
##  1 http   <NA>  <NA>     foo.com         80    /path         <NA>    <NA>  <NA>    
##  2 http   <NA>  <NA>     foo.com         80    /             <NA>    <NA>  <NA>    
##  3 http   user  pass     foo.com         80    /             <NA>    <NA>  <NA>    
##  4 file   <NA>  <NA>     <NA>            0     /tmp/junk.txt <NA>    <NA>  <NA>    
##  5 imap   <NA>  <NA>     mail.python.org 143   /mbox1        <NA>    <NA>  <NA>    
##  6 http   <NA>  <NA>     WWW.PYTHON.ORG  80    /doc/         <NA>    <NA>  frag    
##  7 http   <NA>  <NA>     www.python.org  80    /             <NA>    <NA>  <NA>    
##  8 http   <NA>  <NA>     example.com     80    /             <NA>    <NA>  <NA>    
##  9 http   <NA>  <NA>     example.com     80    /;            <NA>    <NA>  <NA>    
## 10 http   user  <NA>     example.com     8080  /path;param   <NA>    query fragment
## 11 http   <NA>  <NA>     .               80    /             <NA>    <NA>  <NA>    
## 12 http   <NA>  <NA>     foo..com        80    /             <NA>    <NA>  <NA>    
## 13 http   <NA>  <NA>     foo..           80    /             <NA>    <NA>  <NA>

filter(u_parsed, !is.na(user))
## # A tibble: 2 x 9
##   scheme user  password host        port  path        options query fragment
##   <chr>  <chr> <chr>    <chr>       <chr> <chr>       <chr>   <chr> <chr>   
## 1 http   user  pass     foo.com     80    /           <NA>    <NA>  <NA>    
## 2 http   user  <NA>     example.com 8080  /path;param <NA>    query fragment

filter(u_parsed, !is.na(password))
## # A tibble: 1 x 9
##   scheme user  password host    port  path  options query fragment
##   <chr>  <chr> <chr>    <chr>   <chr> <chr> <chr>   <chr> <chr>   
## 1 http   user  pass     foo.com 80    /     <NA>    <NA>  <NA>

filter(u_parsed, !is.na(host))
## # A tibble: 12 x 9
##    scheme user  password host            port  path        options query fragment
##    <chr>  <chr> <chr>    <chr>           <chr> <chr>       <chr>   <chr> <chr>   
##  1 http   <NA>  <NA>     foo.com         80    /path       <NA>    <NA>  <NA>    
##  2 http   <NA>  <NA>     foo.com         80    /           <NA>    <NA>  <NA>    
##  3 http   user  pass     foo.com         80    /           <NA>    <NA>  <NA>    
##  4 imap   <NA>  <NA>     mail.python.org 143   /mbox1      <NA>    <NA>  <NA>    
##  5 http   <NA>  <NA>     WWW.PYTHON.ORG  80    /doc/       <NA>    <NA>  frag    
##  6 http   <NA>  <NA>     www.python.org  80    /           <NA>    <NA>  <NA>    
##  7 http   <NA>  <NA>     example.com     80    /           <NA>    <NA>  <NA>    
##  8 http   <NA>  <NA>     example.com     80    /;          <NA>    <NA>  <NA>    
##  9 http   user  <NA>     example.com     8080  /path;param <NA>    query fragment
## 10 http   <NA>  <NA>     .               80    /           <NA>    <NA>  <NA>    
## 11 http   <NA>  <NA>     foo..com        80    /           <NA>    <NA>  <NA>    
## 12 http   <NA>  <NA>     foo..           80    /           <NA>    <NA>  <NA>

filter(u_parsed, !is.na(path))
## # A tibble: 13 x 9
##    scheme user  password host            port  path          options query fragment
##    <chr>  <chr> <chr>    <chr>           <chr> <chr>         <chr>   <chr> <chr>   
##  1 http   <NA>  <NA>     foo.com         80    /path         <NA>    <NA>  <NA>    
##  2 http   <NA>  <NA>     foo.com         80    /             <NA>    <NA>  <NA>    
##  3 http   user  pass     foo.com         80    /             <NA>    <NA>  <NA>    
##  4 file   <NA>  <NA>     <NA>            0     /tmp/junk.txt <NA>    <NA>  <NA>    
##  5 imap   <NA>  <NA>     mail.python.org 143   /mbox1        <NA>    <NA>  <NA>    
##  6 http   <NA>  <NA>     WWW.PYTHON.ORG  80    /doc/         <NA>    <NA>  frag    
##  7 http   <NA>  <NA>     www.python.org  80    /             <NA>    <NA>  <NA>    
##  8 http   <NA>  <NA>     example.com     80    /             <NA>    <NA>  <NA>    
##  9 http   <NA>  <NA>     example.com     80    /;            <NA>    <NA>  <NA>    
## 10 http   user  <NA>     example.com     8080  /path;param   <NA>    query fragment
## 11 http   <NA>  <NA>     .               80    /             <NA>    <NA>  <NA>    
## 12 http   <NA>  <NA>     foo..com        80    /             <NA>    <NA>  <NA>    
## 13 http   <NA>  <NA>     foo..           80    /             <NA>    <NA>  <NA>

filter(u_parsed, !is.na(query))
## # A tibble: 1 x 9
##   scheme user  password host        port  path        options query fragment
##   <chr>  <chr> <chr>    <chr>       <chr> <chr>       <chr>   <chr> <chr>   
## 1 http   user  <NA>     example.com 8080  /path;param <NA>    query fragment

filter(u_parsed, !is.na(fragment))
## # A tibble: 2 x 9
##   scheme user  password host           port  path        options query fragment
##   <chr>  <chr> <chr>    <chr>          <chr> <chr>       <chr>   <chr> <chr>   
## 1 http   <NA>  <NA>     WWW.PYTHON.ORG 80    /doc/       <NA>    <NA>  frag    
## 2 http   user  <NA>     example.com    8080  /path;param <NA>    query fragment
```

Make sure the vector extractors work the same as the data frame
converter:

``` r
all(
  c(
    identical(u_parsed$scheme, scheme(ugly_urls)),
    identical(u_parsed$user, user(ugly_urls)),
    identical(u_parsed$password, password(ugly_urls)),
    identical(u_parsed$host, host(ugly_urls)),
    identical(u_parsed$path, path(ugly_urls)),
    identical(u_parsed$query, query(ugly_urls)),
    identical(u_parsed$fragment, fragment(ugly_urls))
  )
)
## [1] TRUE
```
