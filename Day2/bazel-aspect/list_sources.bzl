SourcesInfo = provider(fields = ["srcs"])

def _impl(target, ctx):
    own = []
    for f in getattr(ctx.rule.attr, "srcs", []):
       own += f.files.to_list()

    transitive = [
       dep[SourcesInfo].srcs
       for dep in getattr(ctx.rule.attr, "deps", [])
         if SourcesInfo in dep
    ]

    all_srcs = depset(direct = own, transitive = transitive)

    manifest = ctx.actions.declare_file(target.label.name + ".srcs.txt")
    ctx.actions.write(
        manifest,
	"".join([f.short_path + "\n" for f in all_srcs.to_list()]),
    )

    return [
	    SourcesInfo(srcs = all_srcs),
	    OutputGroupInfo(manifest = depset([manifest])),
    ]

list_sources = aspect(
    implementation = _impl,
    attr_aspects = ["deps"],
)
