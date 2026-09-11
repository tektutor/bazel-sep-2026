load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary", "cc_test")

cc_library(
    name = "hello_lib",
    hdrs = ["hello.h"],
    srcs = select({
        # Pick implementation based on target platform
        "@platforms//cpu:x86_64":  ["hello_linux_x86.cpp"],
        "@platforms//cpu:aarch64": ["hello_linux_arm.cpp"],
        "@platforms//os:none":     ["hello_bare_metal.cpp"],
        "//conditions:default":    ["hello_linux_x86.cpp"],
    }),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "hello",
    srcs = ["main.cpp"],
    deps = [":hello_lib"],
    # Binary only compatible with these platforms
    target_compatible_with = select({
        "@platforms//os:linux": [],         # compatible
        "@platforms//os:none":  [],         # compatible
        "//conditions:default": ["@platforms//os:linux"],
    }),
)

cc_test(
    name = "hello_test",
    srcs = ["hello_test.cpp"],
    deps = [
        ":hello_lib",
        "@googletest//:gtest_main",
    ],
    size = "small",
)
