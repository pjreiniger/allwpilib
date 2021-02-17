load(":toolchain.bzl", "create_cross_compile_toolchain")
load("@rules_python//python:defs.bzl", "py_binary")


py_binary(name="create_wrappers", srcs=["create_wrappers.py"])


config_setting(
    name = "raspbian",
    values = {"cpu": "raspbian"},
    visibility = ["//visibility:public"],
)
config_setting(
    name = "bionic",
    values = {"cpu": "bionic"},
    visibility = ["//visibility:public"],
)
config_setting(
    name = "roborio",
    values = {"cpu": "roborio"},
    visibility = ["//visibility:public"],
)

create_cross_compile_toolchain(compiler_dep="roborio-compiler-win", sysroot_subfolder = "frc2021/roborio/arm-frc2021-linux-gnueabi", cpu="roborio", compiler="hello", suite_name = "roborio_suite")
create_cross_compile_toolchain(compiler_dep="bionic-compiler-win", sysroot_subfolder = "bionic/sys-root", cpu="bionic", compiler="hello", suite_name = "bionic_suite")
create_cross_compile_toolchain(compiler_dep="raspbian-compiler-win", sysroot_subfolder = "raspbian10/sys-root", cpu="raspbian", compiler="hello", suite_name = "raspbian_suite")
