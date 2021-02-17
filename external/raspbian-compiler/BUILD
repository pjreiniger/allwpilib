
filegroup(
    name = "gcc",
    srcs = ["raspbian10/bin/arm-raspbian10-linux-gnueabihf-gcc.exe"],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "includes",    
    srcs = glob([
        "raspbian10/sys-root/usr/lib/gcc/arm-linux-gnueabihf/8/include/**",
        "raspbian10/sys-root/usr/lib/gcc/arm-linux-gnueabihf/8/include-fixed/**",
        "raspbian10/sys-root/usr/include/c++/8/**",
        "raspbian10/sys-root/usr/include/arm-linux-gnueabihf/c++/8/**",
        "raspbian10/sys-root/usr/include/c++/8/backward/**",
        "raspbian10/sys-root/usr/include/arm-linux-gnueabihf/**",
        "raspbian10/sys-root/usr/include/**",
    ]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "everything",
    srcs = [":gcc", ":includes"],
    visibility = ["//visibility:public"],
)

