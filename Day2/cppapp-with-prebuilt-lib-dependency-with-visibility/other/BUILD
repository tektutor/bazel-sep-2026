load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary (
   name = "other_app",
   srcs = ["other.cpp"],
   # This should fail
   deps = ["//third_party/libmath:libmath"],
)

