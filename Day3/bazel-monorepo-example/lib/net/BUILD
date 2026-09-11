load("//tools/rules:macros.bzl", "versioned_cc_library")
package(default_visibility = ["//visibility:public"])

versioned_cc_library(
    name      = "net",
    srcs      = ["http.cc"],
    hdrs      = ["http.h"],
    deps      = ["//lib/core"],
    test_srcs = ["http_test.cc"],
)
