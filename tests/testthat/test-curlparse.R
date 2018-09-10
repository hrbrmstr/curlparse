context("minimal package functionality")
test_that("we can do something", {

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

  structure(list(
    scheme = c(
      NA, NA, NA, "http", "http", NA, NA,
      "http", "file", "imap", NA, NA, NA, NA, "http", "http", NA, NA,
      NA, "http", "http", NA, NA, "http", NA, NA, NA, NA, NA, "http",
      "http", "http"
    ),
    user = c(
      NA, NA, NA, NA, NA, NA, NA, "user",
      NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, "user",
      NA, NA, NA, NA, NA, NA, NA, NA
    ),
    password = c(
      NA, NA, NA, NA, NA, NA, NA, "pass", NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
      NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
    ),
    host = c(
      NA, NA, NA, "foo.com", "foo.com", NA, NA, "foo.com", NA, "mail.python.org",
      NA, NA, NA, NA, "WWW.PYTHON.ORG", "www.python.org", NA, NA, NA,
      "example.com", "example.com", NA, NA, "example.com", NA, NA,
      NA, NA, NA, ".", "foo..com", "foo.."
    ),
    port = c(
      NA, NA, NA, "80",
      "80", NA, NA, "80", "0", "143", NA, NA, NA, NA, "80", "80", NA,
      NA, NA, "80", "80", NA, NA, "8080", NA, NA, NA, NA, NA, "80", "80", "80"
    ),
    path = c(
      NA, NA, NA, "/path", "/", NA, NA, "/",
      "/tmp/junk.txt", "/mbox1", NA, NA, NA, NA, "/doc/", "/", NA,
      NA, NA, "/", "/;", NA, NA, "/path;param", NA, NA, NA, NA, NA, "/", "/", "/"
    ),
    options = c(
      NA_character_, NA_character_, NA_character_,
      NA_character_, NA_character_, NA_character_, NA_character_, NA_character_,
      NA_character_, NA_character_, NA_character_, NA_character_, NA_character_,
      NA_character_, NA_character_, NA_character_, NA_character_, NA_character_,
      NA_character_, NA_character_, NA_character_, NA_character_, NA_character_,
      NA_character_, NA_character_, NA_character_, NA_character_, NA_character_,
      NA_character_, NA_character_, NA_character_, NA_character_
    ),
    query = c(
      NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
      NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, "query", NA,
      NA, NA, NA, NA, NA, NA, NA
    ),
    fragment = c(
      NA, NA, NA, NA,
      NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, "frag", NA, NA, NA,
      NA, NA, NA, NA, NA, "fragment", NA, NA, NA, NA, NA, NA, NA, NA
    )
  ),
  class = c("tbl_df", "tbl", "data.frame"), row.names = c(NA, -32L)
  ) -> shld_equal

  u_parsed <- parse_curl(ugly_urls)

  expect_identical(u_parsed, shld_equal)

  expect_true(
    all(
      c(
        expect_true(identical(u_parsed$scheme, scheme(ugly_urls))),
        expect_true(identical(u_parsed$user, user(ugly_urls))),
        expect_true(identical(u_parsed$password, password(ugly_urls))),
        expect_true(identical(u_parsed$host, host(ugly_urls))),
        expect_true(identical(u_parsed$path, path(ugly_urls))),
        expect_true(identical(u_parsed$query, query(ugly_urls))),
        expect_true(identical(u_parsed$fragment, fragment(ugly_urls))
        )
      )
    )
  )

})
