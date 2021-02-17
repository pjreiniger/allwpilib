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

create_cross_compile_toolchain(sysroot_subfolder = "frc2021/roborio/arm-frc2021-linux-gnueabi", cpu="roborio")
create_cross_compile_toolchain(sysroot_subfolder = "bionic/sys-root", cpu="bionic")
create_cross_compile_toolchain(sysroot_subfolder = "raspbian10/sys-root", cpu="raspbian")
