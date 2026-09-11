# tests/BUILD
load("@rules_python//python:defs.bzl", "py_test")
load("@pip//:requirements.bzl", "requirement")

package(default_visibility = ["//visibility:public"])

py_test(
    name = "md2pdf_test",
    srcs = ["test_md2pdf.py"],
    main = "test_md2pdf.py",
    deps = [
        "//src:md2pdf_lib",
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
