load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
     "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
     "feature",
     "flag_group",
     "flag_set",
     "tool_path",
 )


all_compile_actions = [
    ACTION_NAMES.c_compile,
    ACTION_NAMES.cpp_compile,
    ACTION_NAMES.linkstamp_compile,
    ACTION_NAMES.assemble,
    ACTION_NAMES.preprocess_assemble,
    ACTION_NAMES.cpp_header_parsing,
    ACTION_NAMES.cpp_module_compile,
    ACTION_NAMES.cpp_module_codegen,
    ACTION_NAMES.clif_match,
    ACTION_NAMES.lto_backend,
]

all_link_actions = [ # NEW
     ACTION_NAMES.cpp_link_executable,
     ACTION_NAMES.cpp_link_dynamic_library,
     ACTION_NAMES.cpp_link_nodeps_dynamic_library,
 ]

def _impl(ctx):

    # Alias for readability
    wrapper_path = ctx.attr.wrapper_path
    wrapper_extension = ctx.attr.wrapper_extension
    builtin_include_dirs = ctx.attr.builtin_include_dirs

    tool_paths = [ # NEW
        tool_path(name = "gcc",     path = wrapper_path + "/gcc" + wrapper_extension),
        tool_path(name = "ld",      path = wrapper_path + "/ld" + wrapper_extension),
        tool_path(name = "ar",      path = wrapper_path + "/ar" + wrapper_extension),
        tool_path(name = "cpp",     path = wrapper_path + "/cpp" + wrapper_extension),
        tool_path(name = "gcov",    path = wrapper_path + "/gcov" + wrapper_extension),
        tool_path(name = "nm",      path = wrapper_path + "/nm" + wrapper_extension),
        tool_path(name = "objdump", path = wrapper_path + "/objdump" + wrapper_extension),
        tool_path(name = "strip",   path = wrapper_path + "/strip" + wrapper_extension),
    ]

    system_include_flags = []
    for d in builtin_include_dirs:
        system_include_flags.extend(["-isystem", d])

    sysroot_feature = feature(
        name = "sysroot",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_compile_actions,
                flag_groups = [
                    flag_group(
                        flags = system_include_flags
                    ),
                ],
            ),
        ],
    )

    features = [sysroot_feature]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        toolchain_identifier = ctx.attr.toolchain_identifier,
        cxx_builtin_include_directories = builtin_include_dirs,
        host_system_name = "arfsd",
        target_system_name = "arfsd",
        target_cpu = ctx.attr.cpu,
        target_libc = "unknown",
        compiler = ctx.attr.compiler,
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths, # NEW
        builtin_sysroot = None, # ctx.attr.builtin_sysroot,
    )

cc_toolchain_config = rule(
    attrs = {
        "cpu": attr.string(mandatory=True),
        "compiler": attr.string(mandatory = True),
        "builtin_sysroot": attr.string(mandatory = True),
        "toolchain_identifier": attr.string(mandatory = True),
        "wrapper_path": attr.string(mandatory = True),
        "wrapper_extension": attr.string(mandatory = True),
        "builtin_include_dirs": attr.string_list(mandatory = True),
    },
    provides = [CcToolchainConfigInfo],
    implementation = _impl,
)

