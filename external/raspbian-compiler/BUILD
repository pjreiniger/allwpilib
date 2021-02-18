
filegroup(
    name = "gcc",
    srcs = select({
		"@bazel_tools//src/conditions:windows": ["raspbian10/bin/arm-raspbian10-linux-gnueabihf-gcc.exe"],
        "//conditions:default": ["raspbian10/bin/arm-raspbian10-linux-gnueabihf-gcc"],
	}),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "ar",
    srcs = select({
		"@bazel_tools//src/conditions:windows": ["raspbian10/bin/arm-raspbian10-linux-gnueabihf-ar.exe"],
        "//conditions:default": ["raspbian10/bin/arm-raspbian10-linux-gnueabihf-ar"],
	}),
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
    srcs = [":ar", ":gcc", ":includes"],
    visibility = ["//visibility:public"],
)

