load("@wpi_bazel_rules//rules:cc.bzl", "wpilib_cc_binary", "wpilib_cc_library", "wpilib_cc_shared_library", "wpilib_cc_test")
load("@wpi_bazel_rules//rules:cc.bzl", "default_wpilib_cc_dev_main")

def wpilib_hal_plugin(name, test_deps = [], linkopts = [], has_test = False, **kwargs):
    wpilib_cc_shared_library(name, linkopts = linkopts, visibility = ["//visibility:public"], **kwargs)

    default_wpilib_cc_dev_main(
        name = "DevMainCpp",
        wpi_shared_deps = [":" + name],
        linkopts = linkopts,
    )

    if has_test:
        wpilib_cc_test(
            name = "test",
            srcs = native.glob(["src/test/native/cpp/*.cpp"]),
            wpi_shared_deps = [":" + name],
            linkopts = linkopts,
            raw_deps = test_deps,
        )
