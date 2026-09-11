load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_import")
load("//tools/rules:macros.bzl", "versioned_cc_library")
package(default_visibility = ["//visibility:public"])

versioned_cc_library(
    name      = "crypto",
    srcs      = ["crypto.cc"],
    hdrs      = ["crypto.h"],
    deps      = ["//lib/core"],
    test_srcs = ["crypto_test.cc"],
)
cc_binary(
    name       = "libcrypto_shared.so",
    srcs       = ["crypto.cc", "crypto.h"],
    deps       = ["//lib/core"],
    linkshared = True,
    copts      = ["-std=c++17", "-fvisibility=hidden"],
    defines    = ["CRYPTO_BUILDING_DLL"],
)
cc_import(
    name           = "crypto_shared",
    shared_library = ":libcrypto_shared.so",
    hdrs           = ["crypto.h"],
    deps           = ["//lib/core"],
)
