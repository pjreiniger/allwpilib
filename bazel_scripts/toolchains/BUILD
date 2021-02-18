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

raspbian_builtin_include_dirs = [
        "external/raspbian-compiler-linux/raspbian10/sys-root/usr/lib/gcc/arm-linux-gnueabihf/8/include",
        "external/raspbian-compiler-linux/raspbian10/sys-root/usr/lib/gcc/arm-linux-gnueabihf/8/include-fixed",
        "external/raspbian-compiler-linux/raspbian10/sys-root/usr/include/c++/8",
        "external/raspbian-compiler-linux/raspbian10/sys-root/usr/include/arm-linux-gnueabihf/c++/8",
        "external/raspbian-compiler-linux/raspbian10/sys-root/usr/include/c++/8/backward",
        "external/raspbian-compiler-linux/raspbian10/sys-root/usr/include/arm-linux-gnueabihf",
        "external/raspbian-compiler-linux/raspbian10/sys-root/usr/include",
]


bionic_builtin_include_dirs = [
        "external/bionic-compiler-linux/bionic/sys-root/usr/lib/gcc/aarch64-linux-gnu/8/include",
        "external/bionic-compiler-linux/bionic/sys-root/usr/lib/gcc/aarch64-linux-gnu/8/include-fixed",
        "external/bionic-compiler-linux/bionic/sys-root/usr/include/c++/8",
        "external/bionic-compiler-linux/bionic/sys-root/usr/include/aarch64-linux-gnu/c++/8",
        "external/bionic-compiler-linux/bionic/sys-root/usr/include/c++/8/backward",
        "external/bionic-compiler-linux/bionic/sys-root/usr/include/aarch64-linux-gnu",
        "external/bionic-compiler-linux/bionic/sys-root/usr/include",
]

roborio_builtin_include_dirs = [
        "external/roborio-compiler-linux/frc2021/roborio/arm-frc2021-linux-gnueabi/usr/lib/gcc/arm-frc2021-linux-gnueabi/7.3.0/include",
        "external/roborio-compiler-linux/frc2021/roborio/arm-frc2021-linux-gnueabi/usr/lib/gcc/arm-frc2021-linux-gnueabi/7.3.0/include-fixed",
        "external/roborio-compiler-linux/frc2021/roborio/arm-frc2021-linux-gnueabi/usr/include/c++/7.3.0",
        "external/roborio-compiler-linux/frc2021/roborio/arm-frc2021-linux-gnueabi/usr/include/c++/7.3.0/arm-frc2021-linux-gnueabi",
        "external/roborio-compiler-linux/frc2021/roborio/arm-frc2021-linux-gnueabi/usr/include/c++/7.3.0/backward",
        "external/roborio-compiler-linux/frc2021/roborio/arm-frc2021-linux-gnueabi/usr/include",
]

create_cross_compile_toolchain(sysroot_subfolder = "frc2021/roborio/arm-frc2021-linux-gnueabi", cpu="roborio", builtin_include_dirs=roborio_builtin_include_dirs)
create_cross_compile_toolchain(sysroot_subfolder = "bionic/sys-root", cpu="bionic", builtin_include_dirs=bionic_builtin_include_dirs)
create_cross_compile_toolchain(sysroot_subfolder = "raspbian10/sys-root", cpu="raspbian", builtin_include_dirs=raspbian_builtin_include_dirs)
