# tools/aspects/lint.bzl
# Runs clang-tidy on every C++ source in the build graph.
# Usage: bazel build //... --aspects=//tools/aspects:lint.bzl%lint_aspect \
#                          --output_groups=lint_reports
load("@rules_cc//cc:defs.bzl", "CcInfo")

LintInfo = provider(
    doc    = "Lint report files from this target and its deps.",
    fields = {"reports": "depset of clang-tidy report files"},
)

def _lint_aspect_impl(target, ctx):
    if CcInfo not in target:
        return []
    srcs = []
    for s in (ctx.rule.attr.srcs if hasattr(ctx.rule.attr, "srcs") else []):
        for f in s.files.to_list():
            if f.extension in ("cc", "cpp", "cxx", "c"):
                srcs.append(f)
    reports = []
    for src in srcs:
        report = ctx.actions.declare_file(
            "{}__{}.clang-tidy.txt".format(target.label.name, src.basename),
        )
        includes = [
            "-I" + inc
            for inc in target[CcInfo].compilation_context.includes.to_list()
        ]
        ctx.actions.run_shell(
            inputs   = [src] + target[CcInfo].compilation_context.headers.to_list(),
            outputs  = [report],
            command  = 'clang-tidy "{s}" -- -std=c++17 {i} > "{o}" 2>&1 || true'.format(
                s = src.path, o = report.path, i = " ".join(includes),
            ),
            mnemonic         = "ClangTidy",
            progress_message = "Linting {}".format(src.short_path),
        )
        reports.append(report)
    transitive = [
        dep[LintInfo].reports
        for dep in (ctx.rule.attr.deps if hasattr(ctx.rule.attr, "deps") else [])
        if LintInfo in dep
    ]
    all_reports = depset(direct = reports, transitive = transitive)
    return [LintInfo(reports = all_reports), OutputGroupInfo(lint_reports = all_reports)]

lint_aspect = aspect(implementation = _lint_aspect_impl, attr_aspects = ["deps"])
