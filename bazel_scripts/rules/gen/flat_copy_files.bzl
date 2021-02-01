# contents of a file you create named: copy_filegroups.bzl
# known to work in bazel version 0.22.0
def _copy_filegroup_impl(ctx):
    all_input_files = [
        f for t in ctx.attr.targeted_filegroups for f in t.files.to_list()
    ]

    all_outputs = []
    for f in all_input_files:
        out = ctx.actions.declare_file(ctx.attr.output_directory + "/" + f.basename)
        all_outputs += [out]
        ctx.actions.run_shell(
            outputs=[out],
            inputs=depset([f]),
            arguments=[f.path, out.path],
            command="cp $1 $2")

    # Small sanity check
    if len(all_input_files) != len(all_outputs):
        fail("Output count should be 1-to-1 with input count.")

    return [
        DefaultInfo(
            files=depset(all_outputs),
            runfiles=ctx.runfiles(files=all_outputs))
    ]


flat_copy_files = rule(
    implementation=_copy_filegroup_impl,
    attrs={
        "targeted_filegroups": attr.label_list(mandatory=True),
        "output_directory": attr.string(mandatory=True),
    },
)
