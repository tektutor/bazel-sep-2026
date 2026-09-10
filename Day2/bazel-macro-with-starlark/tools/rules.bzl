load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")

def hello_library(name, srcs, hdrs, visibility = None):
    cc_library(
       name = name,
       srcs = srcs,
       hdrs = hdrs,
       copts = ["-std=c++17", "-Wall"],
       visibility = visibility or ["//visibility:public"],
    )

def hello_binary(name, srcs, deps = []):
    cc_binary(
      name = name,
      srcs = srcs,
      deps = deps,
      copts = ["-std=c++17","-Wall"],
    )
