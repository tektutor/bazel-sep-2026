load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_google_protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
package(default_visibility = ["//visibility:public"])

proto_library(
    name = "user_proto",
    srcs = ["user.proto"],
)
cc_proto_library(
    name = "user_cc_proto",
    deps = [":user_proto"],
)
