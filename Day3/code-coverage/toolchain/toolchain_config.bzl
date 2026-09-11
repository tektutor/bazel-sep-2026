load("@rules_cc//cc:action_names.bzl", "ACTION_NAMES")
load(
    "@rules_cc//cc:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)
load("@rules_cc//cc/common:cc_common.bzl", "cc_common")
load(
    "@rules_cc//cc/private/toolchain_config:cc_toolchain_config_info.bzl",
    "CcToolchainConfigInfo",
)

def _my_gcc_toolchain_config_impl(ctx):
    tool_paths = [
        tool_path(name = "gcc",     path = "/usr/bin/gcc"),
        tool_path(name = "g++",     path = "/usr/bin/g++"),
        tool_path(name = "ar",      path = "/usr/bin/ar"),
        tool_path(name = "ld",      path = "/usr/bin/ld"),
        tool_path(name = "nm",      path = "/usr/bin/nm"),
        tool_path(name = "objcopy", path = "/usr/bin/objcopy"),
        tool_path(name = "objdump", path = "/usr/bin/objdump"),
        tool_path(name = "strip",   path = "/usr/bin/strip"),
        tool_path(name = "gcov",    path = "/usr/bin/gcov"),
        tool_path(name = "cpp",     path = "/usr/bin/cpp"),
        tool_path(name = "dwp",     path = "/usr/bin/dwp"),
    ]

    default_compile_flags = feature(
        name    = "default_compile_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.c_compile,
                    ACTION_NAMES.cpp_compile,
                ],
                flag_groups = [
                    flag_group(
                        flags = [
                            "-fstack-protector",
                            "-Wall",
                            "-Wno-free-nonheap-object",
                            "-fno-omit-frame-pointer",
                            "-fno-canonical-system-headers",
                            "-Wno-builtin-macro-redefined",
                            "-D__DATE__=\"redacted\"",
                            "-D__TIMESTAMP__=\"redacted\"",
                            "-D__TIME__=\"redacted\"",
                        ],
                    ),
                ],
            ),
        ],
    )

    cxx_flags = feature(
        name    = "cxx_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [ACTION_NAMES.cpp_compile],
                flag_groups = [
                    flag_group(flags = ["-std=c++17"]),
                ],
            ),
        ],
    )

    dbg_feature = feature(
        name = "dbg",
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.c_compile,
                    ACTION_NAMES.cpp_compile,
                ],
                flag_groups = [
                    flag_group(flags = ["-g", "-O0", "-DDEBUG"]),
                ],
            ),
        ],
    )

    opt_feature = feature(
        name = "opt",
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.c_compile,
                    ACTION_NAMES.cpp_compile,
                ],
                flag_groups = [
                    flag_group(flags = ["-O3", "-DNDEBUG", "-g0"]),
                ],
            ),
        ],
    )

    default_link_flags = feature(
        name    = "default_link_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [ACTION_NAMES.cpp_link_executable],
                flag_groups = [
                    flag_group(
                        flags = [
                            "-B/usr/bin",
                            "-Wl,-no-as-needed",
                            "-Wl,-z,relro,-z,now",
                            "-pass-exit-codes",
                            "-lstdc++",
                            "-lm",
                        ],
                    ),
                ],
            ),
        ],
    )

    return cc_common.create_cc_toolchain_config_info(
        ctx                    = ctx,
        toolchain_identifier   = "my-custom-gcc",
        host_system_name       = "local",
        target_system_name     = "local",
        target_cpu             = "k8",
        target_libc            = "local",
        compiler               = "gcc",
        abi_version            = "local",
        abi_libc_version       = "local",
        tool_paths             = tool_paths,
        cxx_builtin_include_directories = [
            "/usr/lib/gcc/x86_64-linux-gnu/14/include",
            "/usr/local/include",
            "/usr/include/x86_64-linux-gnu",
            "/usr/include",
            "/usr/include/c++/14",
            "/usr/include/x86_64-linux-gnu/c++/14",
            "/usr/include/c++/14/backward",
        ],
        features = [
            default_compile_flags,
            cxx_flags,
            dbg_feature,
            opt_feature,
            default_link_flags,
        ],
    )


my_gcc_toolchain_config = rule(
    implementation = _my_gcc_toolchain_config_impl,
    attrs          = {},
    provides       = [CcToolchainConfigInfo],
)
