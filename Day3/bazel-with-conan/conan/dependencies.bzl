# This Bazel module should be loaded by your WORKSPACE file.
# Add these lines to your WORKSPACE one (assuming that you're using the "bazel_layout"):
# load("@//conan:dependencies.bzl", "load_conan_dependencies")
# load_conan_dependencies()

def load_conan_dependencies():
    native.new_local_repository(
        name="fmt",
        path="/home/jegan/.conan2/p/b/fmta46fc51ef2e5e/p",
        build_file="/home/jegan/bazel-sep-2026/Day3/bazel-with-conan/conan/fmt/BUILD.bazel",
    )
