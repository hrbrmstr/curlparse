
[![Project Status: Active – The project has reached a stable, usable
state and is being actively
developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)
[![Signed
by](https://img.shields.io/badge/Keybase-Verified-brightgreen.svg)](https://keybase.io/hrbrmstr)
![Signed commit
%](https://img.shields.io/badge/Signed_Commits-100%25-lightgrey.svg)
[![Linux build
Status](https://travis-ci.org/hrbrmstr/curlparse.svg?branch=master)](https://travis-ci.org/hrbrmstr/curlparse)
[![Coverage
Status](https://codecov.io/gh/hrbrmstr/curlparse/branch/master/graph/badge.svg)](https://codecov.io/gh/hrbrmstr/curlparse)
![Minimal R
Version](https://img.shields.io/badge/R%3E%3D-3.2.0-blue.svg)
![License](https://img.shields.io/badge/License-MIT-blue.svg)

# curlparse

Parse ‘URLs’ with ‘libcurl’

## Description

Tools are provided to parse URLs using the modern ‘libcurl’ built-in
parser.

## What’s Inside The Tin

The following functions are implemented:

  - `is_valid_url`: Test if a URL is a valid URL
  - `parse_curl`: Parse a character vector of URLs into component parts
  - `scheme`: Extract member components from a URL string
  - `url_parse`: Parse a character vector of URLs into component parts
    (urltools compatibility function)

## Installation

``` r
remotes::install_git("https://git.sr.ht/~hrbrmstr/curlparse")
# or
remotes::install_gitlab("hrbrmstr/curlparse")
# or
remotes::install_github("hrbrmstr/curlparse")
```

NOTE: To use the ‘remotes’ install options you will need to have the
[{remotes} package](https://github.com/r-lib/remotes) installed.

## Usage

``` r
library(curlparse)

# current verison
packageVersion("curlparse")
## [1] '0.2.0'
```

### Process Some URLs

``` r
library(urltools)
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
## # A tibble: 977 x 9
##    scheme user  password host                    port  path            options query fragment
##    <chr>  <chr> <chr>    <chr>                   <chr> <chr>           <chr>   <chr> <chr>   
##  1 http   <NA>  <NA>     reichlab.io             80    /               <NA>    <NA>  <NA>    
##  2 http   <NA>  <NA>     dmlc.ml                 80    /               <NA>    <NA>  <NA>    
##  3 https  <NA>  <NA>     lionel-.github.io       443   /               <NA>    <NA>  <NA>    
##  4 https  <NA>  <NA>     jean9208.github.io      443   /rss-R.xml      <NA>    <NA>  <NA>    
##  5 https  <NA>  <NA>     ryouready.wordpress.com 443   /               <NA>    <NA>  <NA>    
##  6 https  <NA>  <NA>     rveryday.wordpress.com  443   /               <NA>    <NA>  <NA>    
##  7 http   <NA>  <NA>     www.talyarkoni.org      80    /blog           <NA>    <NA>  <NA>    
##  8 https  <NA>  <NA>     rtricks.wordpress.com   443   /               <NA>    <NA>  <NA>    
##  9 https  <NA>  <NA>     xcafebabe.blogspot.com  443   /search/label/R <NA>    <NA>  <NA>    
## 10 http   <NA>  <NA>     4dpiecharts.com         80    /               <NA>    <NA>  <NA>    
## # … with 967 more rows

count(parsed, scheme, sort=TRUE)
## # A tibble: 2 x 2
##   scheme     n
##   <chr>  <int>
## 1 https    618
## 2 http     359

filter(parsed, !is.na(query))
## # A tibble: 7 x 9
##   scheme user  password host                   port  path               options query                           fragment
##   <chr>  <chr> <chr>    <chr>                  <chr> <chr>              <chr>   <chr>                           <chr>   
## 1 http   <NA>  <NA>     freakonometrics.blog.… 80    /index.php         <NA>    ""                              <NA>    
## 2 http   <NA>  <NA>     ludvigolsen.dk         80    /                  <NA>    lang=en                         <NA>    
## 3 https  <NA>  <NA>     medium.com             443   /principles-0/tag… <NA>    source=rss----489c2dec8959--r   <NA>    
## 4 https  <NA>  <NA>     medium.com             443   /tim-black/tagged… <NA>    source=rss----d71cf9ecf7ec--r   <NA>    
## 5 https  <NA>  <NA>     kevinkuang.net         443   /tagged/r-program… <NA>    source=rss----a1ff9aea4bf1--r_… <NA>    
## 6 https  <NA>  <NA>     medium.com             443   /@MattOldach_65321 <NA>    source=rss-459e62b88a2a------2  <NA>    
## 7 https  <NA>  <NA>     medium.com             443   /@zappingseb       <NA>    source=rss-dbc9f652035a------2  <NA>
```

### Benchmark

`curlparse` includes a `url_parse()` function to make it easier to use
this package for current users of `urltools::url_parse()` since it
provides the same API and same results back (including it being a
regular data frame and not a `tbl`).

Spoiler alert: `urltools::url_parse()` is faster by \~100µs (per-100
URLs) for “good” URLs (if there’s a mix of gnarly/bad URLs and valid
ones they get closer to being on-par). The aim was not to try to beat
it, though.

> Per the [blog post introducing this new set of API
> calls](https://daniel.haxx.se/blog/2018/09/09/libcurl-gets-a-url-api/):
> 
> Applications that pass in URLs to libcurl would of course still very
> often need to parse URLs, create URLs or otherwise handle them, but
> libcurl has not been helping with that.
> 
> At the same time, the under-specification of URLs has led to a
> situation where there’s really no stable document anywhere describing
> how URLs are supposed to work and basically every implementer is left
> to handle the WHATWG URL spec, RFC 3986 and the world in between all
> by themselves. Understanding how their URL parsing libraries, libcurl,
> other tools and their favorite browsers differ is complicated.
> 
> By offering applications access to libcurl’s own URL parser, we hope
> to tighten a problematic vulnerable area for applications where the
> URL parser library would believe one thing and libcurl another. This
> could and has sometimes lead to security problems. (See for example
> Exploiting URL Parser in Trending Programming Languages\! by Orange
> Tsai)

So, using this library adds consistency with how `libcurl` sees and
handles URLs.

``` r
library(microbenchmark)

set.seed(0)
test_urls <- sample(blog_urls, 100) # pick 100 URLs at random

microbenchmark(
  curlparse = curlparse::url_parse(test_urls),
  urltools = urltools::url_parse(test_urls), # we loaded urltools before curlparse at the top so namespace loading wasn't a factor for the benchmarks
  times = 500
) -> mb

mb
## Unit: microseconds
##       expr     min       lq     mean   median       uq      max neval cld
##  curlparse 810.579 855.6420 906.5393 890.2775 929.2810 4404.267   500   b
##   urltools 667.676 711.7175 766.3760 735.4835 771.1215 4704.806   500  a

autoplot(mb)
```

<img src="man/figures/README-unnamed-chunk-4-1.png" width="672" />

The individual handlers are a bit more on-par but mostly still slower
(except for `fragment()`). Note that `urltools` has no equivalent
function to just extract query strings so that’s not in the test.

``` r
bind_rows(
  microbenchmark(curlparse = curlparse::scheme(blog_urls), urltools = urltools::scheme(blog_urls)) %>%
    mutate(test = "scheme"),
  microbenchmark(curlparse = curlparse::domain(blog_urls), urltools = urltools::domain(blog_urls)) %>%
    mutate(test = "domain"),
  microbenchmark(curlparse = curlparse::port(blog_urls), urltools = urltools::port(blog_urls)) %>%
    mutate(test = "port"),
  microbenchmark(curlparse = curlparse::path(blog_urls), urltools = urltools::path(blog_urls)) %>%
    mutate(test = "path"),
  microbenchmark(curlparse = curlparse::fragment(blog_urls), urltools = urltools::fragment(blog_urls)) %>%
    mutate(test = "fragment")
) %>% 
  mutate(test = factor(test, levels=c("scheme", "domain", "port", "path", "fragment"))) %>% 
  mutate(time = time / 1000000) %>% 
  ggplot(aes(expr, time)) +
  geom_violin(aes(fill=expr), show.legend = FALSE) +
  scale_y_continuous(name = "milliseconds", expand = c(0,0), limits=c(0, NA)) +
  hrbrthemes::scale_fill_ft() +
  facet_wrap(~test, ncol = 1) +
  coord_flip() +
  labs(x=NULL) +
  hrbrthemes::theme_ft_rc(grid="XY", strip_text_face = "bold") +
  theme(panel.spacing.y=unit(0, "lines"))
```

<img src="man/figures/README-unnamed-chunk-5-1.png" width="576" />

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
## # … with 22 more rows

filter(u_parsed, !is.na(scheme))
## # A tibble: 14 x 9
##    scheme user  password host            port  path          options query fragment
##    <chr>  <chr> <chr>    <chr>           <chr> <chr>         <chr>   <chr> <chr>   
##  1 http   <NA>  <NA>     foo.com         80    /path         <NA>    <NA>  <NA>    
##  2 http   <NA>  <NA>     foo.com         80    /             <NA>    <NA>  <NA>    
##  3 http   user  pass     foo.com         80    /             <NA>    <NA>  <NA>    
##  4 file   <NA>  <NA>     <NA>            0     /tmp/junk.txt <NA>    <NA>  <NA>    
##  5 imap   <NA>  <NA>     mail.python.org 143   /mbox1        <NA>    <NA>  <NA>    
##  6 http   <NA>  <NA>     WWW.PYTHON.ORG  80    /doc/         <NA>    <NA>  frag    
##  7 http   <NA>  <NA>     www.python.org  80    /             <NA>    <NA>  <NA>    
##  8 http   <NA>  <NA>     www.python.org  80    /             <NA>    <NA>  <NA>    
##  9 http   <NA>  <NA>     example.com     80    /             <NA>    ""    <NA>    
## 10 http   <NA>  <NA>     example.com     80    /;            <NA>    <NA>  <NA>    
## 11 http   user  <NA>     example.com     8080  /path;param   <NA>    query fragment
## 12 http   <NA>  <NA>     .               80    /             <NA>    <NA>  <NA>    
## 13 http   <NA>  <NA>     foo..com        80    /             <NA>    <NA>  <NA>    
## 14 http   <NA>  <NA>     foo..           80    /             <NA>    <NA>  <NA>

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
## # A tibble: 13 x 9
##    scheme user  password host            port  path        options query fragment
##    <chr>  <chr> <chr>    <chr>           <chr> <chr>       <chr>   <chr> <chr>   
##  1 http   <NA>  <NA>     foo.com         80    /path       <NA>    <NA>  <NA>    
##  2 http   <NA>  <NA>     foo.com         80    /           <NA>    <NA>  <NA>    
##  3 http   user  pass     foo.com         80    /           <NA>    <NA>  <NA>    
##  4 imap   <NA>  <NA>     mail.python.org 143   /mbox1      <NA>    <NA>  <NA>    
##  5 http   <NA>  <NA>     WWW.PYTHON.ORG  80    /doc/       <NA>    <NA>  frag    
##  6 http   <NA>  <NA>     www.python.org  80    /           <NA>    <NA>  <NA>    
##  7 http   <NA>  <NA>     www.python.org  80    /           <NA>    <NA>  <NA>    
##  8 http   <NA>  <NA>     example.com     80    /           <NA>    ""    <NA>    
##  9 http   <NA>  <NA>     example.com     80    /;          <NA>    <NA>  <NA>    
## 10 http   user  <NA>     example.com     8080  /path;param <NA>    query fragment
## 11 http   <NA>  <NA>     .               80    /           <NA>    <NA>  <NA>    
## 12 http   <NA>  <NA>     foo..com        80    /           <NA>    <NA>  <NA>    
## 13 http   <NA>  <NA>     foo..           80    /           <NA>    <NA>  <NA>

filter(u_parsed, !is.na(path))
## # A tibble: 14 x 9
##    scheme user  password host            port  path          options query fragment
##    <chr>  <chr> <chr>    <chr>           <chr> <chr>         <chr>   <chr> <chr>   
##  1 http   <NA>  <NA>     foo.com         80    /path         <NA>    <NA>  <NA>    
##  2 http   <NA>  <NA>     foo.com         80    /             <NA>    <NA>  <NA>    
##  3 http   user  pass     foo.com         80    /             <NA>    <NA>  <NA>    
##  4 file   <NA>  <NA>     <NA>            0     /tmp/junk.txt <NA>    <NA>  <NA>    
##  5 imap   <NA>  <NA>     mail.python.org 143   /mbox1        <NA>    <NA>  <NA>    
##  6 http   <NA>  <NA>     WWW.PYTHON.ORG  80    /doc/         <NA>    <NA>  frag    
##  7 http   <NA>  <NA>     www.python.org  80    /             <NA>    <NA>  <NA>    
##  8 http   <NA>  <NA>     www.python.org  80    /             <NA>    <NA>  <NA>    
##  9 http   <NA>  <NA>     example.com     80    /             <NA>    ""    <NA>    
## 10 http   <NA>  <NA>     example.com     80    /;            <NA>    <NA>  <NA>    
## 11 http   user  <NA>     example.com     8080  /path;param   <NA>    query fragment
## 12 http   <NA>  <NA>     .               80    /             <NA>    <NA>  <NA>    
## 13 http   <NA>  <NA>     foo..com        80    /             <NA>    <NA>  <NA>    
## 14 http   <NA>  <NA>     foo..           80    /             <NA>    <NA>  <NA>

filter(u_parsed, !is.na(query))
## # A tibble: 2 x 9
##   scheme user  password host        port  path        options query fragment
##   <chr>  <chr> <chr>    <chr>       <chr> <chr>       <chr>   <chr> <chr>   
## 1 http   <NA>  <NA>     example.com 80    /           <NA>    ""    <NA>    
## 2 http   user  <NA>     example.com 8080  /path;param <NA>    query fragment

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

## curlparse Metrics

| Lang         | \# Files |  (%) |  LoC |  (%) | Blank lines |  (%) | \# Lines |  (%) |
| :----------- | -------: | ---: | ---: | ---: | ----------: | ---: | -------: | ---: |
| C            |       21 | 0.41 | 1773 | 0.49 |         336 | 0.63 |      216 | 0.21 |
| C/C++ Header |        3 | 0.06 |  844 | 0.23 |          10 | 0.02 |        1 | 0.00 |
| R            |       23 | 0.45 |  612 | 0.17 |          75 | 0.14 |      699 | 0.68 |
| C++          |        2 | 0.04 |  285 | 0.08 |          65 | 0.12 |       58 | 0.06 |
| Rmd          |        1 | 0.02 |   85 | 0.02 |          49 | 0.09 |       59 | 0.06 |
| Bourne Shell |        1 | 0.02 |    2 | 0.00 |           0 | 0.00 |        0 | 0.00 |

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
