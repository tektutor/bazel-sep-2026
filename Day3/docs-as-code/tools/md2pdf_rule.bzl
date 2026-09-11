# tools/md2pdf_rule.bzl
#
# Custom Bazel rule: md2pdf
#
# Generates a PDF from a list of local README.md files.
# Because the README files are declared as inputs, Bazel caches the output.
# The PDF is only regenerated when one of the README files actually changes.
#
# Usage in BUILD:
#
#   load("//tools:md2pdf_rule.bzl", "md2pdf")
#
#   md2pdf(
#       name  = "training_notes",
#       srcs  = [
#           "//day1:README.md",
#           "//day2:README.md",
#           "//day3:README.md",
#       ],
#       title   = "Bazel Training Notes",
#       folders = "day1,day2,day3",
#   )
#
# Build:
#   bazel build //docs:training_notes
#   # PDF is at: bazel-bin/docs/training_notes.pdf
#   # Second run with no changes: 0 actions — fully cached.

def _md2pdf_impl(ctx):
    # Declare the output PDF file Bazel will track
    out = ctx.actions.declare_file(ctx.attr.name + ".pdf")

    # Build the argument list for md2pdf.py
    # --source is the execroot (workspace root) so md2pdf can find day1/ day2/ etc.
    # We pass --folders as a comma-separated list and split it inside md2pdf.py
    args = [
        "--source",  ".",
        "--folders", ctx.attr.folders,
        "--title",   ctx.attr.title,
        "--output",  out.path,
    ]

    ctx.actions.run(
        executable       = ctx.executable._md2pdf,
        # Declare all README.md files as inputs.
        # Bazel re-runs this action only when one of them changes.
        inputs           = ctx.files.srcs,
        outputs          = [out],
        arguments        = args,
        mnemonic         = "Md2Pdf",
        progress_message = "Generating PDF: {}".format(out.short_path),
    )

    return [DefaultInfo(files = depset([out]))]


md2pdf = rule(
    implementation = _md2pdf_impl,
    attrs = {
        # The README.md source files — changes to any of these trigger a rebuild
        "srcs": attr.label_list(
            allow_files = [".md"],
            mandatory   = True,
            doc         = "README.md files to include in the PDF, in order",
        ),
        # PDF document title
        "title": attr.string(
            default = "Training Notes",
            doc     = "Title shown on the PDF cover page",
        ),
        # Comma-separated folder names passed to md2pdf.py
        # e.g. "day1,day2,day3"
        "folders": attr.string(
            mandatory = True,
            doc       = "Comma-separated folder names e.g. 'day1,day2,day3'",
        ),
        # Private: the md2pdf binary built from //src:md2pdf
        # cfg="exec" means it runs on the host machine (not the target platform)
        "_md2pdf": attr.label(
            default    = Label("//src:md2pdf"),
            executable = True,
            cfg        = "exec",
            doc        = "The md2pdf py_binary target",
        ),
    },
    doc = """\
Generates a PDF from local README.md files.
Output is cached by Bazel — only regenerated when a source file changes.
Find the result at bazel-bin/<package>/<name>.pdf after building.
""",
)
