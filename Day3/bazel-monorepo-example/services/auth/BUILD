load("//tools/rules:macros.bzl", "versioned_cc_library")
package(default_visibility = ["//visibility:public"])

versioned_cc_library(
    name      = "auth_service",
    srcs      = ["auth_service.cc"],
    hdrs      = ["auth_service.h"],
    deps      = ["//lib/core", "//lib/crypto", "//lib/net"],
    test_srcs = ["auth_service_test.cc"],
)
