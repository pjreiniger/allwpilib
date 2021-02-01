

load("//bazel_scripts/rules:cc.bzl", "wpilib_cc_library")

def wpilib_hal_plugin(name, srcs = [], **kwargs):

    wpilib_cc_library(
        name = name + "-lib",
        srcs = srcs or native.glob(["src/main/native/cpp/**/*.cpp"]),
        hdrs = native.glob(["src/main/native/include/**/*.h", "src/main/native/cpp/**/*.h"]),
        includes = ["src/main/native/include", "src/main/native/cpp"],
        tags = ['no-roborio', 'no-bionic', 'no-raspbian'],
        **kwargs
    )
