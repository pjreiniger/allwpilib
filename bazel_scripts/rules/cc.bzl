
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary", "cc_test")

############################################
# Compiler options for C++
############################################

# Windows
WINDOWS_WARNING_ARGS = ["/W3"]
WINDOWS_WARNINGS_AS_ERROR_ARGS = ["/WX"]
UNIX_WARNING_ARGS = ["-Wall", "-Wextra"]
UNIX_WARNINGS_AS_ERROR_ARGS = ["-Werror"]

WINDOWS_COMPILER_ARGS = ["/EHsc", "/FS", "/Zc:inline", "/wd4244", "/wd4267", "/wd4146", "/wd4996",
                        "/Zc:throwingNew", "/D_CRT_SECURE_NO_WARNINGS", "/std:c++17", "/bigobj", "/permissive-"
                        ]
WINDOWS_C_COMPILER_ARGS = ["/FS", "/Zc:inline", "/D_CRT_SECURE_NO_WARNINGS"]
WINDOWS_LINKER_ARGS = ["/DEBUG:FULL", "/PDBALTPATH:%_PDB%"]
WINDOWS_SYMBOL_ARG = "/Zi"

# Linux
LINUX_CROSS_COMPILER_ARGS = ["-std=c++17", "-Wformat=2", "-pedantic", "-Wno-psabi", "-Wno-unused-parameter", "-Wno-error=deprecated-declarations", "-fPIC", "-rdynamic", "-pthread"]
LINUX_CROSS_C_COMPILER_ARGS = ["-Wformat=2", "-pedantic", "-Wno-psabi", "-Wno-unused-parameter", "-fPIC", "-rdynamic", "-pthread"]
LINUX_CROSS_LINKER_ARGS = ["-rdynamic", "-pthread", "-latomic"]

LINUX_COMPILER_ARGS = ["-std=c++17", "-Wformat=2", "-pedantic", "-Wno-psabi", "-Wno-unused-parameter", "-Wno-error=deprecated-declarations", "-fPIC", "-rdynamic", "-pthread"]
LINUX_C_COMPILER_ARGS = ["-Wformat=2", "-pedantic", "-Wno-psabi", "-Wno-unused-parameter", "-fPIC", "-rdynamic", "-pthread"]
LINUX_LINKER_ARGS = ["-rdynamic", "-pthread", "-ldl", "-latomic"]

# Mac
MAC_COMPILER_ARGS = ["-std=c++17", "-pedantic", "-fPIC",
    #"-Wno-unused-parameter", "-Wno-error=deprecated-declarations", "-Wno-missing-field-initializers", "-Wno-unused-private-field",
    "-Wno-unused-const-variable", "-Wno-error=c11-extensions", "-pthread"]
MAC_C_COMPILER_ARGS = ["-pedantic", "-fPIC", "-Wno-unused-parameter", "-Wno-missing-field-initializers", "-Wno-unused-private-field"]
MAC_LINKER_ARGS = ["-framework", "CoreFoundation", "-framework", "AVFoundation", "-framework", "Foundation", "-framework", "CoreMedia", "-framework", "CoreVideo"]

############################################
# Default, gradle style, incude paths
############################################
DEFAULT_CC_INCLUDES = ["src/main/native/include", "src/main/native/cpp"]
DEFAULT_CC_SRC_PATTERN = ["src/main/native/cpp/**/*.cpp"]
DEFAULT_CC_HDR_PATTERN = [
            "src/main/native/include/**/*",
            "src/main/native/cpp/**/*.h",
            "src/main/native/cpp/**/*.inl",
            "src/main/native/cpp/**/*.inc",
]
DEFAULT_CC_TEST_SRC_PATTERN = ["src/test/native/cpp/**/*.cpp", "src/test/native/cpp/**/*.h"]
DEFAULT_CC_TEST_INCLUDE_PATTERN = ["src/test/native/cpp", "src/test/native/include"]

def _get_default_cxx_opts():
    return select({
            "@bazel_tools//src/conditions:windows": WINDOWS_COMPILER_ARGS + WINDOWS_WARNING_ARGS + WINDOWS_WARNINGS_AS_ERROR_ARGS,
            "@bazel_tools//src/conditions:linux_x86_64": LINUX_COMPILER_ARGS + UNIX_WARNING_ARGS + UNIX_WARNINGS_AS_ERROR_ARGS,
            "@bazel_tools//src/conditions:darwin": MAC_COMPILER_ARGS, #  + UNIX_WARNING_ARGS + UNIX_WARNINGS_AS_ERROR_ARGS,
            "//bazel_scripts/toolchains:roborio": LINUX_CROSS_COMPILER_ARGS + UNIX_WARNING_ARGS + UNIX_WARNINGS_AS_ERROR_ARGS,
            "//bazel_scripts/toolchains:bionic": LINUX_CROSS_COMPILER_ARGS + UNIX_WARNING_ARGS + UNIX_WARNINGS_AS_ERROR_ARGS,
            "//bazel_scripts/toolchains:raspbian": LINUX_CROSS_COMPILER_ARGS + UNIX_WARNING_ARGS + UNIX_WARNINGS_AS_ERROR_ARGS,
        })

def _get_default_linker_opts():
    return select({
            "@bazel_tools//src/conditions:windows": WINDOWS_LINKER_ARGS,
            "@bazel_tools//src/conditions:linux_x86_64": LINUX_LINKER_ARGS,
            "@bazel_tools//src/conditions:darwin": MAC_LINKER_ARGS,
            "//bazel_scripts/toolchains:roborio": LINUX_CROSS_LINKER_ARGS,
            "//bazel_scripts/toolchains:bionic": LINUX_CROSS_LINKER_ARGS,
            "//bazel_scripts/toolchains:raspbian": LINUX_CROSS_LINKER_ARGS,
        })

def wpilib_cc_library(name, srcs = [], gen_scrs = [], hdrs = [], includes = [] , copts = [], linkopts = [], **kwargs):

    srcs = gen_scrs + (srcs or native.glob(DEFAULT_CC_SRC_PATTERN))
    hdrs = hdrs or native.glob(DEFAULT_CC_HDR_PATTERN)
    includes = includes or DEFAULT_CC_INCLUDES
    copts = copts + _get_default_cxx_opts()
    linkopts = linkopts + _get_default_linker_opts()

    cc_library(
        name = name,
        srcs = srcs,
        hdrs = hdrs,
        includes = includes,
        copts = copts,
        linkopts = linkopts,
        **kwargs,
    )


def wpilib_cc_test(name, srcs = [], includes = [], deps = [], tags = [], **kwargs):

    srcs = srcs or native.glob(DEFAULT_CC_TEST_SRC_PATTERN)
    includes = includes or DEFAULT_CC_TEST_INCLUDE_PATTERN

    cc_test(
        name = name,
        srcs = srcs,
        includes = includes,
        copts = _get_default_cxx_opts(),
        linkopts = _get_default_linker_opts(),
        deps = deps + ["@gtest//:gtest"],
        tags = ['no-roborio', 'no-bionic', 'no-raspbian'],
        **kwargs,
    )

def wpilib_cc_binary(name, copts = [], linkopts = [], **kwargs):
    cc_binary(name = name,
        copts = copts + _get_default_cxx_opts(),
        linkopts = linkopts + _get_default_linker_opts(),
        **kwargs)
