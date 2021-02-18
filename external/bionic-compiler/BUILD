
filegroup(
    name = "gcc",
    srcs = select({
		"@bazel_tools//src/conditions:windows": ["bionic/bin/aarch64-bionic-linux-gnu-gcc.exe"],
        "//conditions:default": ["bionic/bin/aarch64-bionic-linux-gnu-gcc"],
	}),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "ar",
    srcs = select({
		"@bazel_tools//src/conditions:windows": ["bionic/bin/aarch64-bionic-linux-gnu-ar.exe"],
        "//conditions:default": ["bionic/bin/aarch64-bionic-linux-gnu-ar"],
	}),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "includes",    
    srcs = glob([
        "bionic/sys-root/usr/lib/gcc/aarch64-linux-gnu/8/include/**",
        "bionic/sys-root/usr/lib/gcc/aarch64-linux-gnu/8/include-fixed/**",
        "bionic/sys-root/usr/include/c++/8/**",
        "bionic/sys-root/usr/include/aarch64-linux-gnu/c++/8/**",
        "bionic/sys-root/usr/include/c++/8/backward/**",
        "bionic/sys-root/usr/include/aarch64-linux-gnu/**",
        "bionic/sys-root/usr/include/**",
    ]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "everything",
    srcs = [":ar", ":gcc", ":includes"],
    visibility = ["//visibility:public"],
)

