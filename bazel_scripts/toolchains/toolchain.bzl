load(":cc-toolchain-config.bzl", "cc_toolchain_config")
load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")


def create_cross_compile_toolchain(cpu, sysroot_subfolder):

    # compiler_dep = cpu + "-compiler-win"
    # wrapper_extension = ".bat"
    compiler_dep = cpu + "-compiler-linux"
    wrapper_extension = ".sh"

    suite_name = cpu + "_suite"

    compiler = "Hello"
    tag = "{}_{}".format(suite_name, compiler)


    builtin_sysroot = "external/{}/{}".format(compiler_dep, sysroot_subfolder)
    # builtin_inculde_base = builtin_sysroot + "/usr/lib/gcc/arm-linux-gnueabihf/8"
    builtin_inculde_base = "/root/.cache/bazel/_bazel_root/ff29c1f9165945853fa31de8bcb64911/external/raspbian-compiler-linux/raspbian10/sys-root" + "/usr/lib/gcc/arm-linux-gnueabihf/8"
    # 
    print("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")
    print(builtin_sysroot)
    print(builtin_inculde_base)

    cc_toolchain_config_name = "cc-toolchain_config-{}".format(tag)

    compiler_files_name = "compiler_files-{}".format(tag)
    cc_toolchain_name = "cc-compiler-{}".format(tag)
    toolchain_name = "cc-toolchain-{}".format(tag)
    cc_toolchain_suite_name = suite_name

    sysroot_everything = "@{}//:everything".format(compiler_dep)

    wrapper_path = cpu + "/wrappers"

    toolchain_key = cpu
    
    cc_toolchain_config(
        name = cc_toolchain_config_name,
        cpu = cpu,
        compiler = compiler,
        toolchain_identifier = toolchain_name,
        builtin_sysroot = builtin_sysroot,
        wrapper_path=wrapper_path,
        wrapper_extension=wrapper_extension,
        builtin_inculde_base=builtin_inculde_base)

    native.filegroup(
        name = compiler_files_name,
        srcs = native.glob([wrapper_path + "/**"]) + [
            sysroot_everything
        ] ,
    )

    cc_toolchain(
        name = cc_toolchain_name,
        all_files = compiler_files_name,
        ar_files = compiler_files_name,
        as_files = compiler_files_name,
        compiler_files =  compiler_files_name,
        dwp_files = compiler_files_name,
        # dynamic_runtime_lib = [],
        linker_files =  compiler_files_name,
        objcopy_files = compiler_files_name,
        # static_runtime_lib = [],
        strip_files = compiler_files_name,
        supports_param_files = True,
        toolchain_config = cc_toolchain_config_name,
        visibility = ["//visibility:public"],
    )

    native.toolchain(
        name = toolchain_name,
        exec_compatible_with = ["@bazel_tools//platforms/linux"],
        target_compatible_with = ["@bazel_tools//platforms/linux"],
        toolchain = cc_toolchain_name,
        toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
    )
    
    cc_toolchain_suite(
        name = cc_toolchain_suite_name,
        toolchains = {
            toolchain_key: cc_toolchain_name,
        },
        tags = ["manual"],
    )
