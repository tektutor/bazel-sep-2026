def _generate_header_impl(ctx):
    out = ctx.actions.declare_file(ctx.attr.name + ".h")

    ctx.actions.run(
        outputs = [out],
	executable = ctx.executable._generator,
	arguments = [out.path, ctx.attr.message],
	progress_message = "Generating %s" % out.short_path,
    )

    return [DefaultInfo(files = depset([out]))]


generate_header = rule(
	implementation = _generate_header_impl,
	attrs = {
		"message": attr.string(mandatory = True),
		"_generator": attr.label(
                    default = "//tools:generate-code",
		    executable = True,
		    cfg = "exec",
		),
	},
)
