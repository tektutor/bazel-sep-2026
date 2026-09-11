# Day 1 — Introduction to Bazel

## What is Bazel?
Bazel is an open-source build and test tool from Google. It supports
multiple languages and platforms with a focus on correctness and speed.

## Key Concepts

- **Workspace** — root directory containing `MODULE.bazel`
- **Package** — any directory with a `BUILD` file
- **Target** — a named rule instance inside a BUILD file
- **Label** — unique target ID e.g. `//lib/core:logger`

## Your First BUILD File

```python
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "hello",
    srcs = ["hello.cc"],
)
```

```bash
bazel build //app:hello
bazel run   //app:hello
```

## MODULE.bazel

```python
module(name = "myproject", version = "1.0.0")
bazel_dep(name = "rules_cc", version = "0.2.17")
```
