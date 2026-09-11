# Bazel 9+/rules_cc module extension. Use with BUILD.rules_cc.bazel dependency files.
# Include the generated module snippet from your MODULE.bazel file:
# include("//conan:conan_deps.MODULE.bazel")
load(":conan_deps_repo_rules.bzl", "conan_dependency_repo")

def _load_dependencies_impl(mctx):
    conan_dependency_repo(
        name = "fmt",
        package_path = "/home/jegan/.conan2/p/b/fmta46fc51ef2e5e/p",
        build_file_path = "/home/jegan/bazel-sep-2026/Day3/bazel-with-conan/conan/fmt/BUILD.rules_cc.bazel",
    )

    return mctx.extension_metadata(
        root_module_direct_deps = 'all',
        root_module_direct_dev_deps = [],
        reproducible = True,
    )

conan_extension = module_extension(
    implementation = _load_dependencies_impl,
    os_dependent = True,
    arch_dependent = True,
)