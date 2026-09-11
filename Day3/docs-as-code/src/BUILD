# src/BUILD
load("@rules_python//python:defs.bzl", "py_binary", "py_library")
load("@pip//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

# Library target — imported by both the binary and the tests.
py_library(
    name = "md2pdf_lib",
    srcs = ["md2pdf.py"],
    deps = [
        requirement("markdown"),
        requirement("weasyprint"),
        requirement("pyphen"),
        requirement("pydyf"),
        requirement("tinycss2"),
        requirement("cssselect2"),
        requirement("tinyhtml5"),
        requirement("webencodings"),
        requirement("brotli"),
        requirement("zopfli"),
    ],
)

# Runnable binary — invoked via: bazel run //src:md2pdf -- --source ... --output ...
py_binary(
    name       = "md2pdf",
    srcs       = ["md2pdf.py"],
    main       = "md2pdf.py",
    deps       = [":md2pdf_lib"],
)
