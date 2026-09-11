load("@rules_cc//cc:defs.bzl","cc_common")
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")

load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)

def _impl(ctx):
    tool_paths = [
        tool_path(name = "gcc",     path = "/usr/bin/aarch64-linux-gnu-g++"),
        tool_path(name = "ar",      path = "/usr/bin/aarch64-linux-gnu-ar"),
        tool_path(name = "ld",      path = "/usr/bin/aarch64-linux-gnu-ld"),
        tool_path(name = "nm",      path = "/usr/bin/aarch64-linux-gnu-nm"),
        tool_path(name = "objdump", path = "/usr/bin/aarch64-linux-gnu-objdump"),
        tool_path(name = "strip",   path = "/usr/bin/aarch64-linux-gnu-strip"),
        tool_path(name = "gcov",    path = "/usr/bin/aarch64-linux-gnu-gcov"),
        tool_path(name = "cpp",     path = "/usr/bin/aarch64-linux-gnu-cpp"),
    ]


    static_link = feature (
       name = "static_link",
       enabled = True,
       flag_sets = [
         flag_set (
	      actions = [ACTION_NAMES.cpp_link_executable],
	      flag_groups = [flag_group(flags = ["-static"])],
         ),
       ],
    )

    return cc_common.create_cc_toolchain_config_info (
       ctx = ctx,
       toolchain_identifier = "aarch64-linux-gnu",
       host_system_name = "local",
       target_system_name = "aarch64-linux-gnu",
       target_cpu = "aarch64",
       target_libc = "glibc",
       compiler = "gcc",
       abi_version = "aarch64",
       abi_libc_version = "glibc",
       tool_paths = tool_paths,
       features = [static_link],
       cxx_builtin_include_directories = [
         "/usr/lib/gcc-cross/aarch64-linux-gnu",
         "/usr/aarch64-linux-gnu/include",
       ],
    )

cc_toolchain_config = rule (
  implementation = _impl,
  attrs = {},
)
  
