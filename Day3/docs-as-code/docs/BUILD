# docs/BUILD
#
# Generates the training notes PDF using the md2pdf custom rule.
#
# The PDF is a proper Bazel build output:
#   - Cached in bazel-bin/docs/training_notes.pdf
#   - Only regenerated when a README.md file changes
#   - Tracked in Bazel's dependency graph
#
# Usage:
#   bazel build //docs:training_notes
#   ls bazel-bin/docs/training_notes.pdf
#
# For a remote GitHub repo (no caching — always re-runs):
#   bazel run //src:md2pdf -- \
#       --source https://github.com/tektutor/bazel-may-2026.git \
#       --folders day1 day2 day3 \
#       --output $PWD/notes.pdf

load("//tools:md2pdf_rule.bzl", "md2pdf")

package(default_visibility = ["//visibility:public"])

md2pdf(
    name    = "training_notes",
    title   = "Bazel Training Notes",
    folders = "day1,day2,day3",
    srcs    = [
        "//day1:README.md",
        "//day2:README.md",
        "//day3:README.md",
    ],
)
