load("//tools/rules:cc_rules.bzl", "cc_versioned_binary")
package(default_visibility = ["//visibility:public"])

cc_versioned_binary(
    name        = "cli",
    srcs        = ["main.cc"],
    hdrs        = ["version.h"],
    version     = "1.0.0",
    build_label = "dev",
    deps = [
        "//lib/core",
        "//lib/net",
        "//services/auth:auth_service",
        "//services/gateway",
    ],
)
