# Day 2 — Rules, Macros and Custom Rules

## cc_library vs cc_binary

`cc_library` produces a static archive consumed by other targets.
`cc_binary` produces an executable. Both must be loaded from `@rules_cc` in Bazel 9.

```python
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
```

## Writing a Macro

A macro is a Starlark function that expands into rule calls at load time.

```python
def versioned_cc_library(name, srcs, hdrs, version, **kwargs):
    cc_library(
        name    = name,
        srcs    = srcs,
        hdrs    = hdrs,
        defines = ["VERSION=\\\"{}\\\"".format(version)],
        **kwargs
    )
```

## Writing a Custom Rule

```python
def _codegen_impl(ctx):
    out = ctx.actions.declare_file(ctx.attr.name + ".h")
    ctx.actions.run_shell(
        inputs  = [ctx.file.src],
        outputs = [out],
        command = "python3 gen.py {src} {out}".format(
            src = ctx.file.src.path,
            out = out.path,
        ),
    )
    return [DefaultInfo(files = depset([out]))]

codegen = rule(
    implementation = _codegen_impl,
    attrs = {"src": attr.label(allow_single_file = [".txt"])},
)
```

## select() and config_setting

```python
config_setting(name = "is_prod", define_values = {"env": "prod"})

cc_library(
    name    = "mylib",
    defines = select({
        "//:is_prod": ["NDEBUG"],
        "//conditions:default": [],
    }),
)
```
