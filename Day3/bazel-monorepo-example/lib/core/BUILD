load("//tools/rules:macros.bzl",   "versioned_cc_library")
load("//tools/rules:cc_rules.bzl", "cc_header_only_library")
package(default_visibility = ["//visibility:public"])

versioned_cc_library(
    name      = "core",
    srcs      = ["logger.cc"],
    hdrs      = ["logger.h", "result.h"],
    test_srcs = ["logger_test.cc"],
)
cc_header_only_library(
    name = "result",
    hdrs = ["result.h"],
)
