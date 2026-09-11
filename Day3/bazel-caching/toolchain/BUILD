load("@rules_cc//cc/toolchains:cc_toolchain.bzl", "cc_toolchain")
load(":toolchain_config.bzl", "my_gcc_toolchain_config")

filegroup(name = "empty", srcs = [])

filegroup(
    name = "compiler_deps",
    srcs = ["@@rules_cc++cc_configure_extension+local_config_cc//:compiler_deps"],
)

my_gcc_toolchain_config(name = "my_custom_config")

cc_toolchain(
    name                    = "my_gcc_cc_toolchain",
    toolchain_config        = ":my_custom_config",
    toolchain_identifier    = "my-custom-gcc",
    all_files               = ":compiler_deps",
    compiler_files          = ":compiler_deps",
    linker_files            = ":compiler_deps",
    ar_files                = ":compiler_deps",
    objcopy_files           = ":empty",
    strip_files             = ":empty",
    dwp_files               = ":empty",
    supports_param_files    = True,
    supports_header_parsing = True,
)

# ── x86_64 — rules_cc toolchain type ─────────────────────────────────────
toolchain(
    name           = "my_gcc_toolchain_x86",
    toolchain      = ":my_gcc_cc_toolchain",
    toolchain_type = "@rules_cc//cc:toolchain_type",
    target_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

# ── x86_64 — bazel_tools toolchain type ──────────────────────────────────
toolchain(
    name           = "my_gcc_toolchain_x86_legacy",
    toolchain      = ":my_gcc_cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
    target_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

# ── aarch64 — rules_cc toolchain type ────────────────────────────────────
toolchain(
    name           = "my_gcc_toolchain_aarch64",
    toolchain      = ":my_gcc_cc_toolchain",
    toolchain_type = "@rules_cc//cc:toolchain_type",
    target_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

# ── aarch64 — bazel_tools toolchain type ─────────────────────────────────
toolchain(
    name           = "my_gcc_toolchain_aarch64_legacy",
    toolchain      = ":my_gcc_cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
    target_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)
