# tools/rules/macros.bzl
# Bazel 9.1.0: cc_library, cc_binary, cc_test removed from builtins.
# Load from @rules_cc//cc:defs.bzl.

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

REPO_COPTS = ["-std=c++17", "-Wall", "-Wextra", "-Wno-unused-parameter"]

def versioned_cc_library(
        name,
        srcs,
        hdrs,
        deps = [],
        visibility = None,
        test_srcs = None,
        **kwargs):
    """Wraps cc_library with standard copts and optional auto-generated test target."""
    env_defines = select({
        "//:is_prod": ["BUILD_ENV=\\\"prod\\\"", "NDEBUG"],
        "//:is_dev":  ["BUILD_ENV=\\\"dev\\\""],
        "//conditions:default": ["BUILD_ENV=\\\"dev\\\""],
    })
    cc_library(
        name       = name,
        srcs       = srcs,
        hdrs       = hdrs,
        copts      = REPO_COPTS,
        defines    = env_defines,
        deps       = deps,
        visibility = visibility,
        **kwargs
    )
    if test_srcs:
        cc_test(
            name  = name + "_test",
            srcs  = test_srcs,
            copts = REPO_COPTS,
            deps  = [":" + name, "@com_google_googletest//:gtest_main"],
        )
