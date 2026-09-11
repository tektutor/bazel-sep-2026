def _doc_aspect_impl(target, ctx):
    """
    Runs alongside any cc_library target it is attached to.
    Generates a .txt doc file listing the target's sources and headers.

    target = the cc_library target this aspect is attached to
    ctx    = context giving access to the target's attributes and files
    """

    # Collect source files declared in srcs = [...]
    srcs = []
    if hasattr(ctx.rule.attr, "srcs"):
        for src_group in ctx.rule.attr.srcs:
            for f in src_group.files.to_list():
                srcs.append(f.path)

    # Collect header files declared in hdrs = [...]
    hdrs = []
    if hasattr(ctx.rule.attr, "hdrs"):
        for hdr_group in ctx.rule.attr.hdrs:
            for f in hdr_group.files.to_list():
                hdrs.append(f.path)

    # Nothing to document — skip
    if not srcs and not hdrs:
        return []

    # Declare the output doc file
    # e.g. hello_lib → hello_lib_doc.txt
    doc_file = ctx.actions.declare_file(
        target.label.name + "_doc.txt"
    )

    # Write the doc file content
    content  = "==================================\n"
    content += "Target  : {}\n".format(str(target.label))
    content += "==================================\n"
    content += "Sources :\n"
    for s in srcs:
        content += "  - {}\n".format(s)
    content += "Headers :\n"
    for h in hdrs:
        content += "  - {}\n".format(h)
    content += "==================================\n"

    ctx.actions.write(
        output  = doc_file,
        content = content,
    )

    # Return the doc file as an output group called "docs"
    return [OutputGroupInfo(docs = depset([doc_file]))]


# Define the aspect
doc_aspect = aspect(
    implementation = _doc_aspect_impl,
    attr_aspects   = ["deps"],   # propagate through deps chain
)


# A thin rule that applies the aspect to its deps
def _doc_rule_impl(ctx):
    all_docs = []
    for dep in ctx.attr.deps:
        if OutputGroupInfo in dep:
            all_docs.append(dep[OutputGroupInfo].docs)
    return [DefaultInfo(files = depset(transitive = all_docs))]


doc_rule = rule(
    implementation = _doc_rule_impl,
    attrs = {
        "deps": attr.label_list(aspects = [doc_aspect]),
    },
)
