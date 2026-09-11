# tools/rules/cc_rules.bzl — Bazel 9.1.0 + rules_cc 0.2.17
#
# Key findings from Bazel 9.1.0 source:
#
# 1. cc_library, cc_binary, cc_test, cc_import: removed from builtins.
#    Load from @rules_cc//cc:defs.bzl.
#
# 2. native.cc_library: also removed (same as cc_library).
#    Use the loaded symbol instead.
#
# 3. Builtin cc_common in Bazel 9 is STRIPPED — no configure_features/compile/link.
#    Full cc_common is in @rules_cc//cc/common:cc_common.bzl but requires
#    find_cc_toolchain and allowlisted access.
#
# 4. cc_header_only_library: implemented as a MACRO wrapping cc_library (no srcs).
#    Validation via fail() at macro-expansion time. No cc_common needed.
#
# 5. cc_versioned_binary: MACRO using _write_version_src_rule (ctx.actions.write)
#    + cc_binary. No cc_common needed.

load("@rules_cc//cc:defs.bzl", "CcInfo", "cc_binary", "cc_library")

# ── Provider ──────────────────────────────────────────────────────────────────
VersionInfo = provider(
    doc    = "Version metadata from cc_versioned_binary.",
    fields = {"version": "Semver string", "build_label": "CI label"},
)

# ── _write_version_src_rule ───────────────────────────────────────────────────
def _write_version_src_impl(ctx):
    out = ctx.actions.declare_file(ctx.attr.out_name)
    ctx.actions.write(
        output  = out,
        content = """\
// AUTO-GENERATED — do not edit.
namespace version {{
const char* GetVersion()    {{ return "{v}"; }}
const char* GetBuildLabel() {{ return "{l}"; }}
}}  // namespace version
""".format(v = ctx.attr.version, l = ctx.attr.build_label),
    )
    return [DefaultInfo(files = depset([out]))]

_write_version_src_rule = rule(
    implementation = _write_version_src_impl,
    attrs = {
        "version":     attr.string(mandatory = True),
        "build_label": attr.string(default = "dev"),
        "out_name":    attr.string(mandatory = True),
    },
)

# ── cc_versioned_binary (macro) ───────────────────────────────────────────────
def cc_versioned_binary(
        name,
        srcs,
        hdrs = [],
        deps = [],
        version = "",
        build_label = "dev",
        copts = [],
        linkstatic = True,
        visibility = None):
    """cc_binary with a build-time generated version.cc."""
    if not version:
        fail("cc_versioned_binary requires a non-empty 'version' attribute")
    version_rule = name + "_version_src"
    version_file = "_" + name + "_version.cc"
    _write_version_src_rule(
        name        = version_rule,
        version     = version,
        build_label = build_label,
        out_name    = version_file,
    )
    # cc_binary has no 'hdrs' attribute — pass headers via srcs
    cc_binary(
        name       = name,
        srcs       = srcs + hdrs + [":" + version_rule],
        copts      = ["-std=c++17", "-Wall", "-Wextra", "-Wno-unused-parameter"] + copts,
        deps       = deps,
        linkstatic = linkstatic,
        visibility = visibility,
    )

# ── cc_header_only_library (macro) ────────────────────────────────────────────
# A macro (not a rule) because the full cc_common API (configure_features, compile)
# is not accessible from user Starlark in Bazel 9 — it lives inside rules_cc's
# internal allowlisted code. Using cc_library with empty srcs achieves the same result.
def cc_header_only_library(
        name,
        hdrs,
        srcs = [],
        deps = [],
        visibility = None):
    """
    Header-only library. Fails at load time if any srcs are given.
    Wraps cc_library with srcs=[] so CcInfo propagates headers to dependents.
    """
    if srcs:
        fail("cc_header_only_library '{}' must not have srcs; got: {}".format(name, srcs))
    cc_library(
        name       = name,
        hdrs       = hdrs,
        srcs       = [],
        deps       = deps,
        visibility = visibility,
    )
