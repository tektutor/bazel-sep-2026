load("//tools/rules:macros.bzl", "versioned_cc_library")
package(default_visibility = ["//visibility:public"])

versioned_cc_library(
    name      = "gateway",
    srcs      = ["gateway.cc"],
    hdrs      = ["gateway.h"],
    deps      = ["//lib/core", "//lib/net", "//services/auth:auth_service"],
    test_srcs = ["gateway_test.cc"],
)
