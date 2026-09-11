# tools/aspects/metadata.bzl
# Generates a .metadata.json for every C++ target.
# Usage: bazel build //... --aspects=//tools/aspects:metadata.bzl%metadata_aspect \
#                          --output_groups=metadata_files
load("@rules_cc//cc:defs.bzl", "CcInfo")

MetadataInfo = provider(
    doc    = "JSON metadata about a target and its transitive deps.",
    fields = {"json_files": "depset of generated .json files"},
)

def _metadata_aspect_impl(target, ctx):
    if CcInfo not in target:
        return []
    hdrs      = target[CcInfo].compilation_context.direct_public_headers
    dep_labels = [
        str(dep.label)
        for dep in (ctx.rule.attr.deps if hasattr(ctx.rule.attr, "deps") else [])
    ]
    content = '{{\n  "target": "{l}",\n  "headers": {h},\n  "deps": {d}\n}}'.format(
        l = str(target.label),
        h = str([h.short_path for h in hdrs]).replace("'", '"'),
        d = str(dep_labels).replace("'", '"'),
    )
    out = ctx.actions.declare_file("{}.metadata.json".format(target.label.name))
    ctx.actions.write(output = out, content = content)
    transitive = [
        dep[MetadataInfo].json_files
        for dep in (ctx.rule.attr.deps if hasattr(ctx.rule.attr, "deps") else [])
        if MetadataInfo in dep
    ]
    all_files = depset(direct = [out], transitive = transitive)
    return [MetadataInfo(json_files = all_files), OutputGroupInfo(metadata_files = all_files)]

metadata_aspect = aspect(implementation = _metadata_aspect_impl, attr_aspects = ["deps"])
