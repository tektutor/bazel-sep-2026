# Day 3 — Aspects, Providers and Docs-as-Code

## Aspects

An aspect attaches extra actions to existing targets without modifying
their BUILD files.

```python
def _lint_impl(target, ctx):
    if CcInfo not in target:
        return []
    reports = []
    for src in ctx.rule.attr.srcs:
        for f in src.files.to_list():
            report = ctx.actions.declare_file(f.basename + ".lint")
            ctx.actions.run_shell(
                inputs  = [f],
                outputs = [report],
                command = "clang-tidy {} > {} 2>&1 || true".format(
                    f.path, report.path),
            )
            reports.append(report)
    return [OutputGroupInfo(lint_reports = depset(reports))]

lint_aspect = aspect(
    implementation = _lint_impl,
    attr_aspects   = ["deps"],
)
```

## Providers

```python
VersionInfo = provider(fields = ["version", "label"])

def _impl(ctx):
    return [VersionInfo(version = ctx.attr.version,
                        label   = str(ctx.label))]
```

## Docs-as-Code

Documentation treated as source code:

- Markdown files live next to the code they describe
- The build system generates PDF from them
- Bazel caches the output — PDF only regenerates when a README changes
- CI validates and publishes on every commit

```bash
# Only regenerates if a README.md changed
bazel build //docs:training_notes

# Find the PDF
ls bazel-bin/docs/training_notes.pdf
```
