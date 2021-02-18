load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
     "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
     "feature",
     "flag_group",
     "flag_set",
     "tool_path",
 )

all_link_actions = [ # NEW
     ACTION_NAMES.cpp_link_executable,
     ACTION_NAMES.cpp_link_dynamic_library,
     ACTION_NAMES.cpp_link_nodeps_dynamic_library,
 ]

def _impl(ctx):

    # Alias for readability
    wrapper_path = ctx.attr.wrapper_path
    wrapper_extension = ctx.attr.wrapper_extension
    builtin_inculde_base = ctx.attr.builtin_inculde_base

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

    features = [ # NEW
         feature(
             name = "default_linker_flags",
             enabled = True,
             flag_sets = [
                 flag_set(
                     actions = all_link_actions,
                     flag_groups = ([
                         flag_group(
                             flags = [
                                 "-lstdc++",
                                 #"-L" + builtin_inculde_base,
                                   "-L/root/.cache/bazel/_bazel_root/ff29c1f9165945853fa31de8bcb64911/external/raspbian-compiler-linux/raspbian10/sys-root/usr/lib/arm-linux-gnueabihf",
                                 # "-L/root/.cache/bazel/_bazel_root/ff29c1f9165945853fa31de8bcb64911/external/roborio-compiler-linux/frc2021/roborio/arm-frc2021-linux-gnueabi/usr/lib",
                             ],
                         ),
                     ]),
                 ),
             ],
         ),
     ]
     
    # builtin_inculde_base = '/root/.cache/bazel/_bazel_root/ff29c1f9165945853fa31de8bcb64911/external/roborio-compiler-linux/frc2021/roborio/arm-frc2021-linux-gnueabi/usr/lib/gcc/arm-frc2021-linux-gnueabi/7.3.0'

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        toolchain_identifier = ctx.attr.toolchain_identifier,
        cxx_builtin_include_directories = [
          builtin_inculde_base + '/include',
          builtin_inculde_base + '/include-fixed',
        ],
        host_system_name = "arfsd",
        target_system_name = "arfsd",
        target_cpu = ctx.attr.cpu,
        target_libc = "unknown",
        compiler = ctx.attr.compiler,
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths, # NEW
        builtin_sysroot = ctx.attr.builtin_sysroot,
    )

cc_toolchain_config = rule(
    attrs = {
        "cpu": attr.string(mandatory=True),
        "compiler": attr.string(mandatory = True),
        "builtin_sysroot": attr.string(mandatory = True),
        "toolchain_identifier": attr.string(mandatory = True),
        "wrapper_path": attr.string(mandatory = True),
        "wrapper_extension": attr.string(mandatory = True),
        "builtin_inculde_base": attr.string(mandatory = True),
    },
    provides = [CcToolchainConfigInfo],
    implementation = _impl,
)

