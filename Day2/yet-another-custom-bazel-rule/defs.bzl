def _expand_config_impl(ctx):
    out = ctx.actions.declare_file(ctx.attr.name + ".txt")
    ctx.actions.expand_template(
       template = ctx.file.template,
       output = out,
       substitutions = {
           "{{NAME}}": ctx.attr.app_name,
	   "{{VERSION}}": ctx.attr.version,
       },
    )
    return [DefaultInfo(files = depset([out]))]

expand_config = rule (
    implementation = _expand_config_impl,
    attrs = {
       "template": attr.label(allow_single_file = True, mandatory = True),
       "app_name": attr.string(mandatory = True),
       "version": attr.string(mandatory = True),
    },
)
